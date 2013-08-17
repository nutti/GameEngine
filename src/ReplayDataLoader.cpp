#include "ReplayDataLoader.h"

#include <MAPIL/MAPIL.h>
#include <vector>

#include "Util.h"

namespace GameEngine
{
	const char* REPLAY_FILE_DIR = "replay";
	const char* REPLAY_FILE_NAME_PREFIX = "replay";
	const char* REPLAY_FILE_NAME_SUFFIX = ".rpy";

	class ReplayDataLoader::Impl
	{
	private:
		ReplayDataRecord						m_ReplayDataRecord;		// リプレイデータ
	public:
		Impl();
		~Impl(){}
		ButtonPushedStatus GetButtonState( int stageNo, int frame ) const;
		void Load( const std::string& fileName );
		void Cleanup();
		DisplayedReplayInfo::Entry GetDisplayedInfo( const std::string& fileName ) const;
		const ReplayDataRecord& GetReplayDataRecord() const;
		InitialGameData GetIniGameData( int stageNo ) const;
		int GetGameDifficulty() const;
	};

	ReplayDataLoader::Impl::Impl()
	{
		Cleanup();
	}

	ButtonPushedStatus ReplayDataLoader::Impl::GetButtonState( int stageNo, int frame ) const
	{
		return m_ReplayDataRecord.m_StageKeyStatusList[ stageNo - 1 ].m_StatusList[ frame ];
	}

	void ReplayDataLoader::Impl::Cleanup()
	{
		MAPIL::ZeroObject( &m_ReplayDataRecord.m_StageDataInfo, sizeof( m_ReplayDataRecord.m_StageDataInfo ) );
		m_ReplayDataRecord.m_Crystal = 0;
		m_ReplayDataRecord.m_CrystalUsed = 0;
		MAPIL::ZeroObject( &m_ReplayDataRecord.m_Date, sizeof( m_ReplayDataRecord.m_Date ) );
		m_ReplayDataRecord.m_Difficulty = 0;
		m_ReplayDataRecord.m_Killed = 0;
		MAPIL::ZeroObject( m_ReplayDataRecord.m_Name, sizeof( m_ReplayDataRecord.m_Name ) );
		m_ReplayDataRecord.m_Progress = 0;
		m_ReplayDataRecord.m_Score = 0;
		for( int i = 0; i < STAGE_TOTAL; ++i ){
			m_ReplayDataRecord.m_StageKeyStatusList[ i ].m_StatusList.clear();
		}
	}

	DisplayedReplayInfo::Entry ReplayDataLoader::Impl::GetDisplayedInfo( const std::string& fileName ) const
	{
		DisplayedReplayInfo::Entry entry;

		// ファイルの読み込み
		std::fstream fIn( fileName, std::ios::binary | std::ios::in );
		if( !fIn ){
			MAPIL::ZeroObject( &entry, sizeof( entry ) );
			entry.m_Progress = -1;
			return entry;
		}
		int size = GetFileSize( fIn );
		char* pBuf = new char [ size ];
		fIn.read( pBuf, size * sizeof( char ) );
		fIn.close();

		// XOR暗号復号化
		MAPIL::XOR xor( 60 );
		xor.Decrypt( pBuf, size );
		// シーザ暗号復号化
		MAPIL::Caesar caesar( 10 );
		caesar.Decrypt( pBuf, size );
		// 解凍
		MAPIL::LZ lz( 200, 5 );
		char* pData = new char [ size * 10 ];
		int dataSize = 0;
		lz.Expand( pBuf, size, &pData, size * 10, &dataSize );
		MAPIL::SafeDeleteArray( pBuf );


		// データの設定
		char* p = pData;

		::memcpy( entry.m_Name, p, sizeof( entry.m_Name ) );
		p += sizeof( entry.m_Name );
		entry.m_Progress = GetInt( &p );
		entry.m_Score = GetInt( &p );
		entry.m_Crystal = GetInt( &p );
		entry.m_CrystalUsed = GetInt( &p );
		entry.m_Killed = GetInt( &p );
		entry.m_Difficulty = GetInt( &p );
		entry.m_Date.m_Year = GetInt( &p );
		entry.m_Date.m_Month = *p++;
		entry.m_Date.m_Day = *p++;
		entry.m_Date.m_Hour = *p++;
		entry.m_Date.m_Min = *p++;
		entry.m_Date.m_Sec = *p++;
		// 各ステージデータ開始時のデータを取得
		ReplayDataRecord::StageDataInfo stage[ STAGE_TOTAL ];
		MAPIL::ZeroObject( stage, sizeof( stage ) );
		for( int i = 0; i < STAGE_TOTAL; ++i ){
			stage[ i ].m_IniPosX = GetInt( &p );
			stage[ i ].m_IniPosY = GetInt( &p );
			stage[ i ].m_IniHP = GetInt( &p );
			stage[ i ].m_IniShotPower = GetInt( &p );
			stage[ i ].m_IniScore = GetInt( &p );
			stage[ i ].m_IniKilled = GetInt( &p );
			stage[ i ].m_IniCrystal = GetInt( &p );
			stage[ i ].m_IniCrystalUsed = GetInt( &p );
			stage[ i ].m_IniCons = GetInt( &p );
			for( int j = 0; j < 3; ++j ){
				stage[ i ].m_IniConsGauge[ j ] = GetInt( &p );
			}
			for( int j = 0; j < 3; ++j ){
				stage[ i ].m_IniConsLevel[ j ] = GetInt( &p );
			}
			
			int frameTotal = GetInt( &p );
			// 入力ボタンのロード
			for( unsigned int j = 0; j < frameTotal; ++j ){
				int lo = *p++;
				int hi = *p++;
			}
		}

		for( int i = 0; i < STAGE_TOTAL; ++i ){
			entry.m_StageInfo[ i ].m_Score = stage[ i + 1 ].m_IniScore;
			entry.m_StageInfo[ i ].m_Crystal = stage[ i + 1 ].m_IniCrystal;
			entry.m_StageInfo[ i ].m_Killed = stage[ i + 1 ].m_IniKilled;
		}

		MAPIL::SafeDeleteArray( pData );

		return entry;
	}

	void ReplayDataLoader::Impl::Load( const std::string& fileName )
	{
		// ファイルの読み込み
		std::fstream fIn( fileName, std::ios::binary | std::ios::in );
		int size = GetFileSize( fIn );
		char* pBuf = new char [ size ];
		fIn.read( pBuf, size * sizeof( char ) );
		fIn.close();

		// XOR暗号復号化
		MAPIL::XOR xor( 60 );
		xor.Decrypt( pBuf, size );
		// シーザ暗号復号化
		MAPIL::Caesar caesar( 10 );
		caesar.Decrypt( pBuf, size );
		// 解凍
		MAPIL::LZ lz( 200, 5 );
		char* pData = new char [ size * 1000 ];
		int dataSize = 0;
		lz.Expand( pBuf, size, &pData, size * 1000, &dataSize );
		MAPIL::SafeDeleteArray( pBuf );

		// データ設定
		char* p = pData;
		::memcpy( m_ReplayDataRecord.m_Name, p, sizeof( m_ReplayDataRecord.m_Name ) );
		p += sizeof( m_ReplayDataRecord.m_Name );
		m_ReplayDataRecord.m_Progress = GetInt( &p );
		m_ReplayDataRecord.m_Score = GetInt( &p );
		m_ReplayDataRecord.m_Crystal = GetInt( &p );
		m_ReplayDataRecord.m_CrystalUsed = GetInt( &p );
		m_ReplayDataRecord.m_Killed = GetInt( &p );
		m_ReplayDataRecord.m_Difficulty = GetInt( &p );
		m_ReplayDataRecord.m_Date.m_Year = GetInt( &p );
		m_ReplayDataRecord.m_Date.m_Month = *p++;
		m_ReplayDataRecord.m_Date.m_Day = *p++;
		m_ReplayDataRecord.m_Date.m_Hour = *p++;
		m_ReplayDataRecord.m_Date.m_Min = *p++;
		m_ReplayDataRecord.m_Date.m_Sec = *p++;
		// 各ステージデータ開始時のデータを取得
		for( int i = 0; i < STAGE_TOTAL; ++i ){
			ReplayDataRecord::StageDataInfo stage;
			stage.m_IniPosX = GetInt( &p );
			stage.m_IniPosY = GetInt( &p );
			stage.m_IniHP = GetInt( &p );
			stage.m_IniShotPower = GetInt( &p );
			stage.m_IniScore = GetInt( &p );
			stage.m_IniKilled = GetInt( &p );
			stage.m_IniCrystal = GetInt( &p );
			stage.m_IniCrystalUsed = GetInt( &p );
			stage.m_IniCons = GetInt( &p );
			for( int j = 0; j < 3; ++j ){
				stage.m_IniConsGauge[ j ] = GetInt( &p );
			}
			for( int j = 0; j < 3; ++j ){
				stage.m_IniConsLevel[ j ] = GetInt( &p );
			}
			m_ReplayDataRecord.m_StageDataInfo[ i ] = stage;

			int frameTotal = GetInt( &p );
			// 入力ボタンのロード
			m_ReplayDataRecord.m_StageKeyStatusList[ i ].m_StatusList.resize( frameTotal + 50, 0 );
			for( unsigned int j = 0; j < frameTotal; ++j ){
				int lo = *p++;
				int hi = *p++;
				m_ReplayDataRecord.m_StageKeyStatusList[ i ].m_StatusList[ j ] = ( ( hi & 0xFF ) << 8 ) | ( lo & 0xFF );
			}
		}

		

		MAPIL::SafeDeleteArray( pData );
	}

	const ReplayDataRecord& ReplayDataLoader::Impl::GetReplayDataRecord() const
	{
		return m_ReplayDataRecord;
	}

	InitialGameData ReplayDataLoader::Impl::GetIniGameData( int stageNo ) const
	{
		InitialGameData data;

		MAPIL::ZeroObject( &data, sizeof( data ) );

		// 進行度よりもステージ番号が小さい時に、リプレイ可能
		if( m_ReplayDataRecord.m_Progress >= stageNo ){
			data.m_Cons = m_ReplayDataRecord.m_StageDataInfo[ stageNo - 1 ].m_IniCons;
			::memcpy( data.m_ConsGauge, m_ReplayDataRecord.m_StageDataInfo[ stageNo - 1 ].m_IniConsGauge, sizeof( data.m_ConsGauge ) );
			::memcpy( data.m_ConsLevel, m_ReplayDataRecord.m_StageDataInfo[ stageNo - 1 ].m_IniConsLevel, sizeof( data.m_ConsLevel ) );
			for( int i = 0; i < stageNo; ++i ){
				data.m_Crystal += m_ReplayDataRecord.m_StageDataInfo[ i ].m_IniCrystal;
				data.m_CrystalUsed += m_ReplayDataRecord.m_StageDataInfo[ i ].m_IniCrystalUsed;
				data.m_Killed += m_ReplayDataRecord.m_StageDataInfo[ i ].m_IniKilled;
				data.m_Score += m_ReplayDataRecord.m_StageDataInfo[ i ].m_IniScore;
			}
			data.m_HP = m_ReplayDataRecord.m_StageDataInfo[ stageNo - 1 ].m_IniHP;
			data.m_PosX = m_ReplayDataRecord.m_StageDataInfo[ stageNo - 1 ].m_IniPosX;
			data.m_PosY = m_ReplayDataRecord.m_StageDataInfo[ stageNo - 1 ].m_IniPosY;
			data.m_ShotPower = m_ReplayDataRecord.m_StageDataInfo[ stageNo - 1 ].m_IniShotPower;
		}

		return data;
	}

	int ReplayDataLoader::Impl::GetGameDifficulty() const
	{
		return m_ReplayDataRecord.m_Difficulty;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	ReplayDataLoader::ReplayDataLoader() : m_pImpl( new ReplayDataLoader::Impl )
	{
	}

	ReplayDataLoader::~ReplayDataLoader()
	{
	}

	ButtonPushedStatus ReplayDataLoader::GetButtonState( int stageNo, int frame ) const
	{
		return m_pImpl->GetButtonState( stageNo, frame );
	}
	
	void ReplayDataLoader::Load( const std::string& fileName )
	{
		m_pImpl->Load( fileName );
	}

	void ReplayDataLoader::Cleanup()
	{
		m_pImpl->Cleanup();
	}

	DisplayedReplayInfo::Entry ReplayDataLoader::GetDisplayedInfo( const std::string& fileName ) const
	{
		return m_pImpl->GetDisplayedInfo( fileName );
	}

	const ReplayDataRecord& ReplayDataLoader::GetReplayDataRecord() const
	{
		return m_pImpl->GetReplayDataRecord();
	}

	InitialGameData ReplayDataLoader::GetIniGameData( int stageNo ) const
	{
		return m_pImpl->GetIniGameData( stageNo );
	}

	int ReplayDataLoader::GetGameDifficulty() const
	{
		return m_pImpl->GetGameDifficulty();
	}
}
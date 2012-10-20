#include "ReplayDataLoader.h"

#include <MAPIL/MAPIL.h>
#include <vector>

#include "Util.h"

namespace GameEngine
{
	const char* REPLAY_FILE_NAME_PREFIX = "replay/replay";
	const char* REPLAY_FILE_NAME_SUFFIX = ".rpy";

	struct ReplayDataInfo
	{
		struct StageData
		{
			int		m_IniPosX;				// 初期X座標
			int		m_IniPosY;				// 初期Y座標
			int		m_IniHP;				// 初期HP
			int		m_IniShotPower;			// 初期ショットパワー
			int		m_IniScore;				// 初期の得点
			int		m_IniKilled;			// 初期の撃破数
			int		m_IniCrystal;			// 初期のクリスタル獲得数
			int		m_IniCons;				// 初期の意識状態
			int		m_IniConsGauge[ 3 ];	// 初期の意識ゲージ
			int		m_IniConsLevel[ 3 ];	// 初期の意識レベル
			int		m_FrameTotal;			// ステージのフレーム数
		};
		struct Date
		{
			int		m_Year;
			char	m_Month;
			char	m_Day;
			char	m_Hour;
			char	m_Min;
			char	m_Sec;
		};

		char					m_Name[ 10 ];			// 名前
		int						m_Progress;				// 進行度
		int						m_Score;				// スコア
		int						m_Crystal;				// クリスタル獲得数
		int						m_Killed;				// 撃破数
		int						m_Difficulty;			// 難易度
		Date					m_Date;					// 作成日時

		StageData				m_StageData[ 5 ];		// 初期ステージデータ
	};

	class ReplayDataLoader::Impl
	{
	private:
		std::vector < ButtonPushedStatus >		m_ButtonList;			// 過去のボタンの履歴
		ReplayDataInfo							m_ReplayDataInfo;		// リプレイデータの情報
	public:
		Impl();
		~Impl(){}
		ButtonPushedStatus GetButtonState( int frame ) const;
		void Load( const std::string& fileName );
		void Cleanup();
		DisplayedReplayInfo::Entry GetDisplayedInfo( const std::string& fileName ) const;
	};

	ReplayDataLoader::Impl::Impl()
	{
		MAPIL::ZeroObject( &m_ReplayDataInfo, sizeof( m_ReplayDataInfo ) );
		m_ButtonList.clear();
	}

	ButtonPushedStatus ReplayDataLoader::Impl::GetButtonState( int frame ) const
	{
		return m_ButtonList[ frame ];
	}

	void ReplayDataLoader::Impl::Load( const std::string& fileName )
	{
		std::ifstream fIn( fileName, std::ios::binary | std::ios::in );

		// スコア全体情報を取得
		fIn.read( m_ReplayDataInfo.m_Name, sizeof( m_ReplayDataInfo.m_Name ) );
		m_ReplayDataInfo.m_Progress = ReadInt( &fIn );
		m_ReplayDataInfo.m_Score = ReadInt( &fIn );
		m_ReplayDataInfo.m_Crystal = ReadInt( &fIn );
		m_ReplayDataInfo.m_Killed = ReadInt( &fIn );
		m_ReplayDataInfo.m_Difficulty = ReadInt( &fIn );
		m_ReplayDataInfo.m_Date.m_Year = ReadInt( &fIn );
		m_ReplayDataInfo.m_Date.m_Month = ReadInt( &fIn );
		m_ReplayDataInfo.m_Date.m_Day = ReadInt( &fIn );
		m_ReplayDataInfo.m_Date.m_Hour = ReadInt( &fIn );
		m_ReplayDataInfo.m_Date.m_Min = ReadInt( &fIn );
		m_ReplayDataInfo.m_Date.m_Sec = ReadInt( &fIn );
		// 各ステージデータ開始時のデータを取得
		for( int i = 0; i < 5; ++i ){
			ReplayDataInfo::StageData stage;
			stage.m_IniPosX = ReadInt( &fIn );
			stage.m_IniPosY = ReadInt( &fIn );
			stage.m_IniHP = ReadInt( &fIn );
			stage.m_IniShotPower = ReadInt( &fIn );
			stage.m_IniScore = ReadInt( &fIn );
			stage.m_IniKilled = ReadInt( &fIn );
			stage.m_IniCrystal = ReadInt( &fIn );
			stage.m_IniCons = ReadInt( &fIn );
			for( int j = 0; j < 3; ++j ){
				stage.m_IniConsGauge[ j ] = ReadInt( &fIn );
			}
			for( int j = 0; j < 3; ++j ){
				stage.m_IniConsLevel[ j ] = ReadInt( &fIn );
			}
			stage.m_FrameTotal = ReadInt( &fIn );
		}

		// 入力ボタンのロード
		int frame = ReadInt( &fIn );
		m_ButtonList.resize( frame, 0 );
		for( unsigned int i = 0; i < m_ButtonList.size(); ++i ){
			fIn.read( &m_ButtonList[ i ], sizeof( char ) );
		}

		fIn.close();

	}

	void ReplayDataLoader::Impl::Cleanup()
	{
		MAPIL::ZeroObject( &m_ReplayDataInfo, sizeof( m_ReplayDataInfo ) );
		m_ButtonList.clear();
	}

	DisplayedReplayInfo::Entry ReplayDataLoader::Impl::GetDisplayedInfo( const std::string& fileName ) const
	{
		DisplayedReplayInfo::Entry entry;

		std::ifstream fIn( fileName, std::ios::binary | std::ios::in );
		if( !fIn ){
			MAPIL::ZeroObject( &entry, sizeof( entry ) );
			entry.m_Progress = -1;
			return entry;
		}

		// スコア全体情報を取得
		fIn.read( entry.m_Name, sizeof( entry.m_Name ) );
		entry.m_Progress = ReadInt( &fIn );
		entry.m_Score = ReadInt( &fIn );
		entry.m_Crystal = ReadInt( &fIn );
		entry.m_Killed = ReadInt( &fIn );
		entry.m_Difficulty = ReadInt( &fIn );

		fIn.close();

		return entry;
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

	ButtonPushedStatus ReplayDataLoader::GetButtonState( int frame ) const
	{
		return m_pImpl->GetButtonState( frame );
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
}
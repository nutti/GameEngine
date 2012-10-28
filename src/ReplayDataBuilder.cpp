#include "ReplayDataBuilder.h"

#include <MAPIL/MAPIL.h>
#include <vector>

#include "Util.h"

namespace GameEngine
{

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

		char					m_Name[ 10 ];			// 名前
		int						m_Progress;				// 進行度
		int						m_Score;				// スコア
		int						m_Crystal;				// クリスタル獲得数
		int						m_Killed;				// 撃破数
		int						m_Difficulty;			// 難易度
		Date					m_Date;					// 作成日時
		
		StageData				m_StageData[ 5 ];		// 初期ステージデータ
	};

	class ReplayDataBuilder::Impl
	{
	private:
		std::vector < ButtonPushedStatus >		m_ButtonList;			// 過去のボタンの履歴
		ReplayDataInfo							m_ReplayDataInfo;		// リプレイデータの情報
	public:
		Impl();
		~Impl(){}
		void AddButtonState( ButtonPushedStatus status );
		void Save( const std::string& fileName );
		void Cleanup();
		void SetName( const char* pName );
		void SetProgress( int progress );
		void SetScore( int score );
		void SetCrystal( int crystal );
		void SetKilled( int killed );
		void SetDifficulty( int difficulty );
		void SetDate( const Date& date );
	};

	ReplayDataBuilder::Impl::Impl()
	{
		MAPIL::ZeroObject( &m_ReplayDataInfo, sizeof( m_ReplayDataInfo ) );
		m_ButtonList.clear();
	}

	inline void ReplayDataBuilder::Impl::AddButtonState( ButtonPushedStatus status )
	{
		m_ButtonList.push_back( status );
	}
	
	void ReplayDataBuilder::Impl::Save( const std::string& fileName )
	{
		std::vector < char > data;
		data.reserve( 30000 );		// 30,000バイトを予約

		std::ofstream fOut( fileName, std::ios::binary | std::ios::out );
		if( !fOut ){
			return;
		}

		CopyArray( &data, m_ReplayDataInfo.m_Name, sizeof( m_ReplayDataInfo.m_Name ) );
		CopyInt( &data, m_ReplayDataInfo.m_Progress );
		CopyInt( &data, m_ReplayDataInfo.m_Score );
		CopyInt( &data, m_ReplayDataInfo.m_Crystal );
		CopyInt( &data, m_ReplayDataInfo.m_Killed );
		CopyInt( &data, m_ReplayDataInfo.m_Difficulty );
		CopyInt( &data, m_ReplayDataInfo.m_Date.m_Year );
		data.push_back( m_ReplayDataInfo.m_Date.m_Month );
		data.push_back( m_ReplayDataInfo.m_Date.m_Day );
		data.push_back( m_ReplayDataInfo.m_Date.m_Hour );
		data.push_back( m_ReplayDataInfo.m_Date.m_Min );
		data.push_back( m_ReplayDataInfo.m_Date.m_Sec );
		// 各ステージ開始時のデータを保存
		for( int i = 0; i < 5; ++i ){
			ReplayDataInfo::StageData stage = m_ReplayDataInfo.m_StageData[ i ];
			CopyInt( &data, stage.m_IniPosX );
			CopyInt( &data, stage.m_IniPosY );
			CopyInt( &data, stage.m_IniHP );
			CopyInt( &data, stage.m_IniShotPower );
			CopyInt( &data, stage.m_IniScore );
			CopyInt( &data, stage.m_IniKilled );
			CopyInt( &data, stage.m_IniCrystal );
			CopyInt( &data, stage.m_IniCons );
			for( int j = 0; j < 3; ++j ){
				CopyInt( &data, stage.m_IniConsGauge[ j ] );
			}
			for( int j = 0; j < 3; ++j ){
				CopyInt( &data, stage.m_IniConsLevel[ j ] );
			}
			CopyInt( &data, stage.m_FrameTotal );
		}

		// 入力ボタンの個数を保存
		int frame = 0;
		for( int i = 0; i < 5; ++i ){
			frame += m_ReplayDataInfo.m_StageData[ i ].m_FrameTotal;
		}
		CopyInt( &data, m_ButtonList.size() );
		// 入力ボタンの保存
		for( unsigned int i = 0; i < m_ButtonList.size(); ++i ){
			data.push_back( m_ButtonList[ i ] );
		}

		// 圧縮
		char* pBuf = new char [ data.size() * 2 ];
		int compSize = 0;
		MAPIL::LZ lz( 200, 5 );
		lz.Compress( &data[ 0 ], data.size(), &pBuf, data.size() * 2, &compSize );
		// シーザ暗号化
		MAPIL::Caesar caesar( 10 );
		caesar.Encrypt( pBuf, compSize );
		// XOR暗号化
		MAPIL::XOR xor( 60 );
		xor.Encrypt( pBuf, compSize );
		fOut.write( pBuf, compSize );
		fOut.close();
		MAPIL::SafeDeleteArray( pBuf );
	}

	inline void ReplayDataBuilder::Impl::Cleanup()
	{
		MAPIL::ZeroObject( &m_ReplayDataInfo, sizeof( m_ReplayDataInfo ) );
		m_ButtonList.clear();
	}

	inline void ReplayDataBuilder::Impl::SetName( const char* pName )
	{
		memcpy( m_ReplayDataInfo.m_Name, pName, sizeof( char ) * 9 );
		m_ReplayDataInfo.m_Name[ 9 ] = '\0';
	}

	inline void ReplayDataBuilder::Impl::SetProgress( int progress )
	{
		m_ReplayDataInfo.m_Progress = progress;
	}

	inline void ReplayDataBuilder::Impl::SetScore( int score )
	{
		m_ReplayDataInfo.m_Score = score;
	}

	inline void ReplayDataBuilder::Impl::SetCrystal( int crystal )
	{
		m_ReplayDataInfo.m_Crystal = crystal;
	}

	inline void ReplayDataBuilder::Impl::SetKilled( int killed )
	{
		m_ReplayDataInfo.m_Killed = killed;
	}

	inline void ReplayDataBuilder::Impl::SetDifficulty( int difficulty )
	{
		m_ReplayDataInfo.m_Difficulty = difficulty;
	}

	inline void ReplayDataBuilder::Impl::SetDate( const Date& date )
	{
		m_ReplayDataInfo.m_Date = date;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	ReplayDataBuilder::ReplayDataBuilder() : m_pImpl( new ReplayDataBuilder::Impl )
	{
	}

	ReplayDataBuilder::~ReplayDataBuilder()
	{
	}

	void ReplayDataBuilder::AddButtonState( ButtonPushedStatus status )
	{
		m_pImpl->AddButtonState( status );
	}

	void ReplayDataBuilder::Save( const std::string& fileName )
	{
		m_pImpl->Save( fileName );
	}

	void ReplayDataBuilder::Cleanup()
	{
		m_pImpl->Cleanup();
	}

	void ReplayDataBuilder::SetName( const char* pName )
	{
		m_pImpl->SetName( pName );
	}

	void ReplayDataBuilder::SetProgress( int progress )
	{
		m_pImpl->SetProgress( progress );
	}

	void ReplayDataBuilder::SetScore( int score )
	{
		m_pImpl->SetScore( score );
	}

	void ReplayDataBuilder::SetCrystal( int crystal )
	{
		m_pImpl->SetCrystal( crystal );
	}

	void ReplayDataBuilder::SetKilled( int killed )
	{
		m_pImpl->SetKilled( killed );
	}

	void ReplayDataBuilder::SetDifficulty( int difficulty )
	{
		m_pImpl->SetDifficulty( difficulty );
	}

	void ReplayDataBuilder::SetDate( const Date& date )
	{
		m_pImpl->SetDate( date );
	}

}
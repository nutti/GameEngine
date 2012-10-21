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
		std::ofstream fOut( fileName, std::ios::binary | std::ios::out );

		// スコア全体情報を保存
		fOut.write( m_ReplayDataInfo.m_Name, sizeof( m_ReplayDataInfo.m_Name ) );
		WriteInt( &fOut, m_ReplayDataInfo.m_Progress );
		WriteInt( &fOut, m_ReplayDataInfo.m_Score );
		WriteInt( &fOut, m_ReplayDataInfo.m_Crystal );
		WriteInt( &fOut, m_ReplayDataInfo.m_Killed );
		WriteInt( &fOut, m_ReplayDataInfo.m_Difficulty );
		WriteInt( &fOut, m_ReplayDataInfo.m_Date.m_Year );
		fOut.write( &m_ReplayDataInfo.m_Date.m_Month, sizeof( char ) );
		fOut.write( &m_ReplayDataInfo.m_Date.m_Day, sizeof( char ) );
		fOut.write( &m_ReplayDataInfo.m_Date.m_Hour, sizeof( char ) );
		fOut.write( &m_ReplayDataInfo.m_Date.m_Min, sizeof( char ) );
		fOut.write( &m_ReplayDataInfo.m_Date.m_Sec, sizeof( char ) );
		// 各ステージ開始時のデータを保存
		for( int i = 0; i < 5; ++i ){
			ReplayDataInfo::StageData stage = m_ReplayDataInfo.m_StageData[ i ];
			WriteInt( &fOut, stage.m_IniPosX );
			WriteInt( &fOut, stage.m_IniPosY );
			WriteInt( &fOut, stage.m_IniHP );
			WriteInt( &fOut, stage.m_IniShotPower );
			WriteInt( &fOut, stage.m_IniScore );
			WriteInt( &fOut, stage.m_IniKilled );
			WriteInt( &fOut, stage.m_IniCrystal );
			WriteInt( &fOut, stage.m_IniCons );
			for( int j = 0; j < 3; ++j ){
				WriteInt( &fOut, stage.m_IniConsGauge[ j ] );
			}
			for( int j = 0; j < 3; ++j ){
				WriteInt( &fOut, stage.m_IniConsLevel[ j ] );
			}
			WriteInt( &fOut, stage.m_FrameTotal );
		}

		// 入力ボタンの個数を保存
		int frame = 0;
		for( int i = 0; i < 5; ++i ){
			frame += m_ReplayDataInfo.m_StageData[ i ].m_FrameTotal;
		}
		WriteInt( &fOut, m_ButtonList.size() );
		// 入力ボタンの保存
		for( unsigned int i = 0; i < m_ButtonList.size(); ++i ){
			fOut.write( &m_ButtonList[ i ], sizeof( m_ButtonList[ i ] ) );
		}

		fOut.close();
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
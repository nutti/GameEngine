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
	};

	ReplayDataBuilder::Impl::Impl()
	{
		MAPIL::ZeroObject( &m_ReplayDataInfo, sizeof( m_ReplayDataInfo ) );
		m_ButtonList.clear();
	}

	void ReplayDataBuilder::Impl::AddButtonState( ButtonPushedStatus status )
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
		WriteInt( &fOut, m_ReplayDataInfo.m_Date.m_Month );
		WriteInt( &fOut, m_ReplayDataInfo.m_Date.m_Day );
		WriteInt( &fOut, m_ReplayDataInfo.m_Date.m_Hour );
		WriteInt( &fOut, m_ReplayDataInfo.m_Date.m_Min );
		WriteInt( &fOut, m_ReplayDataInfo.m_Date.m_Sec );
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

	void ReplayDataBuilder::Impl::Cleanup()
	{
		MAPIL::ZeroObject( &m_ReplayDataInfo, sizeof( m_ReplayDataInfo ) );
		m_ButtonList.clear();
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

}
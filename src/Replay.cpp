#include <MAPIL/MAPIL.h>

#include "Replay.h"

#include "ResourceID.h"

#include "Util.h"

namespace GameEngine
{
	enum ReplaySelectState
	{
		REPLAY_SELECT_NO		= 0,		// リプレイ番号を選択中
		REPLAY_SELECT_STAGE		= 1,		// リプレイするステージを選択中
	};

	class Replay::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;
		ResourceMap					m_ResourceMap;
		int							m_Counter;
		DisplayedReplayInfo			m_DisplayedReplayInfo;
		int							m_SelectedReplayNo;		// 選択されたリプレイ番号
		int							m_SelectedStage;		// リプレイするステージ
		int							m_CurSelectState;		// 現在の選択状態
	public:
		Impl();
		~Impl(){}
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachDisplayedReplayInfo( const DisplayedReplayInfo& info );
		int GetReplayStage() const;
		int GetReplayNo() const;
	};

	Replay::Impl::Impl()
	{
		m_Counter = 0;
		m_SelectedReplayNo = 0;
		m_CurSelectState = REPLAY_SELECT_NO;
		m_SelectedStage = STAGE_ID_STAGE_1;
	}

	SceneType Replay::Impl::Update()
	{
		if( m_Counter < 20 ){
			++m_Counter;
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( m_CurSelectState == REPLAY_SELECT_NO ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
				if( m_DisplayedReplayInfo.m_Entries[ m_SelectedReplayNo ].m_Progress != -1 ){
					m_CurSelectState = REPLAY_SELECT_STAGE;
				}
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
				return SCENE_TYPE_MENU;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
				++m_SelectedReplayNo;
				if( m_SelectedReplayNo > 14 ){
					m_SelectedReplayNo = 0;
				}
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				--m_SelectedReplayNo;
				if( m_SelectedReplayNo < 0 ){
					m_SelectedReplayNo = 14;
				}
			}
		}
		else if( m_CurSelectState == REPLAY_SELECT_STAGE ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
				if( m_DisplayedReplayInfo.m_Entries[ m_SelectedReplayNo ].m_Progress >= m_SelectedStage ){
					return SCENE_TYPE_STAGE;
				}
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
				m_CurSelectState = REPLAY_SELECT_NO;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
				++m_SelectedStage;
				if( m_SelectedStage > STAGE_ID_STAGE_5 ){
					m_SelectedStage = STAGE_ID_STAGE_1;
				}
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				--m_SelectedStage;
				if( m_SelectedStage < STAGE_ID_STAGE_1 ){
					m_SelectedStage = STAGE_ID_STAGE_5;
				}
			}
		}

		++m_Counter;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void Replay::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();

		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
							0.0f, 0.0f, 640.0f, 480.0f, 0.0f, false, 0xFF999999 );

		char* difStr[] = { "easy", "normal", "hard", "hazard" };
		char* progStr[] = { "", "stage 1", "stage 2", "stage 3", "stage 4", "stage 5", "all clear" };

		

		// 縦にスクロールした場合
		if( m_Counter < 20 ){
			DrawFontString( m_ResourceMap, 30.0f, 505.0f - m_Counter * 21.0f, 0.5f, 0xFFAAFFAA, "rank" );
			DrawFontString( m_ResourceMap, 100.0f, 505.0f - m_Counter * 21.0f, 0.5f, 0xFFAAFFAA, "name" );
			DrawFontString( m_ResourceMap, 200.0f, 505.0f - m_Counter * 21.0f, 0.5f, 0xFFAAFFAA, "progress" );
			DrawFontString( m_ResourceMap, 350.0f, 505.0f - m_Counter * 21.0f, 0.5f, 0xFFAAFFAA, "score" );
			DrawFontString( m_ResourceMap, 500.0f, 505.0f - m_Counter * 21.0f, 0.5f, 0xFFAAFFAA, "date" );
			for( int i = 0; i < 15; ++i ){
				if( m_DisplayedReplayInfo.m_Entries[ i ].m_Progress != -1 ){
					DrawFontString( m_ResourceMap, 30.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", i + 1 );
					DrawFontString( m_ResourceMap, 100.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f, 0.45f, m_DisplayedReplayInfo.m_Entries[ i ].m_Name );
					DrawFontString( m_ResourceMap, 200.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f, 0.45f, progStr[ m_DisplayedReplayInfo.m_Entries[ i ].m_Progress ] );
					DrawFontString( m_ResourceMap, 350.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", m_DisplayedReplayInfo.m_Entries[ i ].m_Score );
					DrawFontString( m_ResourceMap, 500.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f, 0.45f, "%04d%02d%02d%02d%02d",
									m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Year,
									m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Month,
									m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Day,
									m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Hour,
									m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Min );
				}
			}
		}
		// 通常の表示
		else{
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								0.0f, 99.0f + m_SelectedReplayNo * 17.0f, 50.0f, 1.0f, false, 0xDD5577FF );
			DrawFontString( m_ResourceMap, 30.0f, 85.0f, 0.5f, 0xFFAAFFAA, "rank" );
			DrawFontString( m_ResourceMap, 100.0f, 85.0f, 0.5f, 0xFFAAFFAA, "name" );
			DrawFontString( m_ResourceMap, 200.0f, 85.0f, 0.5f, 0xFFAAFFAA, "progress" );
			DrawFontString( m_ResourceMap, 350.0f, 85.0f, 0.5f, 0xFFAAFFAA, "score" );
			DrawFontString( m_ResourceMap, 500.0f, 85.0f, 0.5f, 0xFFAAFFAA, "date" );
			for( int i = 0; i < 15; ++i ){
				if( m_DisplayedReplayInfo.m_Entries[ i ].m_Progress != -1 ){
					DrawFontString( m_ResourceMap, 30.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", i + 1 );
					DrawFontString( m_ResourceMap, 100.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, m_DisplayedReplayInfo.m_Entries[ i ].m_Name );
					DrawFontString( m_ResourceMap, 200.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, progStr[ m_DisplayedReplayInfo.m_Entries[ i ].m_Progress ] );
					DrawFontString( m_ResourceMap, 350.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", m_DisplayedReplayInfo.m_Entries[ i ].m_Score );
					DrawFontString( m_ResourceMap, 500.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%04d%02d%02d%02d%02d",
									m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Year,
									m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Month,
									m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Day,
									m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Hour,
									m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Min );
				}
			}
			DrawFontString( m_ResourceMap, 30.0f, 380.0f, 0.5f, 0xFFAAFFAA, "killed" );
			DrawFontString( m_ResourceMap, 30.0f, 400.0f, 0.5f, 0xFFAAFFAA, "crystal" );
			DrawFontString( m_ResourceMap, 160.0f, 380.0f, 0.45f,
							"%d", m_DisplayedReplayInfo.m_Entries[ m_SelectedReplayNo ].m_Killed );
			DrawFontString( m_ResourceMap, 160.0f, 400.0f, 0.45f,
							"%d", m_DisplayedReplayInfo.m_Entries[ m_SelectedReplayNo ].m_Crystal );
		}

		if( m_CurSelectState == REPLAY_SELECT_STAGE ){
			for( int i = 0; i < 5; ++i ){
				std::string str = "stage";
				str += i + '1';
				if( m_SelectedStage == ( STAGE_ID_STAGE_1 + i ) ){
					DrawFontString( m_ResourceMap, 300.0f, 380.0f + i * 17.0f, 0.5f, 0xFFAAFFAA, str.c_str() );
				}
				else if( m_DisplayedReplayInfo.m_Entries[ m_SelectedReplayNo ].m_Progress < i ){
					DrawFontString( m_ResourceMap, 300.0f, 380.0f + i * 17.0f, 0.5f, 0xFFAAAAAA, str.c_str() );
				}
				else{
					DrawFontString( m_ResourceMap, 300.0f, 380.0f + i * 17.0f, 0.5f, 0xFFFFFFFF, str.c_str() );
				}
			}
		}


		MAPIL::EndRendering2DGraphics();
	}

	void Replay::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_ButtonStatus = *pHolder;
	}

	void Replay::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	void Replay::Impl::AttachDisplayedReplayInfo( const DisplayedReplayInfo& info )
	{
		m_DisplayedReplayInfo = info;
	}

	int Replay::Impl::GetReplayStage() const
	{
		return m_SelectedStage;
	}

	int Replay::Impl::GetReplayNo() const
	{
		return m_SelectedReplayNo;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	Replay::Replay() : Scene(), m_pImpl( new Replay::Impl )
	{
	}

	Replay::~Replay()
	{
	}

	void Replay::Init()
	{
	}

	SceneType Replay::Update()
	{
		return m_pImpl->Update();
	}

	void Replay::Draw()
	{
		m_pImpl->Draw();
	}

	void Replay::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_pImpl->AttachButtonState( pHolder );
	}

	void Replay::AttachResourceMap( const ResourceMap& map )
	{
		m_pImpl->AttachResourceMap( map );
	}

	void Replay::AttachDisplayedReplayInfo( const DisplayedReplayInfo& info )
	{
		m_pImpl->AttachDisplayedReplayInfo( info );
	}

	int Replay::GetReplayStage() const
	{
		return m_pImpl->GetReplayStage();
	}

	int Replay::GetReplayNo() const
	{
		return m_pImpl->GetReplayNo();
	}
}
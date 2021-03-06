#include <MAPIL/MAPIL.h>

#include "Replay.h"

#include "SceneManager/Replay/ReplayView.h"

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
		int							m_PrepareCounter;
		int							m_SelectCounter;		// 選択時の準備用カウンタ
		int							m_StageSelectCounter;	// ステージ選択時のカウンタ
		int							m_ReturnCounter;		// メニュー画面へ移行カウンタ
		int							m_WaitTimer;			// 待機時間

		std::list < std::shared_ptr < ReplayView > >		m_ViewList;
	public:
		Impl();
		~Impl();
		void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachDisplayedReplayInfo( const DisplayedReplayInfo& info );
		int GetReplayStage() const;
		int GetReplayNo() const;
		void Reflesh();
	};

	Replay::Impl::Impl()
	{
		m_Counter = 0;
		m_SelectedReplayNo = 0;
		m_CurSelectState = REPLAY_SELECT_NO;
		m_SelectedStage = STAGE_ID_STAGE_1;
		m_PrepareCounter = 0;
		m_SelectCounter = 0;
		m_StageSelectCounter = 0;
		m_ReturnCounter = 0;
		m_WaitTimer = 0;

		m_ViewList.clear();
	}

	Replay::Impl::~Impl()
	{
		m_ViewList.clear();
	}

	void Replay::Impl::Init()
	{
		// 背景エフェクトの追加
		ReplayBackgroundView* pReplayBGView = new ReplayBackgroundView();
		pReplayBGView->AttachResourceMap( m_ResourceMap );
		pReplayBGView->Init();
		m_ViewList.push_back( std::shared_ptr < ReplayView > ( pReplayBGView ) );

		// タイトル画像の追加
		ReplayTitleView* pTitleView = new ReplayTitleView();
		pTitleView->AttachResourceMap( m_ResourceMap );
		pTitleView->Init();
		m_ViewList.push_back( std::shared_ptr < ReplayTitleView > ( pTitleView ) );

		// ステージの追加
		ReplayStageView* pStageView = new ReplayStageView();
		pStageView->AttachResourceMap( m_ResourceMap );
		pStageView->AttachDisplayedReplayInfo( &m_DisplayedReplayInfo );
		m_ViewList.push_back( std::shared_ptr < ReplayStageView > ( pStageView ) );
	}

	void Replay::Impl::Reflesh()
	{
		std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < ReplayView > view ){ view->Init(); } );
		m_Counter = 0;
		m_SelectCounter = 0;
		m_PrepareCounter = 0;
		m_StageSelectCounter = 0;
		m_ReturnCounter = 0;
		m_WaitTimer = 0;
	}

	SceneType Replay::Impl::Update()
	{
		std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < ReplayView > view ){ view->Update(); } );

		if( m_Counter < 60 ){
			++m_Counter;
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( m_PrepareCounter > 0 ){
			++m_PrepareCounter;
			if( m_PrepareCounter == 20 ){
				return SCENE_TYPE_STAGE;
			}
			else{
				return SCENE_TYPE_NOT_CHANGE;
			}
		}

		if( m_SelectCounter > 0 ){
			--m_SelectCounter;
		}

		if( m_StageSelectCounter > 0 ){
			--m_StageSelectCounter;
		}

		if( m_ReturnCounter > 0 ){
			if( m_ReturnCounter < 15 ){
				++m_ReturnCounter;
			}
			if( m_ReturnCounter == 15 ){
				return SCENE_TYPE_MENU;
			}
			else{
				return SCENE_TYPE_NOT_CHANGE;
			}
		}

		if( m_WaitTimer > 0 ){
			--m_WaitTimer;
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( m_CurSelectState == REPLAY_SELECT_NO ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
				if( m_DisplayedReplayInfo.m_Entries[ m_SelectedReplayNo ].m_Progress != -1 ){
					m_WaitTimer = 20;
					if( m_SelectedStage > m_DisplayedReplayInfo.m_Entries[ m_SelectedReplayNo ].m_Progress - 1 ){
						m_SelectedStage = STAGE_ID_STAGE_1;
					}
					m_CurSelectState = REPLAY_SELECT_STAGE;
					std::for_each( m_ViewList.begin(), m_ViewList.end(), [this]( std::shared_ptr < ReplayView > view ){ view->SelectReplay( m_SelectedReplayNo, m_SelectedStage ); } );
				}
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
				std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < ReplayView > view ){ view->BackToMenu(); } );
				m_ReturnCounter = 1;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
				++m_SelectedReplayNo;
				if( m_SelectedReplayNo > MAX_REPLAY_ENTRY - 1 ){
					m_SelectedReplayNo = 0;
				}
				m_SelectCounter = 10;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				--m_SelectedReplayNo;
				if( m_SelectedReplayNo < 0 ){
					m_SelectedReplayNo = MAX_REPLAY_ENTRY - 1;
				}
				m_SelectCounter = 10;
			}
		}
		else if( m_CurSelectState == REPLAY_SELECT_STAGE ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
				if( m_DisplayedReplayInfo.m_Entries[ m_SelectedReplayNo ].m_Progress >= m_SelectedStage + 1 ){
					MAPIL::StopStreamingBuffer( GLOBAL_RESOURCE_BGM_ID_MENU );
					m_PrepareCounter = 1;
					std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < ReplayView > view ){ view->Finalize(); } );
				}
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
				m_CurSelectState = REPLAY_SELECT_NO;
				m_WaitTimer = 20;
				std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < ReplayView > view ){ view->CancelStage(); } );
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
				++m_SelectedStage;
				if( m_SelectedStage > m_DisplayedReplayInfo.m_Entries[ m_SelectedReplayNo ].m_Progress - 1 ){
					m_SelectedStage = STAGE_ID_STAGE_1;
				}
				m_StageSelectCounter = 10;
				std::for_each( m_ViewList.begin(), m_ViewList.end(), [this]( std::shared_ptr < ReplayView > view ){ view->ChangeStage( m_SelectedStage ); } );
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				--m_SelectedStage;
				if( m_SelectedStage < STAGE_ID_STAGE_1 ){
					m_SelectedStage = m_DisplayedReplayInfo.m_Entries[ m_SelectedReplayNo ].m_Progress - 1;
				}
				m_StageSelectCounter = 10;
				std::for_each( m_ViewList.begin(), m_ViewList.end(), [this]( std::shared_ptr < ReplayView > view ){ view->ChangeStage( m_SelectedStage ); } );
			}
		}

		++m_Counter;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void Replay::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();

		// 背景画像
		int weight;
		if( m_Counter < 20 ){
			weight = ( m_Counter * 6 ) & 0xFF;
		}
		else{
			if( m_PrepareCounter == 0 ){
				weight = 0x78;		// 120
			}
			else if( m_PrepareCounter > 0 && m_PrepareCounter <= 20 ){
				weight = 0x78 + 10 + m_PrepareCounter * 6;
			}
			else{
				weight = 0xFF;
			}
			if( m_ReturnCounter > 0 ){
				weight = ( 15 - m_ReturnCounter ) * 8;
			}
		}
		int color = weight << 16 | weight << 8 | weight;
		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
							0.0f, 0.0f, false, 0xFF << 24 | color );

		std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < ReplayView > view ){ view->Draw(); } );


		// リプレイデータ一覧表示
		char* rankStr[] = { "calm", "easy", "normal", "hard", "hazard" };
		char* progStr[] = { "", "st 1", "st 2", "st 3", "st 4", "st 5", "all" };
		int alpha = 0;
		if( m_Counter >= 20 && m_Counter < 60 ){
			alpha = ( ( m_Counter - 20 ) * 6 + 10 ) & 0xFF;
		}
		else if( m_Counter >= 60 ){
			if( m_PrepareCounter == 0 ){
				alpha = 0xFF;
			}
			else if( m_PrepareCounter > 0 && m_PrepareCounter <= 20 ){
				alpha = ( 0xFF * ( 20 - m_PrepareCounter ) ) / 20;
			}
			else{
				alpha = 0x00;
			}
			if( m_ReturnCounter > 0 ){
				alpha = ( 15 - m_ReturnCounter ) * 17;
			}
		}
		int color1 = alpha << 24 | 0xAAFFAA;
		int color2 = alpha << 24 | 0xAAAAAA;
		int selColor = alpha << 24 | 0xFFFFFF;

		float titleFont = 1.0f;
		float itemFont = 1.0f;

		enum ItemID
		{
			ITEM_ID_NO		= 0,
			ITEM_ID_NAME	= 1,
			ITEM_ID_RANK	= 2,
			ITEM_ID_PROG	= 3,
			ITEM_ID_SCORE	= 4,
			ITEM_ID_DATE	= 5,
			ITEM_ID_TOTAL,
		};

		float startX[ ITEM_ID_TOTAL ];
		startX[ ITEM_ID_NO ] = 30.0f;
		startX[ ITEM_ID_NAME ] = startX[ ITEM_ID_NO ] + 40.0f;
		startX[ ITEM_ID_RANK ] = startX[ ITEM_ID_NAME ] + 100.0f;
		startX[ ITEM_ID_PROG ] = startX[ ITEM_ID_RANK ] + 100.0f;
		startX[ ITEM_ID_SCORE ] = startX[ ITEM_ID_PROG ] + 70.0f;
		startX[ ITEM_ID_DATE ] = startX[ ITEM_ID_SCORE ] + 110.0f;

		DrawFontString( m_ResourceMap, startX[ ITEM_ID_NO ], 85.0f, titleFont, color1, "no" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_NAME ], 85.0f, titleFont, color1, "name" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_RANK ], 85.0f, titleFont, color1, "rank" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_PROG ], 85.0f, titleFont, color1, "prog" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_SCORE ], 85.0f, titleFont, color1, "score" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_DATE ], 85.0f, titleFont, color1, "date" );
		for( int i = 0; i < MAX_REPLAY_ENTRY; ++i ){
			if( m_DisplayedReplayInfo.m_Entries[ i ].m_Progress != -1 ){
				int c = color2;
				float offsetX = 0.0f;
				float offsetY = 0.0f;
				if( i == m_SelectedReplayNo ){
					if( m_SelectCounter > 0 ){
						offsetX = -2.0f * sin( MAPIL::DegToRad( m_SelectCounter * 45.0 ) );
					}
					else{
						offsetX = -2.0f;
					}
					offsetY = 0.0f;
					c = selColor;
				}

				DrawFontString( m_ResourceMap, startX[ ITEM_ID_NO ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, "%d", i + 1 );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_NAME ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, m_DisplayedReplayInfo.m_Entries[ i ].m_Name );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_RANK ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, rankStr[ m_DisplayedReplayInfo.m_Entries[ i ].m_Difficulty ] );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_PROG ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, progStr[ m_DisplayedReplayInfo.m_Entries[ i ].m_Progress ] );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_SCORE ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, "%d", m_DisplayedReplayInfo.m_Entries[ i ].m_Score );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_DATE ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, "%02d/%02d/%02d %02d:%02d",
								m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Year % 100,
								m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Month,
								m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Day,
								m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Hour,
								m_DisplayedReplayInfo.m_Entries[ i ].m_Date.m_Min );
			}
		}
		DrawFontString( m_ResourceMap, 30.0f, 380.0f, titleFont, color1, "killed" );
		DrawFontString( m_ResourceMap, 30.0f, 400.0f, titleFont, color1, "crystal" );


		// 詳細情報の表示
		int color3 = 0;
		if( m_Counter > 60 ){
			if( m_PrepareCounter == 0 ){
				if( m_SelectCounter > 0 ){
					color3 = ( ( 10 - m_SelectCounter ) * 25 ) << 24 | 0xFFFFFF;
				}
				else{
					color3 = 0xFFFFFFFF;
				}
			}
			else{
				color3 = selColor;
			}
			if( m_ReturnCounter > 0 ){
				color3 = ( ( 15 - m_ReturnCounter ) * 17 ) << 24 | 0xFFFFFF;
			}
		}
		else if( m_Counter >= 20 && m_Counter <= 60 ){
			color3 = ( ( m_Counter- 20 ) * 6 ) << 24 | 0xFFFFFF;
		}

		DrawFontString( m_ResourceMap, 160.0f, 380.0f, itemFont, color3,
						"%d", m_DisplayedReplayInfo.m_Entries[ m_SelectedReplayNo ].m_Killed );
		DrawFontString( m_ResourceMap, 160.0f, 400.0f, itemFont, color3,
						"%d", m_DisplayedReplayInfo.m_Entries[ m_SelectedReplayNo ].m_Crystal );

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

	void Replay::InitImpl()
	{
		m_pImpl->Init();
	}

	void Replay::Reflesh()
	{
		m_pImpl->Reflesh();
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
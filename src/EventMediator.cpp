#include "EventMediator.h"
#include "EventTypes.h"

#include "SceneManager.h"
#include "SceneTypes.h"

#include "GeneralButtonManager.h"

#include "GameStateManager.h"

#include "ResourceManager.h"
#include "ResourceID.h"

#include "ScriptManager.h"

namespace GameEngine
{
	// EventMediator実装クラス
	class EventMediator::Impl
	{
	private:
		std::shared_ptr < SceneManager >			m_pSceneManager;
		std::shared_ptr < GeneralButtonManager >	m_pButtonManager;
		std::shared_ptr < GameStateManager >		m_pGameStateManager;
		std::shared_ptr < ResourceManager >			m_pResourceManager;
		std::shared_ptr < ScriptManager >			m_pScriptManager;

		bool m_HasTermSig;

		void FrameUpdate();
	public:
		Impl();
		~Impl(){}
		void SendEvent( int type );
		void SendEvent( int type, void* pArg );
		void SetSceneManager( std::shared_ptr < SceneManager > pSceneManager );
		void SetButtonManager( std::shared_ptr < GeneralButtonManager > pButtonManager );
		void SetGameStateManger( std::shared_ptr < GameStateManager > pGameStateManager );
		void SetResourceManager( std::shared_ptr < ResourceManager > pResourceManager );
		void SetScriptManager( std::shared_ptr < ScriptManager > pScriptManager );
		bool HasTermSig() const;
	};

	EventMediator::Impl::Impl() : m_HasTermSig( false )
	{
	}

	void EventMediator::Impl::FrameUpdate()
	{
		// ゲームの状態を取得
		GameDataMsg msg;
		//if( m_pSceneManager->GetCurSceneType() == SCENE_TYPE_STAGE ){
		//	msg = m_pGameStateManager->GetGameData();
		//	m_pSceneManager->AttachGameData( msg );
		//}

		// ボタンの取得
		ButtonStatusHolder holder;
		m_pButtonManager->Update();
		m_pButtonManager->GetButtonStatus( &holder );
		m_pSceneManager->AttachButtonState( &holder );

		// 画面の更新
		m_pSceneManager->Update();
		m_pSceneManager->Draw();

		// ゲームデータの更新
		//if( m_pSceneManager->GetCurSceneType() == SCENE_TYPE_STAGE ){
		//	msg = m_pSceneManager->GetFrameScoreData();
		//	m_pGameStateManager->AddGameData( msg );
		//	m_pGameStateManager->UpdateGameData();
		//}

		// 時間の表示（暫定版）
		m_pGameStateManager->UpdatePlayTime();
		int time = m_pGameStateManager->GetPlayTime();
		MAPIL::BeginRendering2DGraphics();
		MAPIL::DrawString( 10.0f, 10.0f, "%d : %d : %d", time / 3600, ( time / 60 ) % 60, time % 60 );
		MAPIL::EndRendering2DGraphics();
	}

	void EventMediator::Impl::SendEvent( int type )
	{
		switch( type ){
			// 初期化要求
			case EVENT_TYPE_INITIALIZE:{
				m_pGameStateManager->StartGameDataRecording();
				m_pSceneManager->ChangeScene( SCENE_TYPE_INITIALIZE );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				m_pScriptManager->BuildFileStructure( "archive/script/build.isc" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_SE, GLOBAL_RESOURCE_ID_ITEM_1_SE, "archive/resource/se/item01.wav" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_SE, GLOBAL_RESOURCE_ID_ITEM_2_SE, "archive/resource/se/item02.wav" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_SE, GLOBAL_RESOURCE_ID_SHOT_SE, "archive/resource/se/shot.wav" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_HP_BAR_TEXTURE, "archive/resource/texture/hpbar.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_CONS_BAR_TEXTURE, "archive/resource/texture/consbar.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_BACKGROUND_TEXTURE, "archive/resource/texture/background.png" );
				m_pResourceManager->LoadGlobalResource(	RESOURCE_TYPE_MULTI_TEXTURE,
														GLOBAL_RESOURCE_ID_FONT_TEXTURE_FIRST,
														GLOBAL_RESOURCE_ID_FONT_TEXTURE_LAST,
														5, 8, 32, 32, "archive/resource/texture/game_font.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_POWER_UP_ITEM_1_TEXTURE, "archive/resource/texture/powerup_item_1.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_POWER_UP_ITEM_2_TEXTURE, "archive/resource/texture/powerup_item_2.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE, "archive/resource/texture/crystal.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_PLAYER_SHOT_1_TEXTURE, "archive/resource/texture/player_shot_1.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_PLAYER_SHOT_COLLIDED_TEXTURE, "archive/resource/texture/player_shot_collided_effect.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_ENEMY_DESTROYED_TEXTURE, "archive/resource/texture/enemy_destroyed.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_SE, GLOBAL_RESOURCE_ID_ENEMY_DESTROYED_SE, "archive/resource/se/rtg_se2.wav" );
				m_pResourceManager->LoadGlobalResource(	RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_PLAYER_SHOT_GREEN_MAIN,
														"archive/resource/texture/player_shot_2.png" );
				m_pResourceManager->LoadGlobalResource(	RESOURCE_TYPE_SE,
														GLOBAL_RESOURCE_SE_ID_PLAYER_CHANGE_MODE,
														"archive/resource/se/change_mode.wav" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_BAR,
														"archive/resource/texture/bar.png" );
				m_pResourceManager->LoadGlobalResource(	RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_CONS_RED_SYMBOL,
														"archive/resource/texture/red_cons_2_1.png" );
				m_pResourceManager->LoadGlobalResource(	RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_CONS_GREEN_SYMBOL,
														"archive/resource/texture/green_cons.png" );
				m_pResourceManager->LoadGlobalResource(	RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_CONS_BLUE_SYMBOL,
														"archive/resource/texture/blue_cons.png" );
				m_pResourceManager->LoadGlobalResource(	RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_PLAYER_SHOT_BLUE_MAIN,
														"archive/resource/texture/player_shot_3.png" );
				m_pResourceManager->LoadGlobalResource(	RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_PLAYER_SHOT_RED_MAIN,
														"archive/resource/texture/player_shot_4.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_SE,
														GLOBAL_RESOURCE_SE_ID_PLAYER_DAMAGED,
														"archive/resource/se/player_damaged.wav" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_SE,
														GLOBAL_RESOURCE_SE_ID_PLAYER_DESTROYED,
														"archive/resource/se/player_destroyed.wav" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_MENU_GAME_START,
														"archive/resource/texture/game_start.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_MENU_GAME_START_SELECTED,
														"archive/resource/texture/game_start_selected.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_MENU_EXIT,
														"archive/resource/texture/exit.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_MENU_EXIT_SELECTED,
														"archive/resource/texture/exit_selected.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_MENU_SCORE,
														"archive/resource/texture/score.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_MENU_SCORE_SELECTED,
														"archive/resource/texture/score_selected.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_MENU_REPLAY,
														"archive/resource/texture/replay.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_MENU_REPLAY_SELECTED,
														"archive/resource/texture/replay_selected.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_MENU_CONFIG,
														"archive/resource/texture/config.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_MENU_CONFIG_SELECTED,
														"archive/resource/texture/config_selected.png" );

				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_HI_SCORE,
														"archive/resource/texture/hi_score.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_GAME_SCORE,
														"archive/resource/texture/game_score.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_GAME_KILLED,
														"archive/resource/texture/game_killed.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_GAME_CRYSTAL,
														"archive/resource/texture/game_crystal.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_GAME_POWER,
														"archive/resource/texture/game_power.png" );
				m_pResourceManager->LoadGlobalResource( RESOURCE_TYPE_TEXTURE,
														GLOBAL_RESOURCE_TEXTURE_ID_GAME_HP,
														"archive/resource/texture/game_hp.png" );

				DisplayedSaveData data;
				for( int i = 0; i < 4; ++i ){
					for( int j = 0; j < 25; ++j ){
						data.m_Difficulty[ i ].m_Record[ j ] = m_pGameStateManager->GetRecord( i, j );
					}
					data.m_Difficulty[ i ].m_PlayTime = m_pGameStateManager->GetPlayTime( i );
					data.m_Difficulty[ i ].m_AllClear = m_pGameStateManager->GetAllClearCount( i );
				}
				data.m_PlayTime = m_pGameStateManager->GetPlayTime();
				m_pSceneManager->AttachDisplayedSaveData( data );
				break;
			}
			// メニュー画面移行要求
			case EVENT_TYPE_MOVE_TO_MENU:{
				m_pResourceManager->ReleaseStageResources();
				ResourceMap rcMap = m_pResourceManager->GetStageResourceMap();
				m_pSceneManager->AttachSceneResourceMap( rcMap );
				m_pSceneManager->ChangeScene( SCENE_TYPE_MENU );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}
			// スコア画面移行要求
			case EVENT_TYPE_MOVE_TO_SCORE:{
				m_pResourceManager->ReleaseStageResources();
				ResourceMap rcMap = m_pResourceManager->GetStageResourceMap();
				m_pSceneManager->AttachSceneResourceMap( rcMap );
				m_pSceneManager->ChangeScene( SCENE_TYPE_SCORE );
				//DisplayedSaveData data;
				//for( int i = 0; i < 4; ++i ){
				//	for( int j = 0; j < 25; ++j ){
				//		data.m_Difficulty[ i ].m_Record[ j ] = m_pGameStateManager->GetRecord( i, j );
				//	}
				//	data.m_Difficulty[ i ].m_PlayTime = m_pGameStateManager->GetPlayTime( i );
				//	data.m_Difficulty[ i ].m_AllClear = m_pGameStateManager->GetAllClearCount( i );
				//}
				//data.m_PlayTime = m_pGameStateManager->GetPlayTime();
				//m_pSceneManager->AttachDisplayedSaveData( data );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}
			case EVENT_TYPE_MOVE_TO_SCORE_ENTRY:{
				m_pResourceManager->ReleaseStageResources();
				ResourceMap rcMap = m_pResourceManager->GetStageResourceMap();
				m_pSceneManager->AttachSceneResourceMap( rcMap );
				m_pSceneManager->ChangeScene( SCENE_TYPE_SCORE_ENTRY );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
			}
			// フレーム更新要求
			case EVENT_TYPE_FRAME_UPDATE:
				FrameUpdate();
				break;
			// ゲーム終了要求
			case EVENT_TYPE_GAME_TERM:
				m_pGameStateManager->EndGameDataRecording();
				m_HasTermSig = true;
				break;
			default:
				break;
		}
	}

	void EventMediator::Impl::SendEvent( int type, void* pArg )
	{
		switch( type ){
			// ステージ変更要求
			case EVENT_TYPE_MOVE_TO_STAGE:{
				m_pResourceManager->ReleaseStageResources();
				int stage = *( static_cast < int* > ( pArg ) );
				m_pScriptManager->BuildScriptData( stage );
				ScriptData data = m_pScriptManager->GetScriptData();
				m_pSceneManager->AttachScriptData( data );
				m_pResourceManager->LoadStageResources( data );
				ResourceMap rcMap = m_pResourceManager->GetStageResourceMap();
				m_pSceneManager->AttachSceneResourceMap( rcMap );
				//m_pGameStateManager->StartGameDataRecording();
				m_pSceneManager->ChangeScene( SCENE_TYPE_STAGE );
				break;
			}
			default:
				break;
		}
	}

	void EventMediator::Impl::SetSceneManager( std::shared_ptr < SceneManager > pSceneManager )
	{
		m_pSceneManager = pSceneManager;
	}

	void EventMediator::Impl::SetButtonManager( std::shared_ptr < GeneralButtonManager > pButtonManager )
	{
		m_pButtonManager = pButtonManager;
	}

	void EventMediator::Impl::SetGameStateManger( std::shared_ptr < GameStateManager > pGameStateManager )
	{
		m_pGameStateManager = pGameStateManager;
	}

	void EventMediator::Impl::SetResourceManager( std::shared_ptr < ResourceManager > pResourceManager )
	{
		m_pResourceManager = pResourceManager;
	}

	void EventMediator::Impl::SetScriptManager( std::shared_ptr < ScriptManager > pScriptManager )
	{
		m_pScriptManager = pScriptManager;
	}

	bool EventMediator::Impl::HasTermSig() const
	{
		return m_HasTermSig;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	EventMediator::EventMediator() : m_pImpl( new EventMediator::Impl )
	{
	}

	EventMediator::~EventMediator()
	{
	}

	void EventMediator::SendEvent( int type )
	{
		m_pImpl->SendEvent( type );
	}

	void EventMediator::SendEvent( int type, void* pArg )
	{
		m_pImpl->SendEvent( type, pArg );
	}

	void EventMediator::SetSceneManager( std::shared_ptr < SceneManager > pSceneManager )
	{
		m_pImpl->SetSceneManager( pSceneManager );
	}

	void EventMediator::SetButtonManager( std::shared_ptr < GeneralButtonManager > pButtonManager )
	{
		m_pImpl->SetButtonManager( pButtonManager );
	}

	void EventMediator::SetGameStateManager( std::shared_ptr < GameStateManager > pGameStateManager )
	{
		m_pImpl->SetGameStateManger( pGameStateManager );
	}

	void EventMediator::SetResourceManager( std::shared_ptr < ResourceManager > pResourceManager )
	{
		m_pImpl->SetResourceManager( pResourceManager );
	}

	void EventMediator::SetScriptManager( std::shared_ptr < ScriptManager > pScriptManager )
	{
		m_pImpl->SetScriptManager( pScriptManager );
	}

	bool EventMediator::HasTermSig() const
	{
		return m_pImpl->HasTermSig();
	}
}
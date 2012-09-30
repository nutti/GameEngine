#include "EventMediator.h"
#include "EventTypes.h"

#include "SceneManager.h"
#include "SceneTypes.h"

#include "GeneralButtonManager.h"

#include "GameStateManager.h"

#include "ResourceManager.h"

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
	};

	EventMediator::Impl::Impl()
	{
	}

	void EventMediator::Impl::FrameUpdate()
	{
		// ゲームの状態を取得
		GameDataMsg msg;
		if( m_pSceneManager->GetCurSceneType() == SCENE_TYPE_STAGE ){
			msg = m_pGameStateManager->GetGameData();
			m_pSceneManager->AttachGameData( msg );
		}

		// ボタンの取得
		ButtonStatusHolder holder;
		m_pButtonManager->Update();
		m_pButtonManager->GetButtonStatus( &holder );
		m_pSceneManager->AttachButtonState( &holder );

		// 画面の更新
		m_pSceneManager->Update();
		m_pSceneManager->Draw();

		// ゲームデータの更新
		if( m_pSceneManager->GetCurSceneType() == SCENE_TYPE_STAGE ){
			msg = m_pSceneManager->GetFrameScoreData();
			m_pGameStateManager->AddGameData( msg );
			m_pGameStateManager->UpdateGameData();
		}
		
	}

	void EventMediator::Impl::SendEvent( int type )
	{
		switch( type ){
			// 初期化要求
			case EVENT_TYPE_INITIALIZE:
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
				break;
			// メニュー画面移行要求
			case EVENT_TYPE_MOVE_TO_MENU:
				m_pSceneManager->ChangeScene( SCENE_TYPE_MENU );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			// フレーム更新要求
			case EVENT_TYPE_FRAME_UPDATE:
				FrameUpdate();
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
				int stage = *( static_cast < int* > ( pArg ) );
				m_pScriptManager->BuildScriptData( stage );
				ScriptData data = m_pScriptManager->GetScriptData();
				m_pSceneManager->AttachScriptData( data );
				m_pResourceManager->LoadStageResources( data );
				ResourceMap rcMap = m_pResourceManager->GetStageResourceMap();
				m_pSceneManager->AttachSceneResourceMap( rcMap );
				m_pGameStateManager->StartGameDataRecording();
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

}
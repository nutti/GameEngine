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
		// ボタンの取得
		ButtonStatusHolder holder;
		m_pButtonManager->Update();
		m_pButtonManager->GetButtonStatus( &holder );
		m_pSceneManager->AttachButtonState( &holder );

		// ゲームデータの更新
		m_pGameStateManager->UpdateGameData();
		
		// 画面の更新
		m_pSceneManager->Update();
		m_pSceneManager->Draw();
		
	}

	void EventMediator::Impl::SendEvent( int type )
	{
		switch( type ){
			// 初期化要求
			case EVENT_TYPE_INITIALIZE:
				m_pSceneManager->ChangeScene( SCENE_TYPE_INITIALIZE );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				m_pScriptManager->BuildFileStructure( "script/build.isc" );
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
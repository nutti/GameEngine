#include <MAPIL/MAPIL.h>

#include "Application.h"

#include "SceneManager.h"

#include "EventMediator.h"
#include "EventTypes.h"

#include "GeneralButtonManager.h"

#include "GameStateManager.h"

#include "ResourceManager.h"

#include "ScriptManager.h"

#include "Util.h"

namespace GameEngine
{
	class Application::Impl
	{
		MAPIL::FPSManager							m_FPSManager;			// FPS制御
		std::shared_ptr < SceneManager >			m_pSceneManager;		// シーン管理クラス
		std::shared_ptr < EventMediator >			m_pEventMediator;		// イベント処理クラス
		std::shared_ptr < GeneralButtonManager >	m_pButtonManager;		// ボタン管理クラス
		std::shared_ptr < GameStateManager >		m_pGameStateManager;	// ゲームの状態管理クラス
		std::shared_ptr < ResourceManager >			m_pResourceManager;		// リソース管理クラス
		std::shared_ptr < ScriptManager >			m_pScriptManager;		// スクリプト管理クラス
	public:
		Impl();
		~Impl(){}
		void Init( bool wndMode );
		void Run();
	};

	Application::Impl::Impl()
	{
		m_pEventMediator.reset( new EventMediator );
		m_pSceneManager.reset( new SceneManager( m_pEventMediator ) );
		m_pButtonManager.reset( new GeneralButtonManager );
		m_pGameStateManager.reset( new GameStateManager );
		m_pResourceManager.reset( new ResourceManager );
		m_pScriptManager.reset( new ScriptManager( m_pEventMediator ) );

		m_pEventMediator->SetSceneManager( m_pSceneManager );
		m_pEventMediator->SetButtonManager( m_pButtonManager );
		m_pEventMediator->SetGameStateManager( m_pGameStateManager );
		m_pEventMediator->SetResourceManager( m_pResourceManager );
		m_pEventMediator->SetScriptManager( m_pScriptManager );
	}

	// 初期化処理
	void Application::Impl::Init( bool wndMode )
	{
		MAPIL::InitMAPIL( "GameEngine", 640, 480 );
		m_FPSManager.SetFPS( 60 );
		m_FPSManager.SetPrecise( 10 );
		m_pEventMediator->SendEvent( EVENT_TYPE_INITIALIZE );
		if( !wndMode ){
			MAPIL::ChangeWindowMode( 1 );
		}
	}

	// アプリケーション実行
	void Application::Impl::Run()
	{
		// メインループ
		while( !MAPIL::ProcessMessage() ){

			MAPIL::UpdateKeyboard();

			// 強制終了用
			if( MAPIL::IsKeyboardKeyPushed( MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_DELETE ) ) ){
				break;
			}
			// ゲーム終了要求を受信
			if( m_pEventMediator->HasTermSig() ){
				break;
			}


			if( m_FPSManager.DoesElapseNextTime() ){
				// 描画設定

				
				MAPIL::BeginRendering();
				MAPIL::EnableBlending();
				MAPIL::DisableLighting();
				MAPIL::EnableZBuffering();
				MAPIL::SetTextureMode( MAPIL::TEXTURE_MODE_2D );
				MAPIL::SetCullingMode( MAPIL::CULL_MODE_DISABLED );
				MAPIL::SetAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
				MAPIL::SetViewPort( 0, 0, 640, 480 );

				if( m_pButtonManager->IsSpecialKeyPushed( SPECIAL_KEY_SS ) ){
					if( !FileExist( "screenshot" ) ){
						CreateDirectory( "screenshot" );
					}
					ScreenShot( "screenshot", "screenshot" );
				}

				// 新しいフレーム開始の合図を送信
				m_pEventMediator->SendEvent( EVENT_TYPE_FRAME_UPDATE );

				// 描画終了
				MAPIL::EndRendering();
			}
			else{
				Sleep( 1 );
			}
		}
	}

	Application::Application() : m_pImpl( new Application::Impl )
	{
	}

	Application::~Application()
	{
	}

	void Application::Init( bool wndMode )
	{
		m_pImpl->Init( wndMode );
	}

	void Application::Run()
	{
		m_pImpl->Run();
	}
}
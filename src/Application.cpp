#include <MAPIL/MAPIL.h>

#include "Application.h"

#include "SceneManager.h"

#include "EventMediator.h"
#include "EventTypes.h"

#include "GeneralButtonManager.h"

#include "GameStateManager.h"

#include "ResourceManager.h"

#include "ScriptManager.h"

#include "ResourceID.h"

#include "Util.h"

#include "Profiler.h"

namespace GameEngine
{
	class Application::Impl
	{
		MAPIL::FPSManager							m_FPSManager;			// FPS����
		std::shared_ptr < SceneManager >			m_pSceneManager;		// �V�[���Ǘ��N���X
		std::shared_ptr < EventMediator >			m_pEventMediator;		// �C�x���g�����N���X
		std::shared_ptr < GeneralButtonManager >	m_pButtonManager;		// �{�^���Ǘ��N���X
		std::shared_ptr < GameStateManager >		m_pGameStateManager;	// �Q�[���̏�ԊǗ��N���X
		std::shared_ptr < ResourceManager >			m_pResourceManager;		// ���\�[�X�Ǘ��N���X
		std::shared_ptr < ScriptManager >			m_pScriptManager;		// �X�N���v�g�Ǘ��N���X

		Profiler					m_Profiler;
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

	// ����������
	void Application::Impl::Init( bool wndMode )
	{
		MAPIL::InitMAPIL( "eriKs", 640, 480 );
		m_pResourceManager->OpenArchive( "resource.dat" );
		m_pResourceManager->LoadGlobalResourceFromArchive(	RESOURCE_TYPE_TEXTURE,
															GLOBAL_RESOURCE_TEXTURE_ID_LOADING_1,
															"archive/resource/texture/loading_1.png" );
		m_pResourceManager->LoadGlobalResourceFromArchive(	RESOURCE_TYPE_TEXTURE,
															GLOBAL_RESOURCE_TEXTURE_ID_LOADING_2,
															"archive/resource/texture/loading_2.png" );
		m_pResourceManager->LoadGlobalResourceFromArchive(	RESOURCE_TYPE_TEXTURE,
															GLOBAL_RESOURCE_TEXTURE_ID_INITIALIZE,
															"archive/resource/texture/eriKs_background_2_small.png" );
		m_pResourceManager->LoadGlobalResourceFromArchive(	RESOURCE_TYPE_TEXTURE,
															GLOBAL_RESOURCE_TEXTURE_ID_TITLE_2,
															"archive/resource/texture/eriKs_title_2.png" );
		m_FPSManager.SetFPS( 60 );
		m_FPSManager.SetPrecise( 100 );
		m_pEventMediator->SendEvent( EVENT_TYPE_INITIALIZE );
		if( !wndMode ){
			MAPIL::ChangeWindowMode( 1 );
		}
		m_Profiler.Clear();
	}

	// �A�v���P�[�V�������s
	void Application::Impl::Run()
	{

		// ���C�����[�v
		while( !MAPIL::ProcessMessage() ){

			MAPIL::UpdateKeyboard();

			// �����I���p
			if( MAPIL::IsKeyboardKeyPushed( MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_DELETE ) ) ){
				break;
			}
			// �Q�[���I���v������M
			if( m_pEventMediator->HasTermSig() ){
				break;
			}


			if( m_FPSManager.DoesElapseNextTime() ){
				// �`��ݒ�

				
				
				MAPIL::BeginRendering();
				MAPIL::EnableBlending();
				MAPIL::DisableLighting();
				MAPIL::EnableZBuffering();
				MAPIL::SetTextureMode( MAPIL::TEXTURE_MODE_2D );
				MAPIL::SetCullingMode( MAPIL::CULL_MODE_COUNTERCLOCKWISE );
				MAPIL::SetAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
				MAPIL::SetViewPort( 0, 0, 640, 480 );

				if( m_pButtonManager->IsSpecialKeyPushed( SPECIAL_KEY_SS ) ){
					if( !FileExist( "screenshot" ) ){
						CreateDirectory( "screenshot" );
					}
					ScreenShot( "screenshot", "screenshot" );
				}

				// �V�����t���[���J�n�̍��}�𑗐M
				m_pEventMediator->SendEvent( EVENT_TYPE_FRAME_UPDATE );

				static int i = 0;
				static float fps = 0.0f;
				++i;

				if( ( i % 60 ) == 0 ){ 
					fps = m_FPSManager.GetNowFPS();
				}

				static int time = 0;

				MAPIL::BeginRendering2DGraphics();
				MAPIL::DrawString( 20.0f, 460.0f, "FPS : %f", fps );
				MAPIL::DrawString( 500.0f, 10.0f, "Rendering : %.1f %%", time / 10.0f );
				MAPIL::EndRendering2DGraphics();

				

				
				m_Profiler.Begin( "Rendering" );
				

				// �`��I��
				MAPIL::EndRendering();

				m_Profiler.End( "Rendering" );

				

				if( i % 60 == 0 ){
					time = m_Profiler.GetProfile( "Rendering" );
					m_Profiler.Clear();
				}
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
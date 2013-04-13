#include <MAPIL/MAPIL.h>

#include <MAPIL/GUI/WinAPIGUIFactory.h>

#include "Application.h"

int SelectWindowMode()
{
	MAPIL::IGUIDevice dev = MAPIL::CreateGUIDevice( MAPIL::GUI_API_WIN32API );
	MAPIL::GUIFactory* pFactory = MAPIL::CreateGUIFactory( dev );
	MAPIL::IWindow window = pFactory->CreateWnd( TSTR( "Windows mode selection" ) );
	window->Create( TSTR( "画面の大きさを選択" ), 200, 40 );
	MAPIL::IRadioButton radio1 = pFactory->CreateRadioButton( TSTR( "Window mode" ) );
	radio1->Create( TSTR( "ウィンドウモード" ), 0, 0, 200, 20, window, 0 );
	MAPIL::IRadioButton radio2 = pFactory->CreateRadioButton( TSTR( "Full screen mode" ) );
	radio2->Create( TSTR( "フルスクリーンモード" ), 0, 20, 200, 20, window, 1 );

	int ret = -1;

	while( !window->ProcessMessage() ){
		if( radio1->Checked() ){
			ret = 0;
			window->Destroy();
		}
		else if( radio2->Checked() ){
			ret = 1;
			window->Destroy();
		}
		Sleep( 1 );
	}

	MAPIL::SafeDelete( pFactory );

	return ret;
}

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmd, int nCmd )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	

	try{
#if defined ( MAKE_MODE_RELEASE )
		int wndMode = SelectWindowMode();

		if( wndMode == -1 ){
			return -1;
		}
#endif

		GameEngine::Application app;
#if defined ( MAKE_MODE_RELEASE )
		// ウィンドウモードで起動
		if( wndMode == 0 ){
			app.Init( true );
		}
		// フルスクリーンモードで起動
		else if( wndMode == 1 ){
			app.Init( false );
		}
#else
		app.Init( true );
		

		/*int wndMode = SelectWindowMode();

		if( wndMode == -1 ){
			return -1;
		}

		// ウィンドウモードで起動
		if( wndMode == 0 ){
			app.Init( true );
		}
		// フルスクリーンモードで起動
		else if( wndMode == 1 ){
			app.Init( false );
		}*/
#endif
		app.Run();
	}
	catch( MAPIL::MapilException& e ){
		::OutputDebugString( e.GetExceptionStr().c_str() );
	}
	catch( std::exception& e ){
		::OutputDebugStringA( e.what() );
	}

	return 0;
}
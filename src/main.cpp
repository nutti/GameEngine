#include <MAPIL/MAPIL.h>

#include "Application.h"

void SelectWindowMode()
{
	class Sig : public MAPIL::KeyboardSignal
	{
	private:
		int d;
	public:
		Sig() : MAPIL::KeyboardSignal(), d( 0 )
		{
		}
		~Sig()
		{
		}
		void Handle()
		{
			d = 1;
		}
		bool HasTermSig()
		{
			return d == 1 ? true : false;
		}
	};

	MAPIL::IGUIDevice dev = MAPIL::CreateGUIDevice( MAPIL::GUI_API_WIN32API );
	MAPIL::GUIFactory* pFactory = MAPIL::CreateGUIFactory( dev );
	MAPIL::IWindow window = pFactory->CreateWnd( TSTR( "Screen mode" ) );
	window->Create( TSTR( "‰æ–Ê‘I‘ð" ), 200, 100 );
	Sig sig;
	window->Connect( &sig );

	while( !window->ProcessMessage() ){
		if( sig.HasTermSig() ){
			MAPIL::SharedPointer < MAPIL::WinAPIWindow > w;
			w.DownCast( window );
			SendMessage( w->GetHWnd(), WM_CLOSE, 0, 0 );
		}
		Sleep( 1 );
	}

	MAPIL::SafeDelete( pFactory );
}

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmd, int nCmd )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	try{
		//SelectWindowMode();

		GameEngine::Application app;
		app.Init();
		app.Run();
	}
	catch( MAPIL::MapilException& e ){
		::OutputDebugString( e.GetExceptionStr().c_str() );
	}

	return 0;
}
#include <MAPIL/MAPIL.h>

#include "Application.h"

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmd, int nCmd )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	try{
		GameEngine::Application app;
		app.Init();
		app.Run();
	}
	catch( MAPIL::MapilException& e ){
		::OutputDebugString( e.GetExceptionStr().c_str() );
	}

	return 0;
}
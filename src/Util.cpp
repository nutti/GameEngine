#include "Util.h"

#include <MAPIL/MAPIL.h>
#include <stdarg.h>
#include <Shlwapi.h>

#pragma comment ( lib, "shlwapi.lib" )

namespace GameEngine
{
	bool IsKeepPushed( const ButtonStatusHolder& holder, GeneralButton button )
	{
		return holder.m_Status[ button ] == BUTTON_STATUS_KEEP;
	}

	bool IsPushed( const ButtonStatusHolder& holder, GeneralButton button )
	{
		return holder.m_Status[ button ] == BUTTON_STATUS_PUSHED;
	}

	void DrawFontString( const ResourceMap& map, float x, float y, float scale, const char* pStr, ... )
	{
		// Analyze the variable length argument.
		::va_list list;
		va_start( list, pStr );
		char str[ 1024 ];
		::vsprintf_s( str, 1024, pStr, list );
		va_end( list );
		
		char* p = str;
		int pos = 0;

		while( *p ){
			if( ::isalpha( *p ) ){
				MAPIL::DrawTexture(	map.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_FONT_TEXTURE_FIRST + ::toupper( *p ) - 'A' + 10 ],
									x + pos * 32 * scale, y, scale, scale );
			}
			else if( ::isdigit( *p ) ){
				MAPIL::DrawTexture(	map.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_FONT_TEXTURE_FIRST + *p - '0' ],
									x + pos * 32 * scale, y, scale, scale );
			}
			++p;
			++pos;
		}
	}

	void DrawFontString( const ResourceMap& map, float x, float y, float scale, int color, const char* pStr, ... )
	{
		// Analyze the variable length argument.
		::va_list list;
		va_start( list, pStr );
		char str[ 1024 ];
		::vsprintf_s( str, 1024, pStr, list );
		va_end( list );
		
		char* p = str;
		int pos = 0;

		while( *p ){
			if( ::isalpha( *p ) ){
				MAPIL::DrawTexture(	map.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_FONT_TEXTURE_FIRST + ::toupper( *p ) - 'A' + 10 ],
									x + pos * 32 * scale, y, scale, scale, true, color );
			}
			else if( ::isdigit( *p ) ){
				MAPIL::DrawTexture(	map.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_FONT_TEXTURE_FIRST + *p - '0' ],
									x + pos * 32 * scale, y, scale, scale, true, color );
			}
			++p;
			++pos;
		}
	}

	const int MAXIMUM_SCREEN_SHOT = 10000;		// 10000–‡ŽB‰e‰Â”\

	static int GetDigit( int num )
	{
		int digit = 1;
		int value = num / 10;
		
		while( value ){
			value /= 10;
			++digit;
		}

		return digit;
	}

	void ScreenShot( const char* pDirPath, const char* pFileName )
	{

		for( int i = 0; i < MAXIMUM_SCREEN_SHOT; ++i ){
			int digit = GetDigit( i );
			std::basic_string < char > s( pDirPath );
			char buf[ 20 ];
			sprintf( buf, "%d", i );
			s += "/";
			s += pFileName;
			s += "_";
			for( int di = 5; di > digit; --di ){
				s += "0";
			}
			s += buf;
			s += ".bmp";
			if( !::PathFileExistsA( s.c_str() ) ){
				MAPIL::CaptureScreen( s.c_str() ); 
				return;
			}
		}
	}
}
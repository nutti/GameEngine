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

	void WriteInt( std::ofstream* pFOut, int val )
	{
		char str[ 4 ];
		MAPIL::TransformInt32IntoChar( val, str, MAPIL::BYTE_ORDER_LITTLE );
		pFOut->write( str, sizeof( str ) );
	}


	int ReadInt( std::ifstream* pFIn )
	{
		char str[ 4 ];
		pFIn->read( str, sizeof( str ) );
		return MAPIL::TransformCharIntoInt32( str, MAPIL::BYTE_ORDER_LITTLE );
	}

	int GetInt( char** p )
	{
		int i = MAPIL::TransformCharIntoInt32( *p, MAPIL::BYTE_ORDER_LITTLE );
		*p += 4;

		return i;
	}

	int GetFileSize( std::fstream &f )
	{
		int size = 0;
		int begin = 0;
		int end = 0;
		f.seekg( 0, std::ios::beg );
		begin = f.tellg();
		f.seekg( 0, std::ios::end );
		end = f.tellg();
		size = end - begin;
		f.seekg( 0, std::ios::beg );

		return size;
	}

	void LoadFile( std::fstream& f, char* pBuf, int size )
	{
		f.read( pBuf, size );
	}

	void GetLineFromString( char** pStr, char* pEnd, char* pOut, int outSize )
	{
		while( *pStr != pEnd ){
			if( **pStr == '\r' ){
				( *pStr )++;
				continue;
			}
			else if( **pStr == '\n' ){
				( *pStr )++;
				break;
			}
			*pOut++ = **pStr;
			( *pStr )++;
		}
		*pOut = '\0';
	}

	void CopyArray( std::vector < char >* pVOut, const char* pIn, int size )
	{
		for( int i = 0; i < size; ++i ){
			pVOut->push_back( pIn[ i ] );
		}
	}

	void CopyInt( std::vector < char >* pVOut, int val )
	{
		char str[ 4 ];
		MAPIL::TransformInt32IntoChar( val, str, MAPIL::BYTE_ORDER_LITTLE );
		for( int i = 0; i < 4; ++i ){
			pVOut->push_back( str[ i ] );
		}
	}

	void CreateDirectory( const std::string& pathName )
	{
		::CreateDirectoryA( pathName.c_str(), NULL );
	}

	bool FileExist( const std::string& fileName )
	{
		if( ::GetFileAttributesA( fileName.c_str() ) != 0xFFFFFFFF ){
			return true;
		}
		
		return false;
	}
}
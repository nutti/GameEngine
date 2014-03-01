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

		int interval = 13;

		scale = 1.0f;

		while( *p ){
			if( ::isalpha( *p ) ){
				ResourceMap::TextureAtlas atlas;
				atlas = map.m_pGlobalResourceMap->m_TexAtlasMap[ GLOBAL_TEX_ATLAS_ID_FONT_FIRST + ::toupper( *p ) - 'A' + 10 ];
				MAPIL::DrawClipedTexture(	map.m_pGlobalResourceMap->m_TextureMap[ atlas.m_TexID ],
											x + pos * interval * scale,
											y,
											scale,
											scale,
											0.0f,
											atlas.m_X,
											atlas.m_Y,
											atlas.m_X + atlas.m_Width,
											atlas.m_Y + atlas.m_Height );
				//MAPIL::DrawTexture(	map.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_FONT_TEXTURE_FIRST + ::toupper( *p ) - 'A' + 10 ],
				//					x + pos * 32 * scale, y, scale, scale );
			}
			else if( ::isdigit( *p ) ){
				ResourceMap::TextureAtlas atlas;
				int d = GLOBAL_TEX_ATLAS_ID_FONT_FIRST + *p - '0';
				atlas = map.m_pGlobalResourceMap->m_TexAtlasMap[ GLOBAL_TEX_ATLAS_ID_FONT_FIRST + *p - '0' ];
				MAPIL::DrawClipedTexture(	map.m_pGlobalResourceMap->m_TextureMap[ atlas.m_TexID ],
											x + pos * interval * scale,
											y,
											scale,
											scale,
											0.0f,
											atlas.m_X,
											atlas.m_Y,
											atlas.m_X + atlas.m_Width,
											atlas.m_Y + atlas.m_Height );
				//MAPIL::DrawTexture(	map.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_FONT_TEXTURE_FIRST + *p - '0' ],
				//					x + pos * 32 * scale, y, scale, scale );
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

		int interval = 13;

		while( *p ){
			if( ::isalpha( *p ) ){
				ResourceMap::TextureAtlas atlas;
				atlas = map.m_pGlobalResourceMap->m_TexAtlasMap[ GLOBAL_TEX_ATLAS_ID_FONT_FIRST + ::toupper( *p ) - 'A' + 10 ];
				MAPIL::DrawClipedTexture(	map.m_pGlobalResourceMap->m_TextureMap[ atlas.m_TexID ],
											x + pos * interval * scale,
											y,
											scale,
											scale,
											0.0f,
											atlas.m_X,
											atlas.m_Y,
											atlas.m_X + atlas.m_Width,
											atlas.m_Y + atlas.m_Height,
											true, color );
			}
			else if( ::isdigit( *p ) ){
				ResourceMap::TextureAtlas atlas;
				atlas = map.m_pGlobalResourceMap->m_TexAtlasMap[ GLOBAL_TEX_ATLAS_ID_FONT_FIRST + *p - '0' ];
				MAPIL::DrawClipedTexture(	map.m_pGlobalResourceMap->m_TextureMap[ atlas.m_TexID ],
											x + pos * interval * scale,
											y,
											scale,
											scale,
											0.0f,
											atlas.m_X,
											atlas.m_Y,
											atlas.m_X + atlas.m_Width,
											atlas.m_Y + atlas.m_Height,
											true, color );
			}
			++p;
			++pos;
		}
	}

	void DrawFontString( const ResourceMap& map, float x, float y, float scale, bool scaleSpacing, int color, const char* pStr, ... )
	{
		// Analyze the variable length argument.
		::va_list list;
		va_start( list, pStr );
		char str[ 1024 ];
		::vsprintf_s( str, 1024, pStr, list );
		va_end( list );
		
		char* p = str;
		int pos = 0;

		int interval = 13;
		if( scaleSpacing ){
			interval *= scale;
		}

		while( *p ){
			if( ::isalpha( *p ) ){
				ResourceMap::TextureAtlas atlas;
				atlas = map.m_pGlobalResourceMap->m_TexAtlasMap[ GLOBAL_TEX_ATLAS_ID_FONT_FIRST + ::toupper( *p ) - 'A' + 10 ];
				MAPIL::DrawClipedTexture(	map.m_pGlobalResourceMap->m_TextureMap[ atlas.m_TexID ],
											x + pos * interval,
											y,
											scale,
											scale,
											0.0f,
											atlas.m_X,
											atlas.m_Y,
											atlas.m_X + atlas.m_Width,
											atlas.m_Y + atlas.m_Height,
											true, color );
			}
			else if( ::isdigit( *p ) ){
				ResourceMap::TextureAtlas atlas;
				atlas = map.m_pGlobalResourceMap->m_TexAtlasMap[ GLOBAL_TEX_ATLAS_ID_FONT_FIRST + *p - '0' ];
				MAPIL::DrawClipedTexture(	map.m_pGlobalResourceMap->m_TextureMap[ atlas.m_TexID ],
											x + pos * interval,
											y,
											scale,
											scale,
											0.0f,
											atlas.m_X,
											atlas.m_Y,
											atlas.m_X + atlas.m_Width,
											atlas.m_Y + atlas.m_Height,
											true, color );
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

	// •¶Žš—ñ‚©‚ç“Á’è‚Ì•¶Žš‚ðíœ
	void DeleteChar( char* pOut, int size, const char* pStr, char deleteChar )
	{
		if( ::strlen( pStr ) >= size ){
			*pOut = '\0';
			return;
		}

		while( *pStr ){
			if( deleteChar != *pStr ){
				*pOut++ = *pStr;
			}
			++pStr;
		}

		*pOut = '\0';
	}

	const char* GetStringFromButton( int button )
	{
		static const int ALLOWED_BUTTONS[] = {
#define BUTTON_CODE_STR
#include "ButtonCode.h"
#undef BUTTON_CODE_STR
		};

		static const char* pStrings[] = {
#define BUTTON_CODE_ID
#include "ButtonCode.h"
#undef BUTTON_CODE_ID
		};

		for( int i = 0; i < sizeof( ALLOWED_BUTTONS ) / sizeof( int ); ++i ){
			if( ALLOWED_BUTTONS[ i ] == button ){
				return pStrings[ i ];
			}
		}

		return NULL;
	}

	int GetButtonFromString( const std::string& button )
	{
		static const int ALLOWED_BUTTONS[] = {
#define BUTTON_CODE_STR
#include "ButtonCode.h"
#undef BUTTON_CODE_STR
		};

		static const char* BUTTON_STRINGS[] = {
#define BUTTON_CODE_ID
#include "ButtonCode.h"
#undef BUTTON_CODE_ID
		};

		for( int i = 0; i < sizeof( BUTTON_STRINGS ) / sizeof( char* ); ++i ){
			if( button == BUTTON_STRINGS[ i ] ){
				return ALLOWED_BUTTONS[ i ];
			}
		}

		return -1;
	}

	int IsDigitString( const char* str )
	{
		if( str[ 0 ] == '\0' ){
			return 0;
		}

		while( *str ){
			if( !isdigit( *str ) ){
				return 0;
			}
			++str;
		}

		return 1;
	}
}
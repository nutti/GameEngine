#ifndef INCLUDED_GAMEENGINE_UTIL_H
#define INCLUDED_GAMEENGINE_UTIL_H

#include <fstream>
#include <vector>

#include "InputTypes.h"
#include "ResourceTypes.h"

namespace GameEngine
{
#define NUM_OF( x ) ( sizeof( x ) / sizeof( x[ 0 ] ) )

	bool IsKeepPushed( const ButtonStatusHolder& holder, GeneralButton button );

	bool IsPushed( const ButtonStatusHolder& holder, GeneralButton button );

	void DrawFontString( const ResourceMap& map, float x, float y, float scale, const char* pStr, ... );

	void DrawFontString( const ResourceMap& map, float x, float y, float scale, int color, const char* pStr, ... );

	void DrawFontString( const ResourceMap& map, float x, float y, float scale, bool scaleSpacing, int color, const char* pStr, ... );

	void ScreenShot( const char* pDirPath, const char* pFileName );

	void WriteInt( std::ofstream* pFOut, int val );

	int ReadInt( std::ifstream* pFIn );

	int GetInt( char** p );

	int GetFileSize( std::fstream& f );

	void LoadFile( std::fstream& f, char* pBuf, int size );

	void GetLineFromString( char** pStr, char* pEnd, char* pOut, int outSize );

	void CopyArray( std::vector < char >* pVOut, const char* pIn, int size );

	void CopyInt( std::vector < char >* pVOut, int val );

	void CreateDirectory( const std::string& pathName );

	bool FileExist( const std::string& fileName );

	// ï∂éöóÒÇ©ÇÁì¡íËÇÃï∂éöÇçÌèú
	void DeleteChar( char* pOut, int size, const char* pStr, char deleteChar );

	const char* GetStringFromButton( int button );

	int GetButtonFromString( const std::string& button );

	int IsDigitString( const char* str );
}

#endif
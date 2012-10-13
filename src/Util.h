#ifndef INCLUDED_GAMEENGINE_UTIL_H
#define INCLUDED_GAMEENGINE_UTIL_H

#include <fstream>

#include "InputTypes.h"
#include "ResourceTypes.h"

namespace GameEngine
{

	bool IsKeepPushed( const ButtonStatusHolder& holder, GeneralButton button );

	bool IsPushed( const ButtonStatusHolder& holder, GeneralButton button );

	void DrawFontString( const ResourceMap& map, float x, float y, float scale, const char* pStr, ... );

	void DrawFontString( const ResourceMap& map, float x, float y, float scale, int color, const char* pStr, ... );

	void ScreenShot( const char* pDirPath, const char* pFileName );

	void WriteInt( std::ofstream* pFOut, int val );

	int ReadInt( std::ifstream* pFIn );
}

#endif
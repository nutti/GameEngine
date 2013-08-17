#ifndef INCLUDED_GAMEENGINE_TEXTURE_ATLAS_PATTERN_FILE_LOADER_H
#define INCLUDED_GAMEENGINE_TEXTURE_ATLAS_PATTERN_FILE_LOADER_H

#include <vector>
#include <string>

namespace GameEngine
{
	class TextureAtlasPatternFileLoader
	{
	private:
		struct AtlasData
		{
			int		m_ID;
			int		m_TexID;
			int		m_PosX;
			int		m_PosY;
			int		m_Width;
			int		m_Height;
		};
		std::vector < AtlasData >		m_AtlasData;

		void Analyze( char* pData, int len );
	public:
		TextureAtlasPatternFileLoader();
		~TextureAtlasPatternFileLoader();
		void Load( const std::string& fileName );
		void Load( int archiveHandle, const std::string& filePath );
		int GetTexID( int index ) const;
		int GetStartX( int index ) const;
		int GetStartY( int index ) const;
		int GetWidth( int index ) const;
		int GetHeight( int index ) const;
		int GetID( int index ) const;
		int GetAtlasTotal() const;
	};
}

#endif
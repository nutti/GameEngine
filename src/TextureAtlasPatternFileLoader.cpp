#include "TextureAtlasPatternFileLoader.h"

#include <fstream>

#include <MAPIL/MAPIL.h>

#include "Util.h"

namespace GameEngine
{
	TextureAtlasPatternFileLoader::TextureAtlasPatternFileLoader() : m_AtlasData()
	{
		m_AtlasData.clear();
	}

	TextureAtlasPatternFileLoader::~TextureAtlasPatternFileLoader()
	{
		m_AtlasData.clear();
	}

	void TextureAtlasPatternFileLoader::Analyze( char* pData, int len )
	{
		char* pEnd = pData + len;

		char str[ 1024 ];

		// ファイル解析
		while( pData != pEnd ){
			GetLineFromString( &pData, pEnd, str, sizeof( str ) );
			int count = 0;
			AtlasData data;
			MAPIL::ZeroObject( &data, sizeof( data ) );
			for( const char* pToken = ::strtok( str, "," ); pToken; pToken = ::strtok( 0, "," ) ){
				char s[ 1024 ];
				DeleteChar( s, sizeof( s ), pToken, ' ' );
				switch( count ){
					// ID
					case 0:
						data.m_ID = ::atoi( s );
						break;
					// TexID
					case 1:
						data.m_TexID = ::atoi( s );
						break;
					// X座標
					case 2:
						data.m_PosX = ::atoi( s );
						break;
					// Y座標
					case 3:
						data.m_PosY = ::atoi( s );
						break;
					// 幅
					case 4:
						data.m_Width = ::atoi( s );
						break;
					// 高さ
					case 5:
						data.m_Height = ::atoi( s );
						break;
					// レジスタ
					default:
						break;
				}
				++count;
			}
			m_AtlasData.push_back( data );
		}
	}

	void TextureAtlasPatternFileLoader::Load( const std::string& fileName )
	{
		std::fstream fIn( fileName, std::ios::binary | std::ios::in );

		if( !fIn ){
			throw MAPIL::MapilException( CURRENT_POSITION, TSTR( "File not found" ), -1 );
		}

		int size = GetFileSize( fIn );
		char* pBuf = new char[ size ];
		LoadFile( fIn, pBuf, size );
		
		// ファイル解析
		Analyze( pBuf, size );

		MAPIL::SafeDeleteArray( pBuf );
		fIn.close();
	}

	void TextureAtlasPatternFileLoader::Load( int archiveHandle, const std::string& filePath )
	{
		// アーカイブからファイルを読み込む
		int size = MAPIL::GetContentsSizeOnArchiveFile( archiveHandle, filePath.c_str() );
		char* pBuf = new char [ size ];
		MAPIL::LoadDataFromArchiveFile( archiveHandle, filePath.c_str(), pBuf );

		// ファイル解析
		Analyze( pBuf, size );

		MAPIL::SafeDeleteArray( pBuf );
	}

	int TextureAtlasPatternFileLoader::GetTexID( int index ) const
	{
		return m_AtlasData[ index ].m_TexID;
	}

	int TextureAtlasPatternFileLoader::GetStartX( int index ) const
	{
		return m_AtlasData[ index ].m_PosX;
	}

	int TextureAtlasPatternFileLoader::GetStartY( int index ) const
	{
		return m_AtlasData[ index ].m_PosY;
	}

	int TextureAtlasPatternFileLoader::GetWidth( int index ) const
	{
		return m_AtlasData[ index ].m_Width;
	}

	int TextureAtlasPatternFileLoader::GetHeight( int index ) const
	{
		return m_AtlasData[ index ].m_Height;
	}

	int TextureAtlasPatternFileLoader::GetID( int index ) const
	{
		return m_AtlasData[ index ].m_ID;
	}

	int TextureAtlasPatternFileLoader::GetAtlasTotal() const
	{
		return m_AtlasData.size();
	}

}
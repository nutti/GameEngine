#include <MAPIL/MAPIL.h>

#include <bitset>

#include "SmokeShotM.h"

#include "../../ResourceTypes.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int SHOT_TEX_ID	= 129;

	SmokeShotM::SmokeShotM( std::shared_ptr < ResourceMap > pMap, int id ) :	RotateShot( pMap, id )
	{
		m_AlphaBlendingMode = MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT;
		m_GUData.m_ColRadiusBase = GameUnit( 6 );
		m_GUData.m_ColRadius = m_GUData.m_ColRadiusBase;
		m_DrawingMultiplicity = 2;
		m_Circle.SetRadius( m_GUData.m_ColRadius.GetFloat() );
	}

	SmokeShotM::~SmokeShotM()
	{
	}

	void SmokeShotM::Draw()
	{
		if( m_StatusFlags[ INVISIBLE ] ){
			return;
		}

		float posX = m_GUData.m_PosX.GetFloat();
		float posY = m_GUData.m_PosY.GetFloat();
		int color = 0xFFFFFFFF;
		int scale = m_ImgScale;

		DrawEffect();

		// è¡ãéÇ≥ÇÍÇÈÇ∆Ç´ÇÕèôÅXÇ…îñÇ≠Ç»Ç¡ÇƒÇ¢Ç≠ÅB
		if( m_StatusFlags[ DEAD ] ){
			color = ( ( 20 - m_DeadCounter ) * 5 ) << 24 | 0xFFFFFF;
			scale += m_DeadCounter * 0.05f;
		}

		// ì_ñ≈
		if( m_Counter % 16 >= 8 ){
			m_DrawingMultiplicity = 2;
		}
		else{
			m_DrawingMultiplicity = 1;
		}

		// ï`âÊ
		MAPIL::Assert( m_AtlasImgID != -1, CURRENT_POSITION, TSTR( "Invalid image ID was input." ), -1 );

		if( m_AlphaBlendingMode != MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT ){
			for( int i = 0; i < m_DrawingMultiplicity; ++i ){

				AddToAtlasSpriteBatch(	true, m_AlphaBlendingMode,
										m_AtlasImgID,
										posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle, true, color );
			}
		}
		else{
			for( int i = 0; i < m_DrawingMultiplicity; ++i ){
				ResourceMap::TextureAtlas atlas;
				atlas = m_pResourceMap->m_pGlobalResourceMap->m_TexAtlasMap[ m_AtlasImgID ];
				MAPIL::DrawClipedTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ atlas.m_TexID ],
											posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle,
											atlas.m_X, atlas.m_Y, atlas.m_X + atlas.m_Width, atlas.m_Y + atlas.m_Height, true, color );
			}
		}
	}

	void SmokeShotM::SetTextureColor( int color )
	{
		m_TexColor = color;
		m_AtlasImgID = SHOT_TEX_ID + m_TexColor;
	}
}
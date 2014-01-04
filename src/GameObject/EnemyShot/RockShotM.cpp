#include <MAPIL/MAPIL.h>

#include <bitset>

#include "RockShotM.h"

#include "../../ResourceTypes.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int SHOT_TEX_ID	= 153;

	RockShotM::RockShotM( std::shared_ptr < ResourceMap > pMap, int id ) :	NoRotateShot( pMap, id )
	{
		m_GUData.m_ColRadiusBase = GameUnit( 14 );
		m_GUData.m_ColRadius = m_GUData.m_ColRadiusBase;
		m_Circle.SetRadius( m_GUData.m_ColRadius.GetFloat() );
	}

	RockShotM::~RockShotM()
	{
	}

	void RockShotM::Draw()
	{
		if( m_StatusFlags[ INVISIBLE ] ){
			return;
		}

		float posX = m_GUData.m_PosX.GetFloat();
		float posY = m_GUData.m_PosY.GetFloat();
		int color = 0xFFFFFFFF;
		float scale = m_ImgScale;
		float angle;

		DrawEffect();

		// è¡ãéÇ≥ÇÍÇÈÇ∆Ç´ÇÕèôÅXÇ…îñÇ≠Ç»Ç¡ÇƒÇ¢Ç≠ÅB
		if( m_StatusFlags[ DEAD ] ){
			color = ( ( 20 - m_DeadCounter ) * 5 ) << 24 | 0xFFFFFF;
			scale += m_DeadCounter * 0.05f;
		}

		// èââÒéûÇÃÇ›ãêëÂâª
		if( m_Counter < 20 ){
			scale = m_ImgScale * m_Counter / 20.0f;
		}

		// Ç™ÇΩÇ¬Ç´
		angle = m_ImgRotAngle + 0.1f * ::sin( m_Counter * 0.2f );

		// ï`âÊ
		for( int i = 0; i < m_DrawingMultiplicity; ++i ){
			MAPIL::Assert( m_AtlasImgID != -1, CURRENT_POSITION, TSTR( "Invalid image ID was input." ), -1 );

			if( m_AlphaBlendingMode != MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT ){
				for( int i = 0; i < m_DrawingMultiplicity; ++i ){

					AddToAtlasSpriteBatch(	false, m_AlphaBlendingMode,
											m_AtlasImgID,
											posX, posY, scale, scale, angle, true, color );
				}
			}
			else{
				for( int i = 0; i < m_DrawingMultiplicity; ++i ){
					ResourceMap::TextureAtlas atlas;
					atlas = m_pResourceMap->m_pGlobalResourceMap->m_TexAtlasMap[ m_AtlasImgID ];
					MAPIL::DrawClipedTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ atlas.m_TexID ],
												posX, posY, scale, scale, angle,
												atlas.m_X, atlas.m_Y, atlas.m_X + atlas.m_Width, atlas.m_Y + atlas.m_Height, true, color );
				}
			}

		}
	}

	void RockShotM::SetTextureColor( int color )
	{
		m_TexColor = color;
		m_AtlasImgID = SHOT_TEX_ID + m_TexColor;
	}
}
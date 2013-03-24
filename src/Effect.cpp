#include "Effect.h"

#include <MAPIL/MAPIL.h>

#include "ResourceTypes.h"

#include "Util.h"

namespace GameEngine
{
	Effect::Effect( std::shared_ptr < ResourceMap > pMap, int id, int subID )
	{
		m_EffectData.m_EffectID = id;
		m_EffectData.m_EffectSubID = subID;
		m_EffectData.m_Counter = 0;
		m_EffectData.m_pResourceMap = pMap;
		m_EffectData.m_PosX = 0.0f;
		m_EffectData.m_PosY = 0.0f;
		//m_EffectData.m_PointSprite = MAPIL::CreatePointSprite( 100,
		//m_EffectData.m_pResourceMap->m_pStageResourceMap->m_TextureMap[ subID ] );
	}

	Effect::~Effect()
	{
		//MAPIL::DeletePointSprite( m_EffectData.m_PointSprite );
	}

	void Effect::Init( float posX, float posY )
	{
	}

	void Effect::SetPos( float posX, float posY )
	{
		m_EffectData.m_PosX = posX;
		m_EffectData.m_PosY = posY;
	}

	void Effect::Draw()
	{
		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
		if( m_EffectData.m_EffectID == EFFECT_ID_PLAYER_SHOT_COLLIDED ){
			float scale = 1.5f - m_EffectData.m_Counter / 20.0f;
			MAPIL::DrawTexture(	m_EffectData.m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_PLAYER_SHOT_COLLIDED_TEXTURE ],
								m_EffectData.m_PosX, m_EffectData.m_PosY - m_EffectData.m_Counter,
								scale, scale, m_EffectData.m_Counter * MAPIL::DegToRad( 12.0f ), true, 0xFFFFFFFF );
		}
		else if( m_EffectData.m_EffectID == EFFECT_ID_ENEMY_DESTORYED ){
			if( m_EffectData.m_EffectSubID == 0 ){
				float scale = m_EffectData.m_Counter / 5.0f;
				MAPIL::DrawTexture(	m_EffectData.m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_ENEMY_DESTROYED_TEXTURE ],
									m_EffectData.m_PosX, m_EffectData.m_PosY,
									scale, scale );
			}
			else if( m_EffectData.m_EffectSubID == 1 ){
				float tblX[ 6 ] = { 0.0f, 8.0f, -8.0f, 8.0f, -8.0f, 0.0f };
				float tblY[ 6 ] = { 0.0f, 8.0f, 8.0f, -8.0f, -8.0f, 0.0f };
				for( int i = 0; i < 6; ++i ){
					if( i < 5 ){
						if( m_EffectData.m_Counter >= i * 5 && m_EffectData.m_Counter < i * 5 + 13 ){
							float scale = ( ( m_EffectData.m_Counter - i * 5 ) % 13 ) / 5.0f;
							MAPIL::DrawTexture(	m_EffectData.m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_ENEMY_DESTROYED_TEXTURE ],
												m_EffectData.m_PosX + tblX[ i ], m_EffectData.m_PosY + tblY[ i ],
												scale, scale );
						}
					}
					else{
						if( m_EffectData.m_Counter >= i * 5 && m_EffectData.m_Counter < i * 5 + 33 ){
							float scale = ( m_EffectData.m_Counter - i * 5 ) / 7.0f;
							MAPIL::DrawTexture(	m_EffectData.m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_ENEMY_DESTROYED_TEXTURE ],
												m_EffectData.m_PosX + tblX[ i ], m_EffectData.m_PosY + tblY[ i ],
												scale, scale );
						}
					}
				}
			}
		}
		else if( m_EffectData.m_EffectID == EFFECT_ID_SPRAY ){
			MAPIL::EndRendering2DGraphics();
			MAPIL::DisableZBuffering();
			MAPIL::DisableFog();
			MAPIL::DrawPointSprite( m_EffectData.m_PointSprite );
			MAPIL::EnableFog();
			MAPIL::EnableZBuffering();
			MAPIL::BeginRendering2DGraphics();
		}
		else if( m_EffectData.m_EffectID == EFFECT_ID_OBTAIN_ITEM_CRYSTAL ){
			DrawFontString(	*m_EffectData.m_pResourceMap,
							m_EffectData.m_PosX, m_EffectData.m_PosY, 0.25f,
							( ( 30 - m_EffectData.m_Counter ) * 8 ) << 24 | 0x3333FF,
							"Crystal %d", m_EffectData.m_EffectSubID );
			DrawFontString(	*m_EffectData.m_pResourceMap,
							m_EffectData.m_PosX, m_EffectData.m_PosY + 7.0f, 0.25f,
							( ( 30 - m_EffectData.m_Counter ) * 8 ) << 24 | 0xFFFF33,
							"Score %d", m_EffectData.m_EffectSubID * 100 );
		}

		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
	}

	bool Effect::Update()
	{
		if( m_EffectData.m_EffectID == EFFECT_ID_PLAYER_SHOT_COLLIDED ){
			if( m_EffectData.m_Counter >= 30 ){
				return false;
			}
		}
		else if( m_EffectData.m_EffectID == EFFECT_ID_ENEMY_DESTORYED ){
			if( m_EffectData.m_EffectSubID == 0 ){
				if( m_EffectData.m_Counter == 0 ){
					MAPIL::PlayStaticBuffer( m_EffectData.m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_ENEMY_DESTROYED_SE ] );
				}
				if( m_EffectData.m_Counter >= 13 ){
					return false;
				}
			}
			else if( m_EffectData.m_EffectSubID == 1 ){
				if( ( m_EffectData.m_Counter % 5 ) == 0 && m_EffectData.m_Counter <= 30 ){
					MAPIL::PlayStaticBuffer( m_EffectData.m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_ENEMY_DESTROYED_SE ] );
				}
				if( m_EffectData.m_Counter >= 60 ){
					return false;
				}
			}
		}
		else if( m_EffectData.m_EffectID == EFFECT_ID_SPRAY ){
			for( int i = 0; i < 100; ++i ){
				/*MAPIL::UpdatePointSprite(	m_EffectData.m_PointSprite, i,
					MAPIL::Vector3 < float > ( 2.0f, 2.0f, 0.001f * m_EffectData.m_Counter ),
					6.0f, 0xFFFFFFFF );*/
			}
			if( m_EffectData.m_Counter >= 120 ){
				return false;
			}
		}
		else if( m_EffectData.m_EffectID == EFFECT_ID_OBTAIN_ITEM_CRYSTAL ){
			m_EffectData.m_PosY -= 2.5f;
			if( m_EffectData.m_Counter >= 30 ){
				return false;
			}
		}

		++m_EffectData.m_Counter;

		return true;
	}
}
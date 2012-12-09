#include "Effect.h"

#include <MAPIL/MAPIL.h>

#include "ResourceTypes.h"

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
	}

	Effect::~Effect()
	{
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

		++m_EffectData.m_Counter;

		return true;
	}
}
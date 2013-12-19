#include <MAPIL/MAPIL.h>

#include <bitset>

#include "NormalShot.h"

#include "../../ResourceID.h"
#include "../../ResourceTypes.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int LAUNCH_EFFECT_TEX_ID		= 1000;
	static const int LAUNCH_EFFECT_SE_ID		= GLOBAL_RESOURCE_SE_ID_ENEMY_NORMAL_SHOT;

	NormalShot::NormalShot( std::shared_ptr < ResourceMap > pMap, int id ) :	EnemyShot( pMap, id )
	{
		m_AlphaBlendingMode = MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT;
		m_GUData.m_ColRadius = GameUnit( 2 );
	}

	NormalShot::~NormalShot()
	{
	}

	void NormalShot::DrawEffect()
	{
		if( m_Counter >= 7 ){
			return;
		}

		float scale = ( 7 - m_Counter ) * 0.6f + 0.4f;

		ResourceMap::TextureAtlas atlas;
		atlas = m_pResourceMap->m_pGlobalResourceMap->m_TexAtlasMap[ LAUNCH_EFFECT_TEX_ID + m_TexColor ];
		MAPIL::DrawClipedTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ atlas.m_TexID ],
									m_BeginPosX.GetFloat(), m_BeginPosY.GetFloat(), scale, scale, 0.0f,
									atlas.m_X, atlas.m_Y, atlas.m_X + atlas.m_Width, atlas.m_Y + atlas.m_Height, true );
	}


	void NormalShot::Draw()
	{
		if( m_StatusFlags[ INVISIBLE ] ){
			return;
		}

		float posX = m_GUData.m_PosX.GetFloat();
		float posY = m_GUData.m_PosY.GetFloat();
		int color = 0xFFFFFFFF;
		int scale = m_ImgScale;

		DrawEffect();

		// 消去されるときは徐々に薄くなっていく。
		if( m_StatusFlags[ DEAD ] ){
			color = ( ( 20 - m_DeadCounter ) * 5 ) << 24 | 0xFFFFFF;
			scale += m_DeadCounter * 0.05f;
		}

		// 描画
		for( int i = 0; i < m_DrawingMultiplicity; ++i ){
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
					MAPIL::DrawClipedTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ atlas.m_TexID ],
												posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle,
												atlas.m_X, atlas.m_Y, atlas.m_X + atlas.m_Width, atlas.m_Y + atlas.m_Height, true, color );
				}
			}

		}
	}

	bool NormalShot::Update()
	{
		if( m_StatusFlags[ PAUSED ] ){
			return true;
		}

		// メッセージ処理
		ProcessMessages();

		// 最初の位置を保存
		if( m_Counter == 0 ){
			m_BeginPosX = m_GUData.m_PosX;
			m_BeginPosY = m_GUData.m_PosY;
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ LAUNCH_EFFECT_SE_ID ] );
		}
		
		// 死亡判定処理
		if( m_StatusFlags[ DEAD ] ){
			++m_DeadCounter;
			if( m_DeadCounter >= 20 ){
				return false;
			}
		}
		else{
			m_GUData.m_PosX += m_GUData.m_Speed * CosGU( m_GUData.m_Angle );
			m_GUData.m_PosY -= m_GUData.m_Speed * SinGU( m_GUData.m_Angle );
			m_Circle.SetCenterX( m_GUData.m_PosX.GetFloat() );
			m_Circle.SetCenterY( m_GUData.m_PosY.GetFloat() );

			if(	m_GUData.m_PosX < GameUnit( 0 ) ||
				m_GUData.m_PosX > GameUnit( 640 ) ||
				m_GUData.m_PosY < GameUnit( -30 ) ||
				m_GUData.m_PosY > GameUnit( 500 ) ){
				return false;
			}
		}

		m_ImgRotAngle = MAPIL::DegToRad( m_GUData.m_Angle.GetFloat() + 90.0f );

		++m_Counter;

		return true;
	}

	void NormalShot::SetTextureColor( int color )
	{
	}
}
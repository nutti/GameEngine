#include <MAPIL/MAPIL.h>

#include "Player.h"

#include "Enemy.h"
#include "Item.h"

#include "ResourceTypes.h"
#include "ScriptTypes.h"
#include "Stage.h"
#include "Effect.h"
#include "PlayerShot.h"
#include "EnemyShotGroup.h"
#include "ScriptEffect.h"
#include "ResourceID.h"

#include "Util.h"

namespace GameEngine
{
	Enemy::Enemy(	std::shared_ptr < ResourceMap > pMap,
					std::shared_ptr < EnemyScriptData > pData,
					int id,
					StageData* pStageData ) :	m_VM(),
												m_ScriptID( id ),
												m_pScriptData( pData )
	{
#if defined ( USE_FLOATING_POINT )
		m_Data.m_ColRadius = 0.0f;
		m_Data.m_PosX = 0.0f;
		m_Data.m_PosY = 0.0f;
		m_Data.m_PosZ = 0.5f;
#elif defined ( USE_GAME_UNIT )
		m_Data.m_GUData.m_PosX = 300;
		m_Data.m_GUData.m_PosY = 100;
		m_Data.m_GUData.m_PosZ = 0.5f;
		m_Data.m_GUData.m_ColRadius = 0; 
#endif

		m_Data.m_pStageData = pStageData;
		m_Data.m_Destroyed = false;
		m_Data.m_pResouceMap = pMap;
		m_Data.m_ScaleX = 1.0f;
		m_Data.m_ScaleY = 1.0f;
		m_Data.m_ScaleZ = 1.0f;
		m_Data.m_RotX = 0.0f;
		m_Data.m_RotY = 0.0f;
		m_Data.m_RotZ = 0.0f;
		m_Data.m_ImgID = -1;
		m_Data.m_Score = 0;
		m_Data.m_ScoreBonus = 0;
		m_Data.m_HP = 2000;
		m_Data.m_MaxHP = 2000;
		m_Data.m_Counter = 0;
		m_Data.m_ConsGauge = 200;
		m_Data.m_MaxConsGauge = 200;
		m_Data.m_DamagedConsGauge = 0;
		m_Data.m_IsBoss = false;
		m_Data.m_IsInvincibleMode = false;
		m_Data.m_IsNonCollisionMode = false;
		m_Data.m_IsConsSkillMode = false;
		m_Data.m_Paused = false;
		m_Data.m_Is3D = false;
		m_Data.m_ConsSkillName.clear();
		m_Data.m_ConsType = 0;
		m_Data.m_ShotGroupList.clear();
		m_Data.m_Name = "No Name";
		MAPIL::ZeroObject( &m_Data.m_Regs, sizeof( m_Data.m_Regs ) );
		m_Data.m_StatusFlags.reset();

		m_PrivateData.m_PrevConsGauge = 200;
		m_PrivateData.m_ConsSkillEffectCounter = 0;
		m_PrivateData.m_ConsSkillEffectPostCounter = 500;
	}

	Enemy::~Enemy()
	{
		for( int i = 0; i < m_Data.m_ShotGroupList.size(); ++i ){
			if( m_Data.m_ShotGroupList[ i ] != NULL ){
				m_Data.m_ShotGroupList[ i ]->DetachEnemyControl();
			}
		}
		m_Data.m_ShotGroupList.clear();
	}

#if defined ( USE_FLOATING_POINT )
	void Enemy::Init( float posX, float posY )
	{
		m_Data.m_PosX = posX;
		m_Data.m_PosY = posY;
		m_VM.Init( &m_pScriptData->m_pElm[ m_ScriptID ].m_Data, &m_Data );
	}
#elif defined ( USE_GAME_UNIT )
	void Enemy::Init( const GameUnit& posX, const GameUnit& posY )
	{
		m_Data.m_GUData.m_PosX = posX;
		m_Data.m_GUData.m_PosY = posY;
		m_VM.Init( &m_pScriptData->m_pElm[ m_ScriptID ].m_Data, &m_Data );
	}
#endif

#if defined ( USE_FLOATING_POINT )
	void Enemy::Draw()
	{
		int attrEffect[] = { 0xFFFFFFFF, 0xFF33FF33, 0xFF3333FF, 0xFFFF3333 };
		if( m_Data.m_IsConsSkillMode ){
			// 効果音再生
			if( m_PrivateData.m_ConsSkillEffectCounter == 1 ){
				MAPIL::PlayStaticBuffer( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_EFFECT_CONS_SKILL_2 ] );
			}
			else if( m_PrivateData.m_ConsSkillEffectCounter == 60 ){
				MAPIL::PlayStaticBuffer( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_EFFECT_CONS_SKILL_1 ] );
				MAPIL::PlayStaticBuffer( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_EFFECT_CONS_SKILL_3 ] );
			}

			// エフェクト表示
			float offsets[ 10 ] = { 0.03f, 0.04f, -0.01f, 0.01f, 0.0f, 0.06f, 0.1f, -0.1f, -0.3f, 1.0f };
			for( int i = 0; i < 10; ++i ){
				if( m_PrivateData.m_ConsSkillEffectCounter < i * 3 + 10 && m_PrivateData.m_ConsSkillEffectCounter >= i * 3 ){
					int counter = m_PrivateData.m_ConsSkillEffectCounter - i * 3;
					MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_2 ],
										m_Data.m_PosX + ( 10 - counter ) * 15.0f * ::sin( MAPIL::DegToRad( i * 36 ) + offsets[ i ] ),
										m_Data.m_PosY + ( 10 - counter ) * 15.0f * ::cos( MAPIL::DegToRad( i * 36 ) + offsets[ i ] ),
										true, attrEffect[ m_Data.m_ConsSkillAttr ] );
				}
			}
			if( m_PrivateData.m_ConsSkillEffectCounter <= 30 && m_PrivateData.m_ConsSkillEffectCounter > 20 ){
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_3 ],
									m_Data.m_PosX, m_Data.m_PosY,
									( 30 - m_PrivateData.m_ConsSkillEffectCounter ) * 0.8f,
									( 30 - m_PrivateData.m_ConsSkillEffectCounter ) * 0.8f, true, attrEffect[ m_Data.m_ConsSkillAttr ] );
			}
			else if( m_PrivateData.m_ConsSkillEffectCounter > 60 ){
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_2 ],
									m_Data.m_PosX, m_Data.m_PosY, 
									m_PrivateData.m_ConsSkillEffectCounter > 80 ? 13.0f : ( m_PrivateData.m_ConsSkillEffectCounter - 54 ) * 0.5f,
									m_PrivateData.m_ConsSkillEffectCounter > 80 ? 13.0f : ( m_PrivateData.m_ConsSkillEffectCounter - 54 ) * 0.5f,
									-m_Data.m_Counter * 0.1f, true,
									0xFF << 24 );
				MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_1 ],
									m_Data.m_PosX, m_Data.m_PosY,
									m_PrivateData.m_ConsSkillEffectCounter > 80 ? 1.0f : ( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 0.05f,
									m_PrivateData.m_ConsSkillEffectCounter > 80 ? 1.0f : ( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 0.05f,
									m_Data.m_Counter * 0.1f, true, attrEffect[ m_Data.m_ConsSkillAttr ]/*0xBBBBBBBB*/ );
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_1 ],
									m_Data.m_PosX, m_Data.m_PosY,
									m_PrivateData.m_ConsSkillEffectCounter > 80 ? 0.7f : ( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 0.035f,
									m_PrivateData.m_ConsSkillEffectCounter > 80 ? 0.7f : ( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 0.035f,
									-m_Data.m_Counter * 0.1f, true, attrEffect[ m_Data.m_ConsSkillAttr ] );
				MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
				if( m_PrivateData.m_ConsSkillEffectCounter <= 260 ){
					MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_3 ],
										m_Data.m_PosX, m_Data.m_PosY,
										( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 0.3f,
										( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 0.3f, true,
										( ( 260 - m_PrivateData.m_ConsSkillEffectCounter ) ) << 24 | attrEffect[ m_Data.m_ConsSkillAttr ] & 0xFFFFFF );
				}
			}
			// 技名表示
			if( !m_Data.m_IsBoss ){
				if( m_PrivateData.m_ConsSkillEffectCounter <= 80 && m_PrivateData.m_ConsSkillEffectCounter > 60 ){
					DrawFontString(	*m_Data.m_pResouceMap.get(),
									m_Data.m_PosX + ( m_PrivateData.m_ConsSkillEffectCounter - 80 ) * 6.0f,
									m_Data.m_PosY - 20.0f,
									0.4f,
									( ( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 7 + 100 ) << 24 | 0xFF55FF, m_Data.m_ConsSkillName.c_str() );
					DrawFontString(	*m_Data.m_pResouceMap.get(),
									m_Data.m_PosX + ( 80 - m_PrivateData.m_ConsSkillEffectCounter ) * 6.0f,
									m_Data.m_PosY - 20.0f,
									0.4f,
									( ( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 7 + 100 ) << 24 | 0xFF55FF, m_Data.m_ConsSkillName.c_str() );
				}
				else if( m_PrivateData.m_ConsSkillEffectCounter > 80 ){
					DrawFontString( *m_Data.m_pResouceMap.get(), m_Data.m_PosX, m_Data.m_PosY - 20.0f, 0.4f, 0xFFFF55FF, m_Data.m_ConsSkillName.c_str() );
				}
			}
		}
		// 意識技後エフェクト
		else{
			if( m_PrivateData.m_ConsSkillEffectPostCounter <= 20 ){
				// 技名表示
				if( !m_Data.m_IsBoss ){
					DrawFontString(	*m_Data.m_pResouceMap.get(),
									m_Data.m_PosX + m_PrivateData.m_ConsSkillEffectPostCounter * 6.0f,
									m_Data.m_PosY - 20.0f,
									0.4f,
									( 240 - m_PrivateData.m_ConsSkillEffectPostCounter * 7 ) << 24 | 0xFF55FF, m_Data.m_ConsSkillName.c_str() );
					DrawFontString(	*m_Data.m_pResouceMap.get(),
									m_Data.m_PosX + ( 20 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 6.0f,
									m_Data.m_PosY - 20.0f,
									0.4f,
									( 240 - m_PrivateData.m_ConsSkillEffectPostCounter * 7 ) << 24 | 0xFF55FF, m_Data.m_ConsSkillName.c_str() );
				}

				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_2 ],
									m_Data.m_PosX, m_Data.m_PosY, 
									( 26 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 0.5f,
									( 26 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 0.5f,
									-m_Data.m_Counter * 0.1f, true,
									0xFF << 24 );
				MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_1 ],
									m_Data.m_PosX, m_Data.m_PosY,
									( 20 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 0.05f,
									( 20 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 0.05f,
									m_Data.m_Counter * 0.1f, true, attrEffect[ m_Data.m_ConsSkillAttr ] );
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_1 ],
									m_Data.m_PosX, m_Data.m_PosY,
									( 20 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 0.035f,
									( 20 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 0.035f,
									-m_Data.m_Counter * 0.1f, true, attrEffect[ m_Data.m_ConsSkillAttr ] );
				MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
			}
		}

		// HP/意識量の表示
		if( !m_Data.m_IsNonCollisionMode && !m_Data.m_IsBoss ){
			MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CONS_BAR_TEXTURE ],
								m_Data.m_PosX + 5.0f, m_Data.m_PosY - 5.0f,
								m_Data.m_ConsGauge / 150.0f, 0.3f, false, 0xAAFFFFFF );
			if( m_Data.m_HP > 0 ){
				MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_HP_BAR_TEXTURE ],
									m_Data.m_PosX + 5.0f, m_Data.m_PosY - 10.0f,
									m_Data.m_HP * 1.5f / m_Data.m_MaxHP, 0.3f, false, 0xAAFFFFFF );
			}
		}

		if( m_Data.m_ImgID == -1 ){
			MAPIL::DrawString( m_Data.m_PosX, m_Data.m_PosY, "▼" );
		}
		else{
			if( !m_Data.m_Is3D ){
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pStageResourceMap->m_TextureMap[ m_Data.m_ImgID ],
									m_Data.m_PosX, m_Data.m_PosY );
			}
			else{
				MAPIL::EndRendering2DGraphics();
				MAPIL::AddModelOn2DBatchWork(	m_Data.m_pResouceMap->m_pStageResourceMap->m_ModelMap[ m_Data.m_ImgID ],
												m_Data.m_PosX, m_Data.m_PosY, m_Data.m_PosZ,
												m_Data.m_ScaleX, m_Data.m_ScaleY, m_Data.m_ScaleZ,
												m_Data.m_RotX, m_Data.m_RotY, m_Data.m_RotZ );
				MAPIL::BeginRendering2DGraphics();
			}
		}
	}

#elif defined ( USE_GAME_UNIT )
void Enemy::Draw()
	{
		float posX = m_Data.m_GUData.m_PosX.GetFloat();
		float posY = m_Data.m_GUData.m_PosY.GetFloat();
		float posZ = m_Data.m_GUData.m_PosZ.GetFloat();
		float colRadius = m_Data.m_GUData.m_ColRadius.GetFloat();

		int attrEffect[] = { 0xFFFFFFFF, 0xFF33FF33, 0xFF3333FF, 0xFFFF3333 };
		if( m_Data.m_IsConsSkillMode ){
			// 効果音再生
			if( m_PrivateData.m_ConsSkillEffectCounter == 1 ){
				MAPIL::PlayStaticBuffer( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_EFFECT_CONS_SKILL_2 ] );
			}
			else if( m_PrivateData.m_ConsSkillEffectCounter == 60 ){
				MAPIL::PlayStaticBuffer( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_EFFECT_CONS_SKILL_1 ] );
				MAPIL::PlayStaticBuffer( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_EFFECT_CONS_SKILL_3 ] );
			}

			// エフェクト表示
			float offsets[ 10 ] = { 0.03f, 0.04f, -0.01f, 0.01f, 0.0f, 0.06f, 0.1f, -0.1f, -0.3f, 1.0f };
			for( int i = 0; i < 10; ++i ){
				if( m_PrivateData.m_ConsSkillEffectCounter < i * 3 + 10 && m_PrivateData.m_ConsSkillEffectCounter >= i * 3 ){
					int counter = m_PrivateData.m_ConsSkillEffectCounter - i * 3;
					MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_2 ],
										posX + ( 10 - counter ) * 15.0f * ::sin( MAPIL::DegToRad( i * 36 ) + offsets[ i ] ),
										posY + ( 10 - counter ) * 15.0f * ::cos( MAPIL::DegToRad( i * 36 ) + offsets[ i ] ),
										true, attrEffect[ m_Data.m_ConsSkillAttr ] );
				}
			}
			if( m_PrivateData.m_ConsSkillEffectCounter <= 30 && m_PrivateData.m_ConsSkillEffectCounter > 20 ){
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_3 ],
									posX, posY,
									( 30 - m_PrivateData.m_ConsSkillEffectCounter ) * 0.8f,
									( 30 - m_PrivateData.m_ConsSkillEffectCounter ) * 0.8f, true, attrEffect[ m_Data.m_ConsSkillAttr ] );
			}
			else if( m_PrivateData.m_ConsSkillEffectCounter > 60 ){
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_2 ],
									posX, posY, 
									m_PrivateData.m_ConsSkillEffectCounter > 80 ? 13.0f : ( m_PrivateData.m_ConsSkillEffectCounter - 54 ) * 0.5f,
									m_PrivateData.m_ConsSkillEffectCounter > 80 ? 13.0f : ( m_PrivateData.m_ConsSkillEffectCounter - 54 ) * 0.5f,
									-m_Data.m_Counter * 0.1f, true,
									0xFF << 24 );
				MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_1 ],
									posX, posY,
									m_PrivateData.m_ConsSkillEffectCounter > 80 ? 1.0f : ( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 0.05f,
									m_PrivateData.m_ConsSkillEffectCounter > 80 ? 1.0f : ( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 0.05f,
									m_Data.m_Counter * 0.1f, true, attrEffect[ m_Data.m_ConsSkillAttr ]/*0xBBBBBBBB*/ );
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_1 ],
									posX, posY,
									m_PrivateData.m_ConsSkillEffectCounter > 80 ? 0.7f : ( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 0.035f,
									m_PrivateData.m_ConsSkillEffectCounter > 80 ? 0.7f : ( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 0.035f,
									-m_Data.m_Counter * 0.1f, true, attrEffect[ m_Data.m_ConsSkillAttr ] );
				MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
				if( m_PrivateData.m_ConsSkillEffectCounter <= 260 ){
					MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_3 ],
										posX, posY,
										( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 0.3f,
										( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 0.3f, true,
										( ( 260 - m_PrivateData.m_ConsSkillEffectCounter ) ) << 24 | attrEffect[ m_Data.m_ConsSkillAttr ] & 0xFFFFFF );
				}
			}
			// 技名表示
			if( !m_Data.m_IsBoss ){
				if( m_PrivateData.m_ConsSkillEffectCounter <= 80 && m_PrivateData.m_ConsSkillEffectCounter > 60 ){
					DrawFontString(	*m_Data.m_pResouceMap.get(),
									posX + ( m_PrivateData.m_ConsSkillEffectCounter - 80 ) * 6.0f,
									posY - 20.0f,
									0.4f,
									( ( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 7 + 100 ) << 24 | 0xFF55FF, m_Data.m_ConsSkillName.c_str() );
					DrawFontString(	*m_Data.m_pResouceMap.get(),
									posX + ( 80 - m_PrivateData.m_ConsSkillEffectCounter ) * 6.0f,
									posY - 20.0f,
									0.4f,
									( ( m_PrivateData.m_ConsSkillEffectCounter - 60 ) * 7 + 100 ) << 24 | 0xFF55FF, m_Data.m_ConsSkillName.c_str() );
				}
				else if( m_PrivateData.m_ConsSkillEffectCounter > 80 ){
					DrawFontString( *m_Data.m_pResouceMap.get(), posX, posY - 20.0f, 0.4f, 0xFFFF55FF, m_Data.m_ConsSkillName.c_str() );
				}
			}
		}
		// 意識技後エフェクト
		else{
			if( m_PrivateData.m_ConsSkillEffectPostCounter <= 20 ){
				// 技名表示
				if( !m_Data.m_IsBoss ){
					DrawFontString(	*m_Data.m_pResouceMap.get(),
									posX + m_PrivateData.m_ConsSkillEffectPostCounter * 6.0f,
									posY - 20.0f,
									0.4f,
									( 240 - m_PrivateData.m_ConsSkillEffectPostCounter * 7 ) << 24 | 0xFF55FF, m_Data.m_ConsSkillName.c_str() );
					DrawFontString(	*m_Data.m_pResouceMap.get(),
									posX + ( 20 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 6.0f,
									posY - 20.0f,
									0.4f,
									( 240 - m_PrivateData.m_ConsSkillEffectPostCounter * 7 ) << 24 | 0xFF55FF, m_Data.m_ConsSkillName.c_str() );
				}

				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_2 ],
									posX, posY, 
									( 26 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 0.5f,
									( 26 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 0.5f,
									-m_Data.m_Counter * 0.1f, true,
									0xFF << 24 );
				MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_1 ],
									posX, posY,
									( 20 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 0.05f,
									( 20 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 0.05f,
									m_Data.m_Counter * 0.1f, true, attrEffect[ m_Data.m_ConsSkillAttr ] );
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_1 ],
									posX, posY,
									( 20 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 0.035f,
									( 20 - m_PrivateData.m_ConsSkillEffectPostCounter ) * 0.035f,
									-m_Data.m_Counter * 0.1f, true, attrEffect[ m_Data.m_ConsSkillAttr ] );
				MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
			}
		}

		// HP/意識量の表示
		if( !m_Data.m_IsNonCollisionMode && !m_Data.m_IsBoss ){
			MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CONS_BAR_TEXTURE ],
								posX + 5.0f, posY - 5.0f,
								m_Data.m_ConsGauge / 150.0f, 0.3f, false, 0xAAFFFFFF );
			if( m_Data.m_HP > 0 ){
				MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_HP_BAR_TEXTURE ],
									posX + 5.0f, posY - 10.0f,
									m_Data.m_HP * 1.5f / m_Data.m_MaxHP, 0.3f, false, 0xAAFFFFFF );
			}
		}

		if( m_Data.m_ImgID == -1 ){
			MAPIL::DrawString( posX, posY, "▼" );
		}
		else{
			if( !m_Data.m_Is3D ){
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pStageResourceMap->m_TextureMap[ m_Data.m_ImgID ],
									posX, posY );
			}
			else{
				MAPIL::EndRendering2DGraphics();
				MAPIL::AddModelOn2DBatchWork(	m_Data.m_pResouceMap->m_pStageResourceMap->m_ModelMap[ m_Data.m_ImgID ],
												posX, posY, posZ,
												m_Data.m_ScaleX, m_Data.m_ScaleY, m_Data.m_ScaleZ,
												m_Data.m_RotX, m_Data.m_RotY, m_Data.m_RotZ );
				MAPIL::BeginRendering2DGraphics();
			}
		}
	}
#endif

	bool Enemy::Update()
	{
		if( m_Data.m_Paused ){
			return true;
		}

		if( m_VM.Terminated() ){
			return false;
		}
		m_VM.Run();

		// 保持している、敵のショットグループの更新
		for( int i = 0; i < m_Data.m_ShotGroupList.size(); ++i ){
			if( m_Data.m_ShotGroupList[ i ] != NULL ){
				if( !m_Data.m_ShotGroupList[ i ]->Update() ){
					MAPIL::SafeDelete( m_Data.m_ShotGroupList[ i ] );
				}
			}
		}

		++m_Data.m_Counter;
		if( m_Data.m_IsConsSkillMode ){
			++m_PrivateData.m_ConsSkillEffectCounter;
			m_PrivateData.m_ConsSkillEffectPostCounter = 0;
		}
		else{
			m_PrivateData.m_ConsSkillEffectCounter = 0;
			++m_PrivateData.m_ConsSkillEffectPostCounter;
		}

		return true;
	}

	void Enemy::Colided( CollisionObject* pObject )
	{
		pObject->ProcessCollision( this );
	}

	void Enemy::ProcessCollision( Player* pPlayer )
	{
	}

	void Enemy::ProcessCollision( Enemy* pEnemy )
	{
	}

#if defined ( USE_FLOATING_POINT )
	void Enemy::ProcessCollision( PlayerShot* pPlayerShot )
	{
		if( m_Data.m_IsNonCollisionMode ){
			return;
		}

		if( !m_Data.m_Destroyed ){
			m_Data.m_pStageData->m_FrameGameData.m_Score += 10;
			// 無敵状態でない時
			if( !m_Data.m_IsInvincibleMode ){

				int damage = 0;
				// 属性がある場合(意識技使用時+複合属性ではない場合)
				if( m_Data.m_IsConsSkillMode && m_Data.m_ConsSkillAttr <= 3 && m_Data.m_ConsSkillAttr > 0 ){
					// 属性が一致している時
					if( m_Data.m_ConsSkillAttr == pPlayerShot->GetConsAttr() ){
						damage = 0;	// ダメージ無効化
					}
					// 属性勝ちしている時
					// (緑->青, 青->赤, 赤->緑)
					else if(	m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_GREEN && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_BLUE ||
								m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_BLUE && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_RED ||
								m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_RED && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_GREEN ){
						damage = 0;		// ダメージ無効化
					}
					// 属性負けしている時
					// (青->緑, 赤->青, 緑->赤)
					else if(	m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_BLUE && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_GREEN ||
								m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_RED && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_BLUE ||
								m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_GREEN && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_RED ){
						damage = pPlayerShot->GetShotPower() * 2;		// ダメージ2倍
						
					}
				}
				// 属性が無い場合
				else{
					damage = pPlayerShot->GetShotPower();
				}

				m_Data.m_HP -= damage;
				if( m_Data.m_IsBoss ){
					StageMessage msg;
					msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_BOSS_DAMAGED;
					m_Data.m_pStageData->m_MsgQueue.push( msg );
				}
			}

			// 最後のダメージを受けた時に発行
			if( !m_SentLastDamagedMsg ){
				if( m_Data.m_HP > 0 ){
					StageMessage msg;
					msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_ENEMY_DAMAGED;
					StageMessage::StageMessageData data;
					data.m_pString = new std::string ( m_Data.m_Name );
					msg.m_MsgDataList.push_back( data );
					data.m_Integer = m_Data.m_HP;
					msg.m_MsgDataList.push_back( data );
					data.m_Integer = m_Data.m_MaxHP;
					msg.m_MsgDataList.push_back( data );
					data.m_Integer = m_Data.m_ConsGauge;
					msg.m_MsgDataList.push_back( data );
					data.m_Integer = 200;
					msg.m_MsgDataList.push_back( data );
					m_Data.m_pStageData->m_MsgQueue.push( msg );
					m_SentLastDamagedMsg = true;
				}
			}
			if( m_Data.m_HP <= 0 ){
				StageMessage msg;
				msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_ENEMY_DAMAGED;
				StageMessage::StageMessageData data;
				data.m_pString = new std::string ( "" );
				msg.m_MsgDataList.push_back( data );
				data.m_Integer = 0;
				msg.m_MsgDataList.push_back( data );
				data.m_Integer = 10000;
				msg.m_MsgDataList.push_back( data );
				data.m_Integer = 0;
				msg.m_MsgDataList.push_back( data );
				data.m_Integer = 10000;
				msg.m_MsgDataList.push_back( data );
				m_Data.m_pStageData->m_MsgQueue.push( msg );
				m_SentLastDamagedMsg = true;
			}



			if( m_Data.m_HP <= 0 ){
				m_Data.m_HP = 0;
				m_Data.m_pStageData->m_FrameGameData.m_Score += m_Data.m_Score;
				++m_Data.m_pStageData->m_FrameGameData.m_Killed;
				m_Data.m_Destroyed = true;
			}
			Effect* pEffect = m_Data.m_pStageData->m_ObjBuilder.CreateEffect( EFFECT_ID_PLAYER_SHOT_COLLIDED, 0 );
			float x;
			float y;
			pPlayerShot->GetPos( &x, &y );
			pEffect->SetPos( x, y );
			m_Data.m_pStageData->m_EffectList.push_back( pEffect );
		}
		else{
			if( m_Data.m_HP > 0 ){
				int dummy;
				dummy += 1;
			}
		}
	}

#elif defined ( USE_GAME_UNIT )
	void Enemy::ProcessCollision( PlayerShot* pPlayerShot )
	{
		// 非衝突モード
		if( m_Data.m_IsNonCollisionMode ){
			return;
		}

		// 属性攻撃を受けたことを通知
		if( pPlayerShot->GetConsAttr() >= PLAYER_CONS_MODE_GREEN ){
			m_Data.m_StatusFlags.set( EnemyData::DAMAGED_BY_CONS_SHOT );
			m_Data.m_StatusFlags.set( EnemyData::DAMAGED_BY_CONS_SHOT_GREEN + pPlayerShot->GetConsAttr() - 1 );
		}
		else{
			m_Data.m_StatusFlags.reset( EnemyData::DAMAGED_BY_CONS_SHOT );
			for( int i = 0; i < PLAYER_CONS_MODE_RED; ++i ){
				m_Data.m_StatusFlags.reset( EnemyData::DAMAGED_BY_CONS_SHOT_GREEN + i );
			}
		}

		// 撃破時は終了
		if( m_Data.m_Destroyed ){
			return;
		}

		//-----------------------
		// 衝突判定
		//-----------------------

		m_Data.m_pStageData->m_FrameGameData.m_Score += 10;
		// 無敵状態でない時
		if( !m_Data.m_IsInvincibleMode ){

			int damage = 0;
			int gaugeDelta = 0;
			int scoreDelta = 0;

			// 属性判定は、意識技の属性 -> 先天属性 の順に判定が行われる

			// 属性がある場合(意識技使用時+複合属性ではない場合)
			if( m_Data.m_IsConsSkillMode && m_Data.m_ConsSkillAttr <= 3 ){
				// 属性が一致している時
				if( m_Data.m_ConsSkillAttr == pPlayerShot->GetConsAttr() ){
					damage = 0;			// ダメージ無効化
					if( m_Data.m_ConsSkillAttr != PLAYER_CONS_MODE_NORMAL ){
						gaugeDelta += 1;	// ゲージ1回復
					}
					scoreDelta = 0;		// スコアボーナス変更なし
				}
				// 属性勝ちしている時
				// (緑->青, 青->赤, 赤->緑)
				else if(	m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_GREEN && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_BLUE ||
							m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_BLUE && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_RED ||
							m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_RED && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_GREEN ){
					damage = 0;			// ダメージ無効化
					gaugeDelta = 0;		// ゲージ変更なし
					scoreDelta += 2 * pPlayerShot->GetShotPower();	// スコアボーナス増加（スキル中は2倍）
				}
				// 属性負けしている時
				// (青->緑, 赤->青, 緑->赤)
				else if(	m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_BLUE && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_GREEN ||
							m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_RED && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_BLUE ||
							m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_GREEN && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_RED ){
					damage = pPlayerShot->GetShotPower() * 2;		// ダメージ2倍
					gaugeDelta -= 1;		// ゲージ1減少
					scoreDelta -= pPlayerShot->GetShotPower();		// スコアボーナス減少
					m_Data.m_DamagedConsGauge += 1;
				}
			}
			// 先天属性がある場合（意識技未使用時+属性がある時）
			else if( !m_Data.m_IsConsSkillMode && m_Data.m_ConsType > 0 ){
				// 属性が一致している時
				if( m_Data.m_ConsType == pPlayerShot->GetConsAttr() ){
					damage = pPlayerShot->GetShotPower();		// 通常ダメージ
					gaugeDelta = 0;		// ゲージ変更なし
					scoreDelta -= pPlayerShot->GetShotPower();		// スコアボーナス変更なし
				}
				// 属性勝ちしている時
				// (緑->青, 青->赤, 赤->緑)
				else if(	m_Data.m_ConsType == PLAYER_CONS_MODE_GREEN && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_BLUE ||
							m_Data.m_ConsType == PLAYER_CONS_MODE_BLUE && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_RED ||
							m_Data.m_ConsType == PLAYER_CONS_MODE_RED && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_GREEN ){
					damage = pPlayerShot->GetShotPower();		// 通常ダメージ
					gaugeDelta = 0;		// ゲージ変更なし
					scoreDelta = 0;	// スコアボーナス増加
				}
				// 属性負けしている時
				// (青->緑, 赤->青, 緑->赤)
				else if(	m_Data.m_ConsType == PLAYER_CONS_MODE_BLUE && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_GREEN ||
							m_Data.m_ConsType == PLAYER_CONS_MODE_RED && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_BLUE ||
							m_Data.m_ConsType == PLAYER_CONS_MODE_GREEN && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_RED ){
					damage = pPlayerShot->GetShotPower();		// 通常ダメージ
					gaugeDelta = 0;								// ゲージ変更なし
					scoreDelta = pPlayerShot->GetShotPower();	// スコアボーナス減少
				}
				// 属性が無い時
				else if( pPlayerShot->GetConsAttr() == 0 ){
					damage = pPlayerShot->GetShotPower();		// 通常ダメージ
					gaugeDelta = 0;								// ゲージ変更なし
					scoreDelta = 0;							// スコアボーナス減少
				}
			}
			// 属性が無い場合
			else{
				damage = pPlayerShot->GetShotPower();		// 通常ダメージ
				gaugeDelta = 0;
				scoreDelta = 0;
			}

			// データの更新
			m_Data.m_HP -= damage;
			m_Data.m_ScoreBonus += scoreDelta;
			m_Data.m_ConsGauge += gaugeDelta;
			/*if( m_Data.m_ScoreBonus < 0 ){
				m_Data.m_ScoreBonus = 0;
			}*/

			if( m_Data.m_IsBoss ){
				StageMessage msg;
				msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_BOSS_DAMAGED;
				m_Data.m_pStageData->m_MsgQueue.push( msg );
			}
		}

		//------------------------------
		// Stageへのメッセージ発行機能
		//------------------------------

		// 最後のダメージを受けた時に発行
		if( !m_SentLastDamagedMsg ){
			if( m_Data.m_HP > 0 ){
				StageMessage msg;
				msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_ENEMY_DAMAGED;
				StageMessage::StageMessageData data;
				data.m_pString = new std::string ( m_Data.m_Name );
				msg.m_MsgDataList.push_back( data );
				data.m_Integer = m_Data.m_HP;
				msg.m_MsgDataList.push_back( data );
				data.m_Integer = m_Data.m_MaxHP;
				msg.m_MsgDataList.push_back( data );
				data.m_Integer = m_Data.m_ConsGauge;
				msg.m_MsgDataList.push_back( data );
				data.m_Integer = 200;
				msg.m_MsgDataList.push_back( data );
				m_Data.m_pStageData->m_MsgQueue.push( msg );
				m_SentLastDamagedMsg = true;
			}
		}
		if( m_Data.m_HP <= 0 ){
			StageMessage msg;
			msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_ENEMY_DAMAGED;
			StageMessage::StageMessageData data;
			data.m_pString = new std::string ( "" );
			msg.m_MsgDataList.push_back( data );
			data.m_Integer = 0;
			msg.m_MsgDataList.push_back( data );
			data.m_Integer = 10000;
			msg.m_MsgDataList.push_back( data );
			data.m_Integer = 0;
			msg.m_MsgDataList.push_back( data );
			data.m_Integer = 10000;
			msg.m_MsgDataList.push_back( data );
			m_Data.m_pStageData->m_MsgQueue.push( msg );
			m_SentLastDamagedMsg = true;
		}


		// 撃破時の処理
		if( m_Data.m_HP <= 0 ){
			m_Data.m_HP = 0;
			// スコア倍率を10倍したもの（難易度倍率*蓄積ポイント/最大HP）
			int scoreFact = 10 + ( ( m_Data.m_pStageData->m_Difficulty + 1 ) * 10 * m_Data.m_ScoreBonus ) / m_Data.m_MaxHP;
			if( scoreFact > ( m_Data.m_pStageData->m_Difficulty + 1 ) * 10 ){
				scoreFact = ( m_Data.m_pStageData->m_Difficulty + 1 ) * 10;
			}
			if( scoreFact < 5 ){
				scoreFact = 5;
			}
			// 獲得スコア
			int addScore = ( m_Data.m_Score * scoreFact ) / 10;

			m_Data.m_pStageData->m_FrameGameData.m_Score += addScore;
			
			// 統計情報の更新
			auto it = m_Data.m_pStageData->m_StageStat.m_EnemyStat.find( m_Data.m_Name );
			if( it == m_Data.m_pStageData->m_StageStat.m_EnemyStat.end() ){
				EnemyStat stat;
				m_Data.m_pStageData->m_StageStat.m_EnemyStat[ m_Data.m_Name ] = stat;
			}
			++m_Data.m_pStageData->m_StageStat.m_EnemyStat[ m_Data.m_Name ].m_Destroy;
			// 減少させた分のゲージのクリスタルを取得
			if( m_Data.m_DamagedConsGauge >= 5 ){
				std::shared_ptr < Item > pNewItem;
				pNewItem.reset( m_Data.m_pStageData->m_ObjBuilder.CreateItem( ITEM_ID_CRYSTAL, m_Data.m_DamagedConsGauge / 5 ) );
				pNewItem->SetPos( m_Data.m_GUData.m_PosX + GameUnit( 70 ), m_Data.m_GUData.m_PosY );
				m_Data.m_pStageData->m_ItemList.push_back( pNewItem );
			}
			// 撃破数加算
			++m_Data.m_pStageData->m_FrameGameData.m_Killed;
			m_Data.m_Destroyed = true;

			// 撃破エフェクトの作成
			Effect* pEffect = m_Data.m_pStageData->m_ObjBuilder.CreateEffect( EFFECT_ID_ENEMY_DESTORYED, 0 );
			pEffect->SetPos( m_Data.m_GUData.m_PosX.GetFloat(), m_Data.m_GUData.m_PosY.GetFloat() );
			pEffect->SetReg( 0, 1 );
			pEffect->SetReg( 1, scoreFact );
			pEffect->SetReg( 2, addScore );
			m_Data.m_pStageData->m_EffectList.push_back( pEffect );
		}

		// 撃破エフェクトの作成
		Effect* pEffect = m_Data.m_pStageData->m_ObjBuilder.CreateEffect( EFFECT_ID_PLAYER_SHOT_COLLIDED, 0 );
		GameUnit x;
		GameUnit y;
		pPlayerShot->GetPos( &x, &y );
		pEffect->SetPos( x.GetFloat(), y.GetFloat() );
		m_Data.m_pStageData->m_EffectList.push_back( pEffect );
	}
#endif

	void Enemy::ProcessCollision( EnemyShot* pEnemyShot )
	{
	}

	void Enemy::ProcessCollision( Item* pItem )
	{
		if( m_Data.m_IsNonCollisionMode ){
			return;
		}
		
		if( !pItem->CanBeConsumed() ){
			return;
		}

		int id = pItem->GetItemID();
		int subID = pItem->GetItemSubID();

		if( id == ITEM_ID_CRYSTAL ){
			m_Data.m_ConsGauge += subID * 5;
			if( m_Data.m_ConsGauge > 200 ){
				m_Data.m_ConsGauge = 200;
			}
		}

		pItem->Consume();
	}

	void Enemy::Damage( int val )
	{
		m_Data.m_HP -= val;
	}

#if defined ( USE_FLOATING_POINT )
	void Enemy::GetPos( float* pPosX, float* pPosY )
	{
		*pPosX = m_Data.m_PosX;
		*pPosY = m_Data.m_PosY;
	}
#elif defined ( USE_GAME_UNIT )
	void Enemy::GetPos( GameUnit* pPosX, GameUnit* pPosY )
	{
		*pPosX = m_Data.m_GUData.m_PosX;
		*pPosY = m_Data.m_GUData.m_PosY;
	}
#endif

	int Enemy::GetHP() const
	{
		return m_Data.m_HP;
	}

	int Enemy::GetMaxHP() const
	{
		return m_Data.m_MaxHP;
	}

	int Enemy::GetConsGauge() const
	{
		return m_Data.m_ConsGauge;
	}

	int Enemy::GetMaxConsGauge() const
	{
		return m_Data.m_MaxConsGauge;
	}

#if defined ( USE_FLOATING_POINT )
	float Enemy::GetCollisionRadius()
	{
		return m_Data.m_ColRadius;
	}
#elif defined ( USE_GAME_UNIT )
	GameUnit Enemy::GetCollisionRadius()
	{
		return m_Data.m_GUData.m_ColRadius;
	}
#endif

	void Enemy::Pause()
	{
		m_Data.m_Paused = true;
	}

	void Enemy::Resume()
	{
		m_Data.m_Paused = false;
	}

	void Enemy::ClearLastDamagedMsg()
	{
		m_SentLastDamagedMsg = false;
	}

	bool Enemy::IsNonCollisionMode() const
	{
		return m_Data.m_IsNonCollisionMode;
	}

	void Enemy::SetReg( int regNo, int val )
	{
		if( regNo < MAX_ENEMY_REGS && regNo >= 0 ){
			m_Data.m_Regs[ regNo ] = val;
		}
	}

	void Enemy::SendEvent( int id )
	{
		m_Data.m_EventQueue.push( id );
	}

	int Enemy::GetSubID() const
	{
		return m_Data.m_SubID;
	}

	std::string Enemy::GetName() const
	{
		return m_Data.m_Name;
	}

	bool Enemy::IsInSkillMode() const
	{
		return m_Data.m_IsConsSkillMode;
	}

	bool Enemy::m_SentLastDamagedMsg = false;
}
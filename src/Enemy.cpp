#include <MAPIL/MAPIL.h>

#include "Enemy.h"
#include "ResourceTypes.h"
#include "ScriptTypes.h"
#include "Stage.h"
#include "Effect.h"
#include "PlayerShot.h"
#include "EnemyShotGroup.h"
#include "ResourceID.h"

namespace GameEngine
{
	Enemy::Enemy(	std::shared_ptr < ResourceMap > pMap,
					std::shared_ptr < EnemyScriptData > pData,
					int id,
					StageData* pStageData ) :	m_VM(),
												m_ScriptID( id ),
												m_pScriptData( pData )
	{
		m_Data.m_pStageData = pStageData;
		m_Data.m_Destroyed = false;
		m_Data.m_pResouceMap = pMap;
		m_Data.m_ColRadius = 0.0f;
		m_Data.m_PosX = 0.0f;
		m_Data.m_PosY = 0.0f;
		m_Data.m_Score = 0;
		m_Data.m_HP = 2000;
		m_Data.m_MaxHP = 2000;
		m_Data.m_Counter = 0;
		m_Data.m_ConsGauge = 200;
		m_Data.m_MaxConsGauge = 200;
		m_Data.m_IsBoss = false;
		m_Data.m_IsInvincibleMode = false;
		m_Data.m_ConsType = 0;
		m_Data.m_ShotGroupList.clear();


		//m_PrivateData.m_ConsSkillModeData.m_Counter = -1;
		//m_PrivateData.m_ConsSkillModeData.m_IsConsSkillMode = false;
		//m_PrivateData.m_ConsSkillModeData.m_PostCounter = 0;
		//m_PrivateData.m_ConsSkillModeData.m_SkillName = "Unknown Skill";
		//m_PrivateData.m_DamagedCounter = 0;
		m_PrivateData.m_PrevConsGauge = 200;
		//m_PrivateData.m_SkillUsedCounter = 0;
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

	void Enemy::Init( float posX, float posY )
	{
		m_Data.m_PosX = posX;
		m_Data.m_PosY = posY;
		m_VM.Init( &m_pScriptData->m_pElm[ m_ScriptID ].m_Data, &m_Data );
	}

	void Enemy::Draw()
	{
		MAPIL::DrawString( m_Data.m_PosX, m_Data.m_PosY, "▼" );
		if( m_Data.m_IsBoss ){
			//MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
			//					150.0f, 10.0f,
			//					m_Data.m_HP * 20.0f / m_Data.m_MaxHP, 0.5f, false, 0xCCFF0000 );
			//MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
			//					150.0f, 20.0f,
			//					m_Data.m_ConsGauge * 10.0f / 1000.0f, 0.5f, false, 0xCC00FF00 );
			//if( m_PrivateData.m_DamagedCounter >= 5 ){
			//	MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
			//	MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
			//						150.0f, 10.0f,
			//						m_Data.m_HP * 20.0f / m_Data.m_MaxHP, 0.5f, false,
			//						( m_PrivateData.m_DamagedCounter * 10 ) << 24 | 0xFFFFFF );
			//	MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
			//}
			//if( m_PrivateData.m_SkillUsedCounter >= 0 ){
			//	if( m_PrivateData.m_SkillUsedCounter >= 30 ){
			//		MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
			//							150.0f + m_Data.m_ConsGauge * 16.0f / 100.0f, 20.0f,
			//							( m_PrivateData.m_PrevConsGauge - m_Data.m_ConsGauge ) * 10.0f / 1000.0f, 0.5f, false,
			//							0xCC00FF00 );
			//		if( ( m_PrivateData.m_SkillUsedCounter % 4 ) == 0 ){
			//			MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
			//			MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
			//								150.0f + m_Data.m_ConsGauge * 16.0f / 100.0f, 20.0f,
			//								( m_PrivateData.m_PrevConsGauge - m_Data.m_ConsGauge ) * 10.0f / 1000.0f, 0.5f, false,
			//								0x99FFFFFF );
			//			MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
			//		}
			//	}
			//	else{
			//		float scale = 1.5f - m_PrivateData.m_SkillUsedCounter / 30.0f;
			//		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
			//		int length = m_PrivateData.m_PrevConsGauge - m_Data.m_ConsGauge;
			//		//MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
			//		//					150.0f + m_Data.m_ConsGauge * 16.0f / 100.0f, 20.0f,
			//		//					( m_PrivateData.m_PrevConsGauge - m_Data.m_ConsGauge ) * 10.0f / 1000.0f + scale, 0.5f + scale, false,
			//		//					( m_PrivateData.m_SkillUsedCounter * 10 ) << 24 | 0x00FF00 );
			//		MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
			//							150.0f + ( m_Data.m_ConsGauge + length / 2 ) * 16.0f / 100.0f, 24.0f,
			//							( m_PrivateData.m_PrevConsGauge - m_Data.m_ConsGauge ) * 10.0f / 1000.0f + scale, 0.5f + scale, true,
			//							( m_PrivateData.m_SkillUsedCounter * 5 ) << 24 | ( m_PrivateData.m_SkillUsedCounter * 10 ) << 8 );
			//		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
			//	}
			//}
			//for( int i = 0; i < 6; ++i ){
			//	MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
			//						150.0f + i * 64.0f, 10.0f,
			//						0.1f, 0.5f, false, 0xFFFFFFFF );
			//}
			//MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
			//						246.0f, 20.0f,
			//						0.1f, 0.5f, false, 0xFFFFFFFF );
			//
			////// スキル時のエフェクト
			////if( m_PrivateData.m_ConsSkillModeData.m_IsConsSkillMode ){
			////	if( m_PrivateData.m_ConsSkillModeData.m_Counter < 30 ){
			////		MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
			////							100.0f, 20.0f,
			////							0.1f, 0.5f, false, 0xFFFFFFFF );
			////	}
			////	else{
			////	}
			////}
			////else{
			////}

		}
		else{
			MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CONS_BAR_TEXTURE ],
								m_Data.m_PosX + 5.0f, m_Data.m_PosY - 5.0f,
								m_Data.m_ConsGauge / 150.0f, 0.3f, false, 0xAAFFFFFF );
			MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_HP_BAR_TEXTURE ],
								m_Data.m_PosX + 5.0f, m_Data.m_PosY - 10.0f,
								m_Data.m_HP * 1.5f / m_Data.m_MaxHP, 0.3f, false, 0xAAFFFFFF );
		}
	}

	bool Enemy::Update()
	{
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
		//--m_PrivateData.m_DamagedCounter;
		//--m_PrivateData.m_SkillUsedCounter;

		//if( m_PrivateData.m_SkillUsedCounter < 0 ){
		//	if( m_PrivateData.m_PrevConsGauge - m_Data.m_ConsGauge > 20 ){
		//		m_PrivateData.m_SkillUsedCounter = 60;
		//	}
		//	else{
		//		m_PrivateData.m_PrevConsGauge = m_Data.m_ConsGauge;
		//	}
		//}
		//else if( m_PrivateData.m_SkillUsedCounter == 0 ){
		//	m_PrivateData.m_PrevConsGauge = m_Data.m_ConsGauge;
		//}

		//if( m_PrivateData.m_ConsSkillModeData.m_IsConsSkillMode ){
		//	++m_PrivateData.m_ConsSkillModeData.m_Counter;
		//	m_PrivateData.m_ConsSkillModeData.m_PostCounter = 0;
		//}
		//else{
		//	m_PrivateData.m_ConsSkillModeData.m_Counter = 0;
		//	++m_PrivateData.m_ConsSkillModeData.m_PostCounter;
		//}

		return true;
	}

	//void Enemy::InvokeConsSkill( std::string skillName )
	//{
	//	m_PrivateData.m_ConsSkillModeData.m_SkillName = skillName;
	//	m_PrivateData.m_ConsSkillModeData.m_IsConsSkillMode = true;
	//}

	//void Enemy::StopConsSkill()
	//{
	//	m_PrivateData.m_ConsSkillModeData.m_IsConsSkillMode = false;
	//	m_PrivateData.m_ConsSkillModeData.m_SkillName = "";
	//}

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

	void Enemy::ProcessCollision( PlayerShot* pPlayerShot )
	{
		if( !m_Data.m_Destroyed ){
			m_Data.m_pStageData->m_FrameGameData.m_Score += 10;
			// 無敵状態でない時
			if( !m_Data.m_IsInvincibleMode ){
				m_Data.m_HP -= pPlayerShot->GetShotPower();
				//if( m_PrivateData.m_DamagedCounter <= 5 ){
				//	m_PrivateData.m_DamagedCounter = 10;
				//}
				if( m_Data.m_IsBoss ){
					StageMessage msg;
					msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_BOSS_DAMAGED;
					m_Data.m_pStageData->m_MsgQueue.push( msg );
				}
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

	void Enemy::ProcessCollision( EnemyShot* pEnemyShot )
	{
	}

	void Enemy::ProcessCollision( Item* pItem )
	{
	}

	void Enemy::GetPos( float* pPosX, float* pPosY )
	{
		*pPosX = m_Data.m_PosX;
		*pPosY = m_Data.m_PosY;
	}

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

	float Enemy::GetCollisionRadius()
	{
		return m_Data.m_ColRadius;
	}
}
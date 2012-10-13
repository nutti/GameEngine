#include <MAPIL/MAPIL.h>

#include "Enemy.h"
#include "ResourceTypes.h"
#include "ScriptTypes.h"
#include "Stage.h"
#include "Effect.h"
#include "PlayerShot.h"
#include "EnemyShotGroup.h"

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
		m_Data.m_Counter = 0;
		m_Data.m_ConsGauge = 200;
		m_Data.m_IsBoss = false;
		m_Data.m_ConsType = 0;
		m_Data.m_ShotGroupList.clear();
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
		MAPIL::DrawString( m_Data.m_PosX, m_Data.m_PosY, "Бе" );
		MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CONS_BAR_TEXTURE ],
							m_Data.m_PosX + 5.0f, m_Data.m_PosY - 5.0f,
							m_Data.m_ConsGauge / 150.0f, 0.3f, false );
		MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_HP_BAR_TEXTURE ],
							m_Data.m_PosX + 5.0f, m_Data.m_PosY - 10.0f,
							m_Data.m_HP * 1.5f / m_Data.m_MaxHP, 0.3f, false );
	}

	bool Enemy::Update()
	{
		if( m_VM.Terminated() ){
			return false;
		}
		m_VM.Run();

		for( int i = 0; i < m_Data.m_ShotGroupList.size(); ++i ){
			if( m_Data.m_ShotGroupList[ i ] != NULL ){
				if( !m_Data.m_ShotGroupList[ i ]->Update() ){
					MAPIL::SafeDelete( m_Data.m_ShotGroupList[ i ] );
				}
			}
		}

		//if( m_Data.m_Counter == 30 ){
		//	m_Data.m_ShotGroupList.push_back( m_Data.m_pStageData->m_ObjBuilder.CreateEnemyShotGroup( 0, &m_Data ) );
		//	int id = m_Data.m_ShotGroupList[ 0 ]->CreateShot();
		//	m_Data.m_ShotGroupList[ 0 ]->SetShotStatus( id, 250.0f, 100.0f, MAPIL::DegToRad( -90.0f ), 1.0f, 2.0f, 0 );
		//}

		//for( int i = 0; i < m_Data.m_ShotGroupList.size(); ++i ){
		//	if( m_Data.m_ShotGroupList[ i ] != NULL && m_Data.m_ShotGroupList[ i ]->IsEmpty() ){
		//		MAPIL::SafeDelete( m_Data.m_ShotGroupList[ i ] );
		//	}
		//}

		++m_Data.m_Counter;

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

	void Enemy::ProcessCollision( PlayerShot* pPlayerShot )
	{
		if( !m_Data.m_Destroyed ){
			m_Data.m_pStageData->m_FrameGameData.m_Score += 10;
			m_Data.m_HP -= pPlayerShot->GetShotPower();
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

	float Enemy::GetCollisionRadius()
	{
		return m_Data.m_ColRadius;
	}
}
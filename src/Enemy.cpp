#include <MAPIL/MAPIL.h>

#include "Enemy.h"
#include "ResourceTypes.h"
#include "ScriptTypes.h"
#include "Stage.h"

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
	}

	Enemy::~Enemy()
	{
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
	}

	bool Enemy::Update()
	{
		if( m_VM.Terminated() ){
			return false;
		}
		m_VM.Run();

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
			++m_Data.m_pStageData->m_Score;
			--m_Data.m_HP;
			if( m_Data.m_HP <= 0 ){
				m_Data.m_pStageData->m_Score += m_Data.m_Score;
				m_Data.m_Destroyed = true;
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
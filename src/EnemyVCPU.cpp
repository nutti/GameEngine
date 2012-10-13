#include "EnemyVCPU.h"

#include <MAPIL/MAPIL.h>

#include "Enemy.h"
#include "EnemyShot.h"
#include "Stage.h"
#include "Player.h"
#include "Item.h"
#include "Effect.h"
#include "EnemyShotGroup.h"

namespace GameEngine
{
	EnemyVCPU::EnemyVCPU() : VM::VCPU()
	{
	}

	EnemyVCPU::~EnemyVCPU()
	{
	}

	void EnemyVCPU::Init( VM::Data* pData, EnemyData* pEnemyData )
	{
		m_pData = pData;
		m_pEnemyData = pEnemyData;
		VM::VCPU::Init();
	}

	void EnemyVCPU::SysGetEnemyPosX()
	{
		Pop();
		Push( m_pEnemyData->m_PosX );
	}

	void EnemyVCPU::SysGetEnemyPosY()
	{
		Pop();
		Push( m_pEnemyData->m_PosY );
	}

	void EnemyVCPU::SysGetEnemyHP()
	{
		Pop();
		Push( m_pEnemyData->m_HP );
	}

	void EnemyVCPU::SysGetEnemySpeed()
	{
		Pop();
		Push( 0 );
	}

	void EnemyVCPU::SysGetEnemyCounter()
	{
		Pop();
		Push( m_pEnemyData->m_Counter );
	}

	void EnemyVCPU::SysGetEnemyCounterF()
	{
		Pop();
		Push( VM::Value( static_cast < float > ( m_pEnemyData->m_Counter ) ) );
	}

	void EnemyVCPU::SysGetEnemyAngle()
	{
		Pop();
		Push( 0 );
	}

	void EnemyVCPU::SysGetEnemyConsGauge()
	{
		Pop();
		Push( m_pEnemyData->m_ConsGauge );
	}

	void EnemyVCPU::SysSetEnemyPos()
	{
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		m_pEnemyData->m_PosX = x;
		m_pEnemyData->m_PosY = y;
	}

	void EnemyVCPU::SysSetEnemyAngle()
	{
		Pop();
		int angle = Top().m_Integer;
		Pop();
	}

	void EnemyVCPU::SysSetEnemySpeed()
	{
		Pop();
		int speed = Top().m_Integer;
		Pop();
	}

	void EnemyVCPU::SysSetEnemyHP()
	{
		Pop();
		int hp = Top().m_Integer;
		Pop();
		m_pEnemyData->m_HP = hp;
		m_pEnemyData->m_MaxHP = hp;
	}

	void EnemyVCPU::SysSetEnemyImgID()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();
	}

	void EnemyVCPU::SysSetEnemyCollisionRadius()
	{
		Pop();
		float radius = Top().m_Float;
		Pop();
		m_pEnemyData->m_ColRadius = radius;
	}

	void EnemyVCPU::SysSetEnemyScore()
	{
		Pop();
		int score = Top().m_Integer;
		Pop();
		m_pEnemyData->m_Score = score;
	}

	void EnemyVCPU::SysSetEnemyBossFlag()
	{
		Pop();
		int flag = Top().m_Integer;
		Pop();
		m_pEnemyData->m_IsBoss = flag;
	}

	void EnemyVCPU::SysSetEnemyConsGauge()
	{
		Pop();
		int gauge = Top().m_Integer;
		Pop();
		m_pEnemyData->m_ConsGauge = gauge;
	}

	void EnemyVCPU::SysCreateEnemyShot1()
	{
		Pop();
		int imgID = Top().m_Integer;
		Pop();
		float radius = Top().m_Float;
		Pop();
		float angle = Top().m_Float;
		Pop();
		float speed = Top().m_Float;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		EnemyShot* pNewShot = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEnemyShot( 0 );
		pNewShot->SetPos( x, y );
		pNewShot->SetAngle( angle );
		pNewShot->SetSpeed( speed );
		pNewShot->SetImage( imgID );
		pNewShot->SetCollisionRadius( radius );
		m_pEnemyData->m_pStageData->m_EnemyShotList.push_back( pNewShot );
	}

	void EnemyVCPU::SysGetPlayerPosX()
	{
		Pop();

		float x;
		float y;
		m_pEnemyData->m_pStageData->m_pPlayer->GetPos( &x, &y );

		Push( x );
	}

	void EnemyVCPU::SysGetPlayerPosY()
	{
		Pop();

		float x;
		float y;
		m_pEnemyData->m_pStageData->m_pPlayer->GetPos( &x, &y );

		Push( y );
	}

	void EnemyVCPU::SysGetRandF()
	{
		Pop();
		Push( m_pEnemyData->m_pStageData->m_RandGen.GetRand() * 1.0f / m_pEnemyData->m_pStageData->m_RandGen.GetRandMax() );
	}

	void EnemyVCPU::SysCreateEffect1()
	{
		Pop();

		int subID = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();

		Effect* pNewEffect = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEffect( id, subID );
		pNewEffect->SetPos( x, y );
		m_pEnemyData->m_pStageData->m_EffectList.push_back( pNewEffect );
	}

	void EnemyVCPU::SysCreateItem()
	{
		Pop();

		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int subID = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		Item* pNewItem = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateItem( id, subID );
		pNewItem->SetPos( x, y );
		m_pEnemyData->m_pStageData->m_ItemList.push_back( pNewItem );
	}

	void EnemyVCPU::SysCreateEnemyShotGroup()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		EnemyShotGroup* pNewGroup = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEnemyShotGroup( id, m_pEnemyData );
		pNewGroup->Init();
		m_pEnemyData->m_ShotGroupList.push_back( pNewGroup );
	}

	void EnemyVCPU::SysPlaySE()
	{
		Pop();
		int id = m_pEnemyData->m_pResouceMap->m_pStageResourceMap->m_SEMap[ Top().m_Integer ];
		Pop();
		MAPIL::PlayStaticBuffer( id );
	}

	void EnemyVCPU::SysStopSE()
	{
		Pop();
		int id = m_pEnemyData->m_pResouceMap->m_pStageResourceMap->m_SEMap[ Top().m_Integer ];
		Pop();
		MAPIL::StopStaticBuffer( id );
	}

	void EnemyVCPU::OpSysCall( int val )
	{
		switch( val ){
			case VM::SYS_GET_RANDOM_F:
				SysGetRandF();
				break;
			case VM::SYS_GET_PLAYER_POSX:
				SysGetPlayerPosX();
				break;
			case VM::SYS_GET_PLAYER_POSY:
				SysGetPlayerPosY();
				break;
			case VM::SYS_ENEMY_GET_POSX:
				SysGetEnemyPosX();
				break;
			case VM::SYS_ENEMY_GET_POSY:
				SysGetEnemyPosY();
				break;
			case VM::SYS_ENEMY_GET_HP:
				SysGetEnemyHP();
				break;
			case VM::SYS_ENEMY_GET_SPEED:
				SysGetEnemySpeed();
				break;
			case VM::SYS_ENEMY_GET_COUNTER:
				SysGetEnemyCounter();
				break;
			case VM::SYS_ENEMY_GET_COUNTER_F:
				SysGetEnemyCounterF();
				break;
			case VM::SYS_ENEMY_GET_CONS_GAUGE:
				SysGetEnemyConsGauge();
				break;

			case VM::SYS_ENEMY_SET_POS:
				SysSetEnemyPos();
				break;
			case VM::SYS_ENEMY_SET_ANGLE:
				SysSetEnemyAngle();
				break;
			case VM::SYS_ENEMY_SET_SPEED:
				SysSetEnemySpeed();
				break;
			case VM::SYS_ENEMY_SET_HP:
				SysSetEnemyHP();
				break;
			case VM::SYS_ENEMY_CREATE_SHOT_1:
				SysCreateEnemyShot1();
				break;
			case VM::SYS_ENEMY_SET_IMAGE:
				SysSetEnemyImgID();
				break;
			case VM::SYS_ENEMY_SET_COLLISION_RADIUS:
				SysSetEnemyCollisionRadius();
				break;
			case VM::SYS_ENEMY_SET_SCORE:
				SysSetEnemyScore();
				break;
			case VM::SYS_ENEMY_SET_BOSS_FLAG:
				SysSetEnemyBossFlag();
				break;
			case VM::SYS_ENEMY_SET_CONS_GAUGE:
				SysSetEnemyConsGauge();
				break;

			case VM::SYS_ENEMY_CREATE_EFFECT_1:
				SysCreateEffect1();
				break;
			case VM::SYS_CREATE_ITEM:
				SysCreateItem();
				break;
			case VM::SYS_ENEMY_CREATE_SHOT_GROUP:
				SysCreateEnemyShotGroup();
				break;

			case VM::SYS_PLAY_SE:
				SysPlaySE();
				break;
			case VM::SYS_STOP_SE:
				SysStopSE();
				break;

			default:
				VM::VCPU::OpSysCall( val );
				break;
		}
	}
}
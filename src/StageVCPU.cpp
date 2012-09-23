#include "StageVCPU.h"

#include "Stage.h"
#include "Enemy.h"

namespace GameEngine
{
	StageVCPU::StageVCPU() : VM::VCPU()
	{
	}

	StageVCPU::~StageVCPU()
	{
	}

	void StageVCPU::Init( VM::Data* pData, StageData* pStageData )
	{
		m_pData = pData;
		m_pStageData = pStageData;
		VM::VCPU::Init();
	}

	void StageVCPU::SysAddEnemy()
	{
		Pop();
		Enemy* pNewEnemy = m_pStageData->m_ObjBuilder.CreateEnemy( 0 );
		pNewEnemy->Init( 0.0f, 0.0f );
		m_pStageData->m_EnemyList.push_back( pNewEnemy );
	}

	void StageVCPU::SysAddEnemyIniPos()
	{
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();
		Enemy* pNewEnemy = m_pStageData->m_ObjBuilder.CreateEnemy( id );
		pNewEnemy->Init( x, y );
		m_pStageData->m_EnemyList.push_back( pNewEnemy );
	}

	void StageVCPU::SysGetFrame()
	{
		Pop();
		Push( m_pStageData->m_Frame );
	}

	void StageVCPU::SysSetFrame()
	{
		Pop();
		int frame = Top().m_Integer;
		m_pStageData->m_Frame = frame;
	}

	void StageVCPU::SysGetRand()
	{
		Pop();
		Push( m_pStageData->m_RandGen.GetRand() * 1.0f / m_pStageData->m_RandGen.GetRandMax() );
		//Push( Rand() * 1.0f / RAND_MAXIMUM );
		//Push( rand() * 1.0f / RAND_MAX );
	}

	void StageVCPU::OpSysCall( int val )
	{
		switch( val ){
			case VM::SYS_STAGE_ADD_ENEMY:
				SysAddEnemy();
				break;
			case VM::SYS_STAGE_ADD_ENEMY_INIPOS:
				SysAddEnemyIniPos();
				break;
			case VM::SYS_STAGE_GET_FRAME:
				SysGetFrame();
				break;
			case VM::SYS_STAGE_SET_FRAME:
				SysSetFrame();
				break;
			case VM::SYS_GET_RANDOM_F:
				SysGetRand();
				break;
			default:
				VM::VCPU::OpSysCall( val );
				break;
		}
	}
}
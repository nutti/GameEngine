#include "EnemyShotGroupVCPU.h"

#include <MAPIL/MAPIL.h>

#include "EnemyShotGroup.h"
#include "Enemy.h"
#include "EnemyShot.h"
#include "Stage.h"
#include "Player.h"

namespace GameEngine
{
	EnemyShotGroupVCPU::EnemyShotGroupVCPU() : VM::VCPU()
	{
	}

	EnemyShotGroupVCPU::~EnemyShotGroupVCPU()
	{
	}

	void EnemyShotGroupVCPU::Init( VM::Data* pData, EnemyShotGroupData* pEnemyShotGroupData )
	{
		m_pData = pData;
		m_pEnemyShotGroupData = pEnemyShotGroupData;
		VM::VCPU::Init();
	}

	void EnemyShotGroupVCPU::SysGetEnemyPosX()
	{
		Pop();
		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			Push( m_pEnemyShotGroupData->m_pEnemyData->m_PosX );
		}
		else{
			Push( 0.0f );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyPosY()
	{
		Pop();
		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			Push( m_pEnemyShotGroupData->m_pEnemyData->m_PosY );
		}
		else{
			Push( 0.0f );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyHP()
	{
		Pop();
		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			Push( m_pEnemyShotGroupData->m_pEnemyData->m_HP );
		}
		else{
			Push( 0 );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemySpeed()
	{
		Pop();
		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			Push( 0.0f );
		}
		else{
			Push( 0.0f );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyCounter()
	{
		Pop();
		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			Push( m_pEnemyShotGroupData->m_pEnemyData->m_Counter );
		}
		else{
			Push( -1 );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyCounterF()
	{
		Pop();
		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			Push( VM::Value( static_cast < float > ( m_pEnemyShotGroupData->m_pEnemyData->m_Counter ) ) );
		}
		else{
			Push( -1.0f );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyAngle()
	{
		Pop();
		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			Push( 0 );
		}
		else{
			Push( 0 );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyConsGauge()
	{
		Pop();
		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			Push( m_pEnemyShotGroupData->m_pEnemyData->m_ConsGauge );
		}
		else{
			Push( -1 );
		}
	}

	void EnemyShotGroupVCPU::SysGetPlayerPosX()
	{
		Pop();

		float x;
		float y;
		m_pEnemyShotGroupData->m_pStageData->m_pPlayer->GetPos( &x, &y );

		Push( x );
	}

	void EnemyShotGroupVCPU::SysGetPlayerPosY()
	{
		Pop();

		float x;
		float y;
		m_pEnemyShotGroupData->m_pStageData->m_pPlayer->GetPos( &x, &y );

		Push( y );
	}

	void EnemyShotGroupVCPU::SysGetRandF()
	{
		Pop();
		Push( m_pEnemyShotGroupData->m_pStageData->m_RandGen.GetRand() * 1.0f / m_pEnemyShotGroupData->m_pStageData->m_RandGen.GetRandMax() );
	}

	void EnemyShotGroupVCPU::SysPlaySE()
	{
		Pop();
		int id = m_pEnemyShotGroupData->m_pStageData->m_ResourceMap.m_pStageResourceMap->m_SEMap[ Top().m_Integer ];
		Pop();
		MAPIL::PlayStaticBuffer( id );
	}

	void EnemyShotGroupVCPU::SysStopSE()
	{
		Pop();
		int id = m_pEnemyShotGroupData->m_pStageData->m_ResourceMap.m_pStageResourceMap->m_SEMap[ Top().m_Integer ];
		Pop();
		MAPIL::StopStaticBuffer( id );
	}

	void EnemyShotGroupVCPU::SysCreateEnemyShot()
	{
		Pop();
		int id = -1;
		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			id = m_pEnemyShotGroupData->m_ShotTotal++;
			m_pEnemyShotGroupData->m_IsNew = false;
			m_pEnemyShotGroupData->m_pShots[ id ] = m_pEnemyShotGroupData->m_pStageData->m_ObjBuilder.CreateEnemyShot( 0 );
			m_pEnemyShotGroupData->m_pShots[ id ]->JoinShotGroup( id, m_pEnemyShotGroupData->m_pShotGroup );
			m_pEnemyShotGroupData->m_pStageData->m_EnemyShotList.push_back( m_pEnemyShotGroupData->m_pShots[ id ] );
		}
		Push( id );
	}

	void EnemyShotGroupVCPU::SysCreateConsEnemyShot()
	{
		Pop();
		int attr = Top().m_Integer;
		Pop();
		int id = -1;
		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			id = m_pEnemyShotGroupData->m_ShotTotal++;
			m_pEnemyShotGroupData->m_IsNew = false;
			m_pEnemyShotGroupData->m_pShots[ id ] = m_pEnemyShotGroupData->m_pStageData->m_ObjBuilder.CreateEnemyShot( 0 );
			m_pEnemyShotGroupData->m_pShots[ id ]->JoinShotGroup( id, m_pEnemyShotGroupData->m_pShotGroup );
			m_pEnemyShotGroupData->m_pShots[ id ]->SetConsAttr( attr );
			m_pEnemyShotGroupData->m_pStageData->m_EnemyShotList.push_back( m_pEnemyShotGroupData->m_pShots[ id ] );
		}
		Push( id );
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotPos()
	{
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetPos( x, y );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotPower()
	{
		Pop();
		int power = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetPower( power );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotAngle()
	{
		Pop();
		float angle = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetAngle( angle );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotSpeed()
	{
		Pop();
		float speed = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetSpeed( speed );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotImage()
	{
		Pop();
		int imgID = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetImage( imgID );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotCollisionRadius()
	{
		Pop();
		float radius = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetCollisionRadius( radius );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotStatus()
	{
		Pop();
		int imgID = Top().m_Integer;
		Pop();
		float radius = Top().m_Float;
		Pop();
		float speed = Top().m_Float;
		Pop();
		float angle = Top().m_Float;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( id >= 0 && m_pEnemyShotGroupData->m_pShots[ id ] != NULL ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetPos( x, y );
			m_pEnemyShotGroupData->m_pShots[ id ]->SetAngle( angle );
			m_pEnemyShotGroupData->m_pShots[ id ]->SetSpeed( speed );
			m_pEnemyShotGroupData->m_pShots[ id ]->SetCollisionRadius( radius );
			m_pEnemyShotGroupData->m_pShots[ id ]->SetImage( imgID );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotMovement()
	{
		Pop();
		float speed = Top().m_Float;
		Pop();
		float angle = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetAngle( angle );
			m_pEnemyShotGroupData->m_pShots[ id ]->SetSpeed( speed );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyShotCounter()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			Push( m_pEnemyShotGroupData->m_pShots[ id ]->GetCounter() );
		}
		else{
			Push( 0 );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyShotPosX()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();
		
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			float x;
			float y;
			m_pEnemyShotGroupData->m_pShots[ id ]->GetPos( &x, &y );
			Push( x );
		}
		else{
			Push( 0 );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyShotPosY()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();
		
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			float x;
			float y;
			m_pEnemyShotGroupData->m_pShots[ id ]->GetPos( &x, &y );
			Push( y );
		}
		else{
			Push( 0 );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyShotPower()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			Push( m_pEnemyShotGroupData->m_pShots[ id ]->GetPower() );
		}
		else{
			Push( 0 );
		}
	}

	void EnemyShotGroupVCPU::SysGetStageFrame()
	{
		Pop();
		Push( m_pEnemyShotGroupData->m_pStageData->m_Frame );
	}

	void EnemyShotGroupVCPU::SysGetEnemyShotGroupReg()
	{
		Pop();
		Push( m_pEnemyShotGroupData->m_Reg );
	}

	void EnemyShotGroupVCPU::SysEnemyShotAddAngle()
	{
		Pop();
		float angle = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->AddAngle( angle );
		}
	}

	void EnemyShotGroupVCPU::SysEnemyShotSetImgScale()
	{
		Pop();
		float scale = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetImageScale( scale );
		}
	}

	void EnemyShotGroupVCPU::SysEnemyShotAddPos()
	{
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->AddPos( x, y );
		}
	}

	void EnemyShotGroupVCPU::SysEnemyShotAddSpeed()
	{
		Pop();
		float speed = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->AddSpeed( speed );
		}
	}

	void EnemyShotGroupVCPU::OpSysCall( int val )
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

			case VM::SYS_PLAY_SE:
				SysPlaySE();
				break;
			case VM::SYS_STOP_SE:
				SysStopSE();
				break;

			case VM::SYS_ENEMY_SHOT_GROUP_CREATE_SHOT:
				SysCreateEnemyShot();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_CREATE_CONS_SHOT:
				SysCreateConsEnemyShot();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_POS:
				SysSetEnemyShotPos();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_POWER:
				SysSetEnemyShotPower();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_ANGLE:
				SysSetEnemyShotAngle();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_SPEED:
				SysSetEnemyShotSpeed();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_IMAGE:
				SysSetEnemyShotImage();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_COLLISION_RADIUS:
				SysSetEnemyShotCollisionRadius();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_STATUS:
				SysSetEnemyShotStatus();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_MOVEMENT:
				SysSetEnemyShotMovement();
				break;
			
			case VM::SYS_ENEMY_SHOT_GROUP_GET_COUNTER:
				SysGetEnemyShotCounter();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_GET_POS_X:
				SysGetEnemyShotPosX();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_GET_POS_Y:
				SysGetEnemyShotPosY();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_GET_POWER:
				SysGetEnemyShotPower();
				break;

			case VM::SYS_ENEMY_SHOT_GROUP_ADD_ANGLE:
				SysEnemyShotAddAngle();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_IMAGE_SCALE:
				SysEnemyShotSetImgScale();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_ADD_POS:
				SysEnemyShotAddPos();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_ADD_SPEED:
				SysEnemyShotAddSpeed();
				break;

			case VM::SYS_STAGE_GET_FRAME:
				SysGetStageFrame();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_GET_REG:
				SysGetEnemyShotGroupReg();
				break;

			default:
				VM::VCPU::OpSysCall( val );
				break;
		}
	}
}
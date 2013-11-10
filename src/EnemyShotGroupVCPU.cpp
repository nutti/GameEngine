#include "EnemyShotGroupVCPU.h"

#include <MAPIL/MAPIL.h>

#include "EnemyShotGroup.h"
#include "Enemy.h"
#include "EnemyShot.h"
#include "Stage.h"
#include "Player.h"
#include "ScriptEffect.h"

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
#if defined ( USE_FLOATING_POINT )
			Push( m_pEnemyShotGroupData->m_pEnemyData->m_PosX );
#elif defined ( USE_GAME_UNIT )
			Push( m_pEnemyShotGroupData->m_pEnemyData->m_GUData.m_PosX.GetFloat() );
#endif
		}
		else{
			Push( 0.0f );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyPosY()
	{
		Pop();
		if( m_pEnemyShotGroupData->m_EnemyControlled ){
#if defined ( USE_FLOATING_POINT )
			Push( m_pEnemyShotGroupData->m_pEnemyData->m_PosY );
#elif defined ( USE_GAME_UNIT )
			Push( m_pEnemyShotGroupData->m_pEnemyData->m_GUData.m_PosY.GetFloat() );
#endif
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

	void EnemyShotGroupVCPU::SysGetEnemyRotAngle()
	{
		Pop();
		int axis = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			switch( axis ){
				case 0:
					Push( m_pEnemyShotGroupData->m_pEnemyData->m_RotX );
					break;
				case 1:
					Push( m_pEnemyShotGroupData->m_pEnemyData->m_RotY );
					break;
				case 2:
					Push( m_pEnemyShotGroupData->m_pEnemyData->m_RotZ );
					break;
				default:
					throw MAPIL::MapilException( CURRENT_POSITION, TSTR( "Axis must be 0-2" ), -1 );
					break;
			}
		}
		else{
			Push( 0.0f );
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
#if defined ( USE_FLOATING_POINT )
		float x;
		float y;
		m_pEnemyShotGroupData->m_pStageData->m_pPlayer->GetPos( &x, &y );
		Push( x );
#elif defined ( USE_GAME_UNIT )
		GameUnit x;
		GameUnit y;
		m_pEnemyShotGroupData->m_pStageData->m_pPlayer->GetPos( &x, &y );
		Push( x.GetFloat() );
#endif
	}

	void EnemyShotGroupVCPU::SysGetPlayerPosY()
	{
		Pop();

#if defined ( USE_FLOATING_POINT )
		float x;
		float y;
		m_pEnemyShotGroupData->m_pStageData->m_pPlayer->GetPos( &x, &y );
		Push( y );
#elif defined ( USE_GAME_UNIT )
		GameUnit x;
		GameUnit y;
		m_pEnemyShotGroupData->m_pStageData->m_pPlayer->GetPos( &x, &y );
		Push( y.GetFloat() );
#endif
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

	void EnemyShotGroupVCPU::SysCreateEnemyShotGroup()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			EnemyShotGroup* pNewGroup = m_pEnemyShotGroupData->m_pStageData->m_ObjBuilder.CreateEnemyShotGroup( id, m_pEnemyShotGroupData->m_pEnemyData );
			pNewGroup->Init();
			pNewGroup->SetMasterEnemyName( m_pEnemyShotGroupData->m_pEnemyData->m_Name );
			m_pEnemyShotGroupData->m_pEnemyData->m_ShotGroupList.push_back( pNewGroup );
		}
	}

	void EnemyShotGroupVCPU::SysCreateEnemyShotGroupReg()
	{
		Pop();
		int reg = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			EnemyShotGroup* pNewGroup = m_pEnemyShotGroupData->m_pStageData->m_ObjBuilder.CreateEnemyShotGroup( id, m_pEnemyShotGroupData->m_pEnemyData );
			pNewGroup->Init();
			pNewGroup->SetReg( reg );
			pNewGroup->SetMasterEnemyName( m_pEnemyShotGroupData->m_pEnemyData->m_Name );
			m_pEnemyShotGroupData->m_pEnemyData->m_ShotGroupList.push_back( pNewGroup );
		}
	}

	void EnemyShotGroupVCPU::SysCreateEnemyShotGroupGReg5()
	{
		GameUnit reg[ 5 ];

		Pop();
		for( int i = 4; i >= 0; --i ){
			reg[ i ].SetRawValue( Top().m_Integer );
			Pop();
		}
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			EnemyShotGroup* pNewGroup = m_pEnemyShotGroupData->m_pStageData->m_ObjBuilder.CreateEnemyShotGroup( id, m_pEnemyShotGroupData->m_pEnemyData );
			pNewGroup->Init();
			for( int i = 0; i < 5; ++i ){
				pNewGroup->SetGReg( i, reg[ i ] );
			}
			pNewGroup->SetMasterEnemyName( m_pEnemyShotGroupData->m_pEnemyData->m_Name );
			m_pEnemyShotGroupData->m_pEnemyData->m_ShotGroupList.push_back( pNewGroup );
		}
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
		
		if( id < 0 ){
			return;
		}

		if( m_pEnemyShotGroupData->m_pShots[ id ] ){
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

	void EnemyShotGroupVCPU::SysSetEnemyShotCollisionRadiusGU()
	{
		Pop();
		GameUnit radius;
		radius.SetRawValue( Top().m_Integer );
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
		if( id < 0 ){
			Push( 0 );
		}
		if( m_pEnemyShotGroupData->m_pShots[ id ] ){
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
#if defined ( USE_FLOATING_POINT )
			float x;
			float y;
			m_pEnemyShotGroupData->m_pShots[ id ]->GetPos( &x, &y );
			Push( x );
#elif defined ( USE_GAME_UNIT )
			GameUnit x;
			GameUnit y;
			m_pEnemyShotGroupData->m_pShots[ id ]->GetPos( &x, &y );
			Push( x.GetFloat() );
#endif
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
#if defined ( USE_FLOATING_POINT )
			float x;
			float y;
			m_pEnemyShotGroupData->m_pShots[ id ]->GetPos( &x, &y );
			Push( y );
#elif defined ( USE_GAME_UNIT )
			GameUnit x;
			GameUnit y;
			m_pEnemyShotGroupData->m_pShots[ id ]->GetPos( &x, &y );
			Push( y.GetFloat() );
#endif
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

	void EnemyShotGroupVCPU::SysGetEnemyShotAngle()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			Push( m_pEnemyShotGroupData->m_pShots[ id ]->GetAngle().GetFloat() );
		}
		else{
			Push( 0.0f );
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

	void EnemyShotGroupVCPU::SysGetEnemyShotGroupFReg()
	{
		Pop();
		Push( m_pEnemyShotGroupData->m_FReg );
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

	void EnemyShotGroupVCPU::SysSetEnemyShotImgRotMode()
	{
		Pop();
		int mode = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetImgRotMode( mode );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotImgRotAnglePerFrame()
	{
		Pop();
		float angle = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetImgRotAnglePerFrame( angle );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotAlphaBlendingMode()
	{
		Pop();
		int mode = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetAlphaBlendingMode( mode );
		}
	}

	void EnemyShotGroupVCPU::SysEnemyShotSetShape()
	{
		Pop();
		int shape = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetShape( shape );
		}
	}

	void EnemyShotGroupVCPU::SysEnemyShotSetLineStatus()
	{
		Pop();
		float thickness = Top().m_Float;
		Pop();
		float y2 = Top().m_Float;
		Pop();
		float x2 = Top().m_Float;
		Pop();
		float y1 = Top().m_Float;
		Pop();
		float x1 = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetLinePos( x1, y1, x2, y2, thickness );
		}
	}

	void EnemyShotGroupVCPU::SysEnableEnemyShotInvisibleMode()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->EnableInvisibleMode();
		}
	}

	void EnemyShotGroupVCPU::SysDisableEnemyShotInvisibleMode()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->DisableInvisibleMode();
		}
	}

	void EnemyShotGroupVCPU::SysEnableEnemyShotInvincibleMode()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->EnableInvincibleMode();
		}
	}

	void EnemyShotGroupVCPU::SysDisableEnemyShotInvincibleMode()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->DisableInvincibleMode();
		}
	}


	void EnemyShotGroupVCPU::SysSetEnemyShotStatusGU()
	{
		GameUnit x;
		GameUnit y;
		GameUnit angle;
		GameUnit speed;
		GameUnit radius;

		Pop();
		int imgID = Top().m_Integer;
		Pop();
		radius.SetRawValue( Top().m_Integer );
		Pop();
		speed.SetRawValue( Top().m_Integer );
		Pop();
		angle.SetRawValue( Top().m_Integer );
		Pop();
		y.SetRawValue( Top().m_Integer );
		Pop();
		x.SetRawValue( Top().m_Integer );
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

	void EnemyShotGroupVCPU::SysGetEnemyPosXGU()
	{
		Pop();
		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			Push( m_pEnemyShotGroupData->m_pEnemyData->m_GUData.m_PosX.GetRawValue() );
		}
		else{
			Push( 0 );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyPosYGU()
	{
		Pop();
		if( m_pEnemyShotGroupData->m_EnemyControlled ){
			Push( m_pEnemyShotGroupData->m_pEnemyData->m_GUData.m_PosY.GetRawValue() );
		}
		else{
			Push( 0 );
		}
	}

	void EnemyShotGroupVCPU::SysGetPlayerPosXGU()
	{
		Pop();
		GameUnit x;
		GameUnit y;
		m_pEnemyShotGroupData->m_pStageData->m_pPlayer->GetPos( &x, &y );
		Push( x.GetRawValue() );
	}

	void EnemyShotGroupVCPU::SysGetPlayerPosYGU()
	{
		Pop();
		GameUnit x;
		GameUnit y;
		m_pEnemyShotGroupData->m_pStageData->m_pPlayer->GetPos( &x, &y );
		Push( y.GetRawValue() );
	}

	void EnemyShotGroupVCPU::SysGetRandGU()
	{
		Pop();
		GameUnit r = m_pEnemyShotGroupData->m_pStageData->m_RandGen.GetRand();
		GameUnit max = m_pEnemyShotGroupData->m_pStageData->m_RandGen.GetRandMax();
		GameUnit l = r  / max;
		Push( l.GetRawValue() );
	}

	void EnemyShotGroupVCPU::SysGetEnemyShotPosXGU()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();
		
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			GameUnit x;
			GameUnit y;
			m_pEnemyShotGroupData->m_pShots[ id ]->GetPos( &x, &y );
			Push( x.GetRawValue() );
		}
		else{
			Push( 0 );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyShotPosYGU()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();
		
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			GameUnit x;
			GameUnit y;
			m_pEnemyShotGroupData->m_pShots[ id ]->GetPos( &x, &y );
			Push( y.GetRawValue() );
		}
		else{
			Push( 0 );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotSpeedGU()
	{
		Pop();
		GameUnit speed;
		speed.SetRawValue( Top().m_Integer );
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( id < 0 ){
			return;
		}

		if( m_pEnemyShotGroupData->m_pShots[ id ] ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetSpeed( speed );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotAngleGU()
	{
		Pop();
		GameUnit angle;
		angle.SetRawValue( Top().m_Integer );
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetAngle( angle );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotPosGU()
	{
		Pop();
		GameUnit y;
		y.SetRawValue( Top().m_Integer );
		Pop();
		GameUnit x;
		x.SetRawValue( Top().m_Integer );
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetPos( x, y );
		}
	}

	void EnemyShotGroupVCPU::SysEnemyShotAddAngleGU()
	{
		Pop();
		GameUnit angle;
		angle.SetRawValue( Top().m_Integer );
		Pop();
		int id = Top().m_Integer;
		Pop();
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->AddAngle( angle );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyShotGroupGReg()
	{
		Pop();
		Push( m_pEnemyShotGroupData->m_Reg );
	}

	void EnemyShotGroupVCPU::SysGetEnemyShotGroupGRegIndex()
	{
		Pop();
		int idx = Top().m_Integer;
		Pop();

		Push( m_pEnemyShotGroupData->m_GUData.m_Reg[ idx ].GetRawValue() );
	}

	void EnemyShotGroupVCPU::SysEnemyShotSetLineStatusGU()
	{
		Pop();
		GameUnit thickness;
		thickness.SetRawValue( Top().m_Integer );
		Pop();
		GameUnit y2;
		y2.SetRawValue( Top().m_Integer );
		Pop();
		GameUnit x2;
		x2.SetRawValue( Top().m_Integer );
		Pop();
		GameUnit y1;
		y1.SetRawValue( Top().m_Integer );
		Pop();
		GameUnit x1;
		x1.SetRawValue( Top().m_Integer );
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetLinePos( x1, y1, x2, y2, thickness );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyShotAngleGU()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			Push( m_pEnemyShotGroupData->m_pShots[ id ]->GetAngle().GetRawValue() );
		}
		else{
			Push( 0 );
		}
	}

	void EnemyShotGroupVCPU::SysGetEnemyShotSpeedGU()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			Push( m_pEnemyShotGroupData->m_pShots[ id ]->GetSpeed().GetRawValue() );
		}
		else{
			Push( 0 );
		}
	}

	void EnemyShotGroupVCPU::SysEnemyShotAddSpeedGU()
	{
		Pop();
		GameUnit speed;
		speed.SetRawValue( Top().m_Integer );
		Pop();
		int id = Top().m_Integer;
		Pop();
		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->AddSpeed( speed );
		}
	}

	void EnemyShotGroupVCPU::SysGetDifficulty()
	{
		Pop();
		Push( m_pEnemyShotGroupData->m_pStageData->m_Difficulty );
	}

	void EnemyShotGroupVCPU::SysSendEventToEnemyShotGroup()
	{
		Pop();
		int ev = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		// グローバルに割り当てられたものに、イベント送信
		std::for_each(	m_pEnemyShotGroupData->m_pStageData->m_EnemyShotGroupList.begin(),
						m_pEnemyShotGroupData->m_pStageData->m_EnemyShotGroupList.end(),
						[id,ev]( EnemyShotGroup* pGroup ){
							if( pGroup == 0 ){
								return;
							}
							if( id == pGroup->GetSubID() ){
								pGroup->SendEvent( ev );
							} } );

		// 敵に割り当てられているものに、イベント送信
		std::for_each(	m_pEnemyShotGroupData->m_pEnemyData->m_ShotGroupList.begin(),
						m_pEnemyShotGroupData->m_pEnemyData->m_ShotGroupList.end(),
						[id,ev]( EnemyShotGroup* pGroup ){
							if( pGroup == 0 ){
								return;
							}
							if( id == pGroup->GetSubID() ){
								pGroup->SendEvent( ev );
							} } );
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotGroupSubID()
	{
		Pop();
		int subID = Top().m_Integer;
		Pop();

		m_pEnemyShotGroupData->m_SubID = subID;
	}

	void EnemyShotGroupVCPU::SysGetEvent()
	{
		Pop();
		
		if( m_pEnemyShotGroupData->m_EventQueue.empty() ){
			Push( -10000 );
		}
		else{
			int ev = m_pEnemyShotGroupData->m_EventQueue.front();
			m_pEnemyShotGroupData->m_EventQueue.pop();
			Push( ev );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotImageAtlas()
	{
		Pop();
		int imgID = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( m_pEnemyShotGroupData->m_pShots[ id ] && id >= 0 ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetAtlasImage( imgID );
		}
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotStatusAtlasGU()
	{
		GameUnit x;
		GameUnit y;
		GameUnit angle;
		GameUnit speed;
		GameUnit radius;

		Pop();
		int imgID = Top().m_Integer;
		Pop();
		radius.SetRawValue( Top().m_Integer );
		Pop();
		speed.SetRawValue( Top().m_Integer );
		Pop();
		angle.SetRawValue( Top().m_Integer );
		Pop();
		y.SetRawValue( Top().m_Integer );
		Pop();
		x.SetRawValue( Top().m_Integer );
		Pop();
		int id = Top().m_Integer;
		Pop();

		if( id >= 0 && m_pEnemyShotGroupData->m_pShots[ id ] != NULL ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetPos( x, y );
			m_pEnemyShotGroupData->m_pShots[ id ]->SetAngle( angle );
			m_pEnemyShotGroupData->m_pShots[ id ]->SetSpeed( speed );
			m_pEnemyShotGroupData->m_pShots[ id ]->SetCollisionRadius( radius );
			m_pEnemyShotGroupData->m_pShots[ id ]->SetAtlasImage( imgID );
		}
	}

	void EnemyShotGroupVCPU::SysCreateScriptEffectReg()
	{
		Pop();
		int reg = Top().m_Integer;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		ScriptEffect* pNewEffect = m_pEnemyShotGroupData->m_pStageData->m_ObjBuilder.CreateScriptEffect( id, NULL );
		pNewEffect->Init( x, y );
		pNewEffect->SetReg( 0, reg );
		m_pEnemyShotGroupData->m_pStageData->m_ScriptEffectList.push_back( pNewEffect );

		Push( -1 );
	}

	void EnemyShotGroupVCPU::SysCreateScriptEffectFReg5()
	{
		float reg[ 5 ];

		Pop();
		for( int i = 4; i >= 0; --i ){
			reg[ i ] = Top().m_Float;
			Pop();
		}
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		ScriptEffect* pNewEffect = m_pEnemyShotGroupData->m_pStageData->m_ObjBuilder.CreateScriptEffect( id, NULL );
		pNewEffect->Init( x, y );
		for( int i = 0; i < 5; ++i ){
			pNewEffect->SetReg( i, reg[ i ] );
		}
		m_pEnemyShotGroupData->m_pStageData->m_ScriptEffectList.push_back( pNewEffect );

		Push( -1 );
	}

	void EnemyShotGroupVCPU::SysSetEnemyShotDrawingMultiplicity()
	{
		Pop();
		int num = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();
		if( id >= 0 && m_pEnemyShotGroupData->m_pShots[ id ] != NULL ){
			m_pEnemyShotGroupData->m_pShots[ id ]->SetDrawingMultiplicity( num );
		}
	}

	void EnemyShotGroupVCPU::OpSysCall( int val )
	{
		switch( val ){
			case VM::SYS_GET_RANDOM_F:
				SysGetRandF();
				break;
			case VM::SYS_GET_RANDOM_GU:
				SysGetRandGU();
				break;

			case VM::SYS_GET_PLAYER_POSX_GU:
				SysGetPlayerPosXGU();
				break;
			case VM::SYS_GET_PLAYER_POSY_GU:
				SysGetPlayerPosYGU();
				break;
			case VM::SYS_ENEMY_GET_POSX_GU:
				SysGetEnemyPosXGU();
				break;
			case VM::SYS_ENEMY_GET_POSY_GU:
				SysGetEnemyPosYGU();
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
			case VM::SYS_ENEMY_GET_ROT_ANGLE:
				SysGetEnemyRotAngle();
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

			case VM::SYS_ENEMY_SHOT_GROUP_SET_STATUS_GU:
				SysSetEnemyShotStatusGU();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_STATUS_ATLAS_GU:
				SysSetEnemyShotStatusAtlasGU();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_GET_POS_X_GU:
				SysGetEnemyShotPosXGU();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_GET_POS_Y_GU:
				SysGetEnemyShotPosYGU();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_SPEED_GU:
				SysSetEnemyShotSpeedGU();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_ANGLE_GU:
				SysSetEnemyShotAngleGU();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_POS_GU:
				SysSetEnemyShotPosGU();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_ADD_ANGLE_GU:
				SysEnemyShotAddAngleGU();
				break;
			case VM::SYS_ENEMY_SHOT_SET_LINE_SHOT_STATUS_GU:
				SysEnemyShotSetLineStatusGU();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_GET_ANGLE_GU:
				SysGetEnemyShotAngleGU();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_ADD_SPEED_GU:
				SysEnemyShotAddSpeedGU();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_GET_SPEED_GU:
				SysGetEnemyShotSpeedGU();
				break;

			case VM::SYS_ENEMY_SHOT_GROUP_CREATE_SHOT:
				SysCreateEnemyShot();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_CREATE_CONS_SHOT:
				SysCreateConsEnemyShot();
				break;
			case VM::SYS_ENEMY_CREATE_SHOT_GROUP:
				SysCreateEnemyShotGroup();
				break;
			case VM::SYS_ENEMY_CREATE_SHOT_GROUP_REG:
				SysCreateEnemyShotGroupReg();
				break;
			case VM::SYS_ENEMY_CREATE_SHOT_GROUP_GREG5:
				SysCreateEnemyShotGroupGReg5();
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
			case VM::SYS_ENEMY_SHOT_GROUP_SET_IMAGE_ATLAS:
				SysSetEnemyShotImageAtlas();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_COLLISION_RADIUS:
				SysSetEnemyShotCollisionRadius();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_COLLISION_RADIUS_GU:
				SysSetEnemyShotCollisionRadiusGU();
				break;

			case VM::SYS_ENEMY_SHOT_GROUP_SET_STATUS:
				SysSetEnemyShotStatus();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_MOVEMENT:
				SysSetEnemyShotMovement();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_DRAWING_MULTIPLICITY:
				SysSetEnemyShotDrawingMultiplicity();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_ALPHA_BLENDING_MODE:
				SysSetEnemyShotAlphaBlendingMode();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_IMG_ROT_ANGLE_PER_FRAME:
				SysSetEnemyShotImgRotAnglePerFrame();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_SET_IMG_ROT_MODE:
				SysSetEnemyShotImgRotMode();
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
			case VM::SYS_ENEMY_SHOT_GROUP_GET_ANGLE:
				SysGetEnemyShotAngle();
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
			case VM::SYS_ENEMY_SHOT_GROUP_SET_SHAPE:
				SysEnemyShotSetShape();
				break;
			case VM::SYS_ENEMY_SHOT_SET_LINE_SHOT_STATUS:
				SysEnemyShotSetLineStatus();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_ENABLE_INVISIBLE_MODE:
				SysEnableEnemyShotInvisibleMode();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_DISABLE_INVISIBLE_MODE:
				SysDisableEnemyShotInvisibleMode();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_ENABLE_INVINCIBLE_MODE:
				SysEnableEnemyShotInvincibleMode();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_DISABLE_INVINCIBLE_MODE:
				SysDisableEnemyShotInvincibleMode();
				break;

			case VM::SYS_SEND_EVENT_TO_ENEMY_SHOT_GROUP:
				SysSendEventToEnemyShotGroup();
				break;
			case VM::SYS_SET_ENEMY_SHOT_GROUP_SUB_ID:
				SysSetEnemyShotGroupSubID();
				break;
			case VM::SYS_GET_EVENT:
				SysGetEvent();
				break;

			case VM::SYS_CREATE_SCRIPT_EFFECT_REG:
				SysCreateScriptEffectReg();
				break;
			case VM::SYS_CREATE_SCRIPT_EFFECT_FREG5:
				SysCreateScriptEffectFReg5();
				break;

			case VM::SYS_STAGE_GET_FRAME:
				SysGetStageFrame();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_GET_REG:
				SysGetEnemyShotGroupReg();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_GET_FREG:
				SysGetEnemyShotGroupFReg();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_GET_GREG:
				SysGetEnemyShotGroupGReg();
				break;
			case VM::SYS_ENEMY_SHOT_GROUP_GET_GREG_INDEX:
				SysGetEnemyShotGroupGRegIndex();
				break;
			case VM::SYS_STAGE_GET_DIFFICULTY:
				SysGetDifficulty();
				break;

			default:
				VM::VCPU::OpSysCall( val );
				break;
		}
	}
}
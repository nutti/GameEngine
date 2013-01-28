#ifndef INCLUDED_GAMEENGINE_ENEMYSHOTGROUPVCPU_H
#define INCLUDED_GAMEENGINE_ENEMYSHOTGROUPVCPU_H

#include "ScriptManager/VM.h"

namespace GameEngine
{
	struct EnemyShotGroupData;
	class EnemyShotGroupVCPU : public VM::VCPU
	{
	private:
		EnemyShotGroupData*		m_pEnemyShotGroupData;

		void SysGetPlayerPosX();
		void SysGetPlayerPosY();
		
		void SysGetRandF();

		void SysGetEnemyPosX();
		void SysGetEnemyPosY();
		void SysGetEnemyHP();
		void SysGetEnemySpeed();
		void SysGetEnemyCounter();
		void SysGetEnemyCounterF();
		void SysGetEnemyAngle();
		void SysGetEnemyConsGauge();

		void SysCreateEnemyShot();
		void SysCreateConsEnemyShot();
		void SysSetEnemyShotPos();
		void SysSetEnemyShotPower();
		void SysSetEnemyShotAngle();
		void SysSetEnemyShotSpeed();
		void SysSetEnemyShotImage();
		void SysSetEnemyShotCollisionRadius();
		void SysSetEnemyShotStatus();
		void SysSetEnemyShotMovement();
		void SysGetEnemyShotCounter();
		void SysGetEnemyShotPosX();
		void SysGetEnemyShotPosY();
		void SysGetEnemyShotPower();

		void SysEnemyShotAddAngle();
		void SysEnemyShotSetImgScale();
		void SysEnemyShotAddPos();
		void SysEnemyShotAddSpeed();

		void SysGetStageFrame();
		void SysGetEnemyShotGroupReg();

		void SysPlaySE();
		void SysStopSE();
	public:
		EnemyShotGroupVCPU();
		~EnemyShotGroupVCPU();
		void Init( VM::Data* pData, EnemyShotGroupData* pEnemyShotGroupData );
		void OpSysCall( int val );
	};
}

#endif
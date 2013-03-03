#ifndef INCLUDED_GAMEENGINE_ENEMYVCPU_H
#define INCLUDED_GAMEENGINE_ENEMYVCPU_H

#include "ScriptManager/VM.h"

namespace GameEngine
{
	struct EnemyData;
	class EnemyVCPU : public VM::VCPU
	{
	private:
		EnemyData*		m_pEnemyData;

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
		void SysGetEnemyReg();

		void SysSetEnemyName();
		void SysSetEnemyPos();
		void SysSetEnemyPos3D();
		void SysSetEnemyAngle();
		void SysSetEnemyAngle3D();
		void SysSetEnemyScale3D();
		void SysSetEnemySpeed();
		void SysSetEnemyHP();
		void SysSetEnemyImgID();
		void SysSetEnemyModelID();
		void SysSetEnemyCollisionRadius();
		void SysSetEnemyScore();
		void SysSetEnemyBossFlag();
		void SysSetEnemyConsGauge();
		void SysEnemyEnableInvincible();
		void SysEnemyDisableInvincible();
		void SysEnemyEnableNonCollisionMode();
		void SysEnemyDisableNonCollisionMode();
		void SysEnemyInvokeConsSkill();
		void SysEnemyStopConsSkill();

		void SysCreateEnemyShot1();
		void SysCreateEffect1();
		void SysCreateEnemyShotGroup();
		void SysCreateEnemyShotGroupReg();

		void SysStageGetBossFlag();
		void SysStageSetBossFlag();
		void SysEnemyShiftNextMode();

		void SysCreateItem();

		void SysPlaySE();
		void SysStopSE();
	public:
		EnemyVCPU();
		~EnemyVCPU();
		void Init( VM::Data* pData, EnemyData* pEnemyData );
		void OpSysCall( int val );
	};
}

#endif
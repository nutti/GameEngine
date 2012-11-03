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

		void SysSetEnemyPos();
		void SysSetEnemyAngle();
		void SysSetEnemySpeed();
		void SysSetEnemyHP();
		void SysSetEnemyImgID();
		void SysSetEnemyCollisionRadius();
		void SysSetEnemyScore();
		void SysSetEnemyBossFlag();
		void SysSetEnemyConsGauge();
		void SysEnemyEnableInvincible();
		void SysEnemyDisableInvincible();

		void SysCreateEnemyShot1();
		void SysCreateEffect1();
		void SysCreateEnemyShotGroup();

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
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

		void SysGetPlayerPosXGU();
		void SysGetPlayerPosYGU();
		void SysGetPlayerPosX();
		void SysGetPlayerPosY();
		void SysGetPlayerHP();
		
		void SysGetRandF();
		void SysGetRandGU();

		void SysGetEnemyPosXGU();
		void SysGetEnemyPosYGU();
		void SysSetEnemyCollisionRadiusGU();
		void SysSetEnemyPosGU();
		void SysSetEnemyPos3DGU();

		void SysGetEnemyPosX();
		void SysGetEnemyPosY();
		void SysGetEnemyHP();
		void SysGetEnemySpeed();
		void SysGetEnemyCounter();
		void SysGetEnemyCounterF();
		void SysGetEnemyAngle();
		void SysGetEnemyConsGauge();
		void SysGetEnemyReg();
		void SysSearchItem();
		void SysGetItemPosX();
		void SysGetItemPosY();

		void SysGetItemPosXGU();
		void SysGetItemPosYGU();

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
		void SysEnemyDamagedByConsShot();
		void SysEnemyDamagedByConsShotIndex();
		void SysSearchEnemyInSkillMode();
		void SysSearchEnemyInSkillModeByName();

		void SysCreateEnemyShot1();
		void SysCreateEnemyShot1GU();
		void SysCreateEffect1();
		void SysCreateEffect1GU();
		void SysCreateEnemyShotGroup();
		void SysCreateEnemyShotGroupReg();
		void SysCreateEnemyShotGroupFReg();
		void SysCreateEnemyShotGroupGReg();
		void SysCreateEnemyIniPosGU();
		void SysCreateEnemyIniPosRegGU();

		void SysStageGetBossFlag();
		void SysStageSetBossFlag();
		void SysEnemyShiftNextMode();
		void SysGetDifficulty();
		void SysSetBossPhaseTotal();
		void SysSetBossPhaseShiftHP();

		void SysCreateItem();
		void SysCreateItemGU();

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
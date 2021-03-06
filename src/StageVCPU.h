#ifndef INCLUDED_GAMEENGINE_STAGEVCPU_H
#define INCLUDED_GAMEENGINE_STAGEVCPU_H

#include "ScriptManager/VM.h"

namespace GameEngine
{
	struct StageData;
	class StageVCPU : public VM::VCPU
	{
	private:
		StageData*			m_pStageData;

		void SysAddEnemy();
		void SysAddEnemyIniPos();
		void SysGetFrame();
		void SysSetFrame();
		void SysUpdate();
		void SysGetRand();
		void SysProcEnemyPatternFile();
		void SysStageCreateBoss();
		void SysSetFrameTotal();
		void SysStageSwitchToNextStage();

		void SysGetPlayerCrystalUsedTotal();
		void SysGetPlayerCrystalTotal();

		void SysStageGetBossFlag();
		void SysStageSetBossFlag();

		void SysSetStageBaseConsLevel();

		void SysPlayBGM();
		void SysStopBGM();
	public:
		StageVCPU();
		~StageVCPU();
		void Init( VM::Data* pData, StageData* pStageData );
		void OpSysCall( int val );
	};
}

#endif
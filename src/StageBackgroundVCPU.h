#ifndef INCLUDED_GAMEENGINE_STAGEBACKGROUNDVCPU_H
#define INCLUDED_GAMEENGINE_STAGEBACKGROUNDVCPU_H

#include "ScriptManager/VM.h"

namespace GameEngine
{
	struct StageBackgroundData;
	class StageBackgroundVCPU : public VM::VCPU
	{
	private:
		StageBackgroundData*		m_pStageBGData;

		void SysStageBackgroundGetCounter();

		void SysEnableCamera();
		void SysSetCameraProj();
		void SysSetCameraView();
		void SysDrawRect3DP();
		void SysBegin3DGraphics();
		void SysEnd3DGraphics();
		void SysEnableFog();
	public:
		StageBackgroundVCPU();
		~StageBackgroundVCPU();
		void Init( VM::Data* pData, StageBackgroundData* pStageBGData );
		void OpSysCall( int val );
	};
}

#endif
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
		void SysSetFogParam();
		void SysDrawModelP();
		void SysSetDirLightDir();
		void SysSetDirLightAmbColor();
		void SysSetDirLightDifColor();
		void SysSetDirLightSpcColor();
		void SysSetDirLightAtten();
		void SysEnableDirLight();
		void SysEnableAlphaBlending();
		void SysDisableAlphaBlending();
		void SysDrawModelPS();
		void SysBegin2DGraphics();
		void SysEnd2DGraphics();
		void SysDrawTexturePS();
		void SysColorARGB();

		void SysStageGetFrame();
		void SysStageGetBossFlag();
	public:
		StageBackgroundVCPU();
		~StageBackgroundVCPU();
		void Init( VM::Data* pData, StageBackgroundData* pStageBGData );
		void OpSysCall( int val );
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_EFFECTVCPU_H
#define INCLUDED_GAMEENGINE_EFFECTVCPU_H

#include "ScriptManager/VM.h"

namespace GameEngine
{
	struct ScriptEffectData;
	class EffectVCPU : public VM::VCPU
	{
	private:
		ScriptEffectData*			m_pEffectData;

		void SysGetScriptEffectReg();
		void SysGetEnemyPosX();
		void SysGetEnemyPosY();
		void SysGetScriptEffectCounter();
		void SysDrawTextureAtlasPSR();

		void SysGetScriptEffectPosX();
		void SysGetScriptEffectPosY();
		
		void SysTerminateScriptEffect();

		void SysUpdate();
		void SysPlayBGM();
		void SysStopBGM();
	public:
		EffectVCPU();
		~EffectVCPU();
		void Init( VM::Data* pData, ScriptEffectData* pEffectData );
		void OpSysCall( int val );
	};
}

#endif
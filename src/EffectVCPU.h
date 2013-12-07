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

		void SysGetEnemyPosXGU();
		void SysGetEnemyPosYGU();

		void SysEnemyAlive();
		void SysEnemyInSkillMode();

		void SysDrawTextureAtlasPSR();
		void SysDrawTextureAtlasPSRC();
		void SysDrawTextureAtlasBlendingPSR();
		void SysDrawTextureAtlasBlendingPSRC();
		void SysDrawClipedTextureAtlasP();
		void SysDrawClipedTextureAtlasPSR();
		void SysDrawClipedTextureAtlasPSRC();
		void SysDrawClipedTextureAtlasBlendingP();
		void SysDrawClipedTextureAtlasBlendingPSR();
		void SysDrawClipedTextureAtlasBlendingPSRC();

		void SysGetScriptEffectPosX();
		void SysGetScriptEffectPosY();
		
		void SysTerminateScriptEffect();

		void SysUpdate();
		void SysPlayBGM();
		void SysStopBGM();
		void SysPlaySE();
		void SysStopSE();
	public:
		EffectVCPU();
		~EffectVCPU();
		void Init( VM::Data* pData, ScriptEffectData* pEffectData );
		void OpSysCall( int val );
	};
}

#endif
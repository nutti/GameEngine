#ifndef INCLUDED_GAMEENGINE_GAMEENGINEVCPU_H
#define INCLUDED_GAMEENGINE_GAMEENGINEVCPU_H

#include "ScriptManager/VM.h"

namespace GameEngine
{
	class GameEngineVCPU : public VM::VCPU
	{
	private:

	public:
		GameEngineVCPU();
		~GameEngineVCPU();
		void Init( VM::Data* pData );
		void OpSysCall( int val );
	};
}

#endif
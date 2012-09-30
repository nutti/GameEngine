#include "GameEngineVCPU.h"

#include <MAPIL/MAPIL.h>

namespace GameEngine
{
	GameEngineVCPU::GameEngineVCPU() : VM::VCPU()
	{
	}

	GameEngineVCPU::~GameEngineVCPU()
	{
	}

	void Init( VM::Data* pData )
	{
	}

	void GameEngineVCPU::OpSysCall( int val )
	{
		switch( val ){
			case VM::SYS_GET_RANDOM_F:
				
				break;


			default:
				VM::VCPU::OpSysCall( val );
				break;
		}
	}
}
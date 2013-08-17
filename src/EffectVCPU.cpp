#include "EffectVCPU.h"

#include <MAPIL/MAPIL.h>

#include "ScriptEffect.h"
#include "Enemy.h"
#include "Stage.h"

namespace GameEngine
{
	EffectVCPU::EffectVCPU() : VM::VCPU()
	{
	}

	EffectVCPU::~EffectVCPU()
	{
	}

	void EffectVCPU::Init( VM::Data* pData, ScriptEffectData* pEffectData )
	{
		m_pData = pData;
		m_pEffectData = pEffectData;
		VM::VCPU::Init();
	}


	void EffectVCPU::SysPlayBGM()
	{
		Pop();
		int id = m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_BGMMap[ Top().m_Integer ];
		MAPIL::PlayStreamingBuffer( id );
	}

	void EffectVCPU::SysStopBGM()
	{
		Pop();
		int id = m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_BGMMap[ Top().m_Integer ];
		MAPIL::StopStreamingBuffer( id );
	}

	void EffectVCPU::SysGetScriptEffectReg()
	{
		Pop();
		int reg = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		MAPIL::Assert( id < sizeof( m_pEffectData->m_Regs ) / sizeof( m_pEffectData->m_Regs[ 0 ] ), -1 );

		Push( m_pEffectData->m_Regs[ id ] );
	}

	void EffectVCPU::SysGetEnemyPosX()
	{
		Pop();
		std::shared_ptr < Enemy > raw = m_pEffectData->m_ParentEnemy.lock();
		if( raw ){
#if defined ( USE_FLOATING_POINT )
			float x;
			float y;
			raw->GetPos( &x, &y );
			Push( x );
#elif defined ( USE_GAME_UNIT )
			GameUnit x;
			GameUnit y;
			raw->GetPos( &x, &y );
			Push( x.GetFloat() );
#endif
		}
		else{
			Push( -1000.0f );
		}
	}

	void EffectVCPU::SysGetEnemyPosY()
	{
		Pop();
		std::shared_ptr < Enemy > raw = m_pEffectData->m_ParentEnemy.lock();
		if( raw ){
#if defined ( USE_FLOATING_POINT )
			float x;
			float y;
			raw->GetPos( &x, &y );
			Push( x );
#elif defined ( USE_GAME_UNIT )
			GameUnit x;
			GameUnit y;
			raw->GetPos( &x, &y );
			Push( y.GetFloat() );
#endif
		}
		else{
			Push( -1000.0f );
		}
	}

	void EffectVCPU::SysGetScriptEffectCounter()
	{
		Pop();

		Push( m_pEffectData->m_Counter );
	}

	void EffectVCPU::SysDrawTextureAtlasPSR()
	{
		Pop();
		float angle = Top().m_Float;
		Pop();
		float sy = Top().m_Float;
		Pop();
		float sx = Top().m_Float;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();


		ResourceMap::TextureAtlas atlas = m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ id ];
		MAPIL::DrawClipedTexture(	m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_TextureMap[ atlas.m_TexID ],
									x, y, sx, sy, angle,
									atlas.m_X, atlas.m_Y, atlas.m_X + atlas.m_Width, atlas.m_Y + atlas.m_Height );
	}

	void EffectVCPU::SysTerminateScriptEffect()
	{
		Pop();

		m_pEffectData->m_StatusFlags.set( ScriptEffectData::TERMINATED );
	}

	void EffectVCPU::SysGetScriptEffectPosX()
	{
		Pop();

		Push( m_pEffectData->m_PosX );
	}

	void EffectVCPU::SysGetScriptEffectPosY()
	{
		Pop();

		Push( m_pEffectData->m_PosY );
	}

	void EffectVCPU::OpSysCall( int val )
	{
		switch( val ){
			case VM::SYS_GET_SCRIPT_EFFECT_REG:
				SysGetScriptEffectReg();
				break;
			case VM::SYS_GET_SCRIPT_EFFECT_COUNTER:
				SysGetScriptEffectCounter();
				break;
			case VM::SYS_ENEMY_GET_POSX:
				SysGetEnemyPosX();
				break;
			case VM::SYS_ENEMY_GET_POSY:
				SysGetEnemyPosY();
				break;

			case VM::SYS_DRAW_TEXTURE_ATLAS_PSR:
				SysDrawTextureAtlasPSR();
				break;

			case VM::SYS_TERMINATE_SCRIPT_EFFECT:
				SysTerminateScriptEffect();
				break;

			case VM::SYS_GET_SCRIPT_EFFECT_POS_X:
				SysGetScriptEffectPosX();
				break;
			case VM::SYS_GET_SCRIPT_EFFECT_POS_Y:
				SysGetScriptEffectPosY();
				break;

			case VM::SYS_PLAY_BGM:
				SysPlayBGM();
				break;
			case VM::SYS_STOP_BGM:
				SysStopBGM();
				break;

			default:
				VM::VCPU::OpSysCall( val );
				break;
		}
	}
}
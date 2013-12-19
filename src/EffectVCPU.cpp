#include "EffectVCPU.h"

#include <MAPIL/MAPIL.h>

#include "ScriptEffect.h"
#include "Enemy.h"
#include "Stage.h"
#include "SpriteBatch.h"

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

	void EffectVCPU::SysGetEnemyPosXGU()
	{
		Pop();
		std::shared_ptr < Enemy > raw = m_pEffectData->m_ParentEnemy.lock();
		if( raw ){
			GameUnit x;
			GameUnit y;
			raw->GetPos( &x, &y );
			Push( x.GetRawValue() );
		}
		else{
			Push( 0 );
		}
	}

	void EffectVCPU::SysGetEnemyPosYGU()
	{
		Pop();
		std::shared_ptr < Enemy > raw = m_pEffectData->m_ParentEnemy.lock();
		if( raw ){
			GameUnit x;
			GameUnit y;
			raw->GetPos( &x, &y );
			Push( y.GetRawValue() );
		}
		else{
			Push( 0 );
		}
	}

	void EffectVCPU::SysEnemyAlive()
	{
		Pop();
		std::shared_ptr < Enemy > raw = m_pEffectData->m_ParentEnemy.lock();
		if( raw ){
			Push( 1 );
		}
		else{
			Push( 0 );
		}
	}

	void EffectVCPU::SysEnemyInSkillMode()
	{
		Pop();
		std::shared_ptr < Enemy > raw = m_pEffectData->m_ParentEnemy.lock();
		if( !raw ){
			Push( 0 );
			return;
		}

		Push( raw->IsInSkillMode() );
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

	void EffectVCPU::SysDrawTextureAtlasPSRC()
	{
		Pop();
		int color = RetPop().m_Integer;
		float angle = RetPop().m_Float;
		float sy = RetPop().m_Float;
		float sx = RetPop().m_Float;
		float y = RetPop().m_Float;
		float x = RetPop().m_Float;
		int id = RetPop().m_Integer;

		ResourceMap::TextureAtlas atlas = m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ id ];
		MAPIL::DrawClipedTexture(	m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_TextureMap[ atlas.m_TexID ],
									x, y, sx, sy, angle,
									atlas.m_X, atlas.m_Y, atlas.m_X + atlas.m_Width, atlas.m_Y + atlas.m_Height,
									true, color );
	}

	void EffectVCPU::SysDrawTextureAtlasBlendingPSR()
	{
		Pop();
		float angle = RetPop().m_Float;
		float sy = RetPop().m_Float;
		float sx = RetPop().m_Float;
		float y = RetPop().m_Float;
		float x = RetPop().m_Float;
		int alphaMode = RetPop().m_Integer;
		int id = RetPop().m_Integer;

		ResourceMap::TextureAtlas atlas = m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ id ];
	
		AddToAtlasSpriteBatch( false, alphaMode, id, x, y, sx, sy, angle );
	}

	void EffectVCPU::SysDrawTextureAtlasBlendingPSRC()
	{
		Pop();
		int color = RetPop().m_Integer;
		float angle = RetPop().m_Float;
		float sy = RetPop().m_Float;
		float sx = RetPop().m_Float;
		float y = RetPop().m_Float;
		float x = RetPop().m_Float;
		int alphaMode = RetPop().m_Integer;
		int id = RetPop().m_Integer;

		ResourceMap::TextureAtlas atlas = m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ id ];
	
		AddToAtlasSpriteBatch( false, alphaMode, id, x, y, sx, sy, angle, true, color );
	}

	void EffectVCPU::SysDrawClipedTextureAtlasP()
	{
		Pop();
		float cy2 = Top().m_Float;
		Pop();
		float cx2 = Top().m_Float;
		Pop();
		float cy1 = Top().m_Float;
		Pop();
		float cx1 = Top().m_Float;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		ResourceMap::TextureAtlas atlas = m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ id ];
		MAPIL::DrawClipedTexture(	m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_TextureMap[ atlas.m_TexID ],
									x, y, 1.0f, 1.0f, 0.0f,
									atlas.m_X + cx1, atlas.m_Y + cy1, atlas.m_X + cx2, atlas.m_Y + cy2 );
	}

	void EffectVCPU::SysDrawClipedTextureAtlasPSR()
	{
		Pop();
		float cy2 = Top().m_Float;
		Pop();
		float cx2 = Top().m_Float;
		Pop();
		float cy1 = Top().m_Float;
		Pop();
		float cx1 = Top().m_Float;
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
									atlas.m_X + cx1, atlas.m_Y + cy1, atlas.m_X + cx2, atlas.m_Y + cy2 );
	}

	void EffectVCPU::SysDrawClipedTextureAtlasPSRC()
	{
		Pop();
		float cy2 = RetPop().m_Float;
		float cx2 = RetPop().m_Float;
		float cy1 = RetPop().m_Float;
		float cx1 = RetPop().m_Float;
		int color = RetPop().m_Integer;
		float angle = RetPop().m_Float;
		float sy = RetPop().m_Float;
		float sx = RetPop().m_Float;
		float y = RetPop().m_Float;
		float x = RetPop().m_Float;
		int id = RetPop().m_Integer;

		ResourceMap::TextureAtlas atlas = m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ id ];
		MAPIL::DrawClipedTexture(	m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_TextureMap[ atlas.m_TexID ],
									x, y, sx, sy, angle,
									atlas.m_X + cx1, atlas.m_Y + cy1, atlas.m_X + cx2, atlas.m_Y + cy2, true, color );
	}

	void EffectVCPU::SysDrawClipedTextureAtlasBlendingP()
	{
		Pop();
		float cy2 = Top().m_Float;
		Pop();
		float cx2 = Top().m_Float;
		Pop();
		float cy1 = Top().m_Float;
		Pop();
		float cx1 = Top().m_Float;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int alphaMode = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		ResourceMap::TextureAtlas atlas = m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ id ];
	
		AddToAtlasSpriteBatch( false, alphaMode, id, x, y, 0.0f, 0.0f, 0.0f );
	}

	void EffectVCPU::SysDrawClipedTextureAtlasBlendingPSR()
	{
		Pop();
		float cy2 = RetPop().m_Float;
		float cx2 = RetPop().m_Float;
		float cy1 = RetPop().m_Float;
		float cx1 = RetPop().m_Float;
		float angle = RetPop().m_Float;
		float sy = RetPop().m_Float;
		float sx = RetPop().m_Float;
		float y = RetPop().m_Float;
		float x = RetPop().m_Float;
		int alphaMode = RetPop().m_Integer;
		int id = RetPop().m_Integer;

		ResourceMap::TextureAtlas atlas = m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ id ];
	
		AddToAtlasSpriteBatch( false, alphaMode, id, x, y, sx, sy, angle );
	}

	void EffectVCPU::SysDrawClipedTextureAtlasBlendingPSRC()
	{
		Pop();
		float cy2 = RetPop().m_Float;
		float cx2 = RetPop().m_Float;
		float cy1 = RetPop().m_Float;
		float cx1 = RetPop().m_Float;
		int color = RetPop().m_Integer;
		float angle = RetPop().m_Float;
		float sy = RetPop().m_Float;
		float sx = RetPop().m_Float;
		float y = RetPop().m_Float;
		float x = RetPop().m_Float;
		int alphaMode = RetPop().m_Integer;
		int id = RetPop().m_Integer;

		ResourceMap::TextureAtlas atlas = m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ id ];
	
		AddToAtlasSpriteBatch( false, alphaMode, id, cx1, cy1, cx2, cy2, x, y, sx, sy, angle, true, color );
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


	void EffectVCPU::SysPlaySE()
	{
		Pop();
		int id = m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_SEMap[ RetPop().m_Integer ];
		MAPIL::PlayStaticBuffer( id );
	}

	void EffectVCPU::SysStopSE()
	{
		Pop();
		int id = m_pEffectData->m_pResourceMap->m_pStageResourceMap->m_SEMap[ RetPop().m_Integer ];
		MAPIL::StopStaticBuffer( id );
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
			case VM::SYS_ENEMY_ALIVE:
				SysEnemyAlive();
				break;
			case VM::SYS_ENEMY_IN_SKILL_MODE:
				SysEnemyInSkillMode();
				break;

			case VM::SYS_ENEMY_GET_POSX_GU:
				SysGetEnemyPosXGU();
				break;
			case VM::SYS_ENEMY_GET_POSY_GU:
				SysGetEnemyPosYGU();
				break;

			case VM::SYS_DRAW_TEXTURE_ATLAS_PSR:
				SysDrawTextureAtlasPSR();
				break;
			case VM::SYS_DRAW_TEXTURE_ATLAS_PSRC:
				SysDrawTextureAtlasPSRC();
				break;
			case VM::SYS_DRAW_TEXTURE_ATLAS_BLENDING_PSRC:
				SysDrawTextureAtlasBlendingPSRC();
				break;
			case VM::SYS_DRAW_TEXTURE_ATLAS_BLENDING_PSR:
				SysDrawTextureAtlasBlendingPSR();
				break;
			case VM::SYS_DRAW_CLIPED_TEXTURE_ATLAS_P:
				SysDrawClipedTextureAtlasP();
				break;
			case VM::SYS_DRAW_CLIPED_TEXTURE_ATLAS_PSR:
				SysDrawClipedTextureAtlasPSR();
				break;
			case VM::SYS_DRAW_CLIPED_TEXTURE_ATLAS_PSRC:
				SysDrawClipedTextureAtlasPSRC();
				break;
			case VM::SYS_DRAW_CLIPED_TEXTURE_ATLAS_BLENDING_P:
				SysDrawClipedTextureAtlasBlendingP();
				break;
			case VM::SYS_DRAW_CLIPED_TEXTURE_ATLAS_BLENDING_PSR:
				SysDrawClipedTextureAtlasBlendingPSR();
				break;
			case VM::SYS_DRAW_CLIPED_TEXTURE_ATLAS_BLENDING_PSRC:
				SysDrawClipedTextureAtlasBlendingPSRC();
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
			case VM::SYS_PLAY_SE:
				SysPlaySE();
				break;
			case VM::SYS_STOP_SE:
				SysStopSE();
				break;

			default:
				VM::VCPU::OpSysCall( val );
				break;
		}
	}
}
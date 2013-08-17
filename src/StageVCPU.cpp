#include "StageVCPU.h"

#include <MAPIL/MAPIL.h>

#include "Stage.h"
#include "Enemy.h"
#include "EnemyPatternFileLoader.h"

namespace GameEngine
{
	StageVCPU::StageVCPU() : VM::VCPU()
	{
	}

	StageVCPU::~StageVCPU()
	{
	}

	void StageVCPU::Init( VM::Data* pData, StageData* pStageData )
	{
		m_pData = pData;
		m_pStageData = pStageData;
		VM::VCPU::Init();
	}

	void StageVCPU::SysAddEnemy()
	{
		Pop();
		Enemy* pNewEnemy = m_pStageData->m_ObjBuilder.CreateEnemy( 0 );
		pNewEnemy->Init( 0.0f, 0.0f );
		m_pStageData->m_EnemyList.push_back( std::shared_ptr < Enemy > ( pNewEnemy ) );
	}

	void StageVCPU::SysAddEnemyIniPos()
	{
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();
		Enemy* pNewEnemy = m_pStageData->m_ObjBuilder.CreateEnemy( id );
		pNewEnemy->Init( x, y );
		m_pStageData->m_EnemyList.push_back( std::shared_ptr < Enemy > ( pNewEnemy ) );
	}

	void StageVCPU::SysGetFrame()
	{
		Pop();
		Push( m_pStageData->m_Frame );
	}

	void StageVCPU::SysSetFrame()
	{
		Pop();
		int frame = Top().m_Integer;
		m_pStageData->m_Frame = frame;
	}

	void StageVCPU::SysGetRand()
	{
		Pop();
		Push( m_pStageData->m_RandGen.GetRand() * 1.0f / m_pStageData->m_RandGen.GetRandMax() );
	}

	void StageVCPU::SysProcEnemyPatternFile()
	{
		Pop();
		int frame = Top().m_Integer;
		Pop();
		int fileID = Top().m_Integer;
		Pop();

		if( ExistEnemyPattern( m_pStageData->m_ResourceMap.m_pStageResourceMap->m_EnemyPatternFileMap[ fileID ], frame ) ){
			EnemyPattern pattern;
			GetEnemyPattern( m_pStageData->m_ResourceMap.m_pStageResourceMap->m_EnemyPatternFileMap[ fileID ], frame, &pattern );
			for( int i = 0; i < pattern.m_InfoList.size(); ++i ){
				Enemy* pNewEnemy = m_pStageData->m_ObjBuilder.CreateEnemy( pattern.m_InfoList[ i ].m_EnemyID );
				pNewEnemy->Init( pattern.m_InfoList[ i ].m_PosX, pattern.m_InfoList[ i ].m_PosY );
				for( int j = 0; j < MAX_ENEMY_REGS; ++j ){
					pNewEnemy->SetReg( j, pattern.m_InfoList[ i ].m_Regs[ j ] );
				}
				m_pStageData->m_EnemyList.push_back( std::shared_ptr < Enemy > ( pNewEnemy ) );
			}
		}
	}

	void StageVCPU::SysPlayBGM()
	{
		Pop();
		int id = m_pStageData->m_ResourceMap.m_pStageResourceMap->m_BGMMap[ Top().m_Integer ];
		MAPIL::PlayStreamingBuffer( id );
	}

	void StageVCPU::SysStopBGM()
	{
		Pop();
		int id = m_pStageData->m_ResourceMap.m_pStageResourceMap->m_BGMMap[ Top().m_Integer ];
		MAPIL::StopStreamingBuffer( id );
	}

	void StageVCPU::SysStageGetBossFlag()
	{
		Pop();
		Push( m_pStageData->m_BossMode );
	}

	void StageVCPU::SysStageSetBossFlag()
	{
		Pop();
		m_pStageData->m_BossMode = Top().m_Integer;
		Pop();
	}

	void StageVCPU::SysStageCreateBoss()
	{
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();
		Enemy* pNewEnemy = m_pStageData->m_ObjBuilder.CreateEnemy( id );
		pNewEnemy->Init( x, y );
		if( !m_pStageData->m_pBoss ){
			StageMessage msg;
			msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_BOSS_MODE_STARTED;
			m_pStageData->m_MsgQueue.push( msg );
			m_pStageData->m_pBoss = pNewEnemy;
		}
		else{
			MAPIL::SafeDelete( m_pStageData->m_pBoss );
			StageMessage msg;
			msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_BOSS_MODE_STARTED;
			m_pStageData->m_MsgQueue.push( msg );
			m_pStageData->m_pBoss = pNewEnemy;
		}
	}

	void StageVCPU::SysSetFrameTotal()
	{
		Pop();
		m_pStageData->m_FrameTotal = Top().m_Integer;
		Pop();
	}

	void StageVCPU::SysStageSwitchToNextStage()
	{
		Pop();
		int stageNo = Top().m_Integer;	// ステージ番号
		Pop();

		// メッセージ送信
		StageMessage msg;
		msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_STAGE_CLEARED;
		StageMessage::StageMessageData data;
		data.m_Integer = stageNo;
		msg.m_MsgDataList.push_back( data );
		m_pStageData->m_MsgQueue.push( msg );
	}

	void StageVCPU::SysSetStageBaseConsLevel()
	{
		Pop();
		int level = Top().m_Integer;
		Pop();

		m_pStageData->m_ConsLevel = level;
	}

	void StageVCPU::SysGetPlayerCrystalUsedTotal()
	{
		Pop();

		Push( m_pStageData->m_TotalGameData.m_CrystalUsed );
	}

	void StageVCPU::SysGetPlayerCrystalTotal()
	{
		Pop();

		Push( m_pStageData->m_TotalGameData.m_CrystalTotal );
	}

	void StageVCPU::OpSysCall( int val )
	{
		switch( val ){
			case VM::SYS_STAGE_ADD_ENEMY:
				SysAddEnemy();
				break;
			case VM::SYS_STAGE_ADD_ENEMY_INIPOS:
				SysAddEnemyIniPos();
				break;
			case VM::SYS_STAGE_GET_FRAME:
				SysGetFrame();
				break;
			case VM::SYS_STAGE_SET_FRAME:
				SysSetFrame();
				break;
			case VM::SYS_GET_RANDOM_F:
				SysGetRand();
				break;
			case VM::SYS_STAGE_PROC_ENEMY_PATTERN_FILE:
				SysProcEnemyPatternFile();
				break;
			case VM::SYS_STAGE_CREATE_BOSS:
				SysStageCreateBoss();
				break;
			case VM::SYS_STAGE_SET_FRAME_TOTAL:
				SysSetFrameTotal();
				break;

			case VM::SYS_STAGE_SWITCH_TO_NEXT_STAGE:
				SysStageSwitchToNextStage();
				break;

			case VM::SYS_PLAY_BGM:
				SysPlayBGM();
				break;
			case VM::SYS_STOP_BGM:
				SysStopBGM();
				break;

			case VM::SYS_SET_STAGE_BASE_CONS_LEVEL:
				SysSetStageBaseConsLevel();
				break;

			case VM::SYS_STAGE_SET_BOSS_FLAG:
				SysStageSetBossFlag();
				break;
			case VM::SYS_STAGE_GET_BOSS_FLAG:
				SysStageGetBossFlag();
				break;

			case VM::SYS_GET_PLAYER_CRYSTAL_USED_TOTAL:
				SysGetPlayerCrystalUsedTotal();
				break;
			case VM::SYS_GET_PLAYER_CRYSTAL_TOTAL:
				SysGetPlayerCrystalTotal();
				break;

			default:
				VM::VCPU::OpSysCall( val );
				break;
		}
	}
}
#include "EnemyVCPU.h"

#include <MAPIL/MAPIL.h>

#include "Enemy.h"
#include "EnemyShot.h"
#include "Stage.h"
#include "Player.h"
#include "Item.h"
#include "Effect.h"
#include "EnemyShotGroup.h"

namespace GameEngine
{
	EnemyVCPU::EnemyVCPU() : VM::VCPU()
	{
	}

	EnemyVCPU::~EnemyVCPU()
	{
	}

	void EnemyVCPU::Init( VM::Data* pData, EnemyData* pEnemyData )
	{
		m_pData = pData;
		m_pEnemyData = pEnemyData;
		VM::VCPU::Init();
	}

	void EnemyVCPU::SysGetEnemyPosX()
	{
		Pop();
#if defined ( USE_FLOATING_POINT )
		Push( m_pEnemyData->m_PosX );
#elif defined ( USE_GAME_UNIT )
		Push( m_pEnemyData->m_GUData.m_PosX.GetFloat() );
#endif
	}

	void EnemyVCPU::SysGetEnemyPosY()
	{
		Pop();
#if defined ( USE_FLOATING_POINT )
		Push( m_pEnemyData->m_PosY );
#elif defined ( USE_GAME_UNIT )
		Push( m_pEnemyData->m_GUData.m_PosY.GetFloat() );
#endif
	}

	void EnemyVCPU::SysGetPlayerHP()
	{
		Pop();
		Push( m_pEnemyData->m_pStageData->m_pPlayer->GetHP() );
	}

	void EnemyVCPU::SysGetEnemyHP()
	{
		Pop();
		Push( m_pEnemyData->m_HP );
	}

	void EnemyVCPU::SysGetEnemySpeed()
	{
		Pop();
		Push( 0 );
	}

	void EnemyVCPU::SysGetEnemyCounter()
	{
		Pop();
		Push( m_pEnemyData->m_Counter );
	}

	void EnemyVCPU::SysGetEnemyCounterF()
	{
		Pop();
		Push( VM::Value( static_cast < float > ( m_pEnemyData->m_Counter ) ) );
	}

	void EnemyVCPU::SysGetEnemyAngle()
	{
		Pop();
		Push( 0 );
	}

	void EnemyVCPU::SysGetEnemyConsGauge()
	{
		Pop();
		Push( m_pEnemyData->m_ConsGauge );
	}

	void EnemyVCPU::SysGetEnemyReg()
	{
		Pop();
		int idx = Top().m_Integer;
		Pop();
		if( idx < MAX_ENEMY_REGS ){
			Push( m_pEnemyData->m_Regs[ idx ] );
		}
		else{
			::exit( 1 );
		}
	}

	void EnemyVCPU::SysSetEnemyName()
	{
		Pop();
		m_pEnemyData->m_Name = Top().m_pString->m_Str;
		Pop();
	}

	void EnemyVCPU::SysSetEnemyPos()
	{
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
#if defined ( USE_FLOATING_POINT )
		m_pEnemyData->m_PosX = x;
		m_pEnemyData->m_PosY = y;
#elif defined ( USE_GAME_UNIT )
		m_pEnemyData->m_GUData.m_PosX = x;
		m_pEnemyData->m_GUData.m_PosY = y;
#endif
	}

	void EnemyVCPU::SysSetEnemyPos3D()
	{
		Pop();
		float z = Top().m_Float;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
#if defined ( USE_FLOATING_POINT )
		m_pEnemyData->m_PosX = x;
		m_pEnemyData->m_PosY = y;
		m_pEnemyData->m_PosZ = z;
#elif defined ( USE_GAME_UNIT )
		m_pEnemyData->m_GUData.m_PosX = x;
		m_pEnemyData->m_GUData.m_PosY = y;
		m_pEnemyData->m_GUData.m_PosZ = z;
#endif
	}

	void EnemyVCPU::SysSetEnemyPos3DGU()
	{
		Pop();
		ScriptGU z = Top().m_Integer;
		Pop();
		ScriptGU y = Top().m_Integer;
		Pop();
		ScriptGU x = Top().m_Integer;
		Pop();
#if defined ( USE_FLOATING_POINT )
		m_pEnemyData->m_PosX = x;
		m_pEnemyData->m_PosY = y;
		m_pEnemyData->m_PosZ = z;
#elif defined ( USE_GAME_UNIT )
		m_pEnemyData->m_GUData.m_PosX.SetRawValue( x );
		m_pEnemyData->m_GUData.m_PosY.SetRawValue( y );
		m_pEnemyData->m_GUData.m_PosZ.SetRawValue( z );
#endif
	}

	void EnemyVCPU::SysSetEnemyAngle()
	{
		Pop();
		int angle = Top().m_Integer;
		Pop();
	}

	void EnemyVCPU::SysSetEnemyAngle3D()
	{
		Pop();
		float rz = Top().m_Float;
		Pop();
		float ry = Top().m_Float;
		Pop();
		float rx = Top().m_Float;
		Pop();
		m_pEnemyData->m_RotX = rx;
		m_pEnemyData->m_RotY = ry;
		m_pEnemyData->m_RotZ = rz;
	}

	void EnemyVCPU::SysSetEnemyScale3D()
	{
		Pop();
		float sz = Top().m_Float;
		Pop();
		float sy = Top().m_Float;
		Pop();
		float sx = Top().m_Float;
		Pop();
		m_pEnemyData->m_ScaleX = sx;
		m_pEnemyData->m_ScaleY = sy;
		m_pEnemyData->m_ScaleZ = sz;
	}

	void EnemyVCPU::SysSetEnemySpeed()
	{
		Pop();
		int speed = Top().m_Integer;
		Pop();
	}

	void EnemyVCPU::SysSetEnemyHP()
	{
		Pop();
		int hp = Top().m_Integer;
		Pop();
		m_pEnemyData->m_HP = hp;
		m_pEnemyData->m_MaxHP = hp;
	}

	void EnemyVCPU::SysSetEnemyImgID()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		m_pEnemyData->m_ImgID = id;
		m_pEnemyData->m_DispMode = EnemyData::DISPLAY_MODE_2D;
	}

	void EnemyVCPU::SysSetEnemyModelID()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		m_pEnemyData->m_ImgID = id;
		m_pEnemyData->m_DispMode = EnemyData::DISPLAY_MODE_3D;
	}

	void EnemyVCPU::SysSetEnemySkinModelID()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		m_pEnemyData->m_ImgID = id;
		m_pEnemyData->m_DispMode = EnemyData::DISPLAY_MODE_3D_ANIM;
	}

	void EnemyVCPU::SysSetEnemyModelAnimTime()
	{
		Pop();
		float time = Top().m_Float;
		Pop();

		m_pEnemyData->m_Time = time;
	}

	void EnemyVCPU::SysSetEnemyCollisionRadius()
	{
		Pop();
		float radius = Top().m_Float;
		Pop();
#if defined ( USE_FLOATING_POINT )
		m_pEnemyData->m_ColRadius = radius;
#elif defined ( USE_GAME_UNIT )
		m_pEnemyData->m_GUData.m_ColRadius = radius;
#endif
	}

	void EnemyVCPU::SysSetEnemyScore()
	{
		Pop();
		int score = Top().m_Integer;
		Pop();
		m_pEnemyData->m_Score = score;
	}

	void EnemyVCPU::SysSetEnemyBossFlag()
	{
		Pop();
		int flag = Top().m_Integer;
		Pop();
		m_pEnemyData->m_IsBoss = flag;
	}

	void EnemyVCPU::SysSetEnemyConsGauge()
	{
		Pop();
		int gauge = Top().m_Integer;
		Pop();
		m_pEnemyData->m_ConsGauge = gauge;
	}

	void EnemyVCPU::SysEnemyEnableInvincible()
	{
		Pop();
		m_pEnemyData->m_IsInvincibleMode = true;
	}

	void EnemyVCPU::SysEnemyDisableInvincible()
	{
		Pop();
		m_pEnemyData->m_IsInvincibleMode = false;
	}

	void EnemyVCPU::SysEnemyEnableNonCollisionMode()
	{
		Pop();
		m_pEnemyData->m_IsNonCollisionMode = true;
	}

	void EnemyVCPU::SysEnemyDisableNonCollisionMode()
	{
		Pop();
		m_pEnemyData->m_IsNonCollisionMode = false;
	}

	void EnemyVCPU::SysEnemyInvokeConsSkill()
	{
		Pop();
		// 第3引数（属性）
		int attr = Top().m_Integer;
		Pop();
		// 第2引数（消費コスト）
		int cost = Top().m_Integer;
		Pop();
		// 第1引数（スキル名）
		std::string* pStr = new std::string;
		*pStr = Top().m_pString->m_Str;
		Pop();

		// 統計情報の更新
		if( m_pEnemyData->m_pStageData->m_StageStat.m_EnemyStat.find( m_pEnemyData->m_Name )
			== m_pEnemyData->m_pStageData->m_StageStat.m_EnemyStat.end() ){
			EnemyStat s;
			m_pEnemyData->m_pStageData->m_StageStat.m_EnemyStat[ m_pEnemyData->m_Name ] = s;
		}
		if( m_pEnemyData->m_pStageData->m_StageStat.m_EnemyStat[ m_pEnemyData->m_Name ].m_SkillStat.find( *pStr )
			== m_pEnemyData->m_pStageData->m_StageStat.m_EnemyStat[ m_pEnemyData->m_Name ].m_SkillStat.end() ){
			SkillStat s;
			m_pEnemyData->m_pStageData->m_StageStat.m_EnemyStat[ m_pEnemyData->m_Name ].m_SkillStat[ *pStr ] = s;
		}
		else{
			++m_pEnemyData->m_pStageData->m_StageStat.m_EnemyStat[ m_pEnemyData->m_Name ].m_SkillStat[ *pStr ].m_Used;
		}

		// 技を使用したのが、ボスの場合
		if( m_pEnemyData->m_IsBoss ){
			// メッセージ構築
			StageMessage msg;
			// 意識技の呼び出しメッセージ
			msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_BOSS_INVOKE_CONS_SKILL;
			StageMessage::StageMessageData data;
			data.m_Integer = cost;
			msg.m_MsgDataList.push_back( data );
			data.m_pString = pStr;
			msg.m_MsgDataList.push_back( data );
			// メッセージ送信
			m_pEnemyData->m_pStageData->m_MsgQueue.push( msg );
			// 意識技エフェクト開始
			m_pEnemyData->m_IsConsSkillMode = true;
			m_pEnemyData->m_ConsSkillName = *pStr;
		}
		// 技を使用したのが、通常の敵の場合
		else{
			// メッセージ構築
			StageMessage msg;
			// 意識技の呼び出しメッセージ
			msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_ENEMY_INVOKE_CONS_SKILL;
			// メッセージ送信
			m_pEnemyData->m_pStageData->m_MsgQueue.push( msg );
			// 意識技エフェクト開始
			m_pEnemyData->m_IsConsSkillMode = true;
			m_pEnemyData->m_ConsSkillName = *pStr;
			MAPIL::SafeDelete( pStr );
		}

		// 意識ゲージ減少
		m_pEnemyData->m_ConsGauge -= cost;
		// 無敵状態に設定
		//m_pEnemyData->m_IsInvincibleMode = true;
		m_pEnemyData->m_ConsSkillAttr = attr;
	}
		
	void EnemyVCPU::SysEnemyStopConsSkill()
	{
		Pop();
		// メッセージ送信
		StageMessage msg;
		msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_BOSS_STOP_CONS_SKILL;
		m_pEnemyData->m_pStageData->m_MsgQueue.push( msg );
		// 無敵状態に設定
		m_pEnemyData->m_IsInvincibleMode = false;
		m_pEnemyData->m_IsConsSkillMode = false;
	}

	void EnemyVCPU::SysCreateEnemyShot1()
	{
		Pop();
		int power = Top().m_Integer;
		Pop();
		int imgID = Top().m_Integer;
		Pop();
		float radius = Top().m_Float;
		Pop();
		float angle = Top().m_Float;
		Pop();
		float speed = Top().m_Float;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		EnemyShot* pNewShot = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEnemyShot( 0 );
		pNewShot->SetPos( x, y );
		pNewShot->SetAngle( angle );
		pNewShot->SetSpeed( speed );
		pNewShot->SetImage( imgID );
		pNewShot->SetPower( power );
		pNewShot->SetCollisionRadius( radius );
		m_pEnemyData->m_pStageData->m_EnemyShotList.push_back( pNewShot );
	}

	void EnemyVCPU::SysGetPlayerPosX()
	{
		Pop();

#if defined ( USE_FLOATING_POINT )
		float x;
		float y;
		m_pEnemyData->m_pStageData->m_pPlayer->GetPos( &x, &y );
		Push( x );
#elif defined ( USE_GAME_UNIT )
		GameUnit x;
		GameUnit y;
		m_pEnemyData->m_pStageData->m_pPlayer->GetPos( &x, &y );
		Push( x.GetFloat() );
#endif
	}

	void EnemyVCPU::SysGetPlayerPosY()
	{
		Pop();
#if defined ( USE_FLOATING_POINT )
		float x;
		float y;
		m_pEnemyData->m_pStageData->m_pPlayer->GetPos( &x, &y );
		Push( y );
#elif defined ( USE_GAME_UNIT )
		GameUnit x;
		GameUnit y;
		m_pEnemyData->m_pStageData->m_pPlayer->GetPos( &x, &y );
		Push( y.GetFloat() );
#endif
	}

	void EnemyVCPU::SysGetRandF()
	{
		Pop();
		Push( m_pEnemyData->m_pStageData->m_RandGen.GetRand() * 1.0f / m_pEnemyData->m_pStageData->m_RandGen.GetRandMax() );
	}

	void EnemyVCPU::SysGetRandGU()
	{
		Pop();
		GameUnit r = m_pEnemyData->m_pStageData->m_RandGen.GetRand();
		GameUnit max = m_pEnemyData->m_pStageData->m_RandGen.GetRandMax();
		GameUnit l = r  / max;
		Push( l.GetRawValue() );
	}

	void EnemyVCPU::SysCreateEffect1()
	{
		Pop();

		int subID = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();

		Effect* pNewEffect = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEffect( id, subID );
		pNewEffect->SetPos( x, y );
		m_pEnemyData->m_pStageData->m_EffectList.push_back( pNewEffect );
	}

	void EnemyVCPU::SysSendEventToEnemyShotGroup()
	{
		Pop();
		int ev = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		// グローバルに割り当てられたものに、イベント送信
		std::for_each(	m_pEnemyData->m_pStageData->m_EnemyShotGroupList.begin(),
						m_pEnemyData->m_pStageData->m_EnemyShotGroupList.end(),
						[id,ev]( EnemyShotGroup* pGroup ){
							if( pGroup == 0 ){
								return;
							}
							if( id == pGroup->GetSubID() ){
								pGroup->SendEvent( ev );
							} } );

		// 敵に割り当てられているものに、イベント送信
		std::for_each(	m_pEnemyData->m_ShotGroupList.begin(),
						m_pEnemyData->m_ShotGroupList.end(),
						[id,ev]( EnemyShotGroup* pGroup ){
							if( pGroup == 0 ){
								return;
							}
							if( id == pGroup->GetSubID() ){
								pGroup->SendEvent( ev );
							} } );
	}

	void EnemyVCPU::SysCreateItem()
	{
		Pop();

		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int subID = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		std::shared_ptr < Item > pNewItem;
		pNewItem.reset( m_pEnemyData->m_pStageData->m_ObjBuilder.CreateItem( id, subID ) );
		//Item* pNewItem = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateItem( id, subID );
		pNewItem->SetPos( x, y );
		m_pEnemyData->m_pStageData->m_ItemList.push_back( pNewItem );
	}

	void EnemyVCPU::SysCreateEnemyShotGroup()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		EnemyShotGroup* pNewGroup = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEnemyShotGroup( id, m_pEnemyData );
		pNewGroup->Init();
		pNewGroup->SetMasterEnemyName( m_pEnemyData->m_Name );
		m_pEnemyData->m_ShotGroupList.push_back( pNewGroup );
	}

	void EnemyVCPU::SysPlaySE()
	{
		Pop();
		int id = m_pEnemyData->m_pResouceMap->m_pStageResourceMap->m_SEMap[ Top().m_Integer ];
		Pop();
		MAPIL::PlayStaticBuffer( id );
	}

	void EnemyVCPU::SysStopSE()
	{
		Pop();
		int id = m_pEnemyData->m_pResouceMap->m_pStageResourceMap->m_SEMap[ Top().m_Integer ];
		Pop();
		MAPIL::StopStaticBuffer( id );
	}

	void EnemyVCPU::SysCreateEnemyShotGroupReg()
	{
		Pop();
		int reg = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		EnemyShotGroup* pNewGroup = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEnemyShotGroup( id, m_pEnemyData );
		pNewGroup->Init();
		pNewGroup->SetReg( reg );
		pNewGroup->SetMasterEnemyName( m_pEnemyData->m_Name );
		m_pEnemyData->m_ShotGroupList.push_back( pNewGroup );
	}

	void EnemyVCPU::SysStageGetBossFlag()
	{
		Pop();
		Push( m_pEnemyData->m_pStageData->m_BossMode );
	}

	void EnemyVCPU::SysStageSetBossFlag()
	{
		Pop();
		m_pEnemyData->m_pStageData->m_BossMode = Top().m_Integer;
		Pop();
	}

	void EnemyVCPU::SysEnemyShiftNextMode()
	{
		Pop();
		// メッセージ送信
		StageMessage msg;
		msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_BOSS_SHIFT_NEXT_MODE;
		m_pEnemyData->m_pStageData->m_MsgQueue.push( msg );
	}

	void EnemyVCPU::SysSearchItem()
	{
		Pop();
		std::shared_ptr < Item > raw = m_pEnemyData->m_RefItem.lock();
		if( !raw ){
			if( m_pEnemyData->m_pStageData->m_ItemList.size() > 0 ){
				ItemList::iterator it = m_pEnemyData->m_pStageData->m_ItemList.begin();
				int random = m_pEnemyData->m_pStageData->m_RandGen.GetRand() * 10 / m_pEnemyData->m_pStageData->m_RandGen.GetRandMax();
				int offset = random % m_pEnemyData->m_pStageData->m_ItemList.size();
				for( int i = 0; i < offset; ++i ){
					++it;
				}
				m_pEnemyData->m_RefItem = ( *it );
			}
		}
	}

	void EnemyVCPU::SysGetItemPosX()
	{
		Pop();
		std::shared_ptr < Item > raw = m_pEnemyData->m_RefItem.lock();
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

	void EnemyVCPU::SysGetItemPosY()
	{
		Pop();
		std::shared_ptr < Item > raw = m_pEnemyData->m_RefItem.lock();
		if( raw ){
#if defined ( USE_FLOATING_POINT )
			float x;
			float y;
			raw->GetPos( &x, &y );
			Push( y );
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

	void EnemyVCPU::SysCreateEnemyShotGroupFReg()
	{
		Pop();
		float reg = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		EnemyShotGroup* pNewGroup = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEnemyShotGroup( id, m_pEnemyData );
		pNewGroup->Init();
		pNewGroup->SetFReg( reg );
		pNewGroup->SetMasterEnemyName( m_pEnemyData->m_Name );
		m_pEnemyData->m_ShotGroupList.push_back( pNewGroup );
	}


	void EnemyVCPU::SysGetPlayerPosXGU()
	{
		Pop();
		GameUnit x;
		GameUnit y;
		m_pEnemyData->m_pStageData->m_pPlayer->GetPos( &x, &y );
		Push( x.GetRawValue() );
	}

	void EnemyVCPU::SysGetPlayerPosYGU()
	{
		Pop();
		GameUnit x;
		GameUnit y;
		m_pEnemyData->m_pStageData->m_pPlayer->GetPos( &x, &y );
		Push( y.GetRawValue() );
	}

	void EnemyVCPU::SysGetEnemyPosXGU()
	{
		Pop();
		Push( m_pEnemyData->m_GUData.m_PosX.GetRawValue() );
	}

	void EnemyVCPU::SysGetEnemyPosYGU()
	{
		Pop();
		Push( m_pEnemyData->m_GUData.m_PosY.GetRawValue() );
	}

	void EnemyVCPU::SysSetEnemyCollisionRadiusGU()
	{
		Pop();
		int radius = Top().m_Integer;
		Pop();
		m_pEnemyData->m_GUData.m_ColRadius.SetRawValue( radius );
	}

	void EnemyVCPU::SysSetEnemyPosGU()
	{
		Pop();
		int y = Top().m_Integer;
		Pop();
		int x = Top().m_Integer;
		Pop();

		m_pEnemyData->m_GUData.m_PosX.SetRawValue( x );
		m_pEnemyData->m_GUData.m_PosY.SetRawValue( y );
	}

	void EnemyVCPU::SysCreateEffect1GU()
	{
		Pop();
		int subID = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();
		int rawY = Top().m_Integer;
		Pop();
		int rawX = Top().m_Integer;
		Pop();

		GameUnit x;
		GameUnit y;
		x.SetRawValue( rawX );
		y.SetRawValue( rawY );

		Effect* pNewEffect = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEffect( id, subID );
		pNewEffect->SetPos( x.GetFloat(), y.GetFloat() );
		m_pEnemyData->m_pStageData->m_EffectList.push_back( pNewEffect );
	}

	void EnemyVCPU::SysCreateItemGU()
	{
		Pop();

		int rawY = Top().m_Integer;
		Pop();
		int rawX = Top().m_Integer;
		Pop();
		int subID = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		GameUnit x;
		GameUnit y;

		x.SetRawValue( rawX );
		y.SetRawValue( rawY );

		std::shared_ptr < Item > pNewItem;
		pNewItem.reset( m_pEnemyData->m_pStageData->m_ObjBuilder.CreateItem( id, subID ) );
		pNewItem->SetPos( x, y );
		m_pEnemyData->m_pStageData->m_ItemList.push_back( pNewItem );
	}

	void EnemyVCPU::SysCreateEnemyShot1GU()
	{
		GameUnit x;
		GameUnit y;
		GameUnit speed;
		GameUnit angle;
		GameUnit radius;


		Pop();
		int power = Top().m_Integer;
		Pop();
		int imgID = Top().m_Integer;
		Pop();
		radius.SetRawValue( Top().m_Integer );
		Pop();
		angle.SetRawValue( Top().m_Integer );
		Pop();
		speed.SetRawValue( Top().m_Integer );
		Pop();
		y.SetRawValue( Top().m_Integer );
		Pop();
		x.SetRawValue( Top().m_Integer );
		Pop();
		EnemyShot* pNewShot = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEnemyShot( 0 );
		pNewShot->SetPos( x, y );
		pNewShot->SetAngle( angle );
		pNewShot->SetSpeed( speed );
		pNewShot->SetImage( imgID );
		pNewShot->SetPower( power );
		pNewShot->SetCollisionRadius( radius );
		m_pEnemyData->m_pStageData->m_EnemyShotList.push_back( pNewShot );
	}

	void EnemyVCPU::SysCreateEnemyShotGroupGReg()
	{
		Pop();
		int reg = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		EnemyShotGroup* pNewGroup = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEnemyShotGroup( id, m_pEnemyData );
		pNewGroup->Init();
		pNewGroup->SetReg( reg );
		pNewGroup->SetMasterEnemyName( m_pEnemyData->m_Name );
		m_pEnemyData->m_ShotGroupList.push_back( pNewGroup );
	}

	void EnemyVCPU::SysCreateEnemyShotGroupGReg5()
	{
		GameUnit reg[ 5 ];

		Pop();
		for( int i = 4; i >= 0; --i ){
			reg[ i ].SetRawValue( Top().m_Integer );
			Pop();
		}
		int id = Top().m_Integer;
		Pop();

		EnemyShotGroup* pNewGroup = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEnemyShotGroup( id, m_pEnemyData );
		pNewGroup->Init();
		for( int i = 0; i < 5; ++i ){
			pNewGroup->SetGReg( i, reg[ i ] );
		}
		pNewGroup->SetMasterEnemyName( m_pEnemyData->m_Name );
		m_pEnemyData->m_ShotGroupList.push_back( pNewGroup );
	}

	void EnemyVCPU::SysGetItemPosXGU()
	{
		Pop();
		std::shared_ptr < Item > raw = m_pEnemyData->m_RefItem.lock();
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

	void EnemyVCPU::SysGetItemPosYGU()
	{
		Pop();
		std::shared_ptr < Item > raw = m_pEnemyData->m_RefItem.lock();
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

	void EnemyVCPU::SysEnemyDamagedByConsShot()
	{
		Pop();
		Push( m_pEnemyData->m_StatusFlags[ EnemyData::DAMAGED_BY_CONS_SHOT ] ? 1 : 0 );
	}

	void EnemyVCPU::SysGetDifficulty()
	{
		Pop();
		Push( m_pEnemyData->m_pStageData->m_Difficulty );
	}

	void EnemyVCPU::SysSetBossPhaseTotal()
	{
		Pop();
		int phase = Top().m_Integer;
		Pop();
		
		m_pEnemyData->m_pStageData->m_BossPhaseTotal = phase;
	}

	void EnemyVCPU::SysSetBossPhaseShiftHP()
	{
		Pop();
		int hp = Top().m_Integer;
		Pop();
		int phase = Top().m_Integer;
		Pop();

		m_pEnemyData->m_pStageData->m_BossPhaseStartHP[ phase ] = hp;
	}

	void EnemyVCPU::SysCreateEnemyIniPosGU()
	{
		GameUnit x;
		GameUnit y;
		int id;

		Pop();
		y.SetRawValue( Top().m_Integer );
		Pop();
		x.SetRawValue( Top().m_Integer );
		Pop();
		id = Top().m_Integer;
		Pop();

		Enemy* pNewEnemy = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEnemy( id );
		pNewEnemy->Init( x, y );
		m_pEnemyData->m_pStageData->m_EnemyList.push_back( std::shared_ptr < Enemy > ( pNewEnemy ) );
	}

	void EnemyVCPU::SysCreateEnemyIniPosRegGU()
	{
		GameUnit x;
		GameUnit y;
		int id;
		int reg;

		Pop();
		reg = Top().m_Integer;
		Pop();
		y.SetRawValue( Top().m_Integer );
		Pop();
		x.SetRawValue( Top().m_Integer );
		Pop();
		id = Top().m_Integer;
		Pop();

		Enemy* pNewEnemy = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEnemy( id );
		pNewEnemy->Init( x, y );
		pNewEnemy->SetReg( 0, reg );
		m_pEnemyData->m_pStageData->m_EnemyList.push_back( std::shared_ptr < Enemy > ( pNewEnemy ) );
	}

	void EnemyVCPU::SysCreateEnemyIniPosReg5GU()
	{
		GameUnit x;
		GameUnit y;
		int id;
		int reg[ 5 ];

		Pop();
		for( int i = 4; i >= 0; --i ){
			reg[ i ] = Top().m_Integer;
			Pop();
		}
		y.SetRawValue( Top().m_Integer );
		Pop();
		x.SetRawValue( Top().m_Integer );
		Pop();
		id = Top().m_Integer;
		Pop();

		Enemy* pNewEnemy = m_pEnemyData->m_pStageData->m_ObjBuilder.CreateEnemy( id );
		pNewEnemy->Init( x, y );
		for( int i = 0; i < 5; ++i ){
			pNewEnemy->SetReg( i, reg[ i ] );
		}
		m_pEnemyData->m_pStageData->m_EnemyList.push_back( std::shared_ptr < Enemy > ( pNewEnemy ) );
	}

	void EnemyVCPU::SysSearchEnemyInSkillMode()
	{
		Pop();

		EnemyList::iterator it;
		for(	it = m_pEnemyData->m_pStageData->m_EnemyList.begin();
				it != m_pEnemyData->m_pStageData->m_EnemyList.end();
				++it ){
			if( ( *it )->IsInSkillMode() ){
				Push( 1 );
				return;
			}
		}
		Push( 0 );
	}

	void EnemyVCPU::SysSearchEnemyInSkillModeByName()
	{
		Pop();
		std::string name = Top().m_pString->m_Str;
		Pop();

		EnemyList::iterator it;
		for(	it = m_pEnemyData->m_pStageData->m_EnemyList.begin();
				it != m_pEnemyData->m_pStageData->m_EnemyList.end();
				++it ){
			if( ( *it )->IsInSkillMode() ){
				if( ( *it )->GetName() == name ){
					Push( 1 );
					return;
				}
			}
		}
		Push( 0 );
	}

	void EnemyVCPU::SysEnemyDamagedByConsShotIndex()
	{
		Pop();
		int idx = Top().m_Integer;
		Pop();

		if( m_pEnemyData->m_StatusFlags[ idx ] ){
			Push( 1 );
		}
		else{
			Push( 0 );
		}
	}

	void EnemyVCPU::SysGetPlayerCurCons()
	{
		Pop();
		
		Push( m_pEnemyData->m_pStageData->m_pPlayer->GetCurCons() );
	}

	void EnemyVCPU::SysGetPlayerConsGauge()
	{
		Pop();
		int cons = Top().m_Integer;
		Pop();

		Push( m_pEnemyData->m_pStageData->m_pPlayer->GetConsGauge( cons ) );
	}

	void EnemyVCPU::SysSetEnemySubID()
	{
		Pop();
		int subID = Top().m_Integer;
		Pop();

		m_pEnemyData->m_SubID = subID;
	}

	void EnemyVCPU::SysSendEventToEnemy()
	{
		Pop();
		int ev = Top().m_Integer;
		Pop();
		int subID = Top().m_Integer;
		Pop();
		std::string name = Top().m_pString->m_Str;

		// イベント送信
		std::for_each(	m_pEnemyData->m_pStageData->m_EnemyList.begin(),
						m_pEnemyData->m_pStageData->m_EnemyList.end(),
						[name,subID,ev]( const std::shared_ptr < Enemy >& pEnemy ){
							// null check
							if( pEnemy == 0 ){
								return;
							}
							// 名前一致チェック
							if( name != pEnemy->GetName() ){
								return;
							}
							// サブIDチェック
							if( subID != pEnemy->GetSubID() ){
								return;
							}
							pEnemy->SendEvent( ev );
						} );
	}

	void EnemyVCPU::SysGetEvent()
	{
		Pop();

		if( m_pEnemyData->m_EventQueue.empty() ){
			Push( -10000 );
		}
		else{
			int ev = m_pEnemyData->m_EventQueue.front();
			m_pEnemyData->m_EventQueue.pop();
			Push( ev );
		}
	}

	void EnemyVCPU::SysGetPlayerScoreTotal()
	{
		Pop();

		Push( m_pEnemyData->m_pStageData->m_TotalGameData.m_Score );
	}

	void EnemyVCPU::SysGetPlayerCrystalTotal()
	{
		Pop();

		Push( m_pEnemyData->m_pStageData->m_TotalGameData.m_CrystalTotal );
	}

	void EnemyVCPU::SysGetPlayerKilledTotal()
	{
		Pop();

		Push( m_pEnemyData->m_pStageData->m_TotalGameData.m_Killed );
	}

	void EnemyVCPU::SysDamageEnemy()
	{
		Pop();
		int damage = Top().m_Integer;
		Pop();

		m_pEnemyData->m_HP -= damage;
	}

	void EnemyVCPU::SysSetEnemyConsAttr()
	{
		Pop();
		int attr = Top().m_Integer;
		Pop();

		m_pEnemyData->m_ConsType = attr;
	}

	void EnemyVCPU::OpSysCall( int val )
	{
		switch( val ){
			case VM::SYS_GET_RANDOM_F:
				SysGetRandF();
				break;
			case VM::SYS_GET_RANDOM_GU:
				SysGetRandGU();
				break;
			case VM::SYS_GET_PLAYER_POSX_GU:
				SysGetPlayerPosXGU();
				break;
			case VM::SYS_GET_PLAYER_POSY_GU:
				SysGetPlayerPosYGU();
				break;
			case VM::SYS_GET_PLAYER_POSX:
				SysGetPlayerPosX();
				break;
			case VM::SYS_GET_PLAYER_POSY:
				SysGetPlayerPosY();
				break;
			case VM::SYS_GET_PLAYER_HP:
				SysGetPlayerHP();
				break;
			case VM::SYS_GET_PLAYER_CUR_CONS:
				SysGetPlayerCurCons();
				break;
			case VM::SYS_GET_PLAYER_CONS_GAUGE:
				SysGetPlayerConsGauge();
				break;
			case VM::SYS_GET_PLAYER_SCORE_TOTAL:
				SysGetPlayerScoreTotal();
				break;
			case VM::SYS_GET_PLAYER_CRYSTAL_TOTAL:
				SysGetPlayerCrystalTotal();
				break;
			case VM::SYS_GET_PLAYER_KILLED_TOTAL:
				SysGetPlayerKilledTotal();
				break;

			case VM::SYS_ENEMY_GET_POSX_GU:
				SysGetEnemyPosXGU();
				break;
			case VM::SYS_ENEMY_GET_POSY_GU:
				SysGetEnemyPosYGU();
				break;
			case VM::SYS_ENEMY_SET_COLLISION_RADIUS_GU:
				SysSetEnemyCollisionRadiusGU();
				break;
			case VM::SYS_ENEMY_SET_POS_GU:
				SysSetEnemyPosGU();
				break;
			case VM::SYS_ENEMY_SET_POS_3D_GU:
				SysSetEnemyPos3DGU();
				break;

			case VM::SYS_ENEMY_GET_POSX:
				SysGetEnemyPosX();
				break;
			case VM::SYS_ENEMY_GET_POSY:
				SysGetEnemyPosY();
				break;
			case VM::SYS_ENEMY_GET_HP:
				SysGetEnemyHP();
				break;
			case VM::SYS_ENEMY_GET_SPEED:
				SysGetEnemySpeed();
				break;
			case VM::SYS_ENEMY_GET_COUNTER:
				SysGetEnemyCounter();
				break;
			case VM::SYS_ENEMY_GET_COUNTER_F:
				SysGetEnemyCounterF();
				break;
			case VM::SYS_ENEMY_GET_CONS_GAUGE:
				SysGetEnemyConsGauge();
				break;
			case VM::SYS_ENEMY_GET_REG:
				SysGetEnemyReg();
				break;
			case VM::SYS_ENEMY_SEARCH_ITEM:
				SysSearchItem();
				break;
			case VM::SYS_ENEMY_GET_ITEM_POS_X:
				SysGetItemPosX();
				break;
			case VM::SYS_ENEMY_GET_ITEM_POS_Y:
				SysGetItemPosY();
				break;
				
			case VM::SYS_ENEMY_GET_ITEM_POS_X_GU:
				SysGetItemPosXGU();
				break;
			case VM::SYS_ENEMY_GET_ITEM_POS_Y_GU:
				SysGetItemPosYGU();
				break;

			case VM::SYS_DAMAGE_ENEMY:
				SysDamageEnemy();
				break;

			case VM::SYS_ENEMY_SET_NAME:
				SysSetEnemyName();
				break;
			case VM::SYS_ENEMY_SET_POS:
				SysSetEnemyPos();
				break;
			case VM::SYS_ENEMY_SET_ANGLE_3D:
				SysSetEnemyAngle3D();
				break;
			case VM::SYS_ENEMY_SET_ANGLE:
				SysSetEnemyAngle();
				break;
			case VM::SYS_ENEMY_SET_SCALE_3D:
				SysSetEnemyScale3D();
				break;
			case VM::SYS_ENEMY_SET_SPEED:
				SysSetEnemySpeed();
				break;
			case VM::SYS_ENEMY_SET_HP:
				SysSetEnemyHP();
				break;
			case VM::SYS_ENEMY_CREATE_SHOT_1:
				SysCreateEnemyShot1();
				break;
			case VM::SYS_ENEMY_CREATE_SHOT_1_GU:
				SysCreateEnemyShot1GU();
				break;
			case VM::SYS_ENEMY_SET_IMAGE:
				SysSetEnemyImgID();
				break;
			case VM::SYS_ENEMY_SET_MODEL:
				SysSetEnemyModelID();
				break;
			case VM::SYS_ENEMY_SET_SKIN_MODEL:
				SysSetEnemySkinModelID();
				break;
			case VM::SYS_ENEMY_SET_MODEL_ANIM_TIME:
				SysSetEnemyModelAnimTime();
				break;
			case VM::SYS_ENEMY_SET_COLLISION_RADIUS:
				SysSetEnemyCollisionRadius();
				break;
			case VM::SYS_ENEMY_SET_SCORE:
				SysSetEnemyScore();
				break;
			case VM::SYS_ENEMY_SET_BOSS_FLAG:
				SysSetEnemyBossFlag();
				break;
			case VM::SYS_ENEMY_SET_CONS_GAUGE:
				SysSetEnemyConsGauge();
				break;
			case VM::SYS_ENEMY_SET_CONS_ATTR:
				SysSetEnemyConsAttr();
				break;
			case VM::SYS_ENEMY_ENABLE_INVINCIBLE:
				SysEnemyEnableInvincible();
				break;
			case VM::SYS_ENEMY_DISABLE_INVINCIBLE:
				SysEnemyDisableInvincible();
				break;
			case VM::SYS_ENEMY_ENABLE_NON_COLLISION_MODE:
				SysEnemyEnableNonCollisionMode();
				break;
			case VM::SYS_ENEMY_DISABLE_NON_COLLISION_MODE:
				SysEnemyDisableNonCollisionMode();
				break;
			case VM::SYS_ENEMY_INVOKE_CONS_SKILL:
				SysEnemyInvokeConsSkill();
				break;
			case VM::SYS_ENEMY_STOP_CONS_SKILL:
				SysEnemyStopConsSkill();
				break;
			case VM::SYS_ENEMY_DAMAGED_BY_CONS_SHOT:
				SysEnemyDamagedByConsShot();
				break;
			case VM::SYS_ENEMY_DAMAGED_BY_CONS_SHOT_INDEX:
				SysEnemyDamagedByConsShotIndex();
				break;
			case VM::SYS_SEARCH_ENEMY_IN_SKILL_MODE:
				SysSearchEnemyInSkillMode();
				break;
			case VM::SYS_SEARCH_ENEMY_IN_SKILL_MODE_BY_NAME:
				SysSearchEnemyInSkillModeByName();
				break;

			case VM::SYS_ENEMY_CREATE_EFFECT_1:
				SysCreateEffect1();
				break;
			case VM::SYS_ENEMY_CREATE_EFFECT_1_GU:
				SysCreateEffect1GU();
				break;
			case VM::SYS_CREATE_ITEM:
				SysCreateItem();
				break;
			case VM::SYS_CREATE_ITEM_GU:
				SysCreateItemGU();
				break;
			case VM::SYS_ENEMY_CREATE_SHOT_GROUP:
				SysCreateEnemyShotGroup();
				break;
			case VM::SYS_ENEMY_CREATE_SHOT_GROUP_REG:
				SysCreateEnemyShotGroupReg();
				break;
			case VM::SYS_ENEMY_CREATE_SHOT_GROUP_FREG:
				SysCreateEnemyShotGroupFReg();
				break;
			case VM::SYS_ENEMY_CREATE_SHOT_GROUP_GREG:
				SysCreateEnemyShotGroupGReg();
				break;
			case VM::SYS_ENEMY_CREATE_SHOT_GROUP_GREG5:
				SysCreateEnemyShotGroupGReg5();
				break;
			case VM::SYS_CREATE_ENEMY_INI_POS_REG_GU:
				SysCreateEnemyIniPosRegGU();
				break;
			case VM::SYS_CREATE_ENEMY_INI_POS_GU:
				SysCreateEnemyIniPosGU();
				break;
			case VM::SYS_CREATE_ENEMY_INI_POS_REG5_GU:
				SysCreateEnemyIniPosReg5GU();
				break;
			case VM::SYS_ENEMY_SHIFT_NEXT_MODE:
				SysEnemyShiftNextMode();
				break;
			case VM::SYS_STAGE_GET_DIFFICULTY:
				SysGetDifficulty();
				break;
			case VM::SYS_STAGE_SET_BOSS_PHASE_TOTAL:
				SysSetBossPhaseTotal();
				break;
			case VM::SYS_STAGE_SET_BOSS_PHASE_SHIFT_HP:
				SysSetBossPhaseShiftHP();
				break;
			
			case VM::SYS_SET_ENEMY_SUB_ID:
				SysSetEnemySubID();
				break;
			case VM::SYS_SEND_EVENT_TO_ENEMY:
				SysSendEventToEnemy();
				break;
			case VM::SYS_GET_EVENT:
				SysGetEvent();
				break;

			case VM::SYS_PLAY_SE:
				SysPlaySE();
				break;
			case VM::SYS_STOP_SE:
				SysStopSE();
				break;
			case VM::SYS_SEND_EVENT_TO_ENEMY_SHOT_GROUP:
				SysSendEventToEnemyShotGroup();
				break;
			case VM::SYS_STAGE_SET_BOSS_FLAG:
				SysStageSetBossFlag();
				break;
			case VM::SYS_STAGE_GET_BOSS_FLAG:
				SysStageGetBossFlag();
				break;

			default:
				VM::VCPU::OpSysCall( val );
				break;
		}
	}
}
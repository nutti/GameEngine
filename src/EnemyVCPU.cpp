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
		m_pEnemyData->m_Is3D = false;
	}

	void EnemyVCPU::SysSetEnemyModelID()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		m_pEnemyData->m_ImgID = id;
		m_pEnemyData->m_Is3D = true;
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
		m_pEnemyData->m_IsInvincibleMode = true;
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

	void EnemyVCPU::OpSysCall( int val )
	{
		switch( val ){
			case VM::SYS_GET_RANDOM_F:
				SysGetRandF();
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
			case VM::SYS_ENEMY_SET_IMAGE:
				SysSetEnemyImgID();
				break;
			case VM::SYS_ENEMY_SET_MODEL:
				SysSetEnemyModelID();
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
			case VM::SYS_ENEMY_SHIFT_NEXT_MODE:
				SysEnemyShiftNextMode();
				break;

			case VM::SYS_PLAY_SE:
				SysPlaySE();
				break;
			case VM::SYS_STOP_SE:
				SysStopSE();
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
#ifndef INCLUDED_GAMEENGINE_ITEM_H
#define INCLUDED_GAMEENGINE_ITEM_H

#include <memory>
#include <bitset>

#include "CollisionObject.h"

namespace GameEngine
{
	enum ItemID
	{
		ITEM_ID_CRYSTAL				= 0,		// 意識の結晶
		ITEM_ID_CONS_LEVEL_RECOVER	= 1,		// 意識レベル回復
		ITEM_ID_CONS_RECOVER		= 2,		// 意識回復
		ITEM_ID_CONS_ENHANCE		= 3,		// 意識増幅
		ITEM_ID_RECOVER				= 4,		// HP回復
		ITEM_ID_POWER_UP			= 5,		// 意識消滅弾強化
	};

	enum ItemConsLevelRecoverSubID
	{
		ITEM_SUB_ID_GREEN			= 0,		// 緑ゲージ
	};

	// フラグ管理
	enum StatusFlag
	{
		STATUS_FLAG_OBTAINED		= 0,	// 取得された
		STATUS_FLAG_CONSUMED		= 1,	// 消費された
		STATUS_FLAG_TOTAL,
	};

	struct StageData;

	struct ItemData
	{
		int			m_ItemID;		// アイテム識別子
		int			m_ItemSubID;	// アイテムサブ識別子
		float		m_PosX;			// 位置（X座標）
		float		m_PosY;			// 位置（Y座標）
		float		m_Vel;			// 速度
		float		m_Angle;		// 角度
		float		m_ColRadius;	// 衝突判定の半径
		bool		m_Near;			// プレイヤーが近くにいる場合true
		int			m_Counter;		// カウンタ
		int			m_ConsumedCounter;	// 敵消化カウンタ
		StageData*	m_pStageData;
		std::bitset < STATUS_FLAG_TOTAL >	m_StatusFlags;		// 状態管理フラグ
	};


	struct ResourceMap;
	class Item : public CollisionObject
	{
	private:
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		ItemData							m_ItemData;
		Player*								m_pPlayer;
	public:
		Item( std::shared_ptr < ResourceMap > pMap, StageData* pStageData, int id, int subID );
		~Item();
		void Init( float posX, float posY );				// 初期化
		void Draw();										// 描画
		bool Update();										// 更新
		void SetPos( float posX, float posY );				// 位置を設定
		void Colided( CollisionObject* pObject );			// 衝突時の処理 ディスパッチャ
		void ProcessCollision( Player* pPlayer );			// 衝突時の処理（プレイヤー）
		void ProcessCollision( Enemy* pEnemy );				// 衝突時の処理（敵）
		void ProcessCollision( PlayerShot* pPlayerShot );	// 衝突時の処理（プレイヤーショット）
		void ProcessCollision( EnemyShot* pEnemyShot );		// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );				// 衝突時の処理（アイテム）
		void GetPos( float* pPosX, float* pPosY );
		float GetCollisionRadius();
		int GetItemID() const;
		int GetItemSubID() const;
		void PlayerIsNear( Player* pPlayer );
		bool CanBeConsumed() const;
		void Consume();
	};
}

#endif
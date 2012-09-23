#ifndef INCLUDED_GAMEENGINE_ITEM_H
#define INCLUDED_GAMEENGINE_ITEM_H

#include <memory>

#include "CollisionObject.h"

namespace GameEngine
{
	struct ItemData
	{
		int			m_ItemID;		// アイテム識別子
		float		m_PosX;			// 位置（X座標）
		float		m_PosY;			// 位置（Y座標）
		float		m_ColRadius;	// 衝突判定の半径
		bool		m_Colided;		// 衝突したらtrue
	};


	struct ResourceMap;
	class Item : public CollisionObject
	{
	private:
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		ItemData							m_ItemData;
	public:
		Item( std::shared_ptr < ResourceMap > pMap, int id );
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
	};
}

#endif
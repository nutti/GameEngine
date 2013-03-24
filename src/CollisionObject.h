#ifndef INCLUDED_GAMEENGINE_COLLISIONOBJECT_H
#define INCLUDED_GAMEENGINE_COLLISIONOBJECT_H

#include "GameObject.h"

#include <memory>

namespace GameEngine
{
	class Player;
	class Enemy;
	class PlayerShot;
	class EnemyShot;
	class Item;
	class CollisionObject : public GameObject
	{
	public:
		CollisionObject();
		virtual ~CollisionObject();
		virtual void Init( float posX, float posY ) = 0;				// 初期化
		virtual void Colided( CollisionObject* pObject ) = 0;			// 衝突時の処理 ディスパッチャ
		virtual void ProcessCollision( Player* pPlayer ) = 0;			// 衝突時の処理（プレイヤー）
		virtual void ProcessCollision( Enemy* pEnemy ) = 0;				// 衝突時の処理（敵）
		virtual void ProcessCollision( PlayerShot* pPlayerShot ) = 0;	// 衝突時の処理（プレイヤーショット）
		virtual void ProcessCollision( EnemyShot* pEnemyShot )= 0;		// 衝突時の処理（敵弾）
		//virtual void ProcessCollision( std::shared_ptr < Item > pItem ) = 0;				// 衝突時の処理（アイテム）
		virtual void ProcessCollision( Item* pItem ) = 0;
		virtual void Draw() = 0;										// 描画
		virtual bool Update() = 0;										// 更新
		virtual void GetPos( float* pX, float* pY ) = 0;				// 位置を取得
		virtual float GetCollisionRadius() = 0;							// 衝突半径を取得
	};
}

#endif
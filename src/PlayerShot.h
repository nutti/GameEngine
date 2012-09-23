#ifndef INCLUDED_GAMEENGINE_PLAYERSHOT_H
#define INCLUDED_GAMEENGINE_PLAYERSHOT_H

#include <memory>

#include "CollisionObject.h"

namespace GameEngine
{

	struct ResourceMap;
	class PlayerShot : public CollisionObject
	{
	private:
		class Impl;
		std::auto_ptr < PlayerShot::Impl >		m_pImpl;
	public:
		PlayerShot( std::shared_ptr < ResourceMap > pMap, int id );
		~PlayerShot();
		void Init( float posX, float posY );							// 初期化
		void Draw();													// 描画
		bool Update();													// 更新
		void SetPos( const Player& player );							// 位置を設定
		void SetPos( float posX, float posY );							// 位置を設定
		void Colided( CollisionObject* pObject );						// 衝突時の処理 ディスパッチャ
		void ProcessCollision( Player* pPlayer );						// 衝突時の処理（プレイヤー）
		void ProcessCollision( Enemy* pEnemy );							// 衝突時の処理（敵）
		void ProcessCollision( PlayerShot* pPlayerShot );				// 衝突時の処理（プレイヤーショット）
		void ProcessCollision( EnemyShot* pEnemyShot );		// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );				// 衝突時の処理（アイテム）
		void GetPos( float* pPosX, float* pPosY );
		float GetCollisionRadius();
	};
}

#endif
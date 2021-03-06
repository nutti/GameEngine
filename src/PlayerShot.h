#ifndef INCLUDED_GAMEENGINE_PLAYERSHOT_H
#define INCLUDED_GAMEENGINE_PLAYERSHOT_H

#include <memory>

#include "CollisionObject.h"

namespace GameEngine
{

	struct ResourceMap;
	struct StageData;
	class PlayerShot : public CollisionObject
	{
	private:
		class Impl;
		std::auto_ptr < PlayerShot::Impl >		m_pImpl;
	public:
		PlayerShot( std::shared_ptr < ResourceMap > pMap, int id );
		~PlayerShot();
		
		void Draw();													// 描画
		bool Update();													// 更新
		void SetPos( const Player& player );							// 位置を設定
		
		void SetShotPower( int power );
		void SetConsAttr( int attr );
		void Colided( CollisionObject* pObject );						// 衝突時の処理 ディスパッチャ
		void ProcessCollision( Player* pPlayer );						// 衝突時の処理（プレイヤー）
		void ProcessCollision( Enemy* pEnemy );							// 衝突時の処理（敵）
		void ProcessCollision( PlayerShot* pPlayerShot );				// 衝突時の処理（プレイヤーショット）
		void ProcessCollision( EnemyShot* pEnemyShot );		// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );				// 衝突時の処理（アイテム）
		
#if defined ( USE_FLOATING_POINT )
		void Init( float posX, float posY );							// 初期化
		void GetPos( float* pPosX, float* pPosY );
		float GetCollisionRadius();
		void SetPos( float posX, float posY );							// 位置を設定
		void SetAngle( float angle );									// 角度を設定
		void SetSpeed( float speed );									// 速度を設定
#elif defined ( USE_GAME_UNIT )
		void Init( const GameUnit& posX, const GameUnit& posY );				// 初期化
		void GetPos( GameUnit* pPosX, GameUnit* pPosY );
		GameUnit GetCollisionRadius();
		void SetPos( const GameUnit& posX, const GameUnit& posY );
		void SetAngle( const GameUnit& angle );
		void SetSpeed( const GameUnit& speed );
#endif
		int GetShotPower() const;
		int GetConsAttr() const;
		void SetPlayer( Player* pPlayer );
		void SetEnemy( Enemy* pEnemy );
		void SetStageData( StageData* pData );
		int GetID() const;
	};
}

#endif
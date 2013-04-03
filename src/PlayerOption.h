#ifndef INCLUDED_GAMEENGINE_PLAYEROPTION_H
#define INCLUDED_GAMEENGINE_PLAYEROPTION_H

#include <memory>

#include "CollisionObject.h"
#include "InputTypes.h"

namespace GameEngine
{

	struct ResourceMap;
	struct StageData;
	class Player;
	class PlayerOption : public CollisionObject
	{
	private:
		class Impl;
		std::auto_ptr < PlayerOption::Impl >		m_pImpl;
	public:
		PlayerOption( std::shared_ptr < ResourceMap > pMap, StageData* pStageData, int id );
		~PlayerOption();
		void AttachButtonState( const ButtonStatusHolder& holder );		// キー入力を設定
		
		void Draw();													// 描画
		bool Update();													// 更新
		void Colided( CollisionObject* pObject );						// 衝突時の処理 ディスパッチャ
		void ProcessCollision( Player* pPlayer );						// 衝突時の処理（プレイヤー）
		void ProcessCollision( Enemy* pEnemy );							// 衝突時の処理（敵）
		void ProcessCollision( PlayerShot* pPlayerShot );				// 衝突時の処理（プレイヤーショット）
		void ProcessCollision( EnemyShot* pEnemyShot );					// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );							// 衝突時の処理（アイテム）
#if defined ( USE_FLOATING_POINT )
		void Init( float posX, float posY );							// 初期化
		void GetPos( float* pPosX, float* pPosY );						// 位置を取得
		float GetCollisionRadius();
		void SetPos( float posX, float posY );
#elif defined ( USE_GAME_UNIT )
		void Init( const GameUnit& posX, const GameUnit& posY );							// 初期化
		void GetPos( GameUnit* pPosX, GameUnit* pPosY );						// 位置を取得
		GameUnit GetCollisionRadius();
		void SetPos( const GameUnit& posX, const GameUnit& posY );
#endif
		void ChangeID( int id );
		void ChangeConsMode( int mode );
		void NotifyOptTotal( int total );
	};
}

#endif
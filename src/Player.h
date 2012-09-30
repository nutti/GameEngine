#ifndef INCLUDED_GAMEENGINE_PLAYER_H
#define INCLUDED_GAMEENGINE_PLAYER_H

#include <memory>

#include "CollisionObject.h"
#include "InputTypes.h"

namespace GameEngine
{

	enum PlayerConsMode
	{
		PLAYER_CONS_MODE_NORMAL		= 0,		// 通常モード
		PLAYER_CONS_MODE_GREEN		= 1,		// 緑（追尾モード）
		PLAYER_CONS_MODE_BLUE		= 2,		// 青（一転集中モード）
		PLAYER_CONS_MODE_RED		= 3,		// 赤（全方位モード）
	};

	struct PlayerData
	{
		float		m_PosX;				// 位置（X座標）
		float		m_PosY;				// 位置（Y座標）
		int			m_HP;				// HP
		int			m_ConsCur;			// 現在の意識状態
		int			m_ConsGauge[ 3 ];	// 意識ゲージ
		int			m_ConsLevel[ 3 ];	// 意識レベル
		int			m_ShotPower;		// ショットのパワー
		float		m_ColRadius;		// 衝突半径
		int			m_Counter;			// カウンタ
	};

	struct ResourceMap;
	struct StageData;
	class Player : public CollisionObject
	{
	private:
		class Impl;
		std::auto_ptr < Player::Impl >		m_pImpl;
	public:
		Player( std::shared_ptr < ResourceMap > pMap, StageData* pStageData );
		~Player();
		void AttachButtonState( const ButtonStatusHolder& holder );		// キー入力を設定
		void Init( float posX, float posY );							// 初期化
		void Draw();													// 描画
		bool Update();													// 更新
		void Colided( CollisionObject* pObject );						// 衝突時の処理 ディスパッチャ
		void ProcessCollision( Player* pPlayer );						// 衝突時の処理（プレイヤー）
		void ProcessCollision( Enemy* pEnemy );							// 衝突時の処理（敵）
		void ProcessCollision( PlayerShot* pPlayerShot );				// 衝突時の処理（プレイヤーショット）
		void ProcessCollision( EnemyShot* pEnemyShot );					// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );							// 衝突時の処理（アイテム）
		void GetPos( float* pPosX, float* pPosY );						// 位置を取得
		float GetCollisionRadius();										// 衝突半径を取得
		int GetHP() const;												// HPを取得
		int GetShotPower() const;										// ショットの威力を取得
		int GetConsGauge( int cons ) const;								// 意識ゲージの取得
		int GetConsLevel( int cons ) const;								// 意識レベルの取得
	};
}

#endif
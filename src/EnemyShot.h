#ifndef INCLUDED_GAMEENGINE_ENEMYSHOT_H
#define INCLUDED_GAMEENGINE_ENEMYSHOT_H

#include <memory>

#include "CollisionObject.h"

namespace GameEngine
{
	enum EnemyShotAttr
	{
		ENEMY_SHOT_ATTR_NORMAL	= 0,
		ENEMY_SHOT_ATTR_GREEN	= 1,
		ENEMY_SHOT_ATTR_BLUE	= 2,
		ENEMY_SHOT_ATTR_RED		= 3,
		ENEMY_SHOT_ATTR_TOTAL,
	};

	struct EnemyShotMessage
	{
		enum EnemyShotMessageID
		{
			ENEMY_SHOT_MESSAGE_ID_PLAYER_DAMAGED			= 0,		// プレイヤーがダメージを受けた
			ENEMY_SHOT_MESSAGE_ID_BOSS_MODE_STARTED			= 1,		// ボスモードへ移行した
			ENEMY_SHOT_MESSAGE_ID_BOSS_INVOKE_CONS_SKILL	= 2,		// ボスがスキルを使用した
			ENEMY_SHOT_MESSAGE_ID_PLAYER_BOMBED				= 3,		// プレイヤーがボムを発動
		};

		int		m_MsgID;
	};

	

	struct ResourceMap;
	class EnemyShotGroup;
	class EnemyShot : public CollisionObject
	{
	private:
		class Impl;
		std::auto_ptr < EnemyShot::Impl >		m_pImpl;
	public:
		EnemyShot( std::shared_ptr < ResourceMap > pMap, int id );
		~EnemyShot();
		void Init( float posX, float posY );				// 初期化
		void Draw();										// 描画
		bool Update();										// 更新
		void SetPos( float posX, float posY );				// 位置を設定
		void SetPower( int power );							// 弾の攻撃力を設定
		void SetAngle( float angle );						// 角度を設定
		void SetSpeed( float speed );						// 速度を設定
		void SetImage( int id );							// 画像を設定
		void SetImageScale( float scale );					// 画像の拡大率を設定
		void SetCollisionRadius( float radius );			// 衝突判定の半径を設定
		void SetConsAttr( int attr );						// 意識技専用弾に設定
		void AddPos( float x, float y );					// 位置を加算
		void AddAngle( float angle );						// 角度を加算
		void AddSpeed( float speed );						// 速度を加算
		void JoinShotGroup( int id, EnemyShotGroup* pGroup );
		void Colided( CollisionObject* pObject );			// 衝突時の処理 ディスパッチャ
		void ProcessCollision( Player* pPlayer );			// 衝突時の処理（プレイヤー）
		void ProcessCollision( Enemy* pEnemy );				// 衝突時の処理（敵）
		void ProcessCollision( PlayerShot* pPlayerShot );	// 衝突時の処理（プレイヤーショット）
		void ProcessCollision( EnemyShot* pEnemyShot );		// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );				// 衝突時の処理（アイテム）
		void GetPos( float* pPosX, float* pPosY );
		int GetPower() const;								// 弾の攻撃力を取得
		float GetCollisionRadius();
		int GetCounter() const;
		void PostMessage( int msgID );						// メッセージの追加
		bool IsDead() const;
		int GetConsAttr() const;
		void Pause();										// 一時停止
		void Resume();										// 一時停止から再開
		void SetImgRotMode( int mode );						// 敵弾の画像回転モードを設定
		void SetImgRotAnglePerFrame( float angle );			// 毎フレーム増加する回転角度を設定
		void SetAlphaBlendingMode( int mode );				// αブレンディングの方法を設定
	};
}

#endif
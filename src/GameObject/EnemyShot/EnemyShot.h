#ifndef INCLUDED_GAMEENGINE_ENEMYSHOT_H
#define INCLUDED_GAMEENGINE_ENEMYSHOT_H

#include <memory>

#include "../../CollisionObject.h"

namespace GameEngine
{
	enum EnemyShotTextureColor
	{
		ENEMY_SHOT_TEX_COLOR_YELLOW		= 0,
		ENEMY_SHOT_TEX_COLOR_BLUE		= 1,
		ENEMY_SHOT_TEX_COLOR_RED		= 2,
		ENEMY_SHOT_TEX_COLOR_GREEN		= 3,
		ENEMY_SHOT_TEX_COLOR_PINK		= 4,
		ENEMY_SHOT_TEX_COLOR_AQUA		= 5,
		ENEMY_SHOT_TEX_COLOR_ORANGE		= 6,
		ENEMY_SHOT_TEX_COLOR_BLACK		= 7,
		ENEMY_SHOT_TEX_COLOR_TOTAL,
	};

	enum EnemyShotID
	{
		ENEMY_SHOT_ID_FOUR_RAYED_STAR_M		= 0,
		ENEMY_SHOT_ID_ELLIPSE_S				= 1,
		ENEMY_SHOT_ID_CRESCENT_M			= 2,
		ENEMY_SHOT_ID_GLORIOUS_L			= 3,
		ENEMY_SHOT_ID_HEXAGRAM_M			= 4,
		ENEMY_SHOT_ID_CHEESE_M				= 5,
		ENEMY_SHOT_ID_GLORIOUS_M			= 6,
		ENEMY_SHOT_ID_CIRCLE_M				= 7,
		ENEMY_SHOT_ID_NEEDLE_M				= 8,
		ENEMY_SHOT_ID_LASER_M				= 9,
		ENEMY_SHOT_ID_SMOKE_M				= 10,
		ENEMY_SHOT_ID_RING					= 11,
		ENEMY_SHOT_ID_BEAM_M				= 12,
		ENEMY_SHOT_ID_TOTAL,
	};

	enum DeleteBy
	{
		DELETE_BY_PLAYER_DAMAGE		= 0,
		DELETE_BY_PLAYER_SKILL		= 1,
		DELETE_BY_TOTAL,
	};

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
	class EnemyShotImpl;
	class EnemyShot : public CollisionObject
	{
	private:
		std::auto_ptr < EnemyShotImpl >		m_pImpl;
	public:
		EnemyShot( std::shared_ptr < ResourceMap > pMap, int id );
		virtual ~EnemyShot();
		virtual void Draw();										// 描画
		virtual bool Update();										// 更新
		void SetPower( int power );							// 弾の攻撃力を設定
		void SetImage( int id );							// 画像を設定
		void SetAtlasImage( int id );						// アトラス画像を設定
		void SetImageScale( float scale );					// 画像の拡大率を設定
		void SetConsAttr( int attr );						// 意識技専用弾に設定
		void JoinShotGroup( int id, EnemyShotGroup* pGroup );
		virtual void Colided( CollisionObject* pObject );			// 衝突時の処理 ディスパッチャ
		void ProcessCollision( Player* pPlayer );			// 衝突時の処理（プレイヤー）
		void ProcessCollision( Enemy* pEnemy );				// 衝突時の処理（敵）
		void ProcessCollision( PlayerShot* pPlayerShot );	// 衝突時の処理（プレイヤーショット）
		void ProcessCollision( EnemyShot* pEnemyShot );		// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );				// 衝突時の処理（アイテム）
		int GetPower() const;								// 弾の攻撃力を取得

		virtual void Init( const GameUnit& posX, const GameUnit& posY );				// 初期化
		virtual void AddPos( const GameUnit& x, const GameUnit& y );					// 位置を加算
		void AddAngle( const GameUnit& angle );						// 角度を加算
		void AddSpeed( const GameUnit& speed );						// 速度を加算
		virtual void GetPos( GameUnit* pPosX, GameUnit* pPosY );
		virtual void SetPos( const GameUnit& posX, const GameUnit& posY );				// 位置を設定
		virtual void SetLinePos(	const GameUnit& x1,
							const GameUnit& y1,
							const GameUnit& x2,
							const GameUnit& y2,
							const GameUnit& thickness );		// 線の値を設定
		void SetAngle( const GameUnit& angle );						// 角度を設定
		void SetSpeed( const GameUnit& speed );						// 速度を設定
		void SetCollisionRadius( const GameUnit& radius );			// 衝突判定の半径を設定
		bool DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius );	// プレイヤーとの衝突判定
		GameUnit GetCollisionRadius();
		GameUnit GetAngle() const;
		GameUnit GetSpeed() const;

		int GetCounter() const;
		void PostMessage( int msgID );						// メッセージの追加
		bool IsDead() const;
		int GetConsAttr() const;
		void Pause();										// 一時停止
		void Resume();										// 一時停止から再開
		void SetImgRotMode( int mode );						// 敵弾の画像回転モードを設定
		void SetImgRotAnglePerFrame( float angle );			// 毎フレーム増加する回転角度を設定
		void SetAlphaBlendingMode( int mode );				// αブレンディングの方法を設定
		void SetShape( int shape );							// 衝突判定の形を設定
		
		void EnableInvincibleMode();				// 消えないモードへ移行
		void DisableInvincibleMode();			// 消えるモードへ移行
		void EnableInvisibleMode();				// 見えないモードへ移行
		void DisableInvisibleMode();			// 見えるモードへ移行

		std::string GetMasterEnemyName() const;		// マスタとなる敵の名前を取得
		void SetDrawingMultiplicity( int num );		// 描画多重度を設定

		void Delete( int by );			// 削除

		void DeleteWhen( int when );
		void NotDeleteWhen( int when );		// 削除タイミングの設定


		// For line interface.
		virtual void SetLength( const GameUnit& length );
	};
}

#endif
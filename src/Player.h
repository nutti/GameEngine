#ifndef INCLUDED_GAMEENGINE_PLAYER_H
#define INCLUDED_GAMEENGINE_PLAYER_H

#include <memory>
#include <list>

#include "CollisionObject.h"
#include "InputTypes.h"
#include "PlayerOption.h"

#include "Math.hpp"

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
		struct GameUnitData
		{
			GameUnit		m_PosX;
			GameUnit		m_PosY;
			GameUnit		m_ColRadius;
		};
		GameUnitData	m_GUData;
		
		int			m_HP;					// HP
		int			m_ConsCur;				// 現在の意識状態
		int			m_ConsGauge[ 3 ];		// 意識ゲージ
		int			m_ConsLevel[ 3 ];		// 意識レベル
		int			m_ShotPower;			// ショットのパワー
		int			m_Counter;				// カウンタ
		int			m_RestInvincibleTime;	// 残りの無敵時間
	};

	struct ResourceMap;
	struct StageData;
	class Player : public CollisionObject
	{
	private:
		const int	INVINCIBLE_TIME;

		ButtonStatusHolder					m_ButtonStatus;
		PlayerData							m_Data;
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		StageData*							m_pStageData;

		std::list < PlayerOption* >			m_PlayerOptList;
		
		void NormalModeShot();
		void GreenModeShot();
		void BlueModeShot();
		void RedModeShot();

		void GreenModeBomb();
		void BlueModeBomb();
		void RedModeBomb();

		void AddOpt();
		
		void Move();
		void ChangeMode();
		void UpdateCons();

	public:
		Player( std::shared_ptr < ResourceMap > pMap, StageData* pStageData );
		~Player();
		void AttachButtonState( const ButtonStatusHolder& holder );		// キー入力を設定
		
		void Draw();													// 描画
		bool Update();													// 更新
		void Colided( CollisionObject* pObject );						// 衝突時の処理 ディスパッチャ
		void ProcessCollision( Player* pPlayer ){};						// 衝突時の処理（プレイヤー）
		void ProcessCollision( Enemy* pEnemy );							// 衝突時の処理（敵）
		void ProcessCollision( PlayerShot* pPlayerShot ){};				// 衝突時の処理（プレイヤーショット）
		void ProcessCollision( EnemyShot* pEnemyShot );					// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );							// 衝突時の処理（アイテム）
		void Init( const GameUnit& posX, const GameUnit& posY ){}		// 初期化
		void GetPos( GameUnit* pPosX, GameUnit* pPosY );				// 位置を取得
		GameUnit GetCollisionRadius();									// 衝突半径を取得
		int GetHP() const;												// HPを取得
		int GetShotPower() const;										// ショットの威力を取得
		int GetConsGauge( int cons ) const;								// 意識ゲージの取得
		int GetConsLevel( int cons ) const;								// 意識レベルの取得
		int GetCurCons() const;											// 現在の意識状態を取得

		void SetPos( const GameUnit& posX, const GameUnit& posY );		// 位置を設定
		void SetHP( int hp );											// HPを設定
		void SetShotPower( int power );									// ショットの威力を設定
		void SetConsGauge( int cons, int val );							// 意識ゲージを設定
		void SetConsLevel( int cons, int level );						// 意識レベルを設定
		void SetCons( int cons );										// 現在の意識状態を設定
	};
}

#endif
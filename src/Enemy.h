#ifndef INCLUDED_GAMEENGINE_ENEMY_H
#define INCLUDED_GAMEENGINE_ENEMY_H

#include "CollisionObject.h"

#include "EnemyVCPU.h"

namespace GameEngine
{
	struct StageData;
	struct ResourceMap;

	struct EnemyData
	{
		struct ItemDrop
		{
			int		m_ItemSubID;	// アイテムのサブID
			int		m_Total;		// 個数
		};

		float		m_PosX;				// 位置（X座標）
		float		m_PosY;				// 位置（Y座標）
		int			m_HP;				// HP
		int			m_MaxHP;			// MaxHP
		int			m_ConsGauge;		// 意識ゲージ
		int			m_ConsType;			// 意識タイプ
		int			m_Counter;			// カウンタ
		float		m_ColRadius;		// 衝突半径
		int			m_Score;			// スコア
		int			m_IsBoss;			// ボスなら1
		bool		m_Destroyed;		// 倒された場合はtrue
		StageData*	m_pStageData;		// ステージデータ
		ItemDrop	m_ItemDrop[ 6 ];	// 落とすアイテムの状態
		std::shared_ptr < ResourceMap >		m_pResouceMap;
	};

	// 速度向上のため、pimplイディオムは使用しない。
	struct EnemyScriptData;
	class Enemy : public CollisionObject
	{
	private:
		EnemyData							m_Data;
		int									m_ScriptID;		// スクリプトID
		EnemyVCPU							m_VM;			// 仮想マシン
		std::shared_ptr < EnemyScriptData >	m_pScriptData;	// スクリプトデータ
	public:
		Enemy(	std::shared_ptr < ResourceMap > pMap,
				std::shared_ptr < EnemyScriptData > pData,
				int id,
				StageData* pStageData );
		~Enemy();
		void Init( float posX, float posY );		// 初期化
		void Draw();								// 描画
		bool Update();								// 更新
		void Colided( CollisionObject* pObject );	// 衝突時の処理 ディスパッチャ
		void ProcessCollision( Player* pPlayer );	// 衝突時の処理（プレイヤー）
		void ProcessCollision( Enemy* pEnemy );		// 衝突時の処理（敵）
		void ProcessCollision( PlayerShot* pPlayerShot );				// 衝突時の処理（プレイヤーショット）
		void ProcessCollision( EnemyShot* pEnemyShot );		// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );				// 衝突時の処理（アイテム）
		void GetPos( float* pPosX, float* pPosY );
		float GetCollisionRadius();
	};
}

#endif
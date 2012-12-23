#ifndef INCLUDED_GAMEENGINE_ENEMY_H
#define INCLUDED_GAMEENGINE_ENEMY_H

#include <vector>

#include "CollisionObject.h"
#include "EnemyVCPU.h"

namespace GameEngine
{

	struct StageData;
	struct ResourceMap;
	class EnemyShotGroup;

	struct EnemyData
	{
		struct ItemDrop
		{
			int		m_ItemSubID;	// アイテムのサブID
			int		m_Total;		// 個数
		};

		float				m_PosX;				// 位置（X座標）
		float				m_PosY;				// 位置（Y座標）
		int					m_HP;				// HP
		int					m_MaxHP;			// MaxHP
		int					m_ConsGauge;		// 意識ゲージ
		int					m_MaxConsGauge;		// Max意識ゲージ
		int					m_ConsType;			// 意識タイプ
		int					m_Counter;			// カウンタ
		float				m_ColRadius;		// 衝突半径
		int					m_Score;			// スコア
		int					m_IsBoss;			// ボスなら1
		bool				m_Destroyed;		// 倒された場合はtrue
		bool				m_IsInvincibleMode;	// 無敵状態の場合はtrue
		StageData*			m_pStageData;		// ステージデータ
		ItemDrop			m_ItemDrop[ 6 ];	// 落とすアイテムの状態
		std::shared_ptr < ResourceMap >		m_pResouceMap;
		std::vector < EnemyShotGroup* >		m_ShotGroupList;		// 現在保持しているショットグループ
	};

	// 速度向上のため、pimplイディオムは使用しない。
	struct EnemyScriptData;
	class Enemy : public CollisionObject
	{
	private:
		// Enemyクラス内部用データ
		struct EnemyPrivateData
		{
			// 意識技
			//struct ConsSkillModeData
			//{
			//	bool			m_IsConsSkillMode;	// 意識スキルモードならtrue
			//	int				m_Counter;			// エフェクト用カウンタ
			//	int				m_PostCounter;		// エフェクト終了ときのカウンタ
			//	std::string		m_SkillName;		// 意識技名
			//};
			//int					m_DamagedCounter;		// ダメージ時のカウンタ
			int					m_PrevConsGauge;		// 前回の意識ゲージ（このゲージが大量に減った時に意識技を使用したとみなす）
			//int					m_SkillUsedCounter;		// 意識技使用時のカウンタ
			//ConsSkillModeData	m_ConsSkillModeData;	// 意識技用のデータ
		};

		EnemyData							m_Data;			// データ
		int									m_ScriptID;		// スクリプトID
		EnemyVCPU							m_VM;			// 仮想マシン
		std::shared_ptr < EnemyScriptData >	m_pScriptData;	// スクリプトデータ
		EnemyPrivateData					m_PrivateData;	// Enemyクラス内部向けデータ
	public:
		Enemy(	std::shared_ptr < ResourceMap > pMap,
				std::shared_ptr < EnemyScriptData > pData,
				int id,
				StageData* pStageData );
		~Enemy();
		void Init( float posX, float posY );				// 初期化
		void Draw();										// 描画
		bool Update();										// 更新
		//void InvokeConsSkill( std::string skillName );		// スキル呼び出し
		//void StopConsSkill();								// スキル終了要求
		void Colided( CollisionObject* pObject );			// 衝突時の処理 ディスパッチャ
		void ProcessCollision( Player* pPlayer );			// 衝突時の処理（プレイヤー）
		void ProcessCollision( Enemy* pEnemy );				// 衝突時の処理（敵）
		void ProcessCollision( PlayerShot* pPlayerShot );	// 衝突時の処理（プレイヤーショット）
		void ProcessCollision( EnemyShot* pEnemyShot );		// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );				// 衝突時の処理（アイテム）
		void GetPos( float* pPosX, float* pPosY );
		int GetHP() const;									// HPを取得
		int GetMaxHP() const;								// 最大HPを取得
		int GetConsGauge() const;							// 意識ゲージの取得
		int GetMaxConsGauge() const;						// 最大意識ゲージの取得
		float GetCollisionRadius();

	};
}

#endif
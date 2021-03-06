#ifndef INCLUDED_GAMEENGINE_ENEMY_H
#define INCLUDED_GAMEENGINE_ENEMY_H

#include <vector>
#include <bitset>
#include <queue>

#include "CollisionObject.h"
#include "EnemyVCPU.h"
#include "EnemyPatternFileLoader.h"


namespace GameEngine
{

	struct StageData;
	struct ResourceMap;
	class EnemyShotGroup;
	class ScriptEffect;

	struct EnemyData
	{
		struct ItemDrop
		{
			int		m_ItemSubID;	// アイテムのサブID
			int		m_Total;		// 個数
		};

#if defined ( USE_FLOATING_POINT )
		float				m_PosX;				// 位置（X座標）
		float				m_PosY;				// 位置（Y座標）
		float				m_PosZ;				// 位置（Z座標）
		float				m_ColRadius;		// 衝突半径
#elif defined ( USE_GAME_UNIT )
		struct GameUnitData
		{
			GameUnit	m_PosX;			// 位置（X座標）
			GameUnit	m_PosY;			// 位置（Y座標）
			GameUnit	m_PosZ;			// 位置（Z座標）
			GameUnit	m_ColRadius;	// 衝突半径
		};
		GameUnitData		m_GUData;
#endif

		// 敵の表示方法
		enum DisplayMode
		{
			DISPLAY_MODE_2D			= 0,		// 2Dテクスチャ表示
			DISPLAY_MODE_3D			= 1,		// 3Dモデル表示
			DISPLAY_MODE_3D_ANIM	= 2,		// 3Dアニメーションモデル表示
		};

		// フラグ管理
		enum StatusFlag
		{
			DAMAGED_BY_CONS_SHOT		= 0,	// 属性攻撃を受けたか?
			DAMAGED_BY_CONS_SHOT_GREEN	= 1,	// 緑属性の攻撃を受けた
			DAMAGED_BY_CONS_SHOT_BLUE	= 2,	// 青属性の攻撃を受けた
			DAMAGED_BY_CONS_SHOT_RED	= 3,	// 赤属性の攻撃を受けた
			STATUS_FLAG_TOTAL,
		};
		std::bitset < STATUS_FLAG_TOTAL >	m_StatusFlags;		// 状態管理フラグ

		std::string			m_Name;				// 敵の名前
		int					m_SubID;			// サブID
		
		float				m_ScaleX;			// 拡大率（X）
		float				m_ScaleY;			// 拡大率（Y）
		float				m_ScaleZ;			// 拡大率（Z）
		float				m_RotX;				// 回転角度（X軸中心）
		float				m_RotY;				// 回転角度（Y軸中心）
		float				m_RotZ;				// 回転角度（Z軸中心）
		double				m_Time;				// 時間（モデル専用データ）
		int					m_ImgID;			// 敵画像
		int					m_HP;				// HP
		int					m_MaxHP;			// MaxHP
		int					m_ConsGauge;		// 意識ゲージ
		int					m_MaxConsGauge;		// Max意識ゲージ
		int					m_ConsType;			// 意識タイプ（属性、意識技で上書きされる）
		int					m_DamagedConsGauge;	// 減少させられた意識ゲージ
		int					m_Counter;			// カウンタ
		int					m_Score;			// スコア
		int					m_ScoreBonus;		// スコアボーナス（1で0.001倍）
		int					m_IsBoss;			// ボスなら1
		bool				m_Destroyed;		// 倒された場合はtrue
		bool				m_IsInvincibleMode;	// 無敵状態の場合はtrue
		bool				m_IsNonCollisionMode;	// 衝突しない時の場合はtrue
		bool				m_IsConsSkillMode;	// 意識技を使用している場合はtrue
		bool				m_Paused;			// 行動停止中の場合true
		DisplayMode			m_DispMode;			// 3D表示か？
		int					m_ConsSkillAttr;	// 意識技の属性
		std::string			m_ConsSkillName;	// 意識技名
		StageData*			m_pStageData;		// ステージデータ
		ItemDrop			m_ItemDrop[ 6 ];	// 落とすアイテムの状態
		std::shared_ptr < ResourceMap >		m_pResouceMap;
		std::vector < EnemyShotGroup* >		m_ShotGroupList;		// 現在保持しているショットグループ
		int					m_Regs[ MAX_ENEMY_REGS ];			// 汎用レジスタ
		std::weak_ptr < Item >		m_RefItem;			// 現在参照しているアイテム
		std::queue < int >					m_EventQueue;		// イベントキュー
		std::weak_ptr < Enemy >		m_Self;			// 自己参照
	};

	// 速度向上のため、pimplイディオムは使用しない。
	struct EnemyScriptData;
	class Enemy : public CollisionObject
	{
	private:
		// Enemyクラス内部用データ
		struct EnemyPrivateData
		{
			int			m_ConsSkillEffectCounter;			// 意識技使用時用カウンタ
			int			m_ConsSkillEffectPostCounter;		// 意識技使用後用カウンタ
			int			m_PrevConsGauge;					// 前回の意識ゲージ（このゲージが大量に減った時に意識技を使用したとみなす）
		};

		EnemyData							m_Data;					// データ
		int									m_ScriptID;				// スクリプトID
		EnemyVCPU							m_VM;					// 仮想マシン
		std::shared_ptr < EnemyScriptData >	m_pScriptData;			// スクリプトデータ
		EnemyPrivateData					m_PrivateData;			// Enemyクラス内部向けデータ
		static bool							m_SentLastDamagedMsg;	// 現フレームで、ダメージを受けたメッセージを送った場合true
	public:
		Enemy(	std::shared_ptr < ResourceMap > pMap,
				std::shared_ptr < EnemyScriptData > pData,
				int id,
				StageData* pStageData );
		~Enemy();
		void SetSelfRef( std::shared_ptr < Enemy > self );
		void Draw();										// 描画
		bool Update();										// 更新
		void Colided( CollisionObject* pObject );			// 衝突時の処理 ディスパッチャ
		void ProcessCollision( Player* pPlayer );			// 衝突時の処理（プレイヤー）
		void ProcessCollision( Enemy* pEnemy );				// 衝突時の処理（敵）
		void ProcessCollision( PlayerShot* pPlayerShot );	// 衝突時の処理（プレイヤーショット）
		void ProcessCollision( EnemyShot* pEnemyShot );		// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );				// 衝突時の処理（アイテム）
		void Damage( int val );								// ダメージを与える
#if defined ( USE_FLOATING_POINT )
		void Init( float posX, float posY );				// 初期化
		void GetPos( float* pPosX, float* pPosY );
		float GetCollisionRadius();
#elif defined ( USE_GAME_UNIT )
		void Init( const GameUnit& posX, const GameUnit& posY );				// 初期化
		void GetPos( GameUnit* pPosX, GameUnit* pPosY );
		GameUnit GetCollisionRadius();
#endif
		int GetHP() const;									// HPを取得
		int GetMaxHP() const;								// 最大HPを取得
		int GetConsGauge() const;							// 意識ゲージの取得
		int GetMaxConsGauge() const;						// 最大意識ゲージの取得
		
		void Pause();										// 一時停止に設定
		void Resume();										// 一時停止から再開
		static void ClearLastDamagedMsg();
		bool IsNonCollisionMode() const;
		void SetReg( int regNo, int val );					// レジスタに値をセット
		void SetTime( double time );

		std::string GetName() const;						// 敵の名前を取得
		void SendEvent( int id );
		int GetSubID() const;
		bool IsInSkillMode() const;							// スキル使用中の場合true

		float GetRotAngle( int axis ) const;				// 回転角度を取得
	};

	
}

#endif
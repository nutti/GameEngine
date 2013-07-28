#ifndef INCLUDED_GAMEENGINE_STAGE_H
#define INCLUDED_GAMEENGINE_STAGE_H

#include <memory>
#include <list>
#include <queue>
#include "Scene.h"

#include "GameObjectBuilder.h"
#include "RandomGenerator.h"

namespace GameEngine
{
	class Enemy;
	class Player;
	class PlayerShot;
	class EnemyShot;
	class Item;
	class Effect;
	class EnemyShotGroup;

	typedef Enemy Boss;

	typedef std::list < Enemy* >			EnemyList;
	typedef std::list < PlayerShot*	>		PlayerShotList;
	typedef std::list < EnemyShot* >		EnemyShotList;
	typedef std::list < std::shared_ptr < Item > >		ItemList;
	typedef std::list < Effect* >			EffectList;
	typedef std::list < EnemyShotGroup* >	EnemyShotGroupList;

	struct StageMessage
	{
		enum StageMessageID
		{
			STAGE_MESSAGE_ID_PLAYER_DAMAGED				= 0,		// プレイヤーがダメージを受けた
			STAGE_MESSAGE_ID_PLAYER_DESTORYED			= 1,		// プレイヤーが撃破された（HPが0以下になった）
			STAGE_MESSAGE_ID_BOSS_MODE_STARTED			= 2,		// ボスモードへ移行した
			STAGE_MESSAGE_ID_BOSS_MODE_ENDED			= 3,		// ボスモードが終了した
			STAGE_MESSAGE_ID_BOSS_DAMAGED				= 4,		// ボスがダメージを受けた
			STAGE_MESSAGE_ID_BOSS_INVOKE_CONS_SKILL		= 5,		// ボスがスキルを使用した
			STAGE_MESSAGE_ID_BOSS_STOP_CONS_SKILL		= 6,		// ボスがスキルの使用を終了した
			STAGE_MESSAGE_ID_BOSS_SHIFT_NEXT_MODE		= 7,		// ボスで次の段階へ進んだ
			STAGE_MESSAGE_ID_ENEMY_INVOKE_CONS_SKILL	= 8,		// 敵（ボス以外）がスキルを使用した
			STAGE_MESSAGE_ID_PLAYER_BOMBED				= 9,		// プレイヤーがボム発動
			STAGE_MESSAGE_ID_ENEMY_DAMAGED				= 10,		// 敵（ボス以外）がダメージを受けた
			STAGE_MESSAGE_ID_STAGE_CLEARED				= 11,		// ステージクリアした
			STAGE_MESSAGE_ID_GAME_OVERED				= 12,		// ゲームオーバーした
			STAGE_MESSAGE_ID_GAME_CLEARED				= 13,		// 全クリした
			STAGE_MESSAGE_ID_ITEM_OBTAINED				= 14,		// アイテムを取得した
			STAGE_MESSAGE_ID_TOTAL,
		};
		union StageMessageData
		{
			int				m_Integer;
			std::string*	m_pString;
			float			m_Float;
		};

		StageMessageID		m_MsgID;
		std::vector < StageMessageData >	m_MsgDataList;
	};

	

	typedef std::queue < StageMessage >			StageMessageQueue;

	

	struct StageData
	{
		int					m_StageNo;				// ステージ番号
		int					m_Difficulty;			// 難易度
		int					m_Frame;				// フレーム数
		int					m_FrameTotal;			// ステージ全体のフレーム数
		bool				m_IsReplay;				// リプレイ状態ならtrue
		GameObjectBuilder	m_ObjBuilder;			// ゲームオブジェクト構築クラス
		RandomGenerator		m_RandGen;

		Player*				m_pPlayer;				// プレイヤー
		EnemyList			m_EnemyList;			// 敵リスト
		Boss*				m_pBoss;				// ボスリスト
		PlayerShotList		m_PlayerShotList;		// プレイヤーショットリスト
		EnemyShotList		m_EnemyShotList;		// 敵ショットリスト
		ItemList			m_ItemList;				// アイテムリスト
		EffectList			m_EffectList;			// エフェクトリスト
		EnemyShotGroupList	m_EnemyShotGroupList;	// 敵ショットグループリスト
		
		GameDataMsg			m_GameData;				// 現フレームにおけるゲームデータ
		GameDataMsg			m_FrameGameData;		// 現フレームで更新されるゲームデータ

		GameDataMsg			m_TotalGameData;		// 現フレームにおける全てのステージゲームデータ

		StageStat			m_StageStat;			// ステージの統計情報

		StageMessageQueue	m_MsgQueue;				// ステージ用メッセージキュー

		bool				m_HasTermSig;
		int					m_BossMode;				// 1ならボスモード

		ResourceMap			m_ResourceMap;

		int					m_ConsLevel;			// 意識レベルのベースレベル

		int					m_BossPhaseTotal;				// ボスの段階数の合計
		int					m_BossPhaseStartHP[ 10 ];		// 段階を開始するHP
	};

	struct ScriptData;
	class Stage : public Scene
	{
	private:
		class Impl;
		std::auto_ptr < Stage::Impl >		m_pImpl;

		void InitImpl();
	public:
		Stage( int stageNo, bool isReplay );
		~Stage();
		//void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachScriptData( const ScriptData& data );
		void AttachGameData( const GameDataMsg& msg );
		
		int GetProgress() const;
		int GetScore() const;
		int GetKilled() const;
		int GetCrystal() const;
		int GetCrystalUsed() const;

		void GetPlayerPos( int* pPosX, int* pPosY ) const;
		int GetPlayerHP() const;
		int GetPlayerShotPower() const;
		int GetPlayerCons() const;
		void GetPlayerConsGauge( int* pGauge ) const;
		void GetPlayerConsLevel( int* pLevel ) const;

		StageStat& GetStageStat();

		ReplayDataRecord::StageKeyStates GetKeyStates() const;


		void SetInitialData( const InitialGameData& data );
		void SetDifficulty( int difficulty );

		int GetNextStageNo() const;
		GameDataMsg GetFrameData() const;

		void MarkFirstTime();
	};
}

#endif
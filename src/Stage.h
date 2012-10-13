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

	typedef std::list < Enemy* >			EnemyList;
	typedef std::list < PlayerShot*	>		PlayerShotList;
	typedef std::list < EnemyShot* >		EnemyShotList;
	typedef std::list < Item* >				ItemList;
	typedef std::list < Effect* >			EffectList;
	typedef std::list < EnemyShotGroup* >	EnemyShotGroupList;

	typedef std::queue < int >			StageMessageQueue;

	enum StageMessage
	{
		STAGE_MESSAGE_PLAYER_DAMAGED			= 0,		// プレイヤーがダメージを受けた
		STAGE_MESSAGE_PLAYER_DESTORYED			= 1,		// プレイヤーが撃破された（HPが0以下になった）
	};

	struct StageData
	{
		int					m_StageNo;				// ステージ番号
		int					m_Frame;				// フレーム数
		bool				m_IsReplay;				// リプレイ状態ならtrue
		GameObjectBuilder	m_ObjBuilder;			// ゲームオブジェクト構築クラス
		RandomGenerator		m_RandGen;

		Player*				m_pPlayer;				// プレイヤー
		EnemyList			m_EnemyList;			// 敵リスト
		PlayerShotList		m_PlayerShotList;		// プレイヤーショットリスト
		EnemyShotList		m_EnemyShotList;		// 敵ショットリスト
		ItemList			m_ItemList;				// アイテムリスト
		EffectList			m_EffectList;			// エフェクトリスト
		EnemyShotGroupList	m_EnemyShotGroupList;	// 敵ショットグループリスト
		
		GameDataMsg			m_GameData;				// 現フレームにおけるゲームデータ
		GameDataMsg			m_FrameGameData;		// 現フレームで更新されるゲームデータ

		StageMessageQueue	m_MsgQueue;				// ステージ用メッセージキュー

		bool				m_HasTermSig;

		ResourceMap			m_ResourceMap;

		int					m_ConsLevel;			// 意識レベルのベースレベル
	};

	struct ScriptData;
	class Stage : public Scene
	{
	private:
		class Impl;
		std::auto_ptr < Stage::Impl >		m_pImpl;
	public:
		Stage( int stageNo, bool isReplay );
		~Stage();
		void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachScriptData( const ScriptData& data );
		void AttachGameData( const GameDataMsg& msg );
		GameDataMsg GetFrameData() const;
	};
}

#endif
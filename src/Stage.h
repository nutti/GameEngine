#ifndef INCLUDED_GAMEENGINE_STAGE_H
#define INCLUDED_GAMEENGINE_STAGE_H

#include <memory>
#include <list>
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

	typedef std::list < Enemy* >		EnemyList;
	typedef std::list < PlayerShot* >	PlayerShotList;
	typedef std::list < EnemyShot* >	EnemyShotList;
	typedef std::list < Item* >			ItemList;
	typedef std::list < Effect* >		EffectList;

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
		
		GameDataMsg			m_GameData;				// 現フレームにおけるゲームデータ
		GameDataMsg			m_FrameGameData;		// 現フレームで更新されるゲームデータ

		ResourceMap			m_ResourceMap;
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
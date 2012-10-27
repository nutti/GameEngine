#ifndef INCLUDED_GAMEENGINE_SCENETYPES_H
#define INCLUDED_GAMEENGINE_SCENETYPES_H

namespace GameEngine
{
	// シーン識別子
	enum SceneType
	{
		SCENE_TYPE_INITIALIZE	= 0,		// 初期化
		SCENE_TYPE_MENU			= 1,		// メニュー
		SCENE_TYPE_STAGE		= 2,		// ステージ
		SCENE_TYPE_SCORE		= 3,		// スコア
		SCENE_TYPE_SCORE_ENTRY	= 4,		// スコア入力
		SCENE_TYPE_REPLAY_ENTRY	= 5,		// リプレイ入力
		SCENE_TYPE_REPLAY		= 6,		// リプレイ
		SCENE_TYPE_NOT_CHANGE	= 100,		// シーン変更なし
		SCENE_TYPE_GAME_TERM	= 101,		// ゲーム終了
		SCENE_TYPE_LOADING		= 102,		// ロード中
		SCENE_TYPE_UNKNOWN		= 200,		// 不明なシーン
	};

	// ステージ識別子
	enum StageID
	{
		STAGE_ID_STAGE_1		= 0,
		STAGE_ID_STAGE_2		= 1,
		STAGE_ID_STAGE_3		= 2,
		STAGE_ID_STAGE_4		= 3,
		STAGE_ID_STAGE_5		= 4,
		STAGE_ID_ALL_CLEARED	= 5,
	};

	// オブジェクト識別子
	enum GameObjectID
	{
		GAME_OBJECT_ID_PLAYER		= 0,	// プレイヤー
		GAME_OBJECT_ID_ENEMY		= 1,	// 敵
		GAME_OBJECT_ID_ITEM			= 2,	// アイテム
		GAME_OBJECT_ID_PLAYER_SHOT	= 3,	// プレイヤーショット
		GAME_OBJECT_ID_ENEMY_SHOT	= 4,	// 敵ショット
		GAME_OBJECT_ID_EFFECT		= 5,	// エフェクト
	};

	// ゲームモード
	enum GameMode
	{
		GAME_MODE_NORMAL			= 0,	// 通常プレイ
		GAME_MODE_REPLAY			= 1,	// リプレイ
	};
}

#endif
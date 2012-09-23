#ifndef INCLUDED_GAMEENGINE_SCENETYPES_H
#define INCLUDED_GAMEENGINE_SCENETYPES_H

namespace GameEngine
{
	// シーン識別子
	enum SceneType
	{
		SCENE_TYPE_INITIALIZE	= 0,
		SCENE_TYPE_MENU			= 1,
		SCENE_TYPE_STAGE		= 2,
		SCENE_TYPE_NOT_CHANGE	= 100,
		SCENE_TYPE_UNKNOWN		= 200,
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
}

#endif
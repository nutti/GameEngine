#ifndef INCLUDED_GAMEENGINE_EVENTTYPES_H
#define INCLUDED_GAMEENGINE_EVENTTYPES_H

namespace GameEngine
{
	enum EventType
	{
		EVENT_TYPE_INITIALIZE				= 0,	// 初期化要求
		EVENT_TYPE_MOVE_TO_MENU				= 1,	// メニュー画面移行要求
		EVENT_TYPE_MOVE_TO_STAGE			= 2,	// ステージ開始要求（第2引数にステージ番号）
		EVENT_TYPE_MOVE_TO_SCORE			= 3,	// スコア画面へ移動
		EVENT_TYPE_MOVE_TO_SCORE_ENTRY		= 4,	// スコアエントリ画面へ移動
		EVENT_TYPE_FRAME_UPDATE				= 100,	// フレーム更新要求
		EVENT_TYPE_GAME_TERM				= 101,	// ゲーム終了要求
		EVENT_TYPE_UNKNOWN					= 200,
	};
}

#endif
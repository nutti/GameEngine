#ifndef INCLUDED_GAMEENGINE_EVENTTYPES_H
#define INCLUDED_GAMEENGINE_EVENTTYPES_H

namespace GameEngine
{
	enum EventType
	{
		EVENT_TYPE_START_GAME									= -1,	// ゲーム起動
		EVENT_TYPE_INITIALIZE									= 0,	// 初期化要求
		EVENT_TYPE_MOVE_TO_MENU									= 1,	// メニュー画面移行要求
		EVENT_TYPE_MOVE_TO_STAGE								= 2,	// ステージ開始要求（第2引数にステージ番号）
		EVENT_TYPE_MOVE_TO_SCORE								= 3,	// スコア画面へ移動
		EVENT_TYPE_MOVE_TO_SCORE_ENTRY							= 4,	// スコアエントリ画面へ移動
		EVENT_TYPE_MOVE_TO_REPLAY_ENTRY_FROM_SCORE_ENTRY		= 5,	// リプレイエントリ画面へ移動（スコアエントリ画面から移動）
		EVENT_TYPE_MOVE_TO_MENU_FROM_REPLAY_ENTRY				= 6,	// メニュー画面移行要求（リプレイエントリ画面から）
		EVENT_TYPE_MOVE_TO_REPLAY_ENTRY_FROM_SELF				= 7,	// リプレイエントリ画面へ移動（リプレイエントリ画面から移動）
		EVENT_TYPE_MOVE_TO_REPLAY								= 8,	// リプレイ画面へ移動
		EVENT_TYPE_MOVE_TO_MENU_FROM_INITIALIZE					= 9,	// 初期化状態からメニュー画面移行要求
		EVENT_TYPE_MOVE_TO_NEXT_STAGE							= 10,	// 次のステージへ移行要求
		EVENT_TYPE_MOVE_TO_DIFFICULTY_SELECTION					= 11,	// 難易度選択画面へ移行
		EVENT_TYPE_MOVE_TO_STAGE_SELECTION						= 12,	// ステージ選択画面へ移行
		EVENT_TYPE_MOVE_TO_DIFFICULTY_SELECTION_IN_STAGE		= 13,	// ステージ選択を含む難易度選択画面へ移行
		EVENT_TYPE_MOVE_TO_CONFIG								= 14,	// コンフィグ画面に移動
		EVENT_TYPE_MOVE_TO_MENU_FROM_CONFIG						= 15,	// メニュー画面移行要求（コンフィグ画面から）
		EVENT_TYPE_MOVE_TO_MENU_FROM_SCORE_ENTRY				= 16,	// メニュー画面移行要求（スコアエントリ画面から移動）
		EVENT_TYPE_BGM_VOLUME_CHANGED							= 50,	// BGMのボリュームが変更された
		EVENT_TYPE_SE_VOLUME_CHANGED							= 51,	// SEのボリュームが変更された
		EVENT_TYPE_PLAY_SPEED_CHANGED							= 52,	// プレイスピードが変更された
		EVENT_TYPE_FPS_CHANGED									= 70,	// FPSが更新された
		EVENT_TYPE_FRAME_UPDATE									= 100,	// フレーム更新要求
		EVENT_TYPE_GAME_TERM									= 101,	// ゲーム終了要求
		EVENT_TYPE_UNKNOWN										= 200,
	};


	// 要求データ
	enum RequestData
	{
		REQUEST_DATA_SCORE_RANK				= 0,	// スコアのランキングを取得する。（第2引数にint型のランクを格納する変数）
		REQUEST_DATA_DISPLAYED_SCORE_DATA	= 1,	// スコア表示用のデータを取得する。（第2引数に表示用スコアデータを格納する変数）
	};
}

#endif
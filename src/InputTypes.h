#ifndef INCLUDED_GAMEENGINE_INPUTTYPES_H
#define INCLUDED_GAMEENGINE_INPUTTYPES_H

namespace GameEngine
{
	// 入力デバイス
	enum InputDevice
	{
		INPUT_DEVICE_KEYBOARD		= 0,		// キーボード
		INPUT_DEVICE_GAMEPAD		= 1,		// ゲームパッド
		INPUT_DEVICE_FILE			= 2,		// ファイル
		INPUT_DEVICE_UNKNOWN		= 200,
	};

	// ゲーム内のボタン
	enum GeneralButton
	{
		GENERAL_BUTTON_SHOT				= 0,		// ショットボタン
		GENERAL_BUTTON_BOMB				= 1,		// ボムボタン
		GENERAL_BUTTON_CHANGE_MODE		= 2,		// 攻撃手段変更
		GENERAL_BUTTON_ADD_OPT			= 3,		// オプションの追加
		GENERAL_BUTTON_MOVE_UP			= 4,		// 移動（上）
		GENERAL_BUTTON_MOVE_DOWN		= 5,		// 移動（下）
		GENERAL_BUTTON_MOVE_LEFT		= 6,		// 移動（左）
		GENERAL_BUTTON_MOVE_RIGHT		= 7,		// 移動（右）
	};

	typedef int ButtonPushedStatus;

	// ボタンの状態
	enum ButtonStatus
	{
		BUTTON_STATUS_NO_EVENT		= 0,		// イベント無し
		BUTTON_STATUS_PUSHED		= 1,		// ボタンが押された
		BUTTON_STATUS_KEEP			= 2,		// ボタンが押し続けられている
	};

	// ボタンの状態を保持
	struct ButtonStatusHolder
	{
	public:
		unsigned char		m_Status[ 8 ];
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_INPUTTYPES_H
#define INCLUDED_GAMEENGINE_INPUTTYPES_H

namespace GameEngine
{
	// 特殊キー
	enum SpecialKey
	{
		SPECIAL_KEY_SS		= 0,		// スクリーンショット用
	};

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
		GENERAL_BUTTON_MOVE_UP			= 0,		// 移動（上）
		GENERAL_BUTTON_MOVE_DOWN		= 1,		// 移動（下）
		GENERAL_BUTTON_MOVE_LEFT		= 3,		// 移動（左）
		GENERAL_BUTTON_MOVE_RIGHT		= 2,		// 移動（右）
		GENERAL_BUTTON_SHOT				= 4,		// ショットボタン
		GENERAL_BUTTON_BOMB				= 5,		// ボムボタン
		GENERAL_BUTTON_GREEN			= 6,		// 緑属性へ変更
		GENERAL_BUTTON_BLUE				= 7,		// 青属性へ変更
		GENERAL_BUTTON_RED				= 8,		// 赤属性へ変更
		GENERAL_BUTTON_TOTAL,
	};

	typedef short ButtonPushedStatus;

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
		unsigned short			m_Status[ GENERAL_BUTTON_TOTAL ];	// 各ボタンの状態
		ButtonPushedStatus		m_RawButtonStatus;					// 加工前のボタン状態
	};
}

#endif
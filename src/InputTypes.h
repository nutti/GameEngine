#ifndef INCLUDED_GAMEENGINE_INPUTTYPES_H
#define INCLUDED_GAMEENGINE_INPUTTYPES_H

namespace GameEngine
{
	// ���̓f�o�C�X
	enum InputDevice
	{
		INPUT_DEVICE_KEYBOARD		= 0,		// �L�[�{�[�h
		INPUT_DEVICE_GAMEPAD		= 1,		// �Q�[���p�b�h
		INPUT_DEVICE_FILE			= 2,		// �t�@�C��
		INPUT_DEVICE_UNKNOWN		= 200,
	};

	// �Q�[�����̃{�^��
	enum GeneralButton
	{
		GENERAL_BUTTON_SHOT				= 0,		// �V���b�g�{�^��
		GENERAL_BUTTON_BOMB				= 1,		// �{���{�^��
		GENERAL_BUTTON_CHANGE_MODE		= 2,		// �U����i�ύX
		GENERAL_BUTTON_ADD_OPT			= 3,		// �I�v�V�����̒ǉ�
		GENERAL_BUTTON_MOVE_UP			= 4,		// �ړ��i��j
		GENERAL_BUTTON_MOVE_DOWN		= 5,		// �ړ��i���j
		GENERAL_BUTTON_MOVE_LEFT		= 6,		// �ړ��i���j
		GENERAL_BUTTON_MOVE_RIGHT		= 7,		// �ړ��i�E�j
	};

	typedef int ButtonPushedStatus;

	// �{�^���̏��
	enum ButtonStatus
	{
		BUTTON_STATUS_NO_EVENT		= 0,		// �C�x���g����
		BUTTON_STATUS_PUSHED		= 1,		// �{�^���������ꂽ
		BUTTON_STATUS_KEEP			= 2,		// �{�^���������������Ă���
	};

	// �{�^���̏�Ԃ�ێ�
	struct ButtonStatusHolder
	{
	public:
		unsigned char		m_Status[ 8 ];
	};
}

#endif
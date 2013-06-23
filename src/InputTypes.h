#ifndef INCLUDED_GAMEENGINE_INPUTTYPES_H
#define INCLUDED_GAMEENGINE_INPUTTYPES_H

namespace GameEngine
{
	// ����L�[
	enum SpecialKey
	{
		SPECIAL_KEY_SS		= 0,		// �X�N���[���V���b�g�p
	};

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
		GENERAL_BUTTON_MOVE_UP			= 0,		// �ړ��i��j
		GENERAL_BUTTON_MOVE_DOWN		= 1,		// �ړ��i���j
		GENERAL_BUTTON_MOVE_LEFT		= 3,		// �ړ��i���j
		GENERAL_BUTTON_MOVE_RIGHT		= 2,		// �ړ��i�E�j
		GENERAL_BUTTON_SHOT				= 4,		// �V���b�g�{�^��
		GENERAL_BUTTON_BOMB				= 5,		// �{���{�^��
		GENERAL_BUTTON_GREEN			= 6,		// �Α����֕ύX
		GENERAL_BUTTON_BLUE				= 7,		// �����֕ύX
		GENERAL_BUTTON_RED				= 8,		// �ԑ����֕ύX
		GENERAL_BUTTON_TOTAL,
	};

	typedef short ButtonPushedStatus;

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
		unsigned short			m_Status[ GENERAL_BUTTON_TOTAL ];	// �e�{�^���̏��
		ButtonPushedStatus		m_RawButtonStatus;					// ���H�O�̃{�^�����
	};
}

#endif
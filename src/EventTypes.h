#ifndef INCLUDED_GAMEENGINE_EVENTTYPES_H
#define INCLUDED_GAMEENGINE_EVENTTYPES_H

namespace GameEngine
{
	enum EventType
	{
		EVENT_TYPE_START_GAME									= -1,	// �Q�[���N��
		EVENT_TYPE_INITIALIZE									= 0,	// �������v��
		EVENT_TYPE_MOVE_TO_MENU									= 1,	// ���j���[��ʈڍs�v��
		EVENT_TYPE_MOVE_TO_STAGE								= 2,	// �X�e�[�W�J�n�v���i��2�����ɃX�e�[�W�ԍ��j
		EVENT_TYPE_MOVE_TO_SCORE								= 3,	// �X�R�A��ʂֈړ�
		EVENT_TYPE_MOVE_TO_SCORE_ENTRY							= 4,	// �X�R�A�G���g����ʂֈړ�
		EVENT_TYPE_MOVE_TO_REPLAY_ENTRY_FROM_SCORE_ENTRY		= 5,	// ���v���C�G���g����ʂֈړ��i�X�R�A�G���g����ʂ���ړ��j
		EVENT_TYPE_MOVE_TO_MENU_FROM_REPLAY_ENTRY				= 6,	// ���j���[��ʈڍs�v���i���v���C�G���g����ʂ���j
		EVENT_TYPE_MOVE_TO_REPLAY_ENTRY_FROM_SELF				= 7,	// ���v���C�G���g����ʂֈړ��i���v���C�G���g����ʂ���ړ��j
		EVENT_TYPE_MOVE_TO_REPLAY								= 8,	// ���v���C��ʂֈړ�
		EVENT_TYPE_MOVE_TO_MENU_FROM_INITIALIZE					= 9,	// ��������Ԃ��烁�j���[��ʈڍs�v��
		EVENT_TYPE_MOVE_TO_NEXT_STAGE							= 10,	// ���̃X�e�[�W�ֈڍs�v��
		EVENT_TYPE_FRAME_UPDATE									= 100,	// �t���[���X�V�v��
		EVENT_TYPE_GAME_TERM									= 101,	// �Q�[���I���v��
		EVENT_TYPE_UNKNOWN										= 200,
	};


	// �v���f�[�^
	enum RequestData
	{
		REQUEST_DATA_SCORE_RANK				= 0,	// �X�R�A�̃����L���O���擾����B�i��2������int�^�̃����N���i�[����ϐ��j
		REQUEST_DATA_DISPLAYED_SCORE_DATA	= 1,	// �X�R�A�\���p�̃f�[�^���擾����B�i��2�����ɕ\���p�X�R�A�f�[�^���i�[����ϐ��j
	};
}

#endif
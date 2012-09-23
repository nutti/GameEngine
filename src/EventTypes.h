#ifndef INCLUDED_GAMEENGINE_EVENTTYPES_H
#define INCLUDED_GAMEENGINE_EVENTTYPES_H

namespace GameEngine
{
	enum EventType
	{
		EVENT_TYPE_INITIALIZE		= 0,	// �������v��
		EVENT_TYPE_MOVE_TO_MENU		= 1,	// ���j���[��ʈڍs�v��
		EVENT_TYPE_MOVE_TO_STAGE	= 2,	// �X�e�[�W�J�n�v���i��2�����ɃX�e�[�W�ԍ��j
		EVENT_TYPE_FRAME_UPDATE		= 100,
		EVENT_TYPE_UNKNOWN			= 200,
	};
}

#endif
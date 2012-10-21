#ifndef INCLUDED_GAMEENGINE_SCENETYPES_H
#define INCLUDED_GAMEENGINE_SCENETYPES_H

namespace GameEngine
{
	// �V�[�����ʎq
	enum SceneType
	{
		SCENE_TYPE_INITIALIZE	= 0,
		SCENE_TYPE_MENU			= 1,
		SCENE_TYPE_STAGE		= 2,
		SCENE_TYPE_SCORE		= 3,
		SCENE_TYPE_SCORE_ENTRY	= 4,
		SCENE_TYPE_REPLAY_ENTRY	= 5,
		SCENE_TYPE_REPLAY		= 6,
		SCENE_TYPE_NOT_CHANGE	= 100,
		SCENE_TYPE_GAME_TERM	= 101,
		SCENE_TYPE_UNKNOWN		= 200,
	};

	// �X�e�[�W���ʎq
	enum StageID
	{
		STAGE_ID_STAGE_1		= 0,
		STAGE_ID_STAGE_2		= 1,
		STAGE_ID_STAGE_3		= 2,
		STAGE_ID_STAGE_4		= 3,
		STAGE_ID_STAGE_5		= 4,
		STAGE_ID_ALL_CLEARED	= 5,
	};

	// �I�u�W�F�N�g���ʎq
	enum GameObjectID
	{
		GAME_OBJECT_ID_PLAYER		= 0,	// �v���C���[
		GAME_OBJECT_ID_ENEMY		= 1,	// �G
		GAME_OBJECT_ID_ITEM			= 2,	// �A�C�e��
		GAME_OBJECT_ID_PLAYER_SHOT	= 3,	// �v���C���[�V���b�g
		GAME_OBJECT_ID_ENEMY_SHOT	= 4,	// �G�V���b�g
		GAME_OBJECT_ID_EFFECT		= 5,	// �G�t�F�N�g
	};

	// �Q�[�����[�h
	enum GameMode
	{
		GAME_MODE_NORMAL			= 0,	// �ʏ�v���C
		GAME_MODE_REPLAY			= 1,	// ���v���C
	};
}

#endif
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
		SCENE_TYPE_NOT_CHANGE	= 100,
		SCENE_TYPE_UNKNOWN		= 200,
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
}

#endif
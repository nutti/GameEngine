#ifndef INCLUDED_GAMEENGINE_STAGE_H
#define INCLUDED_GAMEENGINE_STAGE_H

#include <memory>
#include <list>
#include "Scene.h"

#include "GameObjectBuilder.h"
#include "RandomGenerator.h"

namespace GameEngine
{
	class Enemy;
	class Player;
	class PlayerShot;
	class EnemyShot;
	class Item;
	class Effect;

	typedef std::list < Enemy* >		EnemyList;
	typedef std::list < PlayerShot* >	PlayerShotList;
	typedef std::list < EnemyShot* >	EnemyShotList;
	typedef std::list < Item* >			ItemList;
	typedef std::list < Effect* >		EffectList;

	struct StageData
	{
		int					m_StageNo;				// �X�e�[�W�ԍ�
		int					m_Frame;				// �t���[����
		int					m_Score;				// �X�e�[�W�̃X�R�A
		bool				m_IsReplay;				// ���v���C��ԂȂ�true
		GameObjectBuilder	m_ObjBuilder;			// �Q�[���I�u�W�F�N�g�\�z�N���X
		RandomGenerator		m_RandGen;

		Player*				m_pPlayer;				// �v���C���[
		EnemyList			m_EnemyList;			// �G���X�g
		PlayerShotList		m_PlayerShotList;		// �v���C���[�V���b�g���X�g
		EnemyShotList		m_EnemyShotList;		// �G�V���b�g���X�g
		ItemList			m_ItemList;				// �A�C�e�����X�g
		EffectList			m_EffectList;			// �G�t�F�N�g���X�g
	};

	struct ScriptData;
	class Stage : public Scene
	{
	private:
		class Impl;
		std::auto_ptr < Stage::Impl >		m_pImpl;
	public:
		Stage( int stageNo, bool isReplay );
		~Stage();
		void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachScriptData( const ScriptData& data );
	};
}

#endif
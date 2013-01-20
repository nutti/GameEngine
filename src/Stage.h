#ifndef INCLUDED_GAMEENGINE_STAGE_H
#define INCLUDED_GAMEENGINE_STAGE_H

#include <memory>
#include <list>
#include <queue>
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
	class EnemyShotGroup;

	typedef Enemy Boss;

	typedef std::list < Enemy* >			EnemyList;
	typedef std::list < PlayerShot*	>		PlayerShotList;
	typedef std::list < EnemyShot* >		EnemyShotList;
	typedef std::list < Item* >				ItemList;
	typedef std::list < Effect* >			EffectList;
	typedef std::list < EnemyShotGroup* >	EnemyShotGroupList;

	struct StageMessage
	{
		enum StageMessageID
		{
			STAGE_MESSAGE_ID_PLAYER_DAMAGED				= 0,		// �v���C���[���_���[�W���󂯂�
			STAGE_MESSAGE_ID_PLAYER_DESTORYED			= 1,		// �v���C���[�����j���ꂽ�iHP��0�ȉ��ɂȂ����j
			STAGE_MESSAGE_ID_BOSS_MODE_STARTED			= 2,		// �{�X���[�h�ֈڍs����
			STAGE_MESSAGE_ID_BOSS_MODE_ENDED			= 3,		// �{�X���[�h���I������
			STAGE_MESSAGE_ID_BOSS_DAMAGED				= 4,		// �{�X���_���[�W���󂯂�
			STAGE_MESSAGE_ID_BOSS_INVOKE_CONS_SKILL		= 5,		// �{�X���X�L�����g�p����
			STAGE_MESSAGE_ID_BOSS_STOP_CONS_SKILL		= 6,		// �{�X���X�L���̎g�p���I������
			STAGE_MESSAGE_ID_BOSS_SHIFT_NEXT_MODE		= 7,		// �{�X�Ŏ��̒i�K�֐i��
			STAGE_MESSAGE_ID_ENEMY_INVOKE_CONS_SKILL	= 8,		// �G�i�{�X�ȊO�j���X�L�����g�p����
			STAGE_MESSAGE_ID_PLAYER_BOMBED				= 9,		// �v���C���[���{������
			STAGE_MESSAGE_ID_ENEMY_DAMAGED				= 10,		// �G�i�{�X�ȊO�j���_���[�W���󂯂�
		};
		union StageMessageData
		{
			int				m_Integer;
			std::string*	m_pString;
			float			m_Float;
		};

		StageMessageID		m_MsgID;
		std::vector < StageMessageData >	m_MsgDataList;
	};

	

	typedef std::queue < StageMessage >			StageMessageQueue;

	

	struct StageData
	{
		int					m_StageNo;				// �X�e�[�W�ԍ�
		int					m_Frame;				// �t���[����
		int					m_FrameTotal;			// �X�e�[�W�S�̂̃t���[����
		bool				m_IsReplay;				// ���v���C��ԂȂ�true
		GameObjectBuilder	m_ObjBuilder;			// �Q�[���I�u�W�F�N�g�\�z�N���X
		RandomGenerator		m_RandGen;

		Player*				m_pPlayer;				// �v���C���[
		EnemyList			m_EnemyList;			// �G���X�g
		Boss*				m_pBoss;				// �{�X���X�g
		PlayerShotList		m_PlayerShotList;		// �v���C���[�V���b�g���X�g
		EnemyShotList		m_EnemyShotList;		// �G�V���b�g���X�g
		ItemList			m_ItemList;				// �A�C�e�����X�g
		EffectList			m_EffectList;			// �G�t�F�N�g���X�g
		EnemyShotGroupList	m_EnemyShotGroupList;	// �G�V���b�g�O���[�v���X�g
		
		GameDataMsg			m_GameData;				// ���t���[���ɂ�����Q�[���f�[�^
		GameDataMsg			m_FrameGameData;		// ���t���[���ōX�V�����Q�[���f�[�^

		StageMessageQueue	m_MsgQueue;				// �X�e�[�W�p���b�Z�[�W�L���[

		bool				m_HasTermSig;
		int					m_BossMode;				// 1�Ȃ�{�X���[�h

		ResourceMap			m_ResourceMap;

		int					m_ConsLevel;			// �ӎ����x���̃x�[�X���x��
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
		void AttachGameData( const GameDataMsg& msg );
		int GetProgress() const;
		int GetScore() const;
		int GetKilled() const;
		int GetCrystal() const;
		GameDataMsg GetFrameData() const;
	};
}

#endif
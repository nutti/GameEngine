#ifndef INCLUDED_GAMEENGINE_ENEMY_H
#define INCLUDED_GAMEENGINE_ENEMY_H

#include <vector>

#include "CollisionObject.h"
#include "EnemyVCPU.h"

namespace GameEngine
{

	struct StageData;
	struct ResourceMap;
	class EnemyShotGroup;

	struct EnemyData
	{
		struct ItemDrop
		{
			int		m_ItemSubID;	// �A�C�e���̃T�uID
			int		m_Total;		// ��
		};

		float				m_PosX;				// �ʒu�iX���W�j
		float				m_PosY;				// �ʒu�iY���W�j
		int					m_HP;				// HP
		int					m_MaxHP;			// MaxHP
		int					m_ConsGauge;		// �ӎ��Q�[�W
		int					m_MaxConsGauge;		// Max�ӎ��Q�[�W
		int					m_ConsType;			// �ӎ��^�C�v
		int					m_Counter;			// �J�E���^
		float				m_ColRadius;		// �Փ˔��a
		int					m_Score;			// �X�R�A
		int					m_IsBoss;			// �{�X�Ȃ�1
		bool				m_Destroyed;		// �|���ꂽ�ꍇ��true
		bool				m_IsInvincibleMode;	// ���G��Ԃ̏ꍇ��true
		StageData*			m_pStageData;		// �X�e�[�W�f�[�^
		ItemDrop			m_ItemDrop[ 6 ];	// ���Ƃ��A�C�e���̏��
		std::shared_ptr < ResourceMap >		m_pResouceMap;
		std::vector < EnemyShotGroup* >		m_ShotGroupList;		// ���ݕێ����Ă���V���b�g�O���[�v
	};

	// ���x����̂��߁Apimpl�C�f�B�I���͎g�p���Ȃ��B
	struct EnemyScriptData;
	class Enemy : public CollisionObject
	{
	private:
		// Enemy�N���X�����p�f�[�^
		struct EnemyPrivateData
		{
			// �ӎ��Z
			//struct ConsSkillModeData
			//{
			//	bool			m_IsConsSkillMode;	// �ӎ��X�L�����[�h�Ȃ�true
			//	int				m_Counter;			// �G�t�F�N�g�p�J�E���^
			//	int				m_PostCounter;		// �G�t�F�N�g�I���Ƃ��̃J�E���^
			//	std::string		m_SkillName;		// �ӎ��Z��
			//};
			//int					m_DamagedCounter;		// �_���[�W���̃J�E���^
			int					m_PrevConsGauge;		// �O��̈ӎ��Q�[�W�i���̃Q�[�W����ʂɌ��������Ɉӎ��Z���g�p�����Ƃ݂Ȃ��j
			//int					m_SkillUsedCounter;		// �ӎ��Z�g�p���̃J�E���^
			//ConsSkillModeData	m_ConsSkillModeData;	// �ӎ��Z�p�̃f�[�^
		};

		EnemyData							m_Data;			// �f�[�^
		int									m_ScriptID;		// �X�N���v�gID
		EnemyVCPU							m_VM;			// ���z�}�V��
		std::shared_ptr < EnemyScriptData >	m_pScriptData;	// �X�N���v�g�f�[�^
		EnemyPrivateData					m_PrivateData;	// Enemy�N���X���������f�[�^
	public:
		Enemy(	std::shared_ptr < ResourceMap > pMap,
				std::shared_ptr < EnemyScriptData > pData,
				int id,
				StageData* pStageData );
		~Enemy();
		void Init( float posX, float posY );				// ������
		void Draw();										// �`��
		bool Update();										// �X�V
		//void InvokeConsSkill( std::string skillName );		// �X�L���Ăяo��
		//void StopConsSkill();								// �X�L���I���v��
		void Colided( CollisionObject* pObject );			// �Փˎ��̏��� �f�B�X�p�b�`��
		void ProcessCollision( Player* pPlayer );			// �Փˎ��̏����i�v���C���[�j
		void ProcessCollision( Enemy* pEnemy );				// �Փˎ��̏����i�G�j
		void ProcessCollision( PlayerShot* pPlayerShot );	// �Փˎ��̏����i�v���C���[�V���b�g�j
		void ProcessCollision( EnemyShot* pEnemyShot );		// �Փˎ��̏����i�G�e�j
		void ProcessCollision( Item* pItem );				// �Փˎ��̏����i�A�C�e���j
		void GetPos( float* pPosX, float* pPosY );
		int GetHP() const;									// HP���擾
		int GetMaxHP() const;								// �ő�HP���擾
		int GetConsGauge() const;							// �ӎ��Q�[�W�̎擾
		int GetMaxConsGauge() const;						// �ő�ӎ��Q�[�W�̎擾
		float GetCollisionRadius();

	};
}

#endif
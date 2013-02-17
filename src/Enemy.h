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

		std::string			m_Name;				// �G�̖��O
		float				m_PosX;				// �ʒu�iX���W�j
		float				m_PosY;				// �ʒu�iY���W�j
		float				m_PosZ;				// �ʒu�iZ���W�j
		float				m_ScaleX;			// �g�嗦�iX�j
		float				m_ScaleY;			// �g�嗦�iY�j
		float				m_ScaleZ;			// �g�嗦�iZ�j
		float				m_RotX;				// ��]�p�x�iX�����S�j
		float				m_RotY;				// ��]�p�x�iY�����S�j
		float				m_RotZ;				// ��]�p�x�iZ�����S�j
		int					m_ImgID;			// �G�摜
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
		bool				m_IsNonCollisionMode;	// �Փ˂��Ȃ����̏ꍇ��true
		bool				m_IsConsSkillMode;	// �ӎ��Z���g�p���Ă���ꍇ��true
		bool				m_Paused;			// �s����~���̏ꍇtrue
		bool				m_Is3D;				// 3D�\�����H
		int					m_ConsSkillAttr;	// �ӎ��Z�̑���
		std::string			m_ConsSkillName;	// �ӎ��Z��
		StageData*			m_pStageData;		// �X�e�[�W�f�[�^
		ItemDrop			m_ItemDrop[ 6 ];	// ���Ƃ��A�C�e���̏��
		std::shared_ptr < ResourceMap >		m_pResouceMap;
		std::vector < EnemyShotGroup* >		m_ShotGroupList;		// ���ݕێ����Ă���V���b�g�O���[�v
		std::vector < int >					m_Regs;			// �ėp���W�X�^
	};

	// ���x����̂��߁Apimpl�C�f�B�I���͎g�p���Ȃ��B
	struct EnemyScriptData;
	class Enemy : public CollisionObject
	{
	private:
		// Enemy�N���X�����p�f�[�^
		struct EnemyPrivateData
		{
			int			m_ConsSkillEffectCounter;			// �ӎ��Z�g�p���p�J�E���^
			int			m_ConsSkillEffectPostCounter;		// �ӎ��Z�g�p��p�J�E���^
			int			m_PrevConsGauge;					// �O��̈ӎ��Q�[�W�i���̃Q�[�W����ʂɌ��������Ɉӎ��Z���g�p�����Ƃ݂Ȃ��j
		};

		EnemyData							m_Data;					// �f�[�^
		int									m_ScriptID;				// �X�N���v�gID
		EnemyVCPU							m_VM;					// ���z�}�V��
		std::shared_ptr < EnemyScriptData >	m_pScriptData;			// �X�N���v�g�f�[�^
		EnemyPrivateData					m_PrivateData;			// Enemy�N���X���������f�[�^
		static bool							m_SentLastDamagedMsg;	// ���t���[���ŁA�_���[�W���󂯂����b�Z�[�W�𑗂����ꍇtrue
	public:
		Enemy(	std::shared_ptr < ResourceMap > pMap,
				std::shared_ptr < EnemyScriptData > pData,
				int id,
				StageData* pStageData );
		~Enemy();
		void Init( float posX, float posY );				// ������
		void Draw();										// �`��
		bool Update();										// �X�V
		void Colided( CollisionObject* pObject );			// �Փˎ��̏��� �f�B�X�p�b�`��
		void ProcessCollision( Player* pPlayer );			// �Փˎ��̏����i�v���C���[�j
		void ProcessCollision( Enemy* pEnemy );				// �Փˎ��̏����i�G�j
		void ProcessCollision( PlayerShot* pPlayerShot );	// �Փˎ��̏����i�v���C���[�V���b�g�j
		void ProcessCollision( EnemyShot* pEnemyShot );		// �Փˎ��̏����i�G�e�j
		void ProcessCollision( Item* pItem );				// �Փˎ��̏����i�A�C�e���j
		void Damage( int val );								// �_���[�W��^����
		void GetPos( float* pPosX, float* pPosY );
		int GetHP() const;									// HP���擾
		int GetMaxHP() const;								// �ő�HP���擾
		int GetConsGauge() const;							// �ӎ��Q�[�W�̎擾
		int GetMaxConsGauge() const;						// �ő�ӎ��Q�[�W�̎擾
		float GetCollisionRadius();
		void Pause();										// �ꎞ��~�ɐݒ�
		void Resume();										// �ꎞ��~����ĊJ
		static void ClearLastDamagedMsg();
		bool IsNonCollisionMode() const;
	};

	
}

#endif
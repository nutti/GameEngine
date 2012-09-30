#ifndef INCLUDED_GAMEENGINE_PLAYER_H
#define INCLUDED_GAMEENGINE_PLAYER_H

#include <memory>

#include "CollisionObject.h"
#include "InputTypes.h"

namespace GameEngine
{

	enum PlayerConsMode
	{
		PLAYER_CONS_MODE_NORMAL		= 0,		// �ʏ탂�[�h
		PLAYER_CONS_MODE_GREEN		= 1,		// �΁i�ǔ����[�h�j
		PLAYER_CONS_MODE_BLUE		= 2,		// �i��]�W�����[�h�j
		PLAYER_CONS_MODE_RED		= 3,		// �ԁi�S���ʃ��[�h�j
	};

	struct PlayerData
	{
		float		m_PosX;				// �ʒu�iX���W�j
		float		m_PosY;				// �ʒu�iY���W�j
		int			m_HP;				// HP
		int			m_ConsCur;			// ���݂̈ӎ����
		int			m_ConsGauge[ 3 ];	// �ӎ��Q�[�W
		int			m_ConsLevel[ 3 ];	// �ӎ����x��
		int			m_ShotPower;		// �V���b�g�̃p���[
		float		m_ColRadius;		// �Փ˔��a
		int			m_Counter;			// �J�E���^
	};

	struct ResourceMap;
	struct StageData;
	class Player : public CollisionObject
	{
	private:
		class Impl;
		std::auto_ptr < Player::Impl >		m_pImpl;
	public:
		Player( std::shared_ptr < ResourceMap > pMap, StageData* pStageData );
		~Player();
		void AttachButtonState( const ButtonStatusHolder& holder );		// �L�[���͂�ݒ�
		void Init( float posX, float posY );							// ������
		void Draw();													// �`��
		bool Update();													// �X�V
		void Colided( CollisionObject* pObject );						// �Փˎ��̏��� �f�B�X�p�b�`��
		void ProcessCollision( Player* pPlayer );						// �Փˎ��̏����i�v���C���[�j
		void ProcessCollision( Enemy* pEnemy );							// �Փˎ��̏����i�G�j
		void ProcessCollision( PlayerShot* pPlayerShot );				// �Փˎ��̏����i�v���C���[�V���b�g�j
		void ProcessCollision( EnemyShot* pEnemyShot );					// �Փˎ��̏����i�G�e�j
		void ProcessCollision( Item* pItem );							// �Փˎ��̏����i�A�C�e���j
		void GetPos( float* pPosX, float* pPosY );						// �ʒu���擾
		float GetCollisionRadius();										// �Փ˔��a���擾
		int GetHP() const;												// HP���擾
		int GetShotPower() const;										// �V���b�g�̈З͂��擾
		int GetConsGauge( int cons ) const;								// �ӎ��Q�[�W�̎擾
		int GetConsLevel( int cons ) const;								// �ӎ����x���̎擾
	};
}

#endif
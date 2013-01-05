#ifndef INCLUDED_GAMEENGINE_ENEMYSHOT_H
#define INCLUDED_GAMEENGINE_ENEMYSHOT_H

#include <memory>

#include "CollisionObject.h"

namespace GameEngine
{
	struct EnemyShotMessage
	{
		enum EnemyShotMessageID
		{
			ENEMY_SHOT_MESSAGE_ID_PLAYER_DAMAGED			= 0,		// �v���C���[���_���[�W���󂯂�
			ENEMY_SHOT_MESSAGE_ID_BOSS_MODE_STARTED			= 1,		// �{�X���[�h�ֈڍs����
			ENEMY_SHOT_MESSAGE_ID_BOSS_INVOKE_CONS_SKILL	= 2,		// �{�X���X�L�����g�p����
		};

		int		m_MsgID;
	};

	struct ResourceMap;
	class EnemyShotGroup;
	class EnemyShot : public CollisionObject
	{
	private:
		class Impl;
		std::auto_ptr < EnemyShot::Impl >		m_pImpl;
	public:
		EnemyShot( std::shared_ptr < ResourceMap > pMap, int id );
		~EnemyShot();
		void Init( float posX, float posY );				// ������
		void Draw();										// �`��
		bool Update();										// �X�V
		void SetPos( float posX, float posY );				// �ʒu��ݒ�
		void SetAngle( float angle );						// �p�x��ݒ�
		void SetSpeed( float speed );						// ���x��ݒ�
		void SetImage( int id );							// �摜��ݒ�
		void SetImageScale( float scale );					// �摜�̊g�嗦��ݒ�
		void SetCollisionRadius( float radius );			// �Փ˔���̔��a��ݒ�
		void AddPos( float x, float y );					// �ʒu�����Z
		void AddAngle( float angle );						// �p�x�����Z
		void AddSpeed( float speed );						// ���x�����Z
		void JoinShotGroup( int id, EnemyShotGroup* pGroup );
		void Colided( CollisionObject* pObject );			// �Փˎ��̏��� �f�B�X�p�b�`��
		void ProcessCollision( Player* pPlayer );			// �Փˎ��̏����i�v���C���[�j
		void ProcessCollision( Enemy* pEnemy );				// �Փˎ��̏����i�G�j
		void ProcessCollision( PlayerShot* pPlayerShot );	// �Փˎ��̏����i�v���C���[�V���b�g�j
		void ProcessCollision( EnemyShot* pEnemyShot );		// �Փˎ��̏����i�G�e�j
		void ProcessCollision( Item* pItem );				// �Փˎ��̏����i�A�C�e���j
		void GetPos( float* pPosX, float* pPosY );
		float GetCollisionRadius();
		int GetCounter() const;
		void PostMessage( int msgID );						// ���b�Z�[�W�̒ǉ�
	};
}

#endif
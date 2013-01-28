#ifndef INCLUDED_GAMEENGINE_PLAYERSHOT_H
#define INCLUDED_GAMEENGINE_PLAYERSHOT_H

#include <memory>

#include "CollisionObject.h"

namespace GameEngine
{

	struct ResourceMap;
	struct StageData;
	class PlayerShot : public CollisionObject
	{
	private:
		class Impl;
		std::auto_ptr < PlayerShot::Impl >		m_pImpl;
	public:
		PlayerShot( std::shared_ptr < ResourceMap > pMap, int id );
		~PlayerShot();
		void Init( float posX, float posY );							// ������
		void Draw();													// �`��
		bool Update();													// �X�V
		void SetPos( const Player& player );							// �ʒu��ݒ�
		void SetPos( float posX, float posY );							// �ʒu��ݒ�
		void SetAngle( float angle );									// �p�x��ݒ�
		void SetSpeed( float speed );									// ���x��ݒ�
		void SetShotPower( int power );
		void SetConsAttr( int attr );
		void Colided( CollisionObject* pObject );						// �Փˎ��̏��� �f�B�X�p�b�`��
		void ProcessCollision( Player* pPlayer );						// �Փˎ��̏����i�v���C���[�j
		void ProcessCollision( Enemy* pEnemy );							// �Փˎ��̏����i�G�j
		void ProcessCollision( PlayerShot* pPlayerShot );				// �Փˎ��̏����i�v���C���[�V���b�g�j
		void ProcessCollision( EnemyShot* pEnemyShot );		// �Փˎ��̏����i�G�e�j
		void ProcessCollision( Item* pItem );				// �Փˎ��̏����i�A�C�e���j
		void GetPos( float* pPosX, float* pPosY );
		float GetCollisionRadius();
		int GetShotPower() const;
		int GetConsAttr() const;
		void SetPlayer( Player* pPlayer );
		void SetEnemy( Enemy* pEnemy );
		void SetStageData( StageData* pData );
		int GetID() const;
	};
}

#endif
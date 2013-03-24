#ifndef INCLUDED_GAMEENGINE_COLLISIONOBJECT_H
#define INCLUDED_GAMEENGINE_COLLISIONOBJECT_H

#include "GameObject.h"

#include <memory>

namespace GameEngine
{
	class Player;
	class Enemy;
	class PlayerShot;
	class EnemyShot;
	class Item;
	class CollisionObject : public GameObject
	{
	public:
		CollisionObject();
		virtual ~CollisionObject();
		virtual void Init( float posX, float posY ) = 0;				// ������
		virtual void Colided( CollisionObject* pObject ) = 0;			// �Փˎ��̏��� �f�B�X�p�b�`��
		virtual void ProcessCollision( Player* pPlayer ) = 0;			// �Փˎ��̏����i�v���C���[�j
		virtual void ProcessCollision( Enemy* pEnemy ) = 0;				// �Փˎ��̏����i�G�j
		virtual void ProcessCollision( PlayerShot* pPlayerShot ) = 0;	// �Փˎ��̏����i�v���C���[�V���b�g�j
		virtual void ProcessCollision( EnemyShot* pEnemyShot )= 0;		// �Փˎ��̏����i�G�e�j
		//virtual void ProcessCollision( std::shared_ptr < Item > pItem ) = 0;				// �Փˎ��̏����i�A�C�e���j
		virtual void ProcessCollision( Item* pItem ) = 0;
		virtual void Draw() = 0;										// �`��
		virtual bool Update() = 0;										// �X�V
		virtual void GetPos( float* pX, float* pY ) = 0;				// �ʒu���擾
		virtual float GetCollisionRadius() = 0;							// �Փ˔��a���擾
	};
}

#endif
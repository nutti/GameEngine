#ifndef INCLUDED_GAMEENGINE_ITEM_H
#define INCLUDED_GAMEENGINE_ITEM_H

#include <memory>

#include "CollisionObject.h"

namespace GameEngine
{
	struct ItemData
	{
		int			m_ItemID;		// �A�C�e�����ʎq
		float		m_PosX;			// �ʒu�iX���W�j
		float		m_PosY;			// �ʒu�iY���W�j
		float		m_ColRadius;	// �Փ˔���̔��a
		bool		m_Colided;		// �Փ˂�����true
	};


	struct ResourceMap;
	class Item : public CollisionObject
	{
	private:
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		ItemData							m_ItemData;
	public:
		Item( std::shared_ptr < ResourceMap > pMap, int id );
		~Item();
		void Init( float posX, float posY );				// ������
		void Draw();										// �`��
		bool Update();										// �X�V
		void SetPos( float posX, float posY );				// �ʒu��ݒ�
		void Colided( CollisionObject* pObject );			// �Փˎ��̏��� �f�B�X�p�b�`��
		void ProcessCollision( Player* pPlayer );			// �Փˎ��̏����i�v���C���[�j
		void ProcessCollision( Enemy* pEnemy );				// �Փˎ��̏����i�G�j
		void ProcessCollision( PlayerShot* pPlayerShot );	// �Փˎ��̏����i�v���C���[�V���b�g�j
		void ProcessCollision( EnemyShot* pEnemyShot );		// �Փˎ��̏����i�G�e�j
		void ProcessCollision( Item* pItem );				// �Փˎ��̏����i�A�C�e���j
		void GetPos( float* pPosX, float* pPosY );
		float GetCollisionRadius();
	};
}

#endif
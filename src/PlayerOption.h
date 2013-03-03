#ifndef INCLUDED_GAMEENGINE_PLAYEROPTION_H
#define INCLUDED_GAMEENGINE_PLAYEROPTION_H

#include <memory>

#include "CollisionObject.h"
#include "InputTypes.h"

namespace GameEngine
{

	struct ResourceMap;
	struct StageData;
	class Player;
	class PlayerOption : public CollisionObject
	{
	private:
		class Impl;
		std::auto_ptr < PlayerOption::Impl >		m_pImpl;
	public:
		PlayerOption( std::shared_ptr < ResourceMap > pMap, StageData* pStageData, int id );
		~PlayerOption();
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
		float GetCollisionRadius();
		void SetPos( float posX, float posY );
		void ChangeID( int id );
		void ChangeConsMode( int mode );
		void NotifyOptTotal( int total );
	};
}

#endif
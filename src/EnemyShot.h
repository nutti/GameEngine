#ifndef INCLUDED_GAMEENGINE_ENEMYSHOT_H
#define INCLUDED_GAMEENGINE_ENEMYSHOT_H

#include <memory>

#include "CollisionObject.h"

namespace GameEngine
{
	enum EnemyShotAttr
	{
		ENEMY_SHOT_ATTR_NORMAL	= 0,
		ENEMY_SHOT_ATTR_GREEN	= 1,
		ENEMY_SHOT_ATTR_BLUE	= 2,
		ENEMY_SHOT_ATTR_RED		= 3,
		ENEMY_SHOT_ATTR_TOTAL,
	};

	struct EnemyShotMessage
	{
		enum EnemyShotMessageID
		{
			ENEMY_SHOT_MESSAGE_ID_PLAYER_DAMAGED			= 0,		// �v���C���[���_���[�W���󂯂�
			ENEMY_SHOT_MESSAGE_ID_BOSS_MODE_STARTED			= 1,		// �{�X���[�h�ֈڍs����
			ENEMY_SHOT_MESSAGE_ID_BOSS_INVOKE_CONS_SKILL	= 2,		// �{�X���X�L�����g�p����
			ENEMY_SHOT_MESSAGE_ID_PLAYER_BOMBED				= 3,		// �v���C���[���{���𔭓�
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
		void Draw();										// �`��
		bool Update();										// �X�V
		void SetPower( int power );							// �e�̍U���͂�ݒ�
		void SetImage( int id );							// �摜��ݒ�
		void SetImageScale( float scale );					// �摜�̊g�嗦��ݒ�
		void SetConsAttr( int attr );						// �ӎ��Z��p�e�ɐݒ�
		void JoinShotGroup( int id, EnemyShotGroup* pGroup );
		void Colided( CollisionObject* pObject );			// �Փˎ��̏��� �f�B�X�p�b�`��
		void ProcessCollision( Player* pPlayer );			// �Փˎ��̏����i�v���C���[�j
		void ProcessCollision( Enemy* pEnemy );				// �Փˎ��̏����i�G�j
		void ProcessCollision( PlayerShot* pPlayerShot );	// �Փˎ��̏����i�v���C���[�V���b�g�j
		void ProcessCollision( EnemyShot* pEnemyShot );		// �Փˎ��̏����i�G�e�j
		void ProcessCollision( Item* pItem );				// �Փˎ��̏����i�A�C�e���j
		int GetPower() const;								// �e�̍U���͂��擾
#if defined ( USE_FLOATING_POINT )
		void Init( float posX, float posY );				// ������
		void AddPos( float x, float y );					// �ʒu�����Z
		void AddAngle( float angle );						// �p�x�����Z
		void AddSpeed( float speed );						// ���x�����Z
		void GetPos( float* pPosX, float* pPosY );
		void SetPos( float posX, float posY );				// �ʒu��ݒ�
		void SetLinePos( float x1, float y1, float x2, float y2, float thickness );		// ���̒l��ݒ�
		void SetAngle( float angle );						// �p�x��ݒ�
		void SetSpeed( float speed );						// ���x��ݒ�
		void SetCollisionRadius( float radius );			// �Փ˔���̔��a��ݒ�
		bool DoesColideWithPlayer( float x, float y, float radius );	// �v���C���[�Ƃ̏Փ˔���
		float GetCollisionRadius();
#elif defined ( USE_GAME_UNIT )
		void Init( const GameUnit& posX, const GameUnit& posY );				// ������
		void AddPos( const GameUnit& x, const GameUnit& y );					// �ʒu�����Z
		void AddAngle( const GameUnit& angle );						// �p�x�����Z
		void AddSpeed( const GameUnit& speed );						// ���x�����Z
		void GetPos( GameUnit* pPosX, GameUnit* pPosY );
		void SetPos( const GameUnit& posX, const GameUnit& posY );				// �ʒu��ݒ�
		void SetLinePos(	const GameUnit& x1,
							const GameUnit& y1,
							const GameUnit& x2,
							const GameUnit& y2,
							const GameUnit& thickness );		// ���̒l��ݒ�
		void SetAngle( const GameUnit& angle );						// �p�x��ݒ�
		void SetSpeed( const GameUnit& speed );						// ���x��ݒ�
		void SetCollisionRadius( const GameUnit& radius );			// �Փ˔���̔��a��ݒ�
		bool DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius );	// �v���C���[�Ƃ̏Փ˔���
		GameUnit GetCollisionRadius();
		GameUnit GetAngle() const;
		GameUnit GetSpeed() const;
#endif
		int GetCounter() const;
		void PostMessage( int msgID );						// ���b�Z�[�W�̒ǉ�
		bool IsDead() const;
		int GetConsAttr() const;
		void Pause();										// �ꎞ��~
		void Resume();										// �ꎞ��~����ĊJ
		void SetImgRotMode( int mode );						// �G�e�̉摜��]���[�h��ݒ�
		void SetImgRotAnglePerFrame( float angle );			// ���t���[�����������]�p�x��ݒ�
		void SetAlphaBlendingMode( int mode );				// ���u�����f�B���O�̕��@��ݒ�
		void SetShape( int shape );							// �Փ˔���̌`��ݒ�
		
		void EnableInvincibleMode();				// �����Ȃ����[�h�ֈڍs
		void DisableInvincibleMode();			// �����郂�[�h�ֈڍs
		void EnableInvisibleMode();				// �����Ȃ����[�h�ֈڍs
		void DisableInvisibleMode();			// �����郂�[�h�ֈڍs
	};
}

#endif
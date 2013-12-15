#ifndef INCLUDED_GAMEENGINE_ENEMYSHOT_H
#define INCLUDED_GAMEENGINE_ENEMYSHOT_H

#include <memory>

#include "../../CollisionObject.h"

namespace GameEngine
{
	enum EnemyShotTextureColor
	{
		ENEMY_SHOT_TEX_COLOR_YELLOW		= 0,
		ENEMY_SHOT_TEX_COLOR_BLUE		= 1,
		ENEMY_SHOT_TEX_COLOR_RED		= 2,
		ENEMY_SHOT_TEX_COLOR_GREEN		= 3,
		ENEMY_SHOT_TEX_COLOR_PINK		= 4,
		ENEMY_SHOT_TEX_COLOR_AQUA		= 5,
		ENEMY_SHOT_TEX_COLOR_ORANGE		= 6,
		ENEMY_SHOT_TEX_COLOR_BLACK		= 7,
		ENEMY_SHOT_TEX_COLOR_TOTAL,
	};

	enum EnemyShotID
	{
		ENEMY_SHOT_ID_FOUR_RAYED_STAR_M		= 0,
		ENEMY_SHOT_ID_ELLIPSE_S				= 1,
		ENEMY_SHOT_ID_CRESCENT_M			= 2,
		ENEMY_SHOT_ID_GLORIOUS_L			= 3,
		ENEMY_SHOT_ID_HEXAGRAM_M			= 4,
		ENEMY_SHOT_ID_CHEESE_M				= 5,
		ENEMY_SHOT_ID_GLORIOUS_M			= 6,
		ENEMY_SHOT_ID_CIRCLE_M				= 7,
		ENEMY_SHOT_ID_NEEDLE_M				= 8,
		ENEMY_SHOT_ID_LASER_M				= 9,
		ENEMY_SHOT_ID_SMOKE_M				= 10,
		ENEMY_SHOT_ID_RING					= 11,
		ENEMY_SHOT_ID_BEAM_M				= 12,
		ENEMY_SHOT_ID_TOTAL,
	};

	enum DeleteBy
	{
		DELETE_BY_PLAYER_DAMAGE		= 0,
		DELETE_BY_PLAYER_SKILL		= 1,
		DELETE_BY_TOTAL,
	};

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
	class EnemyShotImpl;
	class EnemyShot : public CollisionObject
	{
	private:
		std::auto_ptr < EnemyShotImpl >		m_pImpl;
	public:
		EnemyShot( std::shared_ptr < ResourceMap > pMap, int id );
		virtual ~EnemyShot();
		virtual void Draw();										// �`��
		virtual bool Update();										// �X�V
		void SetPower( int power );							// �e�̍U���͂�ݒ�
		void SetImage( int id );							// �摜��ݒ�
		void SetAtlasImage( int id );						// �A�g���X�摜��ݒ�
		void SetImageScale( float scale );					// �摜�̊g�嗦��ݒ�
		void SetConsAttr( int attr );						// �ӎ��Z��p�e�ɐݒ�
		void JoinShotGroup( int id, EnemyShotGroup* pGroup );
		virtual void Colided( CollisionObject* pObject );			// �Փˎ��̏��� �f�B�X�p�b�`��
		void ProcessCollision( Player* pPlayer );			// �Փˎ��̏����i�v���C���[�j
		void ProcessCollision( Enemy* pEnemy );				// �Փˎ��̏����i�G�j
		void ProcessCollision( PlayerShot* pPlayerShot );	// �Փˎ��̏����i�v���C���[�V���b�g�j
		void ProcessCollision( EnemyShot* pEnemyShot );		// �Փˎ��̏����i�G�e�j
		void ProcessCollision( Item* pItem );				// �Փˎ��̏����i�A�C�e���j
		int GetPower() const;								// �e�̍U���͂��擾

		virtual void Init( const GameUnit& posX, const GameUnit& posY );				// ������
		virtual void AddPos( const GameUnit& x, const GameUnit& y );					// �ʒu�����Z
		void AddAngle( const GameUnit& angle );						// �p�x�����Z
		void AddSpeed( const GameUnit& speed );						// ���x�����Z
		virtual void GetPos( GameUnit* pPosX, GameUnit* pPosY );
		virtual void SetPos( const GameUnit& posX, const GameUnit& posY );				// �ʒu��ݒ�
		virtual void SetLinePos(	const GameUnit& x1,
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

		std::string GetMasterEnemyName() const;		// �}�X�^�ƂȂ�G�̖��O���擾
		void SetDrawingMultiplicity( int num );		// �`�摽�d�x��ݒ�

		void Delete( int by );			// �폜

		void DeleteWhen( int when );
		void NotDeleteWhen( int when );		// �폜�^�C�~���O�̐ݒ�


		// For line interface.
		virtual void SetLength( const GameUnit& length );
	};
}

#endif
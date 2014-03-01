#ifndef INCLUDED_GAMEENGINE_PLAYER_H
#define INCLUDED_GAMEENGINE_PLAYER_H

#include <memory>
#include <list>

#include "CollisionObject.h"
#include "InputTypes.h"
#include "PlayerOption.h"

#include "Math.hpp"

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
		struct GameUnitData
		{
			GameUnit		m_PosX;
			GameUnit		m_PosY;
			GameUnit		m_ColRadius;
		};
		GameUnitData	m_GUData;
		
		int			m_HP;					// HP
		int			m_ConsCur;				// ���݂̈ӎ����
		int			m_ConsGauge[ 3 ];		// �ӎ��Q�[�W
		int			m_ConsLevel[ 3 ];		// �ӎ����x��
		int			m_ShotPower;			// �V���b�g�̃p���[
		int			m_Counter;				// �J�E���^
		int			m_RestInvincibleTime;	// �c��̖��G����
	};

	struct ResourceMap;
	struct StageData;
	class Player : public CollisionObject
	{
	private:
		const int	INVINCIBLE_TIME;

		ButtonStatusHolder					m_ButtonStatus;
		PlayerData							m_Data;
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		StageData*							m_pStageData;

		std::list < PlayerOption* >			m_PlayerOptList;
		
		void NormalModeShot();
		void GreenModeShot();
		void BlueModeShot();
		void RedModeShot();

		void GreenModeBomb();
		void BlueModeBomb();
		void RedModeBomb();

		void AddOpt();
		
		void Move();
		void ChangeMode();
		void UpdateCons();

	public:
		Player( std::shared_ptr < ResourceMap > pMap, StageData* pStageData );
		~Player();
		void AttachButtonState( const ButtonStatusHolder& holder );		// �L�[���͂�ݒ�
		
		void Draw();													// �`��
		bool Update();													// �X�V
		void Colided( CollisionObject* pObject );						// �Փˎ��̏��� �f�B�X�p�b�`��
		void ProcessCollision( Player* pPlayer ){};						// �Փˎ��̏����i�v���C���[�j
		void ProcessCollision( Enemy* pEnemy );							// �Փˎ��̏����i�G�j
		void ProcessCollision( PlayerShot* pPlayerShot ){};				// �Փˎ��̏����i�v���C���[�V���b�g�j
		void ProcessCollision( EnemyShot* pEnemyShot );					// �Փˎ��̏����i�G�e�j
		void ProcessCollision( Item* pItem );							// �Փˎ��̏����i�A�C�e���j
		void Init( const GameUnit& posX, const GameUnit& posY ){}		// ������
		void GetPos( GameUnit* pPosX, GameUnit* pPosY );				// �ʒu���擾
		GameUnit GetCollisionRadius();									// �Փ˔��a���擾
		int GetHP() const;												// HP���擾
		int GetShotPower() const;										// �V���b�g�̈З͂��擾
		int GetConsGauge( int cons ) const;								// �ӎ��Q�[�W�̎擾
		int GetConsLevel( int cons ) const;								// �ӎ����x���̎擾
		int GetCurCons() const;											// ���݂̈ӎ���Ԃ��擾

		void SetPos( const GameUnit& posX, const GameUnit& posY );		// �ʒu��ݒ�
		void SetHP( int hp );											// HP��ݒ�
		void SetShotPower( int power );									// �V���b�g�̈З͂�ݒ�
		void SetConsGauge( int cons, int val );							// �ӎ��Q�[�W��ݒ�
		void SetConsLevel( int cons, int level );						// �ӎ����x����ݒ�
		void SetCons( int cons );										// ���݂̈ӎ���Ԃ�ݒ�
	};
}

#endif
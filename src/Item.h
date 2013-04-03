#ifndef INCLUDED_GAMEENGINE_ITEM_H
#define INCLUDED_GAMEENGINE_ITEM_H

#include <memory>
#include <bitset>

#include "CollisionObject.h"

#include "Math.hpp"

namespace GameEngine
{
	enum ItemID
	{
		ITEM_ID_CRYSTAL				= 0,		// �ӎ��̌���
		ITEM_ID_CONS_LEVEL_RECOVER	= 1,		// �ӎ����x����
		ITEM_ID_CONS_RECOVER		= 2,		// �ӎ���
		ITEM_ID_CONS_ENHANCE		= 3,		// �ӎ�����
		ITEM_ID_RECOVER				= 4,		// HP��
		ITEM_ID_POWER_UP			= 5,		// �ӎ����Œe����
		ITEM_ID_TOTAL,
	};

	enum ItemConsLevelRecoverSubID
	{
		ITEM_SUB_ID_GREEN			= 0,		// �΃Q�[�W
	};

	// �t���O�Ǘ�
	enum StatusFlag
	{
		STATUS_FLAG_OBTAINED		= 0,	// �擾���ꂽ
		STATUS_FLAG_CONSUMED		= 1,	// ����ꂽ
		STATUS_FLAG_TOTAL,
	};

	struct StageData;

	struct ItemData
	{
#if defined ( USE_FLOATING_POINT )
		float		m_PosX;			// �ʒu�iX���W�j
		float		m_PosY;			// �ʒu�iY���W�j
		float		m_Vel;			// ���x
		float		m_Angle;		// �p�x
		float		m_ColRadius;	// �Փ˔���̔��a
#elif defined ( USE_GAME_UNIT )
		struct GameUnitData
		{
			GameUnit		m_PosX;			// �ʒu�iX���W�j
			GameUnit		m_PosY;			// �ʒu�iY���W�j
			GameUnit		m_Vel;			// ���x
			GameUnit		m_Angle;		// �p�x
			GameUnit		m_ColRadius;	// �Փ˔���̔��a
		};
		GameUnitData	m_GUData;
#endif

		int			m_ItemID;		// �A�C�e�����ʎq
		int			m_ItemSubID;	// �A�C�e���T�u���ʎq
		bool		m_Near;			// �v���C���[���߂��ɂ���ꍇtrue
		int			m_Counter;		// �J�E���^
		int			m_ConsumedCounter;	// �G�����J�E���^
		StageData*	m_pStageData;
		std::bitset < STATUS_FLAG_TOTAL >	m_StatusFlags;		// ��ԊǗ��t���O
	};


	struct ResourceMap;
	class Item : public CollisionObject
	{
	private:
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		ItemData							m_ItemData;
		Player*								m_pPlayer;
	public:
		Item( std::shared_ptr < ResourceMap > pMap, StageData* pStageData, int id, int subID );
		~Item();
		void Draw();										// �`��
		bool Update();										// �X�V
		void Colided( CollisionObject* pObject );			// �Փˎ��̏��� �f�B�X�p�b�`��
		void ProcessCollision( Player* pPlayer );			// �Փˎ��̏����i�v���C���[�j
		void ProcessCollision( Enemy* pEnemy );				// �Փˎ��̏����i�G�j
		void ProcessCollision( PlayerShot* pPlayerShot );	// �Փˎ��̏����i�v���C���[�V���b�g�j
		void ProcessCollision( EnemyShot* pEnemyShot );		// �Փˎ��̏����i�G�e�j
		void ProcessCollision( Item* pItem );				// �Փˎ��̏����i�A�C�e���j
#if defined ( USE_FLOATING_POINT )
		void Init( float posX, float posY );				// ������
		void SetPos( float posX, float posY );				// �ʒu��ݒ�
		void GetPos( float* pPosX, float* pPosY );
		float GetCollisionRadius();
#elif defined ( USE_GAME_UNIT )
		void Init( const GameUnit& posX, const GameUnit& posY );				// ������
		void SetPos( const GameUnit& posX, const GameUnit& posY );				// �ʒu��ݒ�
		void GetPos( GameUnit* pPosX, GameUnit* pPosY );
		GameUnit GetCollisionRadius();
#endif
		int GetItemID() const;
		int GetItemSubID() const;
		void PlayerIsNear( Player* pPlayer );
		bool CanBeConsumed() const;
		void Consume();
	};
}

#endif
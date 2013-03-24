#ifndef INCLUDED_GAMEENGINE_ENEMYSHOTGROUP_H
#define INCLUDED_GAMEENGINE_ENEMYSHOTGROUP_H

#include <memory>



namespace GameEngine
{
	struct ResourceMap;
	struct EnemyData;
	struct StageData;
	class EnemyShot;
	class EnemyShotGroup;
	struct EnemyShotGroupScriptData;

	struct EnemyShotGroupData
	{
		EnemyShot*							m_pShots[ 1000 ];	// �V���b�g
		int									m_ShotTotal;		// �V���b�g��
		int									m_Counter;			// �J�E���^
		EnemyData*							m_pEnemyData;		// �G�f�[�^
		StageData*							m_pStageData;		// �X�e�[�W�f�[�^
		EnemyShotGroup*						m_pShotGroup;		// �V���b�g�O���[�v
		bool								m_IsNew;			// �V�������H
		bool								m_EnemyControlled;	// �G�̊Ǘ����ɂ��邩�H
		int									m_Reg;				// ���W�X�^
		float								m_FReg;				// ���������_���W�X�^
	};
	
	class EnemyShotGroup
	{
	private:
		class Impl;
		std::auto_ptr < EnemyShotGroup::Impl >		m_pImpl;
	public:
		EnemyShotGroup( std::shared_ptr < ResourceMap > pMap,
						std::shared_ptr < EnemyShotGroupScriptData > pData,
						int id,
						EnemyData* pEnemyData );
		~EnemyShotGroup();
		void Init();
		bool Update();
		int CreateShot();
		void DeleteShot( int id );
		void SetReg( int reg );
		void SetFReg( float reg );
		int GetReg() const;
		float GetFReg() const;
		void SetShotPos( int id, float x, float y );
		void SetShotAngle( int id, float angle );
		void SetShotSpeed( int id, float speed );
		void SetShotImage( int id, int imgID );
		void SetCollisionRadius( int id, float radius );
		void SetShotMovement( int id, float angle, float speed );
		void SetShotStatus( int id, float x, float y, float angle, float speed, float radius, int imgID );
		bool IsEmpty() const;			// �S�������ς݂��H
		void DetachEnemyControl();		// �G�̊Ǘ����łȂ��Ȃ�悤�ɂ���B
	};
}

#endif
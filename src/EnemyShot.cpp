#include <MAPIL/MAPIL.h>

#include "EnemyShot.h"
#include "ResourceTypes.h"
#include "EnemyShotGroup.h"
#include "GameObjectImplBase.h"

namespace GameEngine
{

	struct ShotGroupData
	{
		EnemyShotGroup*						m_pShotGroup;		// �������Ă���V���b�g�O���[�v
		int									m_ID;				// �V���b�g�O���[�vID
	};

	class EnemyShot::Impl : public GameObjectImplBase
	{
	private:
		std::shared_ptr < ResourceMap >		m_pResourceMap;		// ���\�[�X�Ǘ��f�[�^
		int									m_ShotID;			// �V���b�gID
		float								m_PosX;				// �ʒu�iX���W�j
		float								m_PosY;				// �ʒu�iY���W�j
		float								m_Angle;			// �p�x
		float								m_Speed;			// ���x
		float								m_ColRadius;		// �Փ˔���̔��a
		int									m_ImgID;			// �摜ID
		bool								m_Colided;			// �Փ˂������H
		int									m_Counter;			// �J�E���^
		ShotGroupData						m_ShotGroupData;	// �V���b�g�O���[�v�f�[�^
	public:
		Impl( std::shared_ptr < ResourceMap > pMap, int id );
		~Impl();
		void Draw();										// �`��
		bool Update();										// �X�V
		void GetPos( float* pX, float* pY );
		void SetPos( float posX, float posY );
		void SetAngle( float angle );						// �p�x��ݒ�
		void SetSpeed( float speed );						// ���x��ݒ�
		void SetImage( int id );							// �摜��ݒ�
		void SetCollisionRadius( float radius );			// �Փ˔���̔��a��ݒ�
		void JoinShotGroup( int id, EnemyShotGroup* pGroup );
		void ProcessCollision( Player* pPlayer );			// �Փˎ��̏����i�v���C���[�j
		float GetCollisionRadius() const;
	};

	EnemyShot::Impl::Impl( std::shared_ptr < ResourceMap > pMap, int id ) :	m_pResourceMap( pMap ),
																			m_ShotID( id ),
																			m_Colided( false )
	{
		m_Counter = 0;
		MAPIL::ZeroObject( &m_ShotGroupData, sizeof( m_ShotGroupData ) );
	}

	EnemyShot::Impl::~Impl()
	{
		if( m_ShotGroupData.m_pShotGroup ){
			m_ShotGroupData.m_pShotGroup->DeleteShot( m_ShotGroupData.m_ID );
		}
		MAPIL::ZeroObject( &m_ShotGroupData, sizeof( m_ShotGroupData ) );
	}

	void EnemyShot::Impl::Draw()
	{
		if( m_Counter >= 6 ){
			MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
								m_PosX, m_PosY, m_Angle + static_cast < float > ( MAPIL::DegToRad( 90.0f ) ) );
		}
		else{
			MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
								m_PosX, m_PosY,
								( 6 - m_Counter ) * 1.0f, ( 3 - m_Counter ) * 1.0f,
								m_Angle + static_cast < float > ( MAPIL::DegToRad( 90.0f ) ) );
		}
	}

	bool EnemyShot::Impl::Update()
	{
		if( m_Counter < 20 ){
			m_Speed -= 0.1f;
		}

		m_PosX += m_Speed * ::cos( m_Angle );
		m_PosY -= m_Speed * ::sin( m_Angle );


		if( m_PosX < 0.0f || m_PosX > 640.0f || m_PosY < -10.0f || m_PosY > 500.0f ){
			return false;
		}

		if( m_Colided ){
			return false;
		}

		++m_Counter;

		return true;
	}

	inline void EnemyShot::Impl::GetPos( float* pX, float* pY )
	{
		*pX = m_PosX;
		*pY = m_PosY;
	}

	inline void EnemyShot::Impl::SetPos( float posX, float posY )
	{
		m_PosX = posX;
		m_PosY = posY;
	}

	inline void EnemyShot::Impl::SetAngle( float angle )
	{
		m_Angle = angle;
	}

	inline void EnemyShot::Impl::SetSpeed( float speed )
	{
		m_Speed = speed + 2.0f;
	}

	inline void EnemyShot::Impl::SetImage( int id )
	{
		m_ImgID = id;
	}

	inline void EnemyShot::Impl::SetCollisionRadius( float radius )
	{
		m_ColRadius = radius;
	}

	inline void EnemyShot::Impl::ProcessCollision( Player* pPlayer )
	{
		m_Colided = true;
	}

	inline float EnemyShot::Impl::GetCollisionRadius() const
	{
		return m_ColRadius;
	}

	inline void EnemyShot::Impl::JoinShotGroup( int id, EnemyShotGroup* pGroup )
	{
		m_ShotGroupData.m_ID = id;
		m_ShotGroupData.m_pShotGroup = pGroup;
	}

	// ----------------------------------
	// �����N���X�̌Ăяo��
	// ----------------------------------

	EnemyShot::EnemyShot( std::shared_ptr < ResourceMap > pMap, int id ) :	CollisionObject(),
																			m_pImpl( new EnemyShot::Impl( pMap, id ) )
	{
	}

	EnemyShot::~EnemyShot()
	{
	}

	void EnemyShot::Init( float posX, float posY )
	{
	}

	void EnemyShot::Draw()
	{
		m_pImpl->Draw();
	}

	bool EnemyShot::Update()
	{
		return m_pImpl->Update();
	}

	void EnemyShot::SetPos( float posX, float posY )
	{
		m_pImpl->SetPos( posX, posY );
	}

	void EnemyShot::Colided( CollisionObject* pObject )
	{
		pObject->ProcessCollision( this );
	}

	void EnemyShot::ProcessCollision( Player* pPlayer )
	{
		m_pImpl->ProcessCollision( pPlayer );
	}

	void EnemyShot::ProcessCollision( Enemy* pEnemy )
	{
	}

	void EnemyShot::ProcessCollision( PlayerShot* pPlayerShot )
	{
	}

	void EnemyShot::ProcessCollision( EnemyShot* pEnemyShot )
	{
	}

	void EnemyShot::ProcessCollision( Item* pItem )
	{
	}

	void EnemyShot::GetPos( float* pPosX, float* pPosY )
	{
		m_pImpl->GetPos( pPosX, pPosY );
	}

	float EnemyShot::GetCollisionRadius()
	{
		return m_pImpl->GetCollisionRadius();
	}

	void EnemyShot::SetAngle( float angle )
	{
		m_pImpl->SetAngle( angle );
	}

	void EnemyShot::SetSpeed( float speed )
	{
		m_pImpl->SetSpeed( speed );
	}

	void EnemyShot::SetImage( int id )
	{
		m_pImpl->SetImage( id );
	}

	void EnemyShot::SetCollisionRadius( float radius )
	{
		m_pImpl->SetCollisionRadius( radius );
	}

	void EnemyShot::JoinShotGroup( int id, EnemyShotGroup* pGroup )
	{
		m_pImpl->JoinShotGroup( id, pGroup );
	}
}
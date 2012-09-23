#include <MAPIL/MAPIL.h>

#include "EnemyShot.h"

namespace GameEngine
{

	class EnemyShot::Impl
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
		void ProcessCollision( Player* pPlayer );			// �Փˎ��̏����i�v���C���[�j
		float GetCollisionRadius() const;
	};

	EnemyShot::Impl::Impl( std::shared_ptr < ResourceMap > pMap, int id ) :	m_pResourceMap( pMap ),
																			m_ShotID( id ),
																			m_Colided( false )
	{
	}

	EnemyShot::Impl::~Impl()
	{
	}

	void EnemyShot::Impl::Draw()
	{
		MAPIL::DrawString( m_PosX, m_PosY, "��" );
	}

	bool EnemyShot::Impl::Update()
	{
		m_PosX += m_Speed * ::cos( m_Angle );
		m_PosY -= m_Speed * ::sin( m_Angle );


		if( m_PosX < 0.0f || m_PosX > 640.0f || m_PosY < -10.0f || m_PosY > 500.0f ){
			return false;
		}

		if( m_Colided ){
			return false;
		}

		return true;
	}

	void EnemyShot::Impl::GetPos( float* pX, float* pY )
	{
		*pX = m_PosX;
		*pY = m_PosY;
	}

	void EnemyShot::Impl::SetPos( float posX, float posY )
	{
		m_PosX = posX;
		m_PosY = posY;
	}

	void EnemyShot::Impl::SetAngle( float angle )
	{
		m_Angle = angle;
	}
	void EnemyShot::Impl::SetSpeed( float speed )
	{
		m_Speed = speed;
	}

	void EnemyShot::Impl::SetImage( int id )
	{
		m_ImgID = id;
	}

	void EnemyShot::Impl::SetCollisionRadius( float radius )
	{
		m_ColRadius = radius;
	}

	void EnemyShot::Impl::ProcessCollision( Player* pPlayer )
	{
		m_Colided = true;
	}

	float EnemyShot::Impl::GetCollisionRadius() const
	{
		return m_ColRadius;
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
}
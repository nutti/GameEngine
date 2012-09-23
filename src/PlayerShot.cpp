#include <MAPIL/MAPIL.h>

#include "PlayerShot.h"
#include "Player.h"

namespace GameEngine
{

	class PlayerShot::Impl
	{
	private:
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		int									m_ShotID;
		float								m_PosX;
		float								m_PosY;
		float								m_ColRadius;
		bool								m_Colided;			// 衝突したか？
	public:
		Impl( std::shared_ptr < ResourceMap > pMap, int id );
		~Impl();
		void Draw();													// 描画
		bool Update();													// 更新
		void GetPos( float* pX, float* pY );
		void SetPos( const Player& player );
		void SetPos( float posX, float posY );
		void ProcessCollision( Enemy* pEnemy );							// 衝突時の処理（敵）
		float GetCollisionRadius() const;
	};

	PlayerShot::Impl::Impl( std::shared_ptr < ResourceMap > pMap, int id ) :	m_pResourceMap( pMap ),
																				m_ShotID( id ),
																				m_Colided( false )
	{
		m_ColRadius = 4.0f;
	}

	PlayerShot::Impl::~Impl()
	{
	}

	void PlayerShot::Impl::Draw()
	{
		MAPIL::DrawString( m_PosX, m_PosY, "○" );
	}

	bool PlayerShot::Impl::Update()
	{
		m_PosY -= 13.0f;

		if( m_PosY <= -10.0f ){
			return false;
		}

		if( m_Colided ){
			return false;
		}

		return true;
	}

	void PlayerShot::Impl::GetPos( float* pX, float* pY )
	{
		*pX = m_PosX;
		*pY = m_PosY;
	}

	void PlayerShot::Impl::SetPos( const Player& player )
	{
		//player.GetPos( &m_PosX, &m_PosY );
	}

	void PlayerShot::Impl::SetPos( float posX, float posY )
	{
		m_PosX = posX;
		m_PosY = posY;
	}

	void PlayerShot::Impl::ProcessCollision( Enemy* pEnemy )
	{
		m_Colided = true;
	}

	float PlayerShot::Impl::GetCollisionRadius() const
	{
		return m_ColRadius;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	PlayerShot::PlayerShot( std::shared_ptr < ResourceMap > pMap, int id ) :	CollisionObject(),
																				m_pImpl( new PlayerShot::Impl( pMap, id ) )
	{
	}

	PlayerShot::~PlayerShot()
	{
	}

	void PlayerShot::Init( float posX, float posY )
	{
	}

	void PlayerShot::Draw()
	{
		m_pImpl->Draw();
	}

	bool PlayerShot::Update()
	{
		return m_pImpl->Update();
	}

	void PlayerShot::SetPos( const Player& player )
	{
		m_pImpl->SetPos( player );
	}

	void PlayerShot::SetPos( float posX, float posY )
	{
		m_pImpl->SetPos( posX, posY );
	}

	void PlayerShot::Colided( CollisionObject* pObject )
	{
		pObject->ProcessCollision( this );
	}

	void PlayerShot::ProcessCollision( Player* pPlayer )
	{
	}

	void PlayerShot::ProcessCollision( Enemy* pEnemy )
	{
		m_pImpl->ProcessCollision( pEnemy );
	}

	void PlayerShot::ProcessCollision( PlayerShot* pPlayerShot )
	{
	}

	void PlayerShot::ProcessCollision( EnemyShot* pEnemyShot )
	{
	}

	void PlayerShot::ProcessCollision( Item* pItem )
	{
	}

	void PlayerShot::GetPos( float* pPosX, float* pPosY )
	{
		m_pImpl->GetPos( pPosX, pPosY );
	}

	float PlayerShot::GetCollisionRadius()
	{
		return m_pImpl->GetCollisionRadius();
	}
}
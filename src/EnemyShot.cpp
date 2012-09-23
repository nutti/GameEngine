#include <MAPIL/MAPIL.h>

#include "EnemyShot.h"

namespace GameEngine
{

	class EnemyShot::Impl
	{
	private:
		std::shared_ptr < ResourceMap >		m_pResourceMap;		// リソース管理データ
		int									m_ShotID;			// ショットID
		float								m_PosX;				// 位置（X座標）
		float								m_PosY;				// 位置（Y座標）
		float								m_Angle;			// 角度
		float								m_Speed;			// 速度
		float								m_ColRadius;		// 衝突判定の半径
		int									m_ImgID;			// 画像ID
		bool								m_Colided;			// 衝突したか？
	public:
		Impl( std::shared_ptr < ResourceMap > pMap, int id );
		~Impl();
		void Draw();										// 描画
		bool Update();										// 更新
		void GetPos( float* pX, float* pY );
		void SetPos( float posX, float posY );
		void SetAngle( float angle );						// 角度を設定
		void SetSpeed( float speed );						// 速度を設定
		void SetImage( int id );							// 画像を設定
		void SetCollisionRadius( float radius );			// 衝突判定の半径を設定
		void ProcessCollision( Player* pPlayer );			// 衝突時の処理（プレイヤー）
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
		MAPIL::DrawString( m_PosX, m_PosY, "●" );
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
	// 実装クラスの呼び出し
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
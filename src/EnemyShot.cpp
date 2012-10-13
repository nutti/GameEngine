#include <MAPIL/MAPIL.h>

#include "EnemyShot.h"
#include "ResourceTypes.h"
#include "EnemyShotGroup.h"
#include "GameObjectImplBase.h"

namespace GameEngine
{

	struct ShotGroupData
	{
		EnemyShotGroup*						m_pShotGroup;		// 所属しているショットグループ
		int									m_ID;				// ショットグループID
	};

	class EnemyShot::Impl : public GameObjectImplBase
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
		int									m_Counter;			// カウンタ
		ShotGroupData						m_ShotGroupData;	// ショットグループデータ
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
		void JoinShotGroup( int id, EnemyShotGroup* pGroup );
		void ProcessCollision( Player* pPlayer );			// 衝突時の処理（プレイヤー）
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

	void EnemyShot::JoinShotGroup( int id, EnemyShotGroup* pGroup )
	{
		m_pImpl->JoinShotGroup( id, pGroup );
	}
}
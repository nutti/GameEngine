#include <MAPIL/MAPIL.h>

#include "EnemyShotGroup.h"
#include "ResourceTypes.h"
#include "EnemyShot.h"
#include "Enemy.h"
#include "Stage.h"

namespace GameEngine
{

	class EnemyShotGroup::Impl
	{
	private:
		std::shared_ptr < ResourceMap >		m_pResourceMap;		// リソース管理データ
		EnemyShot*							m_pShots[ 1000 ];	// ショット
		int									m_ShotTotal;		// ショット数
		EnemyData*							m_pEnemyData;		// 敵データ
		StageData*							m_pStageData;		// ステージデータ
		EnemyShotGroup*						m_pShotGroup;		// ショットグループ
		bool								m_IsNew;			// 新しいか？
		bool								m_EnemyControlled;	// 敵の管理下にあるか？
	public:
		Impl( std::shared_ptr < ResourceMap > pMap, EnemyData* pEnemyData, EnemyShotGroup* pShotGroup );
		~Impl();
		int CreateShot();
		void DeleteShot( int id );
		void SetShotPos( int id, float x, float y );
		void SetShotAngle( int id, float angle );
		void SetShotSpeed( int id, float speed );
		void SetShotImage( int id, int imgID );
		void SetCollisionRadius( int id, float radius );
		void SetShotMovement( int id, float angle, float speed );
		void SetShotStatus( int id, float x, float y, float angle, float speed, float radius, int imgID );
		bool IsEmpty() const;
		void DetachEnemyControl();
	};

	EnemyShotGroup::Impl::Impl(	std::shared_ptr < ResourceMap > pMap,
								EnemyData* pEnemyData,
								EnemyShotGroup* pShotGroup ) :	m_pResourceMap( pMap ),
																m_pEnemyData( pEnemyData ),
																m_pStageData( pEnemyData->m_pStageData ),
																m_pShotGroup( pShotGroup )
	{
		m_ShotTotal = 0;
		m_IsNew = true;
		m_EnemyControlled = true;
		MAPIL::ZeroObject( m_pShots, sizeof( m_pShots ) );
	}

	EnemyShotGroup::Impl::~Impl()
	{
		m_ShotTotal = 0;
		MAPIL::ZeroObject( m_pShots, sizeof( m_pShots ) );
		m_pShotGroup = 0;
	}

	inline int EnemyShotGroup::Impl::CreateShot()
	{
		int id = m_ShotTotal++;
		m_IsNew = false;
		m_pShots[ id ] = m_pStageData->m_ObjBuilder.CreateEnemyShot( 0 );
		m_pShots[ id ]->JoinShotGroup( id, m_pShotGroup );
		m_pStageData->m_EnemyShotList.push_back( m_pShots[ id ] );
		return id;
	}

	inline void EnemyShotGroup::Impl::DeleteShot( int id )
	{
		m_pShots[ id ] = 0;
	}

	inline void EnemyShotGroup::Impl::SetShotPos( int id, float x, float y )
	{
		if( m_pShots[ id ] ){
			m_pShots[ id ]->SetPos( x, y );
		}
	}

	inline void EnemyShotGroup::Impl::SetShotAngle( int id, float angle )
	{
		if( m_pShots[ id ] ){
			m_pShots[ id ]->SetAngle( angle );
		}
	}

	inline void EnemyShotGroup::Impl::SetShotSpeed( int id, float speed )
	{
		if( m_pShots[ id ] ){
			m_pShots[ id ]->SetSpeed( speed );
		}
	}

	inline void EnemyShotGroup::Impl::SetShotImage( int id, int imgID )
	{
		if( m_pShots[ id ] ){
			m_pShots[ id ]->SetImage( imgID );
		}
	}

	inline void EnemyShotGroup::Impl::SetCollisionRadius( int id, float radius )
	{
		if( m_pShots[ id ] ){
			m_pShots[ id ]->SetCollisionRadius( radius );
		}
	}

	inline void EnemyShotGroup::Impl::SetShotMovement( int id, float angle, float speed )
	{
		if( m_pShots[ id ] ){
			m_pShots[ id ]->SetAngle( angle );
			m_pShots[ id ]->SetSpeed( speed );
		}
	}

	inline void EnemyShotGroup::Impl::SetShotStatus( int id, float x, float y, float angle, float speed, float radius, int imgID )
	{
		if( m_pShots[ id ] ){
			m_pShots[ id ]->SetPos( x, y );
			m_pShots[ id ]->SetAngle( angle );
			m_pShots[ id ]->SetSpeed( speed );
			m_pShots[ id ]->SetCollisionRadius( radius );
			m_pShots[ id ]->SetImage( imgID );
		}
	}

	inline bool EnemyShotGroup::Impl::IsEmpty() const
	{
		if( m_IsNew ){
			return false;
		}
		for( int i = 0; i < m_ShotTotal; ++i ){
			if( m_pShots[ i ] ){
				return false;
			}
		}
		return true;
	}

	inline void EnemyShotGroup::Impl::DetachEnemyControl()
	{
		m_EnemyControlled = false;
		m_pEnemyData = NULL;
		m_pStageData->m_EnemyShotGroupList.push_back( m_pShotGroup );
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	EnemyShotGroup::EnemyShotGroup( std::shared_ptr < ResourceMap > pMap, EnemyData* pEnemyData ) :
									m_pImpl( new EnemyShotGroup::Impl( pMap, pEnemyData, this ) )
	{
	}

	EnemyShotGroup::~EnemyShotGroup()
	{
	}

	int EnemyShotGroup::CreateShot()
	{
		return m_pImpl->CreateShot();
	}

	void EnemyShotGroup::DeleteShot( int id )
	{
		m_pImpl->DeleteShot( id );
	}

	void EnemyShotGroup::SetShotPos( int id, float x, float y )
	{
		m_pImpl->SetShotPos( id, x, y );
	}

	void EnemyShotGroup::SetShotAngle( int id, float angle )
	{
		m_pImpl->SetShotAngle( id, angle );
	}

	void EnemyShotGroup::SetShotSpeed( int id, float speed )
	{
		m_pImpl->SetShotSpeed( id, speed );
	}

	void EnemyShotGroup::SetShotImage( int id, int imgID )
	{
		m_pImpl->SetShotImage( id, imgID );
	}

	void EnemyShotGroup::SetCollisionRadius( int id, float radius )
	{
		m_pImpl->SetCollisionRadius( id, radius );
	}

	void EnemyShotGroup::SetShotMovement( int id, float angle, float speed )
	{
		m_pImpl->SetShotMovement( id, angle, speed );
	}

	void EnemyShotGroup::SetShotStatus( int id, float x, float y, float angle, float speed, float radius, int imgID )
	{
		m_pImpl->SetShotStatus( id, x, y, angle, speed, radius, imgID );
	}

	bool EnemyShotGroup::IsEmpty() const
	{
		return m_pImpl->IsEmpty();
	}

	void EnemyShotGroup::DetachEnemyControl()
	{
		m_pImpl->DetachEnemyControl();
	}
}
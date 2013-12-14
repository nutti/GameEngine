#include <MAPIL/MAPIL.h>

#include "EnemyShotGroup.h"
#include "ResourceTypes.h"
#include "GameObject/EnemyShot/EnemyShot.h"
#include "Enemy.h"
#include "Stage.h"

#include "GameObjectImplBase.h"

#include "EnemyShotGroupVCPU.h"

#include "ScriptTypes.h"

namespace GameEngine
{

	class EnemyShotGroup::Impl : public GameObjectImplBase
	{
	private:
		std::shared_ptr < ResourceMap >					m_pResourceMap;				// リソース管理データ
		EnemyShotGroupData								m_EnemyShotGroupData;		// ショットグループデータ
		EnemyShotGroupVCPU								m_VM;						// 仮想マシン
		std::shared_ptr < EnemyShotGroupScriptData >	m_pScriptData;				// スクリプトデータ
		int												m_ScriptID;					// スクリプトID
	public:
		Impl(	std::shared_ptr < ResourceMap > pMap,
				std::shared_ptr < EnemyShotGroupScriptData > pData,
				int id,
				EnemyData* pEnemyData,
				EnemyShotGroup* pShotGroup );
		~Impl();
		void Init();
		bool Update();
		int CreateShot();
		void DeleteShot( int id );
		void SetReg( int reg );
		void SetFReg( float reg );
		void SetGReg( int no, GameUnit reg );
		int GetReg() const;
		float GetFReg() const;
		GameUnit GetGReg( int no ) const;
		void SetShotPos( int id, float x, float y );
		void SetShotAngle( int id, float angle );
		void SetShotSpeed( int id, float speed );
		void SetShotImage( int id, int imgID );
		void SetCollisionRadius( int id, float radius );
		void SetShotMovement( int id, float angle, float speed );
		void SetShotStatus( int id, float x, float y, float angle, float speed, float radius, int imgID );
		void SendEvent( int id );
		int GetSubID() const;
		bool IsEmpty() const;
		void DetachEnemyControl();
		std::string GetMasterEnemyName() const;		// マスタとなる敵の名前を取得
		void SetMasterEnemyName( const std::string& name );
	};

	EnemyShotGroup::Impl::Impl(	std::shared_ptr < ResourceMap > pMap,
								std::shared_ptr < EnemyShotGroupScriptData > pData,
								int id,
								EnemyData* pEnemyData,
								EnemyShotGroup* pShotGroup ) :	m_pResourceMap( pMap ),
																m_pScriptData( pData ),
																m_ScriptID( id )
	{
		m_EnemyShotGroupData.m_pEnemyData = pEnemyData;
		m_EnemyShotGroupData.m_pStageData = pEnemyData->m_pStageData;
		m_EnemyShotGroupData.m_pShotGroup = pShotGroup;
		m_EnemyShotGroupData.m_Counter = 0;
		m_EnemyShotGroupData.m_ShotTotal = 0;
		m_EnemyShotGroupData.m_IsNew = true;
		m_EnemyShotGroupData.m_EnemyControlled = true;
		m_EnemyShotGroupData.m_Reg = 0;
		m_EnemyShotGroupData.m_FReg = 0.0f;
		m_EnemyShotGroupData.m_SubID = -1;
		MAPIL::ZeroObject( m_EnemyShotGroupData.m_pShots, sizeof( m_EnemyShotGroupData.m_pShots ) );
	}

	EnemyShotGroup::Impl::~Impl()
	{
		m_EnemyShotGroupData.m_ShotTotal = 0;
		MAPIL::ZeroObject( m_EnemyShotGroupData.m_pShots, sizeof( m_EnemyShotGroupData.m_pShots ) );
		m_EnemyShotGroupData.m_pShotGroup = 0;
	}

	void EnemyShotGroup::Impl::Init()
	{
		m_VM.Init( &m_pScriptData->m_pElm[ m_ScriptID ].m_Data, &m_EnemyShotGroupData );
	}

	bool EnemyShotGroup::Impl::Update()
	{
		if( m_VM.Terminated() ){
			return false;
		}
		m_VM.Run();

		++m_EnemyShotGroupData.m_Counter;

		return !IsEmpty();
	}

	inline int EnemyShotGroup::Impl::CreateShot()
	{
		int id = m_EnemyShotGroupData.m_ShotTotal++;
		m_EnemyShotGroupData.m_IsNew = false;
		m_EnemyShotGroupData.m_pShots[ id ] = m_EnemyShotGroupData.m_pStageData->m_ObjBuilder.CreateEnemyShot( 0 );
		m_EnemyShotGroupData.m_pShots[ id ]->JoinShotGroup( id, m_EnemyShotGroupData.m_pShotGroup );
		m_EnemyShotGroupData.m_pStageData->m_EnemyShotList.push_back( m_EnemyShotGroupData.m_pShots[ id ] );
		return id;
	}

	inline void EnemyShotGroup::Impl::DeleteShot( int id )
	{
		m_EnemyShotGroupData.m_pShots[ id ] = 0;
	}

	inline void EnemyShotGroup::Impl::SetReg( int reg )
	{
		m_EnemyShotGroupData.m_Reg = reg;
	}

	inline void EnemyShotGroup::Impl::SetFReg( float reg )
	{
		m_EnemyShotGroupData.m_FReg = reg;
	}

	inline void EnemyShotGroup::Impl::SetGReg( int no, GameUnit reg )
	{
		m_EnemyShotGroupData.m_GUData.m_Reg[ no ] = reg;
	}

	inline int EnemyShotGroup::Impl::GetReg() const
	{
		return m_EnemyShotGroupData.m_Reg;
	}

	inline float EnemyShotGroup::Impl::GetFReg() const
	{
		return m_EnemyShotGroupData.m_FReg;
	}

	inline GameUnit EnemyShotGroup::Impl::GetGReg( int no ) const
	{
		return m_EnemyShotGroupData.m_GUData.m_Reg[ no ];
	}

	inline void EnemyShotGroup::Impl::SetShotPos( int id, float x, float y )
	{
		if( m_EnemyShotGroupData.m_pShots[ id ] ){
			m_EnemyShotGroupData.m_pShots[ id ]->SetPos( x, y );
		}
	}

	inline void EnemyShotGroup::Impl::SetShotAngle( int id, float angle )
	{
		if( m_EnemyShotGroupData.m_pShots[ id ] ){
			m_EnemyShotGroupData.m_pShots[ id ]->SetAngle( angle );
		}
	}

	inline void EnemyShotGroup::Impl::SetShotSpeed( int id, float speed )
	{
		if( m_EnemyShotGroupData.m_pShots[ id ] ){
			m_EnemyShotGroupData.m_pShots[ id ]->SetSpeed( speed );
		}
	}

	inline void EnemyShotGroup::Impl::SetShotImage( int id, int imgID )
	{
		if( m_EnemyShotGroupData.m_pShots[ id ] ){
			m_EnemyShotGroupData.m_pShots[ id ]->SetImage( imgID );
		}
	}

	inline void EnemyShotGroup::Impl::SetCollisionRadius( int id, float radius )
	{
		if( m_EnemyShotGroupData.m_pShots[ id ] ){
			m_EnemyShotGroupData.m_pShots[ id ]->SetCollisionRadius( radius );
		}
	}

	inline void EnemyShotGroup::Impl::SetShotMovement( int id, float angle, float speed )
	{
		if( m_EnemyShotGroupData.m_pShots[ id ] ){
			m_EnemyShotGroupData.m_pShots[ id ]->SetAngle( angle );
			m_EnemyShotGroupData.m_pShots[ id ]->SetSpeed( speed );
		}
	}

	inline void EnemyShotGroup::Impl::SetShotStatus( int id, float x, float y, float angle, float speed, float radius, int imgID )
	{
		if( m_EnemyShotGroupData.m_pShots[ id ] ){
			m_EnemyShotGroupData.m_pShots[ id ]->SetPos( x, y );
			m_EnemyShotGroupData.m_pShots[ id ]->SetAngle( angle );
			m_EnemyShotGroupData.m_pShots[ id ]->SetSpeed( speed );
			m_EnemyShotGroupData.m_pShots[ id ]->SetCollisionRadius( radius );
			m_EnemyShotGroupData.m_pShots[ id ]->SetImage( imgID );
		}
	}

	inline void EnemyShotGroup::Impl::SendEvent( int id )
	{
		m_EnemyShotGroupData.m_EventQueue.push( id );
	}

	inline int EnemyShotGroup::Impl::GetSubID() const
	{
		return m_EnemyShotGroupData.m_SubID;
	}

	inline bool EnemyShotGroup::Impl::IsEmpty() const
	{
		if( m_EnemyShotGroupData.m_IsNew ){
			return false;
		}
		for( int i = 0; i < m_EnemyShotGroupData.m_ShotTotal; ++i ){
			if( m_EnemyShotGroupData.m_pShots[ i ] ){
				return false;
			}
		}
		return true;
	}

	inline void EnemyShotGroup::Impl::DetachEnemyControl()
	{
		m_EnemyShotGroupData.m_IsNew = false;
		m_EnemyShotGroupData.m_EnemyControlled = false;
		m_EnemyShotGroupData.m_pEnemyData = NULL;
		m_EnemyShotGroupData.m_pStageData->m_EnemyShotGroupList.push_back( m_EnemyShotGroupData.m_pShotGroup );
	}

	inline std::string EnemyShotGroup::Impl::GetMasterEnemyName() const
	{
		return m_EnemyShotGroupData.m_MasterEnemyName;
	}

	inline void EnemyShotGroup::Impl::SetMasterEnemyName( const std::string& name )
	{
		m_EnemyShotGroupData.m_MasterEnemyName = name;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	EnemyShotGroup::EnemyShotGroup( std::shared_ptr < ResourceMap > pMap,
									std::shared_ptr < EnemyShotGroupScriptData > pData,
									int id,
									EnemyData* pEnemyData ) :
									m_pImpl( new EnemyShotGroup::Impl( pMap, pData, id, pEnemyData, this ) )
	{
	}

	EnemyShotGroup::~EnemyShotGroup()
	{
	}

	void EnemyShotGroup::Init()
	{
		m_pImpl->Init();
	}

	bool EnemyShotGroup::Update()
	{
		return m_pImpl->Update();
	}

	int EnemyShotGroup::CreateShot()
	{
		return m_pImpl->CreateShot();
	}

	void EnemyShotGroup::DeleteShot( int id )
	{
		m_pImpl->DeleteShot( id );
	}

	void EnemyShotGroup::SetReg( int reg )
	{
		m_pImpl->SetReg( reg );
	}

	void EnemyShotGroup::SetFReg( float reg )
	{
		m_pImpl->SetFReg( reg );
	}

	void EnemyShotGroup::SetGReg( int no, GameUnit reg )
	{
		m_pImpl->SetGReg( no, reg );
	}

	int EnemyShotGroup::GetReg() const
	{
		return m_pImpl->GetReg();
	}

	float EnemyShotGroup::GetFReg() const
	{
		return m_pImpl->GetFReg();
	}

	GameUnit EnemyShotGroup::GetGReg( int no ) const
	{
		return m_pImpl->GetGReg( no );
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

	void EnemyShotGroup::SendEvent( int id )
	{
		m_pImpl->SendEvent( id );
	}

	int EnemyShotGroup::GetSubID() const
	{
		return m_pImpl->GetSubID();
	}

	bool EnemyShotGroup::IsEmpty() const
	{
		return m_pImpl->IsEmpty();
	}

	void EnemyShotGroup::DetachEnemyControl()
	{
		m_pImpl->DetachEnemyControl();
	}

	std::string EnemyShotGroup::GetMasterEnemyName() const
	{
		return m_pImpl->GetMasterEnemyName();
	}

	void EnemyShotGroup::SetMasterEnemyName( const std::string& name )
	{
		m_pImpl->SetMasterEnemyName( name );
	}
}
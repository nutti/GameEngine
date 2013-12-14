#include <MAPIL/MAPIL.h>

#include <bitset>

#include "EnemyShot.h"
#include "EnemyShotImpl.h"
#include "../../ResourceTypes.h"
#include "../../EnemyShotGroup.h"


#include "../../SpriteBatch.h"

namespace GameEngine
{

	// ----------------------------------
	// À‘•ƒNƒ‰ƒX‚ÌŒÄ‚Ño‚µ
	// ----------------------------------

	EnemyShot::EnemyShot( std::shared_ptr < ResourceMap > pMap, int id ) :	CollisionObject(),
																			m_pImpl( new EnemyShotImpl( pMap, id ) )
	{
	}

	EnemyShot::~EnemyShot()
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

	

	void EnemyShot::SetPower( int power )
	{
		m_pImpl->SetPower( power );
	}

	void EnemyShot::SetConsAttr( int attr )
	{
		m_pImpl->SetConsAttr( attr );
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

	

	int EnemyShot::GetPower() const
	{
		return m_pImpl->GetPower();
	}

	

	void EnemyShot::SetImage( int id )
	{
		m_pImpl->SetImage( id );
	}

	void EnemyShot::SetAtlasImage( int id )
	{
		m_pImpl->SetAtlasImage( id );
	}

	void EnemyShot::SetImageScale( float scale )
	{
		m_pImpl->SetImageScale( scale );
	}

	

	void EnemyShot::JoinShotGroup( int id, EnemyShotGroup* pGroup )
	{
		m_pImpl->JoinShotGroup( id, pGroup );
	}

	int EnemyShot::GetCounter() const
	{
		return m_pImpl->GetCounter();
	}



	void EnemyShot::PostMessage( int msgID )
	{
		m_pImpl->PostMessage( msgID );
	}

	bool EnemyShot::IsDead() const
	{
		return m_pImpl->IsDead();
	}

	int EnemyShot::GetConsAttr() const
	{
		return m_pImpl->GetConsAttr();
	}

	void EnemyShot::Pause()
	{
		m_pImpl->Pause();
	}

	void EnemyShot::Resume()
	{
		m_pImpl->Resume();
	}

	void EnemyShot::SetImgRotMode( int mode )
	{
		m_pImpl->SetImgRotMode( mode );
	}

	void EnemyShot::SetImgRotAnglePerFrame( float angle )
	{
		m_pImpl->SetImgRotAnglePerFrame( angle );
	}
	
	void EnemyShot::SetAlphaBlendingMode( int mode )
	{
		m_pImpl->SetAlphaBlendingMode( mode );
	}

	void EnemyShot::Init( const GameUnit& posX, const GameUnit& posY )
	{
	}

	void EnemyShot::SetPos( const GameUnit& posX, const GameUnit& posY )
	{
		m_pImpl->SetPos( posX, posY );
	}

	void EnemyShot::SetLinePos(	const GameUnit& x1,
								const GameUnit& y1,
								const GameUnit& x2,
								const GameUnit& y2,
								const GameUnit& thickness )
	{
		m_pImpl->SetLinePos( x1, y1, x2, y2, thickness );
	}

	void EnemyShot::GetPos( GameUnit* pPosX, GameUnit* pPosY )
	{
		m_pImpl->GetPos( pPosX, pPosY );
	}

	GameUnit EnemyShot::GetCollisionRadius()
	{
		return m_pImpl->GetCollisionRadius();
	}

	void EnemyShot::SetAngle( const GameUnit& angle )
	{
		m_pImpl->SetAngle( angle );
	}

	void EnemyShot::SetSpeed( const GameUnit& speed )
	{
		m_pImpl->SetSpeed( speed );
	}

	void EnemyShot::SetCollisionRadius( const GameUnit& radius )
	{
		m_pImpl->SetCollisionRadius( radius );
	}

	void EnemyShot::AddPos( const GameUnit& x, const GameUnit& y )
	{
		m_pImpl->AddPos( x, y );
	}

	void EnemyShot::AddAngle( const GameUnit& angle )
	{
		m_pImpl->AddAngle( angle );
	}

	void EnemyShot::AddSpeed( const GameUnit& speed )
	{
		m_pImpl->AddSpeed( speed );
	}

	bool EnemyShot::DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius )
	{
		return m_pImpl->DoesColideWithPlayer( x, y, radius );
	}
	
	GameUnit EnemyShot::GetAngle() const
	{
		return m_pImpl->GetAngle();
	}

	GameUnit EnemyShot::GetSpeed() const
	{
		return m_pImpl->GetSpeed();
	}

	void EnemyShot::SetShape( int shape )
	{
		m_pImpl->SetShape( shape );
	}

	

	void EnemyShot::EnableInvincibleMode()
	{
		m_pImpl->EnableInvincibleMode();
	}

	void EnemyShot::DisableInvincibleMode()
	{
		m_pImpl->DisableInvincibleMode();
	}

	void EnemyShot::EnableInvisibleMode()
	{
		m_pImpl->EnableInvisibleMode();
	}

	void EnemyShot::DisableInvisibleMode()
	{
		m_pImpl->DisableInvisibleMode();
	}

	std::string EnemyShot::GetMasterEnemyName() const
	{
		return m_pImpl->GetMasterEnemyName();
	}

	void EnemyShot::SetDrawingMultiplicity( int num )
	{
		m_pImpl->SetDrawingMultiplicity( num );
	}

	void EnemyShot::Delete( int by )
	{
		m_pImpl->Delete( by );
	}

	void EnemyShot::DeleteWhen( int when )
	{
		m_pImpl->DeleteWhen( when );
	}

	void EnemyShot::NotDeleteWhen( int when )
	{
		m_pImpl->NotDeleteWhen( when );
	}

	void EnemyShot::SetLength( const GameUnit& length )
	{
	}
}
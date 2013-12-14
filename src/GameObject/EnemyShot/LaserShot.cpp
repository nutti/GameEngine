#include <MAPIL/MAPIL.h>

#include <bitset>

#include "LaserShot.h"
#include "LaserShotImpl.h"
#include "../../ResourceTypes.h"


#include "../../SpriteBatch.h"

namespace GameEngine
{

	// ----------------------------------
	// À‘•ƒNƒ‰ƒX‚ÌŒÄ‚Ño‚µ
	// ----------------------------------

	LaserShot::LaserShot( std::shared_ptr < ResourceMap > pMap, int id ) :	EnemyShot( pMap, id ),
																			m_pImpl( new LaserShotImpl( pMap, id ) )
	{
	}

	LaserShot::~LaserShot()
	{
	}

	

	void LaserShot::Draw()
	{
		m_pImpl->Draw();
	}

	bool LaserShot::Update()
	{
		return m_pImpl->Update();
	}


	void LaserShot::Colided( CollisionObject* pObject )
	{
		pObject->ProcessCollision( this );
	}


	void LaserShot::Init( const GameUnit& posX, const GameUnit& posY )
	{
	}

	void LaserShot::SetPos( const GameUnit& posX, const GameUnit& posY )
	{
		m_pImpl->SetPos( posX, posY );
	}

	void LaserShot::SetLinePos(	const GameUnit& x1,
								const GameUnit& y1,
								const GameUnit& x2,
								const GameUnit& y2,
								const GameUnit& thickness )
	{
		m_pImpl->SetLinePos( x1, y1, x2, y2, thickness );
	}

	void LaserShot::GetPos( GameUnit* pPosX, GameUnit* pPosY )
	{
		m_pImpl->GetPos( pPosX, pPosY );
	}

	void LaserShot::AddPos( const GameUnit& x, const GameUnit& y )
	{
		m_pImpl->AddPos( x, y );
	}

	void LaserShot::SetLength( const GameUnit& length )
	{
		m_pImpl->SetLength( length );
	}

}
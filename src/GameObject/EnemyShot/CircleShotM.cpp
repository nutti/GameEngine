#include <MAPIL/MAPIL.h>

#include <bitset>

#include "CircleShotM.h"

#include "../../ResourceTypes.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int SHOT_TEX_ID	= 105;

	CircleShotM::CircleShotM( std::shared_ptr < ResourceMap > pMap, int id ) :	NoRotateShot( pMap, id )
	{
		m_GUData.m_ColRadiusBase = GameUnit( 2 );
		m_GUData.m_ColRadius = m_GUData.m_ColRadiusBase;
		m_Circle.SetRadius( m_GUData.m_ColRadius.GetFloat() );
	}

	CircleShotM::~CircleShotM()
	{
	}

	void CircleShotM::SetTextureColor( int color )
	{
		m_TexColor = color;
		m_AtlasImgID = SHOT_TEX_ID + m_TexColor;
	}
}
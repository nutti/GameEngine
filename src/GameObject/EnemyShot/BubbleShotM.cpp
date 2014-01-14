#include <MAPIL/MAPIL.h>

#include <bitset>

#include "BubbleShotM.h"

#include "../../ResourceTypes.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int SHOT_TEX_ID	= 161;

	BubbleShotM::BubbleShotM( std::shared_ptr < ResourceMap > pMap, int id ) :	NoRotateShot( pMap, id )
	{
		m_GUData.m_ColRadiusBase = GameUnit( 4 );
		m_GUData.m_ColRadius = m_GUData.m_ColRadiusBase;
		m_Circle.SetRadius( m_GUData.m_ColRadius.GetFloat() );
	}

	BubbleShotM::~BubbleShotM()
	{
	}

	void BubbleShotM::SetTextureColor( int color )
	{
		m_TexColor = color;
		m_AtlasImgID = SHOT_TEX_ID + m_TexColor;
	}
}
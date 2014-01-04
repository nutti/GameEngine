#include <MAPIL/MAPIL.h>

#include <bitset>

#include "HexagramShotM.h"

#include "../../ResourceTypes.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int FOUR_RAYED_STAR_SHOT_TEX_ID	= 81;

	HexagramShotM::HexagramShotM( std::shared_ptr < ResourceMap > pMap, int id ) :	RotateShot( pMap, id )
	{
		m_GUData.m_ColRadiusBase = GameUnit( 3 );
		m_GUData.m_ColRadius = m_GUData.m_ColRadiusBase;
		m_Circle.SetRadius( m_GUData.m_ColRadius.GetFloat() );
	}

	HexagramShotM::~HexagramShotM()
	{
	}

	void HexagramShotM::SetTextureColor( int color )
	{
		m_TexColor = color;
		m_AtlasImgID = FOUR_RAYED_STAR_SHOT_TEX_ID + m_TexColor;
	}
}
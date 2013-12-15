#include <MAPIL/MAPIL.h>

#include <bitset>

#include "FourRayedStarShot.h"

#include "../../ResourceTypes.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int FOUR_RAYED_STAR_SHOT_TEX_ID	= 48;

	FourRayedStarShot::FourRayedStarShot( std::shared_ptr < ResourceMap > pMap, int id ) :	NormalShot( pMap, id )
	{
		m_GUData.m_ColRadius = GameUnit( 3 );
	}

	FourRayedStarShot::~FourRayedStarShot()
	{
	}

	void FourRayedStarShot::SetTextureColor( int color )
	{
		m_TexColor = color;
		m_AtlasImgID = FOUR_RAYED_STAR_SHOT_TEX_ID + m_TexColor;
	}
}
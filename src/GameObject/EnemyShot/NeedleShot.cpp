#include <MAPIL/MAPIL.h>

#include <bitset>

#include "NeedleShot.h"

#include "../../ResourceTypes.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int NEEDLE_SHOT_TEX_ID	= 113;

	NeedleShot::NeedleShot( std::shared_ptr < ResourceMap > pMap, int id ) :	NormalShot( pMap, id )
	{
		m_GUData.m_ColRadius = GameUnit( 2 );
	}

	NeedleShot::~NeedleShot()
	{
	}

	void NeedleShot::SetTextureColor( int color )
	{
		m_TexColor = color;
		m_AtlasImgID = NEEDLE_SHOT_TEX_ID + m_TexColor;
	}
}
#include <MAPIL/MAPIL.h>

#include <bitset>

#include "CheeseShotM.h"

#include "../../ResourceTypes.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int SHOT_TEX_ID	= 89;

	CheeseShotM::CheeseShotM( std::shared_ptr < ResourceMap > pMap, int id ) :	NormalShot( pMap, id )
	{
		m_GUData.m_ColRadius = GameUnit( 3 );
	}

	CheeseShotM::~CheeseShotM()
	{
	}

	void CheeseShotM::SetTextureColor( int color )
	{
		m_TexColor = color;
		m_AtlasImgID = SHOT_TEX_ID + m_TexColor;
	}
}
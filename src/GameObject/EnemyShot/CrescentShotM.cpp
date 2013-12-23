#include <MAPIL/MAPIL.h>

#include <bitset>

#include "CrescentShotM.h"

#include "../../ResourceTypes.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int SHOT_TEX_ID	= 65;

	CrescentShotM::CrescentShotM( std::shared_ptr < ResourceMap > pMap, int id ) :	NormalShot( pMap, id )
	{
		m_GUData.m_ColRadius = GameUnit( 3 );
		m_Circle.SetRadius( m_GUData.m_ColRadius.GetFloat() );
	}

	CrescentShotM::~CrescentShotM()
	{
	}

	void CrescentShotM::SetTextureColor( int color )
	{
		m_TexColor = color;
		m_AtlasImgID = SHOT_TEX_ID + m_TexColor;
	}
}
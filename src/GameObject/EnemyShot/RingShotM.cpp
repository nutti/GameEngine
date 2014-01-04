#include <MAPIL/MAPIL.h>

#include <bitset>

#include "RingShotM.h"

#include "../../ResourceTypes.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int SHOT_TEX_ID	= 137;

	RingShotM::RingShotM( std::shared_ptr < ResourceMap > pMap, int id ) :	RotateShot( pMap, id )
	{
		m_AlphaBlendingMode = MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT;
		m_GUData.m_ColRadiusBase = GameUnit( 3 );
		m_GUData.m_ColRadius = m_GUData.m_ColRadiusBase;
		m_Circle.SetRadius( m_GUData.m_ColRadius.GetFloat() );
	}

	RingShotM::~RingShotM()
	{
	}

	void RingShotM::SetTextureColor( int color )
	{
		m_TexColor = color;
		m_AtlasImgID = SHOT_TEX_ID + m_TexColor;
	}
}
#include <MAPIL/MAPIL.h>

#include <bitset>

#include "GloriousCircleShotL.h"

#include "../../ResourceID.h"
#include "../../ResourceTypes.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int LAUNCH_EFFECT_TEX_ID		= 1000;
	static const int LAUNCH_EFFECT_SE_ID		= GLOBAL_RESOURCE_SE_ID_ENEMY_NORMAL_SHOT;
	static const int SHOT_TEX_ID				= 73;

	GloriousCircleShotL::GloriousCircleShotL( std::shared_ptr < ResourceMap > pMap, int id ) :	GloriousShot( pMap, id )
	{
		m_AlphaBlendingMode = MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT;
		m_ImgRotAngle = 0.0f;
		m_GUData.m_ColRadiusBase = GameUnit( 4 );
		m_GUData.m_ColRadius = m_GUData.m_ColRadiusBase;
		m_Circle.SetRadius( m_GUData.m_ColRadius.GetFloat() );
	}

	GloriousCircleShotL::~GloriousCircleShotL()
	{
	}


	void GloriousCircleShotL::SetTextureColor( int color )
	{
		m_TexColor = color;
		m_AtlasImgID = SHOT_TEX_ID + m_TexColor;
	}
}
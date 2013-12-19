#ifndef INCLUDED_GAMEENGINE_GLORIOUSCIRCLESHOTM_H
#define INCLUDED_GAMEENGINE_GLORIOUSCIRCLESHOTM_H

#include <memory>

#include "GloriousShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class GloriousCircleShotM : public GloriousShot
	{
	protected:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
	public:
		GloriousCircleShotM( std::shared_ptr < ResourceMap > pMap, int id );
		~GloriousCircleShotM();
		void SetTextureColor( int color );
	};
}

#endif
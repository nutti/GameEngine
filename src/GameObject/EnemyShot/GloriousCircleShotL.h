#ifndef INCLUDED_GAMEENGINE_GLORIOUSCIRCLESHOTL_H
#define INCLUDED_GAMEENGINE_GLORIOUSCIRCLESHOTL_H

#include <memory>

#include "GloriousShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class GloriousCircleShotL : public GloriousShot
	{
	protected:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
	public:
		GloriousCircleShotL( std::shared_ptr < ResourceMap > pMap, int id );
		~GloriousCircleShotL();
		void SetTextureColor( int color );
	};
}

#endif
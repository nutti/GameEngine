#ifndef INCLUDED_GAMEENGINE_FOURRAYEDSTARSHOT_H
#define INCLUDED_GAMEENGINE_FOURRAYEDSTARSHOT_H

#include <memory>

#include "NormalShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class FourRayedStarShot : public NormalShot
	{
	private:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
	public:
		FourRayedStarShot( std::shared_ptr < ResourceMap > pMap, int id );
		~FourRayedStarShot();
		virtual void SetTextureColor( int color );
	};
}

#endif
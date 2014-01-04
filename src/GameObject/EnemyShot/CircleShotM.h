#ifndef INCLUDED_GAMEENGINE_CIRCLESHOTM_H
#define INCLUDED_GAMEENGINE_CIRCLESHOTM_H

#include <memory>

#include "NoRotateShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class CircleShotM : public NoRotateShot
	{
	private:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
	public:
		CircleShotM( std::shared_ptr < ResourceMap > pMap, int id );
		~CircleShotM();
		void SetTextureColor( int color );
	};
}

#endif
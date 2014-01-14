#ifndef INCLUDED_GAMEENGINE_BUBBLESHOTM_H
#define INCLUDED_GAMEENGINE_BUBBLESHOTM_H

#include <memory>

#include "NoRotateShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class BubbleShotM : public NoRotateShot
	{
	private:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
	public:
		BubbleShotM( std::shared_ptr < ResourceMap > pMap, int id );
		~BubbleShotM();
		void SetTextureColor( int color );
	};
}

#endif
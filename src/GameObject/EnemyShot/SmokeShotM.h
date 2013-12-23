#ifndef INCLUDED_GAMEENGINE_SMOKESHOTM_H
#define INCLUDED_GAMEENGINE_SMOKESHOTM_H

#include <memory>

#include "RotateShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class SmokeShotM : public RotateShot
	{
	private:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
	public:
		SmokeShotM( std::shared_ptr < ResourceMap > pMap, int id );
		~SmokeShotM();
		void Draw();
		void SetTextureColor( int color );
	};
}

#endif
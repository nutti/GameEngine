#ifndef INCLUDED_GAMEENGINE_RINGSHOTM_H
#define INCLUDED_GAMEENGINE_RINGSHOTM_H

#include <memory>

#include "RotateShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class RingShotM : public RotateShot
	{
	private:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
	public:
		RingShotM( std::shared_ptr < ResourceMap > pMap, int id );
		~RingShotM();
		void SetTextureColor( int color );
	};
}

#endif
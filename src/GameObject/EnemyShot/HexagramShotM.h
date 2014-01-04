#ifndef INCLUDED_GAMEENGINE_HEXAGRAMSHOTM_H
#define INCLUDED_GAMEENGINE_HEXAGRAMSHOTM_H

#include <memory>

#include "RotateShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class HexagramShotM : public RotateShot
	{
	private:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
	public:
		HexagramShotM( std::shared_ptr < ResourceMap > pMap, int id );
		~HexagramShotM();
		virtual void SetTextureColor( int color );
	};
}

#endif
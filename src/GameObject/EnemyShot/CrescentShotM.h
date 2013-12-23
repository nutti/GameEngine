#ifndef INCLUDED_GAMEENGINE_CRESCENTSHOTM_H
#define INCLUDED_GAMEENGINE_CRESCENTSHOTM_H

#include <memory>

#include "NormalShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class CrescentShotM : public NormalShot
	{
	private:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
	public:
		CrescentShotM( std::shared_ptr < ResourceMap > pMap, int id );
		~CrescentShotM();
		void SetTextureColor( int color );
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_CHEESESHOTM_H
#define INCLUDED_GAMEENGINE_CHEESESHOTM_H

#include <memory>

#include "NormalShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class CheeseShotM : public NormalShot
	{
	private:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
	public:
		CheeseShotM( std::shared_ptr < ResourceMap > pMap, int id );
		~CheeseShotM();
		void SetTextureColor( int color );
	};
}

#endif
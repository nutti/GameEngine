#ifndef INCLUDED_GAMEENGINE_ELIPSESHOTS_H
#define INCLUDED_GAMEENGINE_ELIPSESHOTS_H

#include <memory>

#include "NormalShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class ElipseShotS : public NormalShot
	{
	private:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
	public:
		ElipseShotS( std::shared_ptr < ResourceMap > pMap, int id );
		~ElipseShotS();
		void SetTextureColor( int color );
	};
}

#endif
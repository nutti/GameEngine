#ifndef INCLUDED_GAMEENGINE_NEEDLESHOT_H
#define INCLUDED_GAMEENGINE_NEEDLESHOT_H

#include <memory>

#include "NormalShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class NeedleShot : public NormalShot
	{
	private:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
	public:
		NeedleShot( std::shared_ptr < ResourceMap > pMap, int id );
		~NeedleShot();
		void SetTextureColor( int color );
	};
}

#endif
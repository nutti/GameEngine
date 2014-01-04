#ifndef INCLUDED_GAMEENGINE_ROCKSHOTM_H
#define INCLUDED_GAMEENGINE_ROCKSHOTM_H

#include <memory>

#include "NoRotateShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class RockShotM : public NoRotateShot
	{
	private:
	public:
		RockShotM( std::shared_ptr < ResourceMap > pMap, int id );
		~RockShotM();
		void Draw();
		void SetTextureColor( int color );
	};
}

#endif
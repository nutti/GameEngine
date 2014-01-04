#ifndef INCLUDED_GAMEENGINE_NOROTATESHOT_H
#define INCLUDED_GAMEENGINE_NOROTATESHOT_H

#include <memory>

#include "EnemyShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class NoRotateShot : public EnemyShot
	{
	protected:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
		void DrawEffect();
	public:
		NoRotateShot( std::shared_ptr < ResourceMap > pMap, int id );
		virtual ~NoRotateShot();
		virtual void Draw();										// ï`âÊ
		bool Update();										// çXêV
		virtual void SetTextureColor( int color );
	};
}

#endif
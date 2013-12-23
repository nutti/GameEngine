#ifndef INCLUDED_GAMEENGINE_ROTATESHOT_H
#define INCLUDED_GAMEENGINE_ROTATESHOT_H

#include <memory>

#include "EnemyShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class RotateShot : public EnemyShot
	{
	protected:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
		void DrawEffect();
	public:
		RotateShot( std::shared_ptr < ResourceMap > pMap, int id );
		virtual ~RotateShot();
		virtual void Draw();										// ï`âÊ
		bool Update();										// çXêV
		virtual void SetTextureColor( int color );
	};
}

#endif
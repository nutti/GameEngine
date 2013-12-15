#ifndef INCLUDED_GAMEENGINE_NORMALSHOT_H
#define INCLUDED_GAMEENGINE_NORMALSHOT_H

#include <memory>

#include "EnemyShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class NormalShot : public EnemyShot
	{
	protected:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
		void DrawEffect();
	public:
		NormalShot( std::shared_ptr < ResourceMap > pMap, int id );
		virtual ~NormalShot();
		void Draw();										// ï`âÊ
		bool Update();										// çXêV
		virtual void SetTextureColor( int color );
	};
}

#endif
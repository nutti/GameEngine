#ifndef INCLUDED_GAMEENGINE_GLORIOUSSHOT_H
#define INCLUDED_GAMEENGINE_GLORIOUSSHOT_H

#include <memory>

#include "EnemyShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class GloriousShot : public EnemyShot
	{
	protected:
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
		void DrawEffect();
	public:
		GloriousShot( std::shared_ptr < ResourceMap > pMap, int id );
		virtual ~GloriousShot();
		void Draw();										// �`��
		bool Update();										// �X�V
		virtual void SetTextureColor( int color );
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_LASERSHOT_H
#define INCLUDED_GAMEENGINE_LASERSHOT_H

#include <memory>

#include "EnemyShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class LaserShot : public EnemyShot
	{
	private:
		GameUnit			m_Length;
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
		void DrawEffect();
	public:
		LaserShot( std::shared_ptr < ResourceMap > pMap, int id );
		~LaserShot();
		void Draw();										// 描画
		bool Update();										// 更新
		void Init( const GameUnit& posX, const GameUnit& posY );				// 初期化
		void SetLinePos(	const GameUnit& x1,
							const GameUnit& y1,
							const GameUnit& x2,
							const GameUnit& y2,
							const GameUnit& thickness );		// 線の値を設定
		bool DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius );
		void SetLength( const GameUnit& length );
	};
}

#endif
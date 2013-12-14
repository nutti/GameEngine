#ifndef INCLUDED_GAMEENGINE_LASERSHOTIMPL_H
#define INCLUDED_GAMEENGINE_LASERSHOTIMPL_H

#include <memory>

#include "../../CollisionObject.h"
#include "LaserShot.h"
#include "EnemyShotImpl.h"

namespace GameEngine
{

	class LaserShotImpl : public EnemyShotImpl
	{
	private:
		GameUnit			m_Length;
	public:
		LaserShotImpl( std::shared_ptr < ResourceMap > pMap, int id );
		~LaserShotImpl();
		void Draw();										// 描画
		bool Update();										// 更新
		void ProcessCollision( Player* pPlayer );			// 衝突時の処理（プレイヤー）
		void SetPos( const GameUnit& posX, const GameUnit& posY );
		void SetLinePos(	const GameUnit& x1,
							const GameUnit& y1,
							const GameUnit& x2,
							const GameUnit& y2,
							const GameUnit& thickness );		// 線の値を設定
		void GetPos( GameUnit* pX, GameUnit* pY );
		void AddPos( const GameUnit& x, const GameUnit& y );					// 位置を加算
		bool DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius );
		void SetLength( const GameUnit& length );
	};
}

#endif
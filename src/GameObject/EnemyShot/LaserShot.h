#ifndef INCLUDED_GAMEENGINE_LASERSHOT_H
#define INCLUDED_GAMEENGINE_LASERSHOT_H

#include <memory>

#include "EnemyShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class LaserShotImpl;
	class LaserShot : public EnemyShot
	{
	private:
		std::auto_ptr < LaserShotImpl >		m_pImpl;
	public:
		LaserShot( std::shared_ptr < ResourceMap > pMap, int id );
		~LaserShot();
		void Draw();										// 描画
		bool Update();										// 更新
		void Colided( CollisionObject* pObject );			// 衝突時の処理 ディスパッチャ
		void Init( const GameUnit& posX, const GameUnit& posY );				// 初期化
		void AddPos( const GameUnit& x, const GameUnit& y );					// 位置を加算
		void GetPos( GameUnit* pPosX, GameUnit* pPosY );
		void SetPos( const GameUnit& posX, const GameUnit& posY );				// 位置を設定
		void SetLinePos(	const GameUnit& x1,
							const GameUnit& y1,
							const GameUnit& x2,
							const GameUnit& y2,
							const GameUnit& thickness );		// 線の値を設定
		void SetLength( const GameUnit& length );
	};
}

#endif
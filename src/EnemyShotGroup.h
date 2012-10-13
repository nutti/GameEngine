#ifndef INCLUDED_GAMEENGINE_ENEMYSHOTGROUP_H
#define INCLUDED_GAMEENGINE_ENEMYSHOTGROUP_H

#include <memory>

namespace GameEngine
{

	struct ResourceMap;
	struct EnemyData;
	class EnemyShotGroup
	{
	private:
		class Impl;
		std::auto_ptr < EnemyShotGroup::Impl >		m_pImpl;
	public:
		EnemyShotGroup( std::shared_ptr < ResourceMap > pMap, EnemyData* pEnemyData );
		~EnemyShotGroup();
		int CreateShot();
		void DeleteShot( int id );
		void SetShotPos( int id, float x, float y );
		void SetShotAngle( int id, float angle );
		void SetShotSpeed( int id, float speed );
		void SetShotImage( int id, int imgID );
		void SetCollisionRadius( int id, float radius );
		void SetShotMovement( int id, float angle, float speed );
		void SetShotStatus( int id, float x, float y, float angle, float speed, float radius, int imgID );
		bool IsEmpty() const;			// ëSïîè¡ãéçœÇ›Ç©ÅH
		void DetachEnemyControl();		// ìGÇÃä«óùâ∫Ç≈Ç»Ç≠Ç»ÇÈÇÊÇ§Ç…Ç∑ÇÈÅB
	};
}

#endif
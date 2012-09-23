#ifndef INCLUDED_GAMEENGINE_GAMEOBJECTBUILDER_H
#define INCLUDED_GAMEENGINE_GAMEOBJECTBUILDER_H

#include <memory>

#include "SceneTypes.h"

namespace GameEngine
{
	class CollisionObject;
	class Player;
	class Enemy;
	class PlayerShot;
	class EnemyShot;
	class Item;
	class Effect;
	struct ResourceMap;
	struct ScriptData;
	struct StageData;
	class GameObjectBuilder
	{
	private:
		class Impl;
		std::auto_ptr < GameObjectBuilder::Impl >		m_pImpl;
	public:
		GameObjectBuilder();
		~GameObjectBuilder();
		CollisionObject* CreateCollisionObject( GameObjectID id );
		CollisionObject* CreateCollisionObject( GameObjectID id, int arg );
		Player* CreatePlayer();
		Enemy* CreateEnemy( int id );
		PlayerShot* CreatePlayerShot( int id );
		EnemyShot* CreateEnemyShot( int id );
		Effect* CreateEffect( int id );
		Item* CreateItem( int id );
		void AttachResourceMap( const ResourceMap& map );
		void AttachScriptData( const ScriptData& data );
		void AttachStageData( StageData* pData );
	};
}

#endif
#include "GameObjectBuilder.h"

#include "Player.h"
#include "Enemy.h"
#include "PlayerShot.h"
#include "EnemyShot.h"
#include "Item.h"
#include "Effect.h"
#include "EnemyShotGroup.h"

#include "ResourceTypes.h"
#include "ScriptTypes.h"
#include "Stage.h"

namespace GameEngine
{
	class GameObjectBuilder::Impl
	{
	private:
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		std::shared_ptr < ScriptData >		m_pScriptData;
		StageData*							m_pStageData;
	public:
		Impl();
		~Impl(){}
		CollisionObject* CreateCollisionObject( GameObjectID id );
		CollisionObject* CreateCollisionObject( GameObjectID id, int arg );
		Player* CreatePlayer();
		Enemy* CreateEnemy( int id );
		PlayerShot* CreatePlayerShot( int id );
		EnemyShot* CreateEnemyShot( int id );
		Effect* CreateEffect( int id, int subID );
		Item* CreateItem( int id, int subID );
		EnemyShotGroup* CreateEnemyShotGroup( int id, EnemyData* pData );
		void AttachResourceMap( const ResourceMap& map );
		void AttachScriptData( const ScriptData& data );
		void AttachStageData( StageData* pData );
	};

	GameObjectBuilder::Impl::Impl()
	{
	}

	CollisionObject* GameObjectBuilder::Impl::CreateCollisionObject( GameObjectID id )
	{
		switch( id ){
			case GAME_OBJECT_ID_PLAYER:
				return new Player( m_pResourceMap, m_pStageData );
				break;
			default:
				break;
		}

		return NULL;
	}

	CollisionObject* GameObjectBuilder::Impl::CreateCollisionObject( GameObjectID id, int arg )
	{
		switch( id ){
			case GAME_OBJECT_ID_ENEMY:
				return new Enemy( m_pResourceMap, m_pScriptData->m_pEnemyScriptData, arg, m_pStageData );
				break;
			case GAME_OBJECT_ID_PLAYER_SHOT:{
				PlayerShot* pNewShot = new PlayerShot( m_pResourceMap, arg );
				pNewShot->SetPos( *m_pStageData->m_pPlayer );
				return pNewShot;
				break;
			}
			default:
				break;
		}

		return NULL;
	}

	inline Player* GameObjectBuilder::Impl::CreatePlayer()
	{
		return new Player( m_pResourceMap, m_pStageData );
	}

	inline Enemy* GameObjectBuilder::Impl::CreateEnemy( int id )
	{
		return new Enemy( m_pResourceMap, m_pScriptData->m_pEnemyScriptData, id, m_pStageData );
	}

	inline PlayerShot* GameObjectBuilder::Impl::CreatePlayerShot( int id )
	{
		return new PlayerShot( m_pResourceMap, id );
	}

	inline EnemyShot* GameObjectBuilder::Impl::CreateEnemyShot( int id )
	{
		return new EnemyShot( m_pResourceMap, id );
	}

	inline Effect* GameObjectBuilder::Impl::CreateEffect( int id, int subID )
	{
		return new Effect( m_pResourceMap, id, subID );
	}

	inline Item* GameObjectBuilder::Impl::CreateItem( int id, int subID )
	{
		return new Item( m_pResourceMap, m_pStageData, id, subID );
	}

	inline EnemyShotGroup* GameObjectBuilder::Impl::CreateEnemyShotGroup( int id, EnemyData* pData )
	{
		return new EnemyShotGroup( m_pResourceMap, m_pScriptData->m_pEnemyShotGroupScriptData, id, pData );
	}

	inline void GameObjectBuilder::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_pResourceMap.reset( new ResourceMap );
		*m_pResourceMap = map;
	}

	inline void GameObjectBuilder::Impl::AttachScriptData( const ScriptData& data )
	{
		m_pScriptData.reset( new ScriptData );
		*m_pScriptData = data;
	}

	inline void GameObjectBuilder::Impl::AttachStageData( StageData* pData )
	{
		m_pStageData = pData;
	}

	// ----------------------------------
	// ŽÀ‘•ƒNƒ‰ƒX‚ÌŒÄ‚Ño‚µ
	// ----------------------------------

	GameObjectBuilder::GameObjectBuilder() : m_pImpl( new GameObjectBuilder::Impl )
	{
	}

	GameObjectBuilder::~GameObjectBuilder()
	{
	}

	CollisionObject* GameObjectBuilder::CreateCollisionObject( GameObjectID id )
	{
		return m_pImpl->CreateCollisionObject( id );
	}

	CollisionObject* GameObjectBuilder::CreateCollisionObject( GameObjectID id, int arg )
	{
		return m_pImpl->CreateCollisionObject( id, arg );
	}

	Player* GameObjectBuilder::CreatePlayer()
	{
		return m_pImpl->CreatePlayer();
	}

	Enemy* GameObjectBuilder::CreateEnemy( int id )
	{
		return m_pImpl->CreateEnemy( id );
	}

	PlayerShot* GameObjectBuilder::CreatePlayerShot( int id )
	{
		return m_pImpl->CreatePlayerShot( id );
	}

	EnemyShot* GameObjectBuilder::CreateEnemyShot( int id )
	{
		return m_pImpl->CreateEnemyShot( id );
	}

	Effect* GameObjectBuilder::CreateEffect( int id, int subID )
	{
		return m_pImpl->CreateEffect( id, subID );
	}

	Item* GameObjectBuilder::CreateItem( int id, int subID )
	{
		return m_pImpl->CreateItem( id, subID );
	}

	EnemyShotGroup* GameObjectBuilder::CreateEnemyShotGroup( int id, EnemyData* pData )
	{
		return m_pImpl->CreateEnemyShotGroup( id, pData );
	}

	void GameObjectBuilder::AttachResourceMap( const ResourceMap& map )
	{
		m_pImpl->AttachResourceMap( map );
	}

	void GameObjectBuilder::AttachScriptData( const ScriptData& data )
	{
		m_pImpl->AttachScriptData( data );
	}

	void GameObjectBuilder::AttachStageData( StageData* pData )
	{
		m_pImpl->AttachStageData( pData );
	}

	
}
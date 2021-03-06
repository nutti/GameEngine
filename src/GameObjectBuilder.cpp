#include "GameObjectBuilder.h"

#include "Player.h"
#include "Enemy.h"
#include "PlayerShot.h"
#include "GameObject/EnemyShot/EnemyShot.h"
#include "GameObject/EnemyShot/LaserShot.h"
#include "GameObject/EnemyShot/CircleShotM.h"
#include "GameObject/EnemyShot/FourRayedStarShot.h"
#include "GameObject/EnemyShot/ElipseShotS.h"
#include "GameObject/EnemyShot/CrescentShotM.h"
#include "GameObject/EnemyShot/CheeseShotM.h"
#include "GameObject/EnemyShot/GloriousCircleShotM.h"
#include "GameObject/EnemyShot/GloriousCircleShotL.h"
#include "GameObject/EnemyShot/NeedleShot.h"
#include "GameObject/EnemyShot/HexagramShotM.h"
#include "GameObject/EnemyShot/SmokeShotM.h"
#include "GameObject/EnemyShot/RingShotM.h"
#include "GameObject/EnemyShot/BeamShotM.h"
#include "GameObject/EnemyShot/RockShotM.h"
#include "GameObject/EnemyShot/BubbleShotM.h"
#include "Item.h"
#include "Effect.h"
#include "EnemyShotGroup.h"
#include "ScriptEffect.h"

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
		ScriptEffect* CreateScriptEffect( int id, std::weak_ptr < Enemy > pEnemy );
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
		switch( id ){
			case ENEMY_SHOT_ID_FOUR_RAYED_STAR_M:
				return new FourRayedStarShot( m_pResourceMap, id );
			case ENEMY_SHOT_ID_ELLIPSE_S:
				return new ElipseShotS( m_pResourceMap, id );
			case ENEMY_SHOT_ID_CRESCENT_M:
				return new CrescentShotM( m_pResourceMap, id );
			case ENEMY_SHOT_ID_GLORIOUS_CIRCLE_L:
				return new GloriousCircleShotL( m_pResourceMap, id );
			case ENEMY_SHOT_ID_HEXAGRAM_M:
				return new HexagramShotM( m_pResourceMap, id );
			case ENEMY_SHOT_ID_CHEESE_M:
				return new CheeseShotM( m_pResourceMap, id );
			case ENEMY_SHOT_ID_GLORIOUS_CIRCLE_M:
				return new GloriousCircleShotM( m_pResourceMap, id );
			case ENEMY_SHOT_ID_CIRCLE_M:
				return new CircleShotM( m_pResourceMap, id );
			case ENEMY_SHOT_ID_NEEDLE_M:
				return new NeedleShot( m_pResourceMap, id );
			case ENEMY_SHOT_ID_LASER_M:
				return new LaserShot( m_pResourceMap, id );
			case ENEMY_SHOT_ID_SMOKE_M:
				return new SmokeShotM( m_pResourceMap, id );
			case ENEMY_SHOT_ID_RING_M:
				return new RingShotM( m_pResourceMap, id );
			case ENEMY_SHOT_ID_BEAM_M:
				return new BeamShotM( m_pResourceMap, id );
			case ENEMY_SHOT_ID_ROCK_M:
				return new RockShotM( m_pResourceMap, id );
			case ENEMY_SHOT_ID_BUBBLE_M:
				return new BubbleShotM( m_pResourceMap, id );
			default:
				return new EnemyShot( m_pResourceMap, id );
		}
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

	inline ScriptEffect* GameObjectBuilder::Impl::CreateScriptEffect( int id, std::weak_ptr < Enemy > pEnemy )
	{
		return new ScriptEffect( m_pResourceMap, m_pScriptData->m_pEffectScriptData, pEnemy, id );
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
	// 実装クラスの呼び出し
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

	ScriptEffect* GameObjectBuilder::CreateScriptEffect( int id, std::weak_ptr < Enemy > pEnemy )
	{
		return m_pImpl->CreateScriptEffect( id, pEnemy );
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
#include <MAPIL/MAPIL.h>

#include "Player.h"
#include "PlayerShot.h"
#include "EnemyShot.h"
#include "Enemy.h"
#include "ResourceTypes.h"
#include "Util.h"
#include "Item.h"
#include "PlayerOption.h"

#include "GameObjectImplBase.h"

#include "Stage.h"

#include "ResourceID.h"

namespace GameEngine
{

	class Player::Impl : public GameObjectImplBase
	{
	private:
		const int	INVINCIBLE_TIME;

		ButtonStatusHolder					m_ButtonStatus;
		PlayerData							m_Data;
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		StageData*							m_pStageData;

		std::list < PlayerOption* >			m_PlayerOptList;
		
		void NormalModeShot();
		void GreenModeShot();
		void BlueModeShot();
		void RedModeShot();

		void GreenModeBomb();
		void BlueModeBomb();
		void RedModeBomb();

		void AddOpt();
		
		void Move();
		void ChangeMode();
		void UpdateCons();
	public:
		Impl( std::shared_ptr < ResourceMap > pMap, StageData* pStageData );
		~Impl();
		void AttachButtonState( const ButtonStatusHolder& holder );		// キー入力を設定
		void Draw();													// 描画
		bool Update();													// 更新
		void ProcessCollision( Enemy* pEnemy );							// 衝突時の処理（敵）
		void ProcessCollision( EnemyShot* pEnemyShot );					// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );
#if defined ( USE_FLOATING_POINT )
		void GetPos( float* pX, float* pY );
		float GetCollisionRadius() const;
#elif defined ( USE_GAME_UNIT )
		void GetPos( GameUnit* pPosX, GameUnit* pPosY );						// 位置を取得
		GameUnit GetCollisionRadius();										// 衝突半径を取得
#endif
		int GetHP() const;												// HPを取得
		int GetConsGauge( int cons ) const;								// 意識ゲージの取得
		int GetConsLevel( int cons ) const;								// 意識レベルの取得
		int GetShotPower() const;
		int GetCurCons() const;

		void SetPos( const GameUnit& posX, const GameUnit& posY );		// 位置を設定
		void SetHP( int hp );											// HPを設定
		void SetShotPower( int power );									// ショットの威力を設定
		void SetConsGauge( int cons, int val );							// 意識ゲージを設定
		void SetConsLevel( int cons, int level );						// 意識レベルを設定
		void SetCons( int cons );										// 現在の意識状態を設定
	};

	Player::Impl::Impl( std::shared_ptr < ResourceMap > pMap, StageData* pStageData ) :	GameObjectImplBase(),
																						m_pResourceMap( pMap ),
																						m_pStageData( pStageData ),
																						INVINCIBLE_TIME( 240 )
	{
		MAPIL::ZeroObject( &m_Data, sizeof( m_Data ) );

#if defined ( USE_FLOATING_POINT )
		m_Data.m_PosX = 300.0f;
		m_Data.m_PosY = 400.0f;
		m_Data.m_ColRadius = 2.0f;
#elif defined ( USE_GAME_UNIT )
		m_Data.m_GUData.m_PosX = 300;
		m_Data.m_GUData.m_PosY = 400;
		m_Data.m_GUData.m_ColRadius = 2;
#endif
		
		m_Data.m_HP = 10;
		m_Data.m_ConsGauge[ 0 ] = m_Data.m_ConsGauge[ 1 ] = m_Data.m_ConsGauge[ 2 ] = 1000;
		m_Data.m_ConsLevel[ 0 ] = m_Data.m_ConsLevel[ 1 ] = m_Data.m_ConsLevel[ 2 ] = 1000;
		m_Data.m_ShotPower = 0;
		m_Data.m_Counter = 0;
		m_Data.m_ConsCur = PLAYER_CONS_MODE_NORMAL;
#if defined ( MAKE_MODE_RELEASE )
		m_Data.m_RestInvincibleTime = 0;
#else
		m_Data.m_RestInvincibleTime = 10000;
		//m_Data.m_RestInvincibleTime = 0;
#endif

		m_PlayerOptList.clear();
	}

	Player::Impl::~Impl()
	{
		MAPIL::ZeroObject( &m_Data, sizeof( m_Data ) );
		m_PlayerOptList.clear();
	}

#if defined ( USE_FLOATING_POINT )
	void Player::Impl::NormalModeShot()
	{
		if( ( m_Data.m_Counter % 3 ) == 0 ){
			PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
			pNewShot->SetPos( m_Data.m_PosX - 7.0f, m_Data.m_PosY + 5.0f );
			pNewShot->SetShotPower( 3 );
			m_pStageData->m_PlayerShotList.push_back( pNewShot );
			pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
			pNewShot->SetPos( m_Data.m_PosX + 7.0f, m_Data.m_PosY + 5.0f );
			pNewShot->SetShotPower( 3 );
			m_pStageData->m_PlayerShotList.push_back( pNewShot );
			if( m_Data.m_ShotPower >= 10 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
				pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
				pNewShot->SetShotPower( 2 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			if( m_Data.m_ShotPower >= 20 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
				pNewShot->SetPos( m_Data.m_PosX - 14.0f, m_Data.m_PosY + 2.0f );
				pNewShot->SetShotPower( 2 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
				pNewShot->SetPos( m_Data.m_PosX + 14.0f, m_Data.m_PosY + 2.0f );
				pNewShot->SetShotPower( 2 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			if( m_Data.m_ShotPower >= 30 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
				pNewShot->SetPos( m_Data.m_PosX - 21.0f, m_Data.m_PosY + 1.0f );
				pNewShot->SetShotPower( 2 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
				pNewShot->SetPos( m_Data.m_PosX + 21.0f, m_Data.m_PosY + 1.0f );
				pNewShot->SetShotPower( 2 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_SHOT_SE ] );
		}
	}
#elif defined ( USE_GAME_UNIT )
	void Player::Impl::NormalModeShot()
	{
		if( ( m_Data.m_Counter % 3 ) == 0 ){
			PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
			pNewShot->SetPos( m_Data.m_GUData.m_PosX - GameUnit( 7 ), m_Data.m_GUData.m_PosY + GameUnit( 5 ) );
			pNewShot->SetShotPower( 3 );
			m_pStageData->m_PlayerShotList.push_back( pNewShot );
			pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
			pNewShot->SetPos( m_Data.m_GUData.m_PosX + GameUnit( 7 ), m_Data.m_GUData.m_PosY + GameUnit( 5 ) );
			pNewShot->SetShotPower( 3 );
			m_pStageData->m_PlayerShotList.push_back( pNewShot );
			if( m_Data.m_ShotPower >= 10 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
				pNewShot->SetPos( m_Data.m_GUData.m_PosX, m_Data.m_GUData.m_PosY + GameUnit( 3 ) );
				pNewShot->SetShotPower( 2 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			if( m_Data.m_ShotPower >= 20 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
				pNewShot->SetPos( m_Data.m_GUData.m_PosX - GameUnit( 14 ), m_Data.m_GUData.m_PosY + GameUnit( 2 ) );
				pNewShot->SetShotPower( 2 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
				pNewShot->SetPos( m_Data.m_GUData.m_PosX + GameUnit( 14 ), m_Data.m_GUData.m_PosY + GameUnit( 2 ) );
				pNewShot->SetShotPower( 2 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			if( m_Data.m_ShotPower >= 30 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
				pNewShot->SetPos( m_Data.m_GUData.m_PosX - GameUnit( 21 ), m_Data.m_GUData.m_PosY + GameUnit( 1 ) );
				pNewShot->SetShotPower( 2 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
				pNewShot->SetPos( m_Data.m_GUData.m_PosX + GameUnit( 21 ), m_Data.m_GUData.m_PosY + GameUnit( 1 ) );
				pNewShot->SetShotPower( 2 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_SHOT_SE ] );
		}
	}
#endif

#if defined ( USE_FLOATING_POINT )

	void Player::Impl::GreenModeShot()
	{
		if( ( m_Data.m_Counter % 3 ) == 0 ){
			PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
			pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
			pNewShot->SetAngle( MAPIL::DegToRad( 90.0f ) );
			pNewShot->SetSpeed( 15.0f );
			pNewShot->SetShotPower( 5 );
			pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
			m_pStageData->m_PlayerShotList.push_back( pNewShot );
			pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
			pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
			pNewShot->SetSpeed( 15.0f );
			pNewShot->SetAngle( MAPIL::DegToRad( 10.0f + 90.0f ) );
			pNewShot->SetShotPower( 5 );
			pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
			m_pStageData->m_PlayerShotList.push_back( pNewShot );
			pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
			pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
			pNewShot->SetSpeed( 15.0f );
			pNewShot->SetAngle( MAPIL::DegToRad( -10.0f + 90.0f ) );
			pNewShot->SetShotPower( 5 );
			pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
			m_pStageData->m_PlayerShotList.push_back( pNewShot );
			if( m_Data.m_ShotPower >= 10 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
				pNewShot->SetSpeed( 15.0f );
				pNewShot->SetAngle( MAPIL::DegToRad( 20.0f + 90.0f ) );
				pNewShot->SetShotPower( 5 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
				pNewShot->SetSpeed( 15.0f );
				pNewShot->SetAngle( MAPIL::DegToRad( -20.0f + 90.0f ) );
				pNewShot->SetShotPower( 5 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			if( m_Data.m_ShotPower >= 20 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_PosX , m_Data.m_PosY + 3.0f );
				pNewShot->SetSpeed( 15.0f );
				pNewShot->SetAngle( MAPIL::DegToRad( 30.0f + 90.0f ) );
				pNewShot->SetShotPower( 5 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
				pNewShot->SetSpeed( 15.0f );
				pNewShot->SetAngle( MAPIL::DegToRad( -30.0f + 90.0f ) );
				pNewShot->SetShotPower( 5 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			if( m_Data.m_ShotPower >= 30 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
				pNewShot->SetSpeed( 15.0f );
				pNewShot->SetAngle( MAPIL::DegToRad( 40.0f + 90.0f ) );
				pNewShot->SetShotPower( 5 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
				pNewShot->SetSpeed( 15.0f );
				pNewShot->SetAngle( MAPIL::DegToRad( -40.0f + 90.0f ) );
				pNewShot->SetShotPower( 5 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_SHOT_SE ] );
		}
	}

#elif defined ( USE_GAME_UNIT )

	void Player::Impl::GreenModeShot()
	{
		if( ( m_Data.m_Counter % 3 ) == 0 ){
			PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
			pNewShot->SetPos( m_Data.m_GUData.m_PosX, m_Data.m_GUData.m_PosY + GameUnit( 3 ) );
			pNewShot->SetAngle( GameUnit( 90 ) );
			pNewShot->SetSpeed( GameUnit( 15 ) );
			pNewShot->SetShotPower( 5 );
			pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
			m_pStageData->m_PlayerShotList.push_back( pNewShot );
			pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
			pNewShot->SetPos( m_Data.m_GUData.m_PosX, m_Data.m_GUData.m_PosY + GameUnit( 3 ) );
			pNewShot->SetSpeed( GameUnit( 15 ) );
			pNewShot->SetAngle( GameUnit( 90 + 10 ) );
			pNewShot->SetShotPower( 5 );
			pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
			m_pStageData->m_PlayerShotList.push_back( pNewShot );
			pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
			pNewShot->SetPos( m_Data.m_GUData.m_PosX, m_Data.m_GUData.m_PosY + GameUnit( 3 ) );
			pNewShot->SetSpeed( GameUnit( 15 ) );
			pNewShot->SetAngle( GameUnit( 90 - 10 ) );
			pNewShot->SetShotPower( 5 );
			pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
			m_pStageData->m_PlayerShotList.push_back( pNewShot );
			if( m_Data.m_ShotPower >= 10 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_GUData.m_PosX, m_Data.m_GUData.m_PosY + GameUnit( 3 ) );
				pNewShot->SetSpeed( GameUnit( 15 ) );
				pNewShot->SetAngle( GameUnit( 90 + 20 ) );
				pNewShot->SetShotPower( 5 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_GUData.m_PosX, m_Data.m_GUData.m_PosY + GameUnit( 3 ) );
				pNewShot->SetSpeed( GameUnit( 15 ) );
				pNewShot->SetAngle( GameUnit( 90 - 20 ) );
				pNewShot->SetShotPower( 5 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			if( m_Data.m_ShotPower >= 20 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_GUData.m_PosX , m_Data.m_GUData.m_PosY + GameUnit( 3 ) );
				pNewShot->SetSpeed( GameUnit( 15 ) );
				pNewShot->SetAngle( GameUnit( 90 + 30 ) );
				pNewShot->SetShotPower( 5 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_GUData.m_PosX, m_Data.m_GUData.m_PosY + GameUnit( 3 ) );
				pNewShot->SetSpeed( GameUnit( 15 ) );
				pNewShot->SetAngle( GameUnit( 90 - 30 ) );
				pNewShot->SetShotPower( 5 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			if( m_Data.m_ShotPower >= 30 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_GUData.m_PosX, m_Data.m_GUData.m_PosY + GameUnit( 3 ) );
				pNewShot->SetSpeed( GameUnit( 15 ) );
				pNewShot->SetAngle( GameUnit( 90 + 40 ) );
				pNewShot->SetShotPower( 5 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_GUData.m_PosX, m_Data.m_GUData.m_PosY + GameUnit( 3 ) );
				pNewShot->SetSpeed( GameUnit( 15 ) );
				pNewShot->SetAngle( GameUnit( 90 - 40 ) );
				pNewShot->SetShotPower( 5 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_GREEN );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_SHOT_SE ] );
		}
	}

#endif

#if defined ( USE_FLOATING_POINT )
	void Player::Impl::BlueModeShot()
	{
		PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 2 );
		pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY );
		pNewShot->SetShotPower( 2 + m_Data.m_ShotPower / 10 );
		pNewShot->SetPlayer( m_pStageData->m_pPlayer );
		pNewShot->SetConsAttr( PLAYER_CONS_MODE_BLUE );
		m_pStageData->m_PlayerShotList.push_back( pNewShot );
		pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 2 );
		pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 8.0f );
		pNewShot->SetShotPower( 2 + m_Data.m_ShotPower / 10 );
		pNewShot->SetPlayer( m_pStageData->m_pPlayer );
		pNewShot->SetConsAttr( PLAYER_CONS_MODE_BLUE );
		m_pStageData->m_PlayerShotList.push_back( pNewShot );
		MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_SHOT_SE ] );
	}
#elif defined ( USE_GAME_UNIT )
	void Player::Impl::BlueModeShot()
	{
		PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 2 );
		pNewShot->SetPos( m_Data.m_GUData.m_PosX, m_Data.m_GUData.m_PosY );
		pNewShot->SetShotPower( 2 + m_Data.m_ShotPower / 10 );
		pNewShot->SetPlayer( m_pStageData->m_pPlayer );
		pNewShot->SetConsAttr( PLAYER_CONS_MODE_BLUE );
		m_pStageData->m_PlayerShotList.push_back( pNewShot );
		pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 2 );
		pNewShot->SetPos( m_Data.m_GUData.m_PosX, m_Data.m_GUData.m_PosY + 8.0f );
		pNewShot->SetShotPower( 2 + m_Data.m_ShotPower / 10 );
		pNewShot->SetPlayer( m_pStageData->m_pPlayer );
		pNewShot->SetConsAttr( PLAYER_CONS_MODE_BLUE );
		m_pStageData->m_PlayerShotList.push_back( pNewShot );
		MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_SHOT_SE ] );
	}
#endif

#if defined ( USE_FLOATING_POINT )
	void Player::Impl::RedModeShot()
	{
		if( ( m_Data.m_Counter % ( 12 - 2 * ( m_Data.m_ShotPower / 10 ) ) ) == 0 ){
			for( int i = 0; i < 36; ++i ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 3 );
				pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY );
				pNewShot->SetAngle( MAPIL::DegToRad( i * 10.0f ) );
				pNewShot->SetSpeed( 10.0f );
				pNewShot->SetShotPower( 10 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_RED );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_SHOT_SE ] );	
		}
	}
#elif defined ( USE_GAME_UNIT )
	void Player::Impl::RedModeShot()
	{
		if( ( m_Data.m_Counter % ( 12 - 2 * ( m_Data.m_ShotPower / 10 ) ) ) == 0 ){
			for( int i = 0; i < 36; ++i ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 3 );
				pNewShot->SetPos( m_Data.m_GUData.m_PosX, m_Data.m_GUData.m_PosY );
				pNewShot->SetAngle( GameUnit( i * 10 ) );
				pNewShot->SetSpeed( GameUnit( 10 ) );
				pNewShot->SetShotPower( 10 );
				pNewShot->SetConsAttr( PLAYER_CONS_MODE_RED );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
			}
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_SHOT_SE ] );	
		}
	}
#endif

#if defined ( USE_FLOATING_POINT )
	void Player::Impl::GreenModeBomb()
	{
		// 無敵時間の設定
		m_Data.m_RestInvincibleTime = 180;

		// メッセージ構築
		StageMessage msg;
		// ボム発動メッセージ
		msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_PLAYER_BOMBED;
		// ボムの属性
		StageMessage::StageMessageData data;
		data.m_Integer = PLAYER_CONS_MODE_GREEN;
		msg.m_MsgDataList.push_back( data );
		// プレイヤーの位置
		data.m_Float = m_Data.m_PosX;
		msg.m_MsgDataList.push_back( data );
		data.m_Float = m_Data.m_PosY;
		msg.m_MsgDataList.push_back( data );
		// ボムの持続時間
		data.m_Integer = m_Data.m_RestInvincibleTime;
		msg.m_MsgDataList.push_back( data );
		// メッセージ送信
		m_pStageData->m_MsgQueue.push( msg );
	}
#elif defined ( USE_GAME_UNIT )
	void Player::Impl::GreenModeBomb()
	{
		// 無敵時間の設定
		m_Data.m_RestInvincibleTime = 180;

		// メッセージ構築
		StageMessage msg;
		// ボム発動メッセージ
		msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_PLAYER_BOMBED;
		// ボムの属性
		StageMessage::StageMessageData data;
		data.m_Integer = PLAYER_CONS_MODE_GREEN;
		msg.m_MsgDataList.push_back( data );
		// プレイヤーの位置
		data.m_Float = m_Data.m_GUData.m_PosX.GetFloat();
		msg.m_MsgDataList.push_back( data );
		data.m_Float = m_Data.m_GUData.m_PosY.GetFloat();
		msg.m_MsgDataList.push_back( data );
		// ボムの持続時間
		data.m_Integer = m_Data.m_RestInvincibleTime;
		msg.m_MsgDataList.push_back( data );
		// メッセージ送信
		m_pStageData->m_MsgQueue.push( msg );
	}
#endif

#if defined ( USE_FLOATING_POINT )
	void Player::Impl::BlueModeBomb()
	{
		// 無敵時間の設定
		m_Data.m_RestInvincibleTime = 120;

		// メッセージ構築
		StageMessage msg;
		// ボム発動メッセージ
		msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_PLAYER_BOMBED;
		// ボムの属性
		StageMessage::StageMessageData data;
		data.m_Integer = PLAYER_CONS_MODE_BLUE;
		msg.m_MsgDataList.push_back( data );
		// プレイヤーの位置
		data.m_Float = m_Data.m_PosX;
		msg.m_MsgDataList.push_back( data );
		data.m_Float = m_Data.m_PosY;
		msg.m_MsgDataList.push_back( data );
		// ボムの持続時間
		data.m_Integer = m_Data.m_RestInvincibleTime;
		msg.m_MsgDataList.push_back( data );
		// メッセージ送信
		m_pStageData->m_MsgQueue.push( msg );
	}
#elif defined ( USE_GAME_UNIT )
	void Player::Impl::BlueModeBomb()
	{
		// 無敵時間の設定
		m_Data.m_RestInvincibleTime = 120;

		// メッセージ構築
		StageMessage msg;
		// ボム発動メッセージ
		msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_PLAYER_BOMBED;
		// ボムの属性
		StageMessage::StageMessageData data;
		data.m_Integer = PLAYER_CONS_MODE_BLUE;
		msg.m_MsgDataList.push_back( data );
		// プレイヤーの位置
		data.m_Float = m_Data.m_GUData.m_PosX.GetFloat();
		msg.m_MsgDataList.push_back( data );
		data.m_Float = m_Data.m_GUData.m_PosY.GetFloat();
		msg.m_MsgDataList.push_back( data );
		// ボムの持続時間
		data.m_Integer = m_Data.m_RestInvincibleTime;
		msg.m_MsgDataList.push_back( data );
		// メッセージ送信
		m_pStageData->m_MsgQueue.push( msg );
	}
#endif

#if defined ( USE_FLOATING_POINT )
	void Player::Impl::RedModeBomb()
	{
		// 無敵時間の設定
		m_Data.m_RestInvincibleTime = 150;

		// メッセージ構築
		StageMessage msg;
		// ボム発動メッセージ
		msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_PLAYER_BOMBED;
		// ボムの属性
		StageMessage::StageMessageData data;
		data.m_Integer = PLAYER_CONS_MODE_RED;
		msg.m_MsgDataList.push_back( data );
		// プレイヤーの位置
		data.m_Float = m_Data.m_PosX;
		msg.m_MsgDataList.push_back( data );
		data.m_Float = m_Data.m_PosY;
		msg.m_MsgDataList.push_back( data );
		// ボムの持続時間
		data.m_Integer = m_Data.m_RestInvincibleTime;
		msg.m_MsgDataList.push_back( data );
		// メッセージ送信
		m_pStageData->m_MsgQueue.push( msg );
	}
#elif defined ( USE_GAME_UNIT )
	void Player::Impl::RedModeBomb()
	{
		// 無敵時間の設定
		m_Data.m_RestInvincibleTime = 150;

		// メッセージ構築
		StageMessage msg;
		// ボム発動メッセージ
		msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_PLAYER_BOMBED;
		// ボムの属性
		StageMessage::StageMessageData data;
		data.m_Integer = PLAYER_CONS_MODE_RED;
		msg.m_MsgDataList.push_back( data );
		// プレイヤーの位置
		data.m_Float = m_Data.m_GUData.m_PosX.GetFloat();
		msg.m_MsgDataList.push_back( data );
		data.m_Float = m_Data.m_GUData.m_PosY.GetFloat();
		msg.m_MsgDataList.push_back( data );
		// ボムの持続時間
		data.m_Integer = m_Data.m_RestInvincibleTime;
		msg.m_MsgDataList.push_back( data );
		// メッセージ送信
		m_pStageData->m_MsgQueue.push( msg );
	}
#endif

	void Player::Impl::AddOpt()
	{
		const int ADD_OPT_COST = 15;		// オプション追加のためのコスト
		const int OPT_TOTAL_MAX = 4;		// オプション最大数

		// クリスタル数が足りない
		if( m_pStageData->m_GameData.m_CrystalTotal - m_pStageData->m_GameData.m_CrystalUsed < ADD_OPT_COST ){
			return;
		}
		
		// オプション数が最大値に達している
		if( m_PlayerOptList.size() >= OPT_TOTAL_MAX ){
			return;
		}
		
		PlayerOption* pNewOpt = new PlayerOption( m_pResourceMap, m_pStageData, m_PlayerOptList.size() );
		pNewOpt->ChangeConsMode( m_Data.m_ConsCur );
		m_PlayerOptList.push_back( pNewOpt );
		m_pStageData->m_FrameGameData.m_CrystalUsed += ADD_OPT_COST;

		std::list < PlayerOption* > ::iterator it = m_PlayerOptList.begin();
		for( ; it != m_PlayerOptList.end(); ++it ){
			( *it )->NotifyOptTotal( m_PlayerOptList.size() );
		}
	}

#if defined ( USE_FLOATING_POINT )
	void Player::Impl::Move()
	{
		const float PLAYER_BASE_VELOCITY = 3.2f;

		// 移動
		if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_RIGHT ) ){
			if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				m_Data.m_PosX += PLAYER_BASE_VELOCITY / 1.414f;
				m_Data.m_PosY -= PLAYER_BASE_VELOCITY / 1.414f;
			}
			else if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
				m_Data.m_PosX += PLAYER_BASE_VELOCITY / 1.414f;
				m_Data.m_PosY += PLAYER_BASE_VELOCITY / 1.414f;
			}
			else{
				m_Data.m_PosX += PLAYER_BASE_VELOCITY;
			}
		}
		else if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_LEFT ) ){
			if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				m_Data.m_PosX -= PLAYER_BASE_VELOCITY / 1.414f;
				m_Data.m_PosY -= PLAYER_BASE_VELOCITY / 1.414f;
			}
			else if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
				m_Data.m_PosX -= PLAYER_BASE_VELOCITY / 1.414f;
				m_Data.m_PosY += PLAYER_BASE_VELOCITY / 1.414f;
			}
			else{
				m_Data.m_PosX -= PLAYER_BASE_VELOCITY;
			}
		}
		else if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
			m_Data.m_PosY -= PLAYER_BASE_VELOCITY;
		}
		else if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
			m_Data.m_PosY += PLAYER_BASE_VELOCITY;
		}

		if( m_Data.m_PosX > 512.0f - m_Data.m_ColRadius ){
			m_Data.m_PosX = 512.0f - m_Data.m_ColRadius;
		}
		else if( m_Data.m_PosX < 128.0f + m_Data.m_ColRadius ){
			m_Data.m_PosX = 128.0f + m_Data.m_ColRadius;
		}
		if( m_Data.m_PosY > 470.0f ){
			m_Data.m_PosY = 470.0f;
		}
		else if( m_Data.m_PosY < 0.0f ){
			m_Data.m_PosY = 0.0f;
		}
	}
#elif defined ( USE_GAME_UNIT )
	void Player::Impl::Move()
	{
		const GameUnit PLAYER_BASE_VELOCITY = 3.2f;

		// 移動
		if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_RIGHT ) ){
			if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				GameUnit v = PLAYER_BASE_VELOCITY;
				v /= GameUnit( 1.414f );
				m_Data.m_GUData.m_PosX += v;
				m_Data.m_GUData.m_PosY -= v;
			}
			else if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
				GameUnit v = PLAYER_BASE_VELOCITY;
				v /= GameUnit( 1.414f );
				m_Data.m_GUData.m_PosX += v;
				m_Data.m_GUData.m_PosY += v;
			}
			else{
				m_Data.m_GUData.m_PosX += PLAYER_BASE_VELOCITY;
			}
		}
		else if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_LEFT ) ){
			if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				GameUnit v = PLAYER_BASE_VELOCITY;
				v /= GameUnit( 1.414f );
				m_Data.m_GUData.m_PosX -= v;
				m_Data.m_GUData.m_PosY -= v;
			}
			else if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
				GameUnit v = PLAYER_BASE_VELOCITY;
				v /= GameUnit( 1.414f );
				m_Data.m_GUData.m_PosX -= v;
				m_Data.m_GUData.m_PosY += v;
			}
			else{
				m_Data.m_GUData.m_PosX -= PLAYER_BASE_VELOCITY;
			}
		}
		else if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
			m_Data.m_GUData.m_PosY -= PLAYER_BASE_VELOCITY;
		}
		else if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
			m_Data.m_GUData.m_PosY += PLAYER_BASE_VELOCITY;
		}

		if( m_Data.m_GUData.m_PosX > GameUnit( 512 ) - m_Data.m_GUData.m_ColRadius ){
			m_Data.m_GUData.m_PosX = GameUnit( 512 ) - m_Data.m_GUData.m_ColRadius;
		}
		else if( m_Data.m_GUData.m_PosX < GameUnit( 128 ) + m_Data.m_GUData.m_ColRadius ){
			m_Data.m_GUData.m_PosX = GameUnit( 128 ) + m_Data.m_GUData.m_ColRadius;
		}
		if( m_Data.m_GUData.m_PosY > GameUnit( 470 ) ){
			m_Data.m_GUData.m_PosY = GameUnit( 470 );
		}
		else if( m_Data.m_GUData.m_PosY < GameUnit( 0 ) ){
			m_Data.m_GUData.m_PosY = GameUnit( 0 );
		}
	}
#endif

	void Player::Impl::ChangeMode()
	{
		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_CHANGE_MODE ) ){
			++m_Data.m_ConsCur;
			if( m_Data.m_ConsCur == PLAYER_CONS_MODE_RED + 1 ){
				m_Data.m_ConsCur = PLAYER_CONS_MODE_NORMAL;
			}

			// オプションへモード変更を通知
			std::list < PlayerOption* > ::iterator it = m_PlayerOptList.begin();
			for( ; it != m_PlayerOptList.end(); ++it ){
				( *it )->ChangeConsMode( m_Data.m_ConsCur );
			}

			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_PLAYER_CHANGE_MODE ] );	
		}
	}

	void Player::Impl::UpdateCons()
	{
		for( int i = 0; i < 3; ++i ){
			// 意識レベルの計算
			if( ( m_Data.m_Counter % 4 ) == 0 ){
				--m_Data.m_ConsLevel[ i ];
				if( m_pStageData->m_ConsLevel > m_Data.m_ConsLevel[ i ] ){
					m_Data.m_ConsLevel[ i ] = m_pStageData->m_ConsLevel;
				}
			}
			// 意識ゲージの計算
			if( m_Data.m_ConsCur == i + 1 ){
				if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
					m_Data.m_ConsGauge[ i ] -= 1;
				}
				/*if( m_Data.m_Counter % 2 ){
					--m_Data.m_ConsGauge[ i ];
				}*/
				if( m_Data.m_ConsGauge[ i ] < 0 ){
					m_Data.m_ConsGauge[ i ] = 0;
				}
			}
			else{
				if( ( m_Data.m_Counter % 25 ) == 0 ){
					m_Data.m_ConsGauge[ i ] += m_Data.m_ConsLevel[ i ] / 100;
					if( m_Data.m_ConsGauge[ i ] > 1000 ){
						m_Data.m_ConsGauge[ i ] = 1000;
					}
				}
			}
		}
	}

	inline void Player::Impl::AttachButtonState( const ButtonStatusHolder& holder )
	{
		m_ButtonStatus = holder;
	}

#if defined ( USE_FLOATING_POINT )

	void Player::Impl::Draw()
	{
		if( m_Data.m_RestInvincibleTime <= 0 || ( m_Data.m_Counter % 3 ) == 0 ){
			if( m_Data.m_ConsCur == PLAYER_CONS_MODE_NORMAL ){
			//	MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE ],
				//					m_Data.m_PosX, m_Data.m_PosY );
				MAPIL::AddModelOn2DBatchWork(	m_pResourceMap->m_pGlobalResourceMap->m_ModelMap[ GLOBAL_RESOURCE_MODEL_ID_PLAYER ],
												m_Data.m_PosX, m_Data.m_PosY, 0.5f,
												0.05f, 0.05f, 0.05f,
												MAPIL::DegToRad( 1.0f * ( m_Data.m_Counter % 360 ) ),
												MAPIL::DegToRad( 1.3f * ( m_Data.m_Counter % 200 ) ),
												MAPIL::DegToRad( 1.5f * ( m_Data.m_Counter % 540 ) ) );
			}
			else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_GREEN ){
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE ],
									m_Data.m_PosX, m_Data.m_PosY, true, 0xFF55FF55 );
			}
			else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_BLUE ){
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE ],
									m_Data.m_PosX, m_Data.m_PosY, true, 0xFF5555FF );
			}
			else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_RED ){
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE ],
									m_Data.m_PosX, m_Data.m_PosY, true, 0xFFFF5555 );
			}
		}

		if( m_Data.m_ConsCur != PLAYER_CONS_MODE_NORMAL ){
			MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								m_Data.m_PosX + 15, m_Data.m_PosY + 20,
								m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] / 300.0f, 0.3f, false,
								0xAAAAAAAA );
			
		}

		// オプションの描画
		std::list < PlayerOption* > ::iterator it = m_PlayerOptList.begin();
		for( ; it != m_PlayerOptList.end(); ++it ){
			( *it )->Draw();
		}
	}

#elif defined ( USE_GAME_UNIT )

	void Player::Impl::Draw()
	{
		float posX = m_Data.m_GUData.m_PosX.GetFloat();
		float posY = m_Data.m_GUData.m_PosY.GetFloat();

		if( m_Data.m_RestInvincibleTime <= 0 || ( m_Data.m_Counter % 3 ) == 0 ){
			if( m_Data.m_ConsCur == PLAYER_CONS_MODE_NORMAL ){
			//	MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE ],
				//					posX, posY );
				MAPIL::AddModelOn2DBatchWork(	m_pResourceMap->m_pGlobalResourceMap->m_ModelMap[ GLOBAL_RESOURCE_MODEL_ID_PLAYER ],
												posX, posY, 0.5f,
												0.05f, 0.05f, 0.05f,
												MAPIL::DegToRad( 1.0f * ( m_Data.m_Counter % 360 ) ),
												MAPIL::DegToRad( 1.3f * ( m_Data.m_Counter % 200 ) ),
												MAPIL::DegToRad( 1.5f * ( m_Data.m_Counter % 540 ) ) );
			}
			else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_GREEN ){
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE ],
									posX, posY, true, 0xFF55FF55 );
			}
			else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_BLUE ){
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE ],
									posX, posY, true, 0xFF5555FF );
			}
			else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_RED ){
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE ],
									posX, posY, true, 0xFFFF5555 );
			}
			MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								posX, posY,
								0.5f, 0.5f, true,
								0xFFFF0000 );
		}

		if( m_Data.m_ConsCur != PLAYER_CONS_MODE_NORMAL ){
			MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								posX + 15, posY + 20,
								m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] / 300.0f, 0.3f, false,
								0xAAAAAAAA );
			
		}

		// オプションの描画
		std::list < PlayerOption* > ::iterator it = m_PlayerOptList.begin();
		for( ; it != m_PlayerOptList.end(); ++it ){
			( *it )->Draw();
		}
	}
#endif

#if defined ( USE_FLOATING_POINT )
	bool Player::Impl::Update()
	{
		ChangeMode();
		Move();

		// ショット
		if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			if( m_Data.m_ConsCur == PLAYER_CONS_MODE_NORMAL || m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] <= 0 ){
				NormalModeShot();
			}
			else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_GREEN ){
				GreenModeShot();
			}
			else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_BLUE ){
				BlueModeShot();
			}
			else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_RED )
			{
				RedModeShot();
			}
		}
		
		// ボム
		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
			if( m_Data.m_RestInvincibleTime <= 0 ){
				if( m_Data.m_ConsCur == PLAYER_CONS_MODE_GREEN && m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] >= 500 ){
					m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] -= 500;
					GreenModeBomb();
				}
				else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_BLUE && m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] >= 500 ){
					BlueModeBomb();
					m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] -= 500;
				}
				else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_RED && m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] >= 500 ){
					RedModeBomb();
					m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] -= 500;
				}
			}
		}

		// オプション追加
		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_ADD_OPT ) ){
			AddOpt();
		}

		UpdateCons();

		// オプションの更新
		std::list < PlayerOption* > ::iterator it = m_PlayerOptList.begin();
		for( ; it != m_PlayerOptList.end(); ){
			( *it )->SetPos( m_Data.m_PosX, m_Data.m_PosY );
			( *it )->AttachButtonState( m_ButtonStatus );
			if( !( *it )->Update() ){
				delete ( *it );
				it = m_PlayerOptList.erase( it );
				// 消えたオプション分だけ、ID割り振り変更
				int count = 0;
				std::list < PlayerOption* > ::iterator it2 = m_PlayerOptList.begin();
				for( ; it2 != m_PlayerOptList.end(); ++it2 ){
					( *it2 )->ChangeID( count );
					( *it2 )->NotifyOptTotal( m_PlayerOptList.size() );
					++count;
				}
				continue;
			}
			++it;
		}

		--m_Data.m_RestInvincibleTime;
		++m_Data.m_Counter;
		
		return true;
	}
#elif defined ( USE_GAME_UNIT )
	bool Player::Impl::Update()
	{
		ChangeMode();
		Move();

		// ショット
		if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			if( m_Data.m_ConsCur == PLAYER_CONS_MODE_NORMAL || m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] <= 0 ){
				NormalModeShot();
			}
			else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_GREEN ){
				GreenModeShot();
			}
			else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_BLUE ){
				BlueModeShot();
			}
			else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_RED )
			{
				RedModeShot();
			}
		}
		
		// ボム
		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
			if( m_Data.m_RestInvincibleTime <= 0 &&
				m_pStageData->m_GameData.m_CrystalTotal - m_pStageData->m_GameData.m_CrystalUsed >= 500 ){
				if( m_Data.m_ConsCur == PLAYER_CONS_MODE_GREEN && m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] >= 500 ){
					m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] -= 500;
					GreenModeBomb();
				}
				else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_BLUE && m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] >= 500 ){
					BlueModeBomb();
					m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] -= 500;
				}
				else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_RED && m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] >= 500 ){
					RedModeBomb();
					m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] -= 500;
				}
				m_pStageData->m_FrameGameData.m_CrystalUsed += 500;
			}
		}

		// オプション追加
		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_ADD_OPT ) ){
			AddOpt();
		}

		UpdateCons();

		// オプションの更新
		std::list < PlayerOption* > ::iterator it = m_PlayerOptList.begin();
		for( ; it != m_PlayerOptList.end(); ){
			( *it )->SetPos( m_Data.m_GUData.m_PosX, m_Data.m_GUData.m_PosY );
			( *it )->AttachButtonState( m_ButtonStatus );
			if( !( *it )->Update() ){
				delete ( *it );
				it = m_PlayerOptList.erase( it );
				// 消えたオプション分だけ、ID割り振り変更
				int count = 0;
				std::list < PlayerOption* > ::iterator it2 = m_PlayerOptList.begin();
				for( ; it2 != m_PlayerOptList.end(); ++it2 ){
					( *it2 )->ChangeID( count );
					( *it2 )->NotifyOptTotal( m_PlayerOptList.size() );
					++count;
				}
				continue;
			}
			++it;
		}

		--m_Data.m_RestInvincibleTime;
		++m_Data.m_Counter;
		
		return true;
	}
#endif
	

	inline void Player::Impl::ProcessCollision( Enemy* pEnemy )
	{
		// 無敵時間中
		if( m_Data.m_RestInvincibleTime > 0 ){
			return;
		}
		
		// 衝突判定無効化中
		if( pEnemy->IsNonCollisionMode() ){
			return;
		}

		int damage = 3;

		// ダメージが発生した時
		if( damage > 0 ){
			// ダメージ時のメッセージ送信
			StageMessage msg;
			msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_PLAYER_DAMAGED;
			StageMessage::StageMessageData data;
			data.m_Integer = 0;
			msg.m_MsgDataList.push_back( data );
			m_pStageData->m_MsgQueue.push( msg );
			m_Data.m_HP -= damage;
			// ゲームオーバー処理
			if( m_Data.m_HP <= 0 ){
				MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_PLAYER_DESTROYED ] );
				msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_PLAYER_DESTORYED;
				StageMessage::StageMessageData data;
				data.m_Integer = 0;
				msg.m_MsgDataList.push_back( data );
				m_pStageData->m_MsgQueue.push( msg );
			}
			// 無敵時間の調整
			m_Data.m_RestInvincibleTime = INVINCIBLE_TIME;
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_PLAYER_DAMAGED ] );
		}
	}

	void Player::Impl::ProcessCollision( EnemyShot* pEnemyShot )
	{
		// 無敵時間中
		if( m_Data.m_RestInvincibleTime > 0 ){
			return;
		}
		// 弾は消滅済み
		if( pEnemyShot->IsDead() ){
			return;
		}

		int damage = 0;
		int scoreDelta = 0;
		bool hasAttr = pEnemyShot->GetConsAttr() >= ENEMY_SHOT_ATTR_GREEN;
		// 敵弾が属性を持っているとき
		if( hasAttr ){
			// 属性が一致している時
			if( pEnemyShot->GetConsAttr() == m_Data.m_ConsCur ){
				m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] += 10;
				if( m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] > 1000 ){
					m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] = 1000;
				}
				damage = 0;			// ダメージ無効化
				scoreDelta = 100;	// 100点追加
			}
			// 属性が無い時
			else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_NORMAL ){
				damage = pEnemyShot->GetPower();
				for( int i = 0; i < 3; ++i ){
					m_Data.m_ConsGauge[ i ] -= 100;
				}
			}
			// 属性負けしている時
			// (緑->青, 青->赤, 赤->緑)
			else if(	pEnemyShot->GetConsAttr() == ENEMY_SHOT_ATTR_GREEN && m_Data.m_ConsCur == PLAYER_CONS_MODE_BLUE ||
						pEnemyShot->GetConsAttr() == ENEMY_SHOT_ATTR_BLUE && m_Data.m_ConsCur == PLAYER_CONS_MODE_RED ||
						pEnemyShot->GetConsAttr() == ENEMY_SHOT_ATTR_RED && m_Data.m_ConsCur == PLAYER_CONS_MODE_GREEN ){
				damage = pEnemyShot->GetPower() * 2;		// ダメージ2倍
				m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] -= 500;
				for( int i = 0; i < 3; ++i ){
					m_Data.m_ConsGauge[ i ] -= 100;
				}
			}
			// 属性勝ちしている時
			// (青->緑, 赤->青, 緑->赤)
			else if(	pEnemyShot->GetConsAttr() == ENEMY_SHOT_ATTR_BLUE && m_Data.m_ConsCur == PLAYER_CONS_MODE_GREEN ||
						pEnemyShot->GetConsAttr() == ENEMY_SHOT_ATTR_RED && m_Data.m_ConsCur == PLAYER_CONS_MODE_BLUE ||
						pEnemyShot->GetConsAttr() == ENEMY_SHOT_ATTR_GREEN && m_Data.m_ConsCur == PLAYER_CONS_MODE_RED ){
				damage = 0;		// ダメージ無効化
			}
		}
		// 属性を持たない時
		else{
			damage = pEnemyShot->GetPower();
			if( m_Data.m_ConsCur != PLAYER_CONS_MODE_NORMAL ){
				m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] -= 200;
			}
			for( int i = 0; i < 3; ++i ){
				m_Data.m_ConsGauge[ i ] -= 100;
				if( m_Data.m_ConsGauge[ i ] < 0 ){
					m_Data.m_ConsGauge[ i ] = 0;
				}
			}
		}

		// スコア追加
		m_pStageData->m_FrameGameData.m_Score += scoreDelta;

		// ダメージが発生した時
		if( damage > 0 ){
			// ダメージ時のメッセージ送信
			StageMessage msg;
			msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_PLAYER_DAMAGED;
			StageMessage::StageMessageData data;
			data.m_Integer = 0;
			msg.m_MsgDataList.push_back( data );
			m_pStageData->m_MsgQueue.push( msg );
			m_Data.m_HP -= damage;
			if( m_pStageData->m_TotalGameData.m_CrystalTotal - m_pStageData->m_TotalGameData.m_CrystalUsed >= 1000 ){
				m_pStageData->m_FrameGameData.m_CrystalUsed += 1000;
			}
			else{
				m_pStageData->m_FrameGameData.m_CrystalUsed += m_pStageData->m_TotalGameData.m_CrystalTotal - m_pStageData->m_TotalGameData.m_CrystalUsed;
			}
			// ゲームオーバー処理
			if( m_Data.m_HP <= 0 ){
				MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_PLAYER_DESTROYED ] );
				msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_PLAYER_DESTORYED;
				StageMessage::StageMessageData data;
				data.m_Integer = 0;
				msg.m_MsgDataList.push_back( data );
				m_pStageData->m_MsgQueue.push( msg );
			}
			// 無敵時間の調整
			m_Data.m_RestInvincibleTime = INVINCIBLE_TIME;
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_PLAYER_DAMAGED ] );
		}
	}

	void Player::Impl::ProcessCollision( Item* pItem )
	{
		if( pItem->GetItemID() == ITEM_ID_CRYSTAL ){
			m_pStageData->m_FrameGameData.m_Score += 100 * pItem->GetItemSubID();
			m_pStageData->m_FrameGameData.m_CrystalTotal += pItem->GetItemSubID();
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_ITEM_1_SE ] );
		}
		else if( pItem->GetItemID() == ITEM_ID_POWER_UP ){
			m_Data.m_ShotPower += 1;
			if( m_Data.m_ShotPower > 50 ){
				m_Data.m_ShotPower = 50;
			}
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_ITEM_2_SE ] );
		}
		else if( pItem->GetItemID() == ITEM_ID_RECOVER ){
			++m_Data.m_HP;
			//if( m_Data.m_HP > 10 ){
			//	m_Data.m_HP = 10;
			//}
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_ITEM_2_SE ] );
		}
		else if( pItem->GetItemID() == ITEM_ID_CONS_LEVEL_RECOVER ){
			int subID = pItem->GetItemSubID();
			//m_Data.m_ConsLevel[ subID ] += 100;
			//if( m_Data.m_ConsLevel[ subID ] > 1000 ){
				m_Data.m_ConsLevel[ subID ] = 1000;
			//}
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_ITEM_2_SE ] );
		}
	}

#if defined ( USE_FLOATING_POINT )

	inline void Player::Impl::GetPos( float* pX, float* pY )
	{
		*pX = m_Data.m_PosX;
		*pY = m_Data.m_PosY;
	}

	inline float Player::Impl::GetCollisionRadius() const
	{
		return m_Data.m_ColRadius;
	}

#elif defined ( USE_GAME_UNIT )

	void Player::Impl::GetPos( GameUnit* pPosX, GameUnit* pPosY )
	{
		*pPosX = m_Data.m_GUData.m_PosX;
		*pPosY = m_Data.m_GUData.m_PosY;
	}

	GameUnit Player::Impl::GetCollisionRadius()
	{
		return m_Data.m_GUData.m_ColRadius;
	}

#endif

	inline int Player::Impl::GetHP() const
	{
		return m_Data.m_HP;
	}

	inline int Player::Impl::GetConsGauge( int cons ) const
	{
		return m_Data.m_ConsGauge[ cons ];
	}

	inline int Player::Impl::GetConsLevel( int cons ) const
	{
		return m_Data.m_ConsLevel[ cons ];
	}

	inline int Player::Impl::GetShotPower() const
	{
		return m_Data.m_ShotPower;
	}

	inline int Player::Impl::GetCurCons() const
	{
		return m_Data.m_ConsCur;
	}

	inline void Player::Impl::SetPos( const GameUnit& posX, const GameUnit& posY )
	{
		m_Data.m_GUData.m_PosX = posX;
		m_Data.m_GUData.m_PosY = posY;
	}

	inline void Player::Impl::SetHP( int hp )
	{
		m_Data.m_HP = hp;
	}

	inline void Player::Impl::SetShotPower( int power )
	{
		m_Data.m_ShotPower = power;
	}

	inline void Player::Impl::SetConsGauge( int cons, int val )
	{
		m_Data.m_ConsGauge[ cons ] = val;
	}

	inline void Player::Impl::SetConsLevel( int cons, int level )
	{
		m_Data.m_ConsLevel[ cons ] = level;
	}

	inline void Player::Impl::SetCons( int cons )
	{
		m_Data.m_ConsCur = cons;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	Player::Player( std::shared_ptr < ResourceMap > pMap, StageData* pStageData ) :	CollisionObject(),
																					m_pImpl( new Player::Impl( pMap, pStageData ) )
	{
	}

	Player::~Player()
	{
	}

	void Player::AttachButtonState( const ButtonStatusHolder& holder )
	{
		m_pImpl->AttachButtonState( holder );
	}

	void Player::Draw()
	{
		m_pImpl->Draw();
	}

	bool Player::Update()
	{
		return m_pImpl->Update();
	}

	void Player::Colided( CollisionObject* pObject )
	{
		pObject->ProcessCollision( this );
	}

	void Player::ProcessCollision( Player* pPlayer )
	{
	}

	void Player::ProcessCollision( Enemy* pEnemy )
	{
		m_pImpl->ProcessCollision( pEnemy );
	}

	void Player::ProcessCollision( PlayerShot* pPlayerShot )
	{
	}

	void Player::ProcessCollision( EnemyShot* pEnemyShot )
	{
		m_pImpl->ProcessCollision( pEnemyShot );
	}

	void Player::ProcessCollision( Item* pItem )
	{
		m_pImpl->ProcessCollision( pItem );
	}

#if defined ( USE_FLOATING_POINT )

	void Player::Init( float posX, float posY )
	{
	}

	void Player::GetPos( float* pPosX, float* pPosY )
	{
		m_pImpl->GetPos( pPosX, pPosY );
	}

	float Player::GetCollisionRadius()
	{
		return m_pImpl->GetCollisionRadius();
	}

#elif defined ( USE_GAME_UNIT )

	void Player::Init( const GameUnit& posX, const GameUnit& posY )
	{
	}

	void Player::GetPos( GameUnit* pPosX, GameUnit* pPosY )
	{
		m_pImpl->GetPos( pPosX, pPosY );
	}

	GameUnit Player::GetCollisionRadius()
	{
		return m_pImpl->GetCollisionRadius();
	}

#endif

	int Player::GetHP() const
	{
		return m_pImpl->GetHP();
	}

	int Player::GetConsGauge( int cons ) const
	{
		return m_pImpl->GetConsGauge( cons );
	}

	int Player::GetConsLevel( int cons ) const
	{
		return m_pImpl->GetConsLevel( cons );
	}

	int Player::GetShotPower() const
	{
		return m_pImpl->GetShotPower();
	}

	int Player::GetCurCons() const
	{
		return m_pImpl->GetCurCons();
	}

	void Player::SetPos( const GameUnit& posX, const GameUnit& posY )
	{
		m_pImpl->SetPos( posX, posY );
	}

	void Player::SetHP( int hp )
	{
		m_pImpl->SetHP( hp );
	}

	void Player::SetShotPower( int power )
	{
		m_pImpl->SetShotPower( power );
	}

	void Player::SetConsGauge( int cons, int val )
	{
		m_pImpl->SetConsGauge( cons, val );
	}

	void Player::SetConsLevel( int cons, int level )
	{
		m_pImpl->SetConsLevel( cons, level );
	}

	void Player::SetCons( int cons )
	{
		m_pImpl->SetCons( cons );
	}
}
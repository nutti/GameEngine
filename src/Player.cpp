#include <MAPIL/MAPIL.h>

#include "Player.h"
#include "PlayerShot.h"
#include "ResourceTypes.h"
#include "Util.h"
#include "Item.h"

#include "Stage.h"

namespace GameEngine
{

	class Player::Impl
	{
	private:
		ButtonStatusHolder					m_ButtonStatus;
		PlayerData							m_Data;
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		StageData*							m_pStageData;
		
		void NormalModeShot();
		void GreenModeShot();
		void Move();
		void ChangeMode();
	public:
		Impl( std::shared_ptr < ResourceMap > pMap, StageData* pStageData );
		~Impl();
		void AttachButtonState( const ButtonStatusHolder& holder );		// キー入力を設定
		void Draw();													// 描画
		bool Update();													// 更新
		void GetPos( float* pX, float* pY ) const;
		void ProcessCollision( Enemy* pEnemy );							// 衝突時の処理（敵）
		void ProcessCollision( EnemyShot* pEnemyShot );					// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );
		float GetCollisionRadius() const;
		int GetHP() const;												// HPを取得
		int GetConsGauge( int cons ) const;								// 意識ゲージの取得
		int GetConsLevel( int cons ) const;								// 意識レベルの取得
		int GetShotPower() const;
	};

	Player::Impl::Impl( std::shared_ptr < ResourceMap > pMap, StageData* pStageData ) : m_pResourceMap( pMap ), m_pStageData( pStageData )
	{
		MAPIL::ZeroObject( &m_Data, sizeof( m_Data ) );
		m_Data.m_HP = 10;
		m_Data.m_ConsGauge[ 0 ] = m_Data.m_ConsGauge[ 1 ] = m_Data.m_ConsGauge[ 2 ] = 200;
		m_Data.m_ConsLevel[ 0 ] = m_Data.m_ConsLevel[ 1 ] = m_Data.m_ConsLevel[ 2 ] = 0;
		m_Data.m_PosX = 300.0f;
		m_Data.m_PosY = 400.0f;
		m_Data.m_ShotPower = 0;
		m_Data.m_ColRadius = 3.0f;
		m_Data.m_Counter = 0;
		m_Data.m_ConsCur = PLAYER_CONS_MODE_NORMAL;
	}

	Player::Impl::~Impl()
	{
		MAPIL::ZeroObject( &m_Data, sizeof( m_Data ) );
	}

	void Player::Impl::NormalModeShot()
	{
		if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			if( ( m_Data.m_Counter % 3 ) == 0 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
				pNewShot->SetPos( m_Data.m_PosX - 7.0f, m_Data.m_PosY + 5.0f );
				pNewShot->SetShotPower( 5 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
				pNewShot->SetPos( m_Data.m_PosX + 7.0f, m_Data.m_PosY + 5.0f );
				pNewShot->SetShotPower( 5 );
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
	}

	void Player::Impl::GreenModeShot()
	{
		if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			if( ( m_Data.m_Counter % 3 ) == 0 ){
				PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
				pNewShot->SetAngle( MAPIL::DegToRad( 90.0f ) );
				pNewShot->SetSpeed( 15.0f );
				pNewShot->SetShotPower( 5 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_PosX - 5.0f, m_Data.m_PosY + 3.0f );
				pNewShot->SetSpeed( 15.0f );
				pNewShot->SetAngle( MAPIL::DegToRad( 10.0f + 90.0f ) );
				pNewShot->SetShotPower( 1 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_PosX - 5.0f, m_Data.m_PosY + 3.0f );
				pNewShot->SetSpeed( 15.0f );
				pNewShot->SetAngle( MAPIL::DegToRad( -10.0f + 90.0f ) );
				pNewShot->SetShotPower( 1 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				if( m_Data.m_ShotPower >= 10 ){
					PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
					pNewShot->SetPos( m_Data.m_PosX - 5.0f, m_Data.m_PosY + 3.0f );
					pNewShot->SetSpeed( 15.0f );
					pNewShot->SetAngle( MAPIL::DegToRad( 20.0f + 90.0f ) );
					pNewShot->SetShotPower( 1 );
					m_pStageData->m_PlayerShotList.push_back( pNewShot );
					pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
					pNewShot->SetPos( m_Data.m_PosX + 5.0f, m_Data.m_PosY + 3.0f );
					pNewShot->SetSpeed( 15.0f );
					pNewShot->SetAngle( MAPIL::DegToRad( -20.0f + 90.0f ) );
					pNewShot->SetShotPower( 1 );
					m_pStageData->m_PlayerShotList.push_back( pNewShot );
				}
				if( m_Data.m_ShotPower >= 20 ){
					PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
					pNewShot->SetPos( m_Data.m_PosX - 5.0f, m_Data.m_PosY + 3.0f );
					pNewShot->SetSpeed( 15.0f );
					pNewShot->SetAngle( MAPIL::DegToRad( 30.0f + 90.0f ) );
					pNewShot->SetShotPower( 1 );
					m_pStageData->m_PlayerShotList.push_back( pNewShot );
					pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
					pNewShot->SetPos( m_Data.m_PosX + 5.0f, m_Data.m_PosY + 3.0f );
					pNewShot->SetSpeed( 15.0f );
					pNewShot->SetAngle( MAPIL::DegToRad( -30.0f + 90.0f ) );
					pNewShot->SetShotPower( 1 );
					m_pStageData->m_PlayerShotList.push_back( pNewShot );
				}
				if( m_Data.m_ShotPower >= 30 ){
					PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
					pNewShot->SetPos( m_Data.m_PosX - 5.0f, m_Data.m_PosY + 3.0f );
					pNewShot->SetSpeed( 15.0f );
					pNewShot->SetAngle( MAPIL::DegToRad( 40.0f + 90.0f ) );
					pNewShot->SetShotPower( 1 );
					m_pStageData->m_PlayerShotList.push_back( pNewShot );
					pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
					pNewShot->SetPos( m_Data.m_PosX + 5.0f, m_Data.m_PosY + 3.0f );
					pNewShot->SetSpeed( 15.0f );
					pNewShot->SetAngle( MAPIL::DegToRad( -40.0f + 90.0f ) );
					pNewShot->SetShotPower( 1 );
					m_pStageData->m_PlayerShotList.push_back( pNewShot );
				}
				MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_SHOT_SE ] );
			}
		}
	}

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

	void Player::Impl::ChangeMode()
	{
		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_CHANGE_MODE ) ){
			++m_Data.m_ConsCur;
			if( m_Data.m_ConsCur == PLAYER_CONS_MODE_RED + 1 ){
				m_Data.m_ConsCur = PLAYER_CONS_MODE_NORMAL;
			}
		}
	}

	void Player::Impl::AttachButtonState( const ButtonStatusHolder& holder )
	{
		m_ButtonStatus = holder;
	}

	void Player::Impl::Draw()
	{
		MAPIL::DrawString( m_Data.m_PosX, m_Data.m_PosY, "■" );
	}

	bool Player::Impl::Update()
	{
		ChangeMode();
		Move();

		if( m_Data.m_ConsCur == PLAYER_CONS_MODE_NORMAL ){
			NormalModeShot();
		}
		else if( m_Data.m_ConsCur == PLAYER_CONS_MODE_GREEN ){
			GreenModeShot();
		}

		++m_Data.m_Counter;
		
		return true;
	}

	void Player::Impl::GetPos( float* pX, float* pY ) const
	{
		*pX = m_Data.m_PosX;
		*pY = m_Data.m_PosY;
	}

	void Player::Impl::ProcessCollision( Enemy* pEnemy )
	{
		--m_Data.m_HP;
	}

	void Player::Impl::ProcessCollision( EnemyShot* pEnemyShot )
	{
		--m_Data.m_HP;
	}

	void Player::Impl::ProcessCollision( Item* pItem )
	{
		if( pItem->GetItemID() == ITEM_ID_CRYSTAL ){
			m_pStageData->m_FrameGameData.m_Score += 100;
			++m_pStageData->m_FrameGameData.m_CrystalTotal;
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_ITEM_1_SE ] );
		}
		else if( pItem->GetItemID() == ITEM_ID_POWER_UP ){
			m_Data.m_ShotPower += 1;
			if( m_Data.m_ShotPower > 30 ){
				m_Data.m_ShotPower = 30;
			}
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_ITEM_2_SE ] );
		}
	}

	float Player::Impl::GetCollisionRadius() const
	{
		return m_Data.m_ColRadius;
	}

	int Player::Impl::GetHP() const
	{
		return m_Data.m_HP;
	}

	int Player::Impl::GetConsGauge( int cons ) const
	{
		return m_Data.m_ConsGauge[ cons ];
	}

	int Player::Impl::GetConsLevel( int cons ) const
	{
		return m_Data.m_ConsLevel[ cons ];
	}

	int Player::Impl::GetShotPower() const
	{
		return m_Data.m_ShotPower;
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

	void Player::Init( float posX, float posY )
	{
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

	void Player::GetPos( float* pPosX, float* pPosY )
	{
		m_pImpl->GetPos( pPosX, pPosY );
	}

	float Player::GetCollisionRadius()
	{
		return m_pImpl->GetCollisionRadius();
	}

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
}
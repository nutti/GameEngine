#include <MAPIL/MAPIL.h>

#include "Player.h"
#include "PlayerShot.h"
#include "ResourceTypes.h"
#include "Util.h"
#include "Item.h"

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
		
		void NormalModeShot();
		void GreenModeShot();
		void BlueModeShot();
		void RedModeShot();
		void Move();
		void ChangeMode();
		void UpdateCons();
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
		int GetCurCons() const;
	};

	Player::Impl::Impl( std::shared_ptr < ResourceMap > pMap, StageData* pStageData ) :	GameObjectImplBase(),
																						m_pResourceMap( pMap ),
																						m_pStageData( pStageData ),
																						INVINCIBLE_TIME( 240 )
	{
		MAPIL::ZeroObject( &m_Data, sizeof( m_Data ) );
		m_Data.m_HP = 1;
		m_Data.m_ConsGauge[ 0 ] = m_Data.m_ConsGauge[ 1 ] = m_Data.m_ConsGauge[ 2 ] = 1000;
		m_Data.m_ConsLevel[ 0 ] = m_Data.m_ConsLevel[ 1 ] = m_Data.m_ConsLevel[ 2 ] = 1000;
		m_Data.m_PosX = 300.0f;
		m_Data.m_PosY = 400.0f;
		m_Data.m_ShotPower = 0;
		m_Data.m_ColRadius = 3.0f;
		m_Data.m_Counter = 0;
		m_Data.m_ConsCur = PLAYER_CONS_MODE_NORMAL;
		m_Data.m_RestInvincibleTime = 0;
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
				pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
				pNewShot->SetSpeed( 15.0f );
				pNewShot->SetAngle( MAPIL::DegToRad( 10.0f + 90.0f ) );
				pNewShot->SetShotPower( 1 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
				pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
				pNewShot->SetSpeed( 15.0f );
				pNewShot->SetAngle( MAPIL::DegToRad( -10.0f + 90.0f ) );
				pNewShot->SetShotPower( 1 );
				m_pStageData->m_PlayerShotList.push_back( pNewShot );
				if( m_Data.m_ShotPower >= 10 ){
					PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
					pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
					pNewShot->SetSpeed( 15.0f );
					pNewShot->SetAngle( MAPIL::DegToRad( 20.0f + 90.0f ) );
					pNewShot->SetShotPower( 1 );
					m_pStageData->m_PlayerShotList.push_back( pNewShot );
					pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
					pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
					pNewShot->SetSpeed( 15.0f );
					pNewShot->SetAngle( MAPIL::DegToRad( -20.0f + 90.0f ) );
					pNewShot->SetShotPower( 1 );
					m_pStageData->m_PlayerShotList.push_back( pNewShot );
				}
				if( m_Data.m_ShotPower >= 20 ){
					PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
					pNewShot->SetPos( m_Data.m_PosX , m_Data.m_PosY + 3.0f );
					pNewShot->SetSpeed( 15.0f );
					pNewShot->SetAngle( MAPIL::DegToRad( 30.0f + 90.0f ) );
					pNewShot->SetShotPower( 1 );
					m_pStageData->m_PlayerShotList.push_back( pNewShot );
					pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
					pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
					pNewShot->SetSpeed( 15.0f );
					pNewShot->SetAngle( MAPIL::DegToRad( -30.0f + 90.0f ) );
					pNewShot->SetShotPower( 1 );
					m_pStageData->m_PlayerShotList.push_back( pNewShot );
				}
				if( m_Data.m_ShotPower >= 30 ){
					PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
					pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
					pNewShot->SetSpeed( 15.0f );
					pNewShot->SetAngle( MAPIL::DegToRad( 40.0f + 90.0f ) );
					pNewShot->SetShotPower( 1 );
					m_pStageData->m_PlayerShotList.push_back( pNewShot );
					pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 1 );
					pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 3.0f );
					pNewShot->SetSpeed( 15.0f );
					pNewShot->SetAngle( MAPIL::DegToRad( -40.0f + 90.0f ) );
					pNewShot->SetShotPower( 1 );
					m_pStageData->m_PlayerShotList.push_back( pNewShot );
				}
				MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_SHOT_SE ] );
			}
		}
	}

	void Player::Impl::BlueModeShot()
	{
		if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 2 );
			pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY );
			pNewShot->SetShotPower( 4 + m_Data.m_ShotPower / 10 );
			pNewShot->SetPlayer( m_pStageData->m_pPlayer );
			m_pStageData->m_PlayerShotList.push_back( pNewShot );
			pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 2 );
			pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY + 8.0f );
			pNewShot->SetShotPower( 4 + m_Data.m_ShotPower / 10 );
			pNewShot->SetPlayer( m_pStageData->m_pPlayer );
			m_pStageData->m_PlayerShotList.push_back( pNewShot );
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_SHOT_SE ] );
		}
	}

	void Player::Impl::RedModeShot()
	{
		if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			if( ( m_Data.m_Counter % ( 12 - 2 * ( m_Data.m_ShotPower / 10 ) ) ) == 0 ){
				for( int i = 0; i < 36; ++i ){
					PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 3 );
					pNewShot->SetPos( m_Data.m_PosX, m_Data.m_PosY );
					pNewShot->SetAngle( MAPIL::DegToRad( i * 10.0f ) );
					pNewShot->SetSpeed( 10.0f );
					pNewShot->SetShotPower( 2 );
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
					m_Data.m_ConsGauge[ i ] -= 2;
				}
				if( m_Data.m_Counter % 2 ){
					--m_Data.m_ConsGauge[ i ];
				}
				if( m_Data.m_ConsGauge[ i ] < 0 ){
					m_Data.m_ConsGauge[ i ] = 0;
				}
			}
			else{
				if( ( m_Data.m_Counter % 30 ) == 0 ){
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

	void Player::Impl::Draw()
	{
		if( m_Data.m_RestInvincibleTime <= 0 || ( m_Data.m_Counter % 3 ) == 0 ){
			if( m_Data.m_ConsCur == PLAYER_CONS_MODE_NORMAL ){
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE ],
									m_Data.m_PosX, m_Data.m_PosY );
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
	}

	bool Player::Impl::Update()
	{
		ChangeMode();
		Move();

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

		UpdateCons();

		--m_Data.m_RestInvincibleTime;
		++m_Data.m_Counter;
		
		return true;
	}

	inline void Player::Impl::GetPos( float* pX, float* pY ) const
	{
		*pX = m_Data.m_PosX;
		*pY = m_Data.m_PosY;
	}

	inline void Player::Impl::ProcessCollision( Enemy* pEnemy )
	{
		--m_Data.m_HP;
	}

	void Player::Impl::ProcessCollision( EnemyShot* pEnemyShot )
	{
		if( m_Data.m_RestInvincibleTime <= 0 ){
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_PLAYER_DAMAGED ] );
			m_pStageData->m_MsgQueue.push( STAGE_MESSAGE_PLAYER_DAMAGED );
			--m_Data.m_HP;
			if( m_Data.m_HP <= 0 ){
				MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_PLAYER_DESTROYED ] );
				m_pStageData->m_MsgQueue.push( STAGE_MESSAGE_PLAYER_DESTORYED );
			}
		
			if( m_Data.m_ConsCur != PLAYER_CONS_MODE_NORMAL ){
				m_Data.m_ConsGauge[ m_Data.m_ConsCur - 1 ] -= 200;
			}
			for( int i = 0; i < 3; ++i ){
				m_Data.m_ConsGauge[ i ] -= 100;
				if( m_Data.m_ConsGauge[ i ] < 0 ){
					m_Data.m_ConsGauge[ i ] = 0;
				}
			}

			m_Data.m_RestInvincibleTime = INVINCIBLE_TIME;
		}
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
			if( m_Data.m_ShotPower > 50 ){
				m_Data.m_ShotPower = 50;
			}
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_ID_ITEM_2_SE ] );
		}
	}

	inline float Player::Impl::GetCollisionRadius() const
	{
		return m_Data.m_ColRadius;
	}

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

	int Player::GetCurCons() const
	{
		return m_pImpl->GetCurCons();
	}
}
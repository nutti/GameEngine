#include <MAPIL/MAPIL.h>

#include "PlayerShot.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"
#include "ResourceTypes.h"
#include "ResourceID.h"
#include "SpriteBatch.h"

#include "GameObjectImplBase.h"

namespace GameEngine
{
	enum PlayerShotID
	{
		PLAYER_SHOT_ID_NORMAL_MAIN		= 0,	// 通常モードのメインショット
		PLAYER_SHOT_ID_GREEN_MAIN		= 1,	// 緑モードのメインショット
		PLAYER_SHOT_ID_BLUE_MAIN		= 2,	// 青モードのメインショット
		PLAYER_SHOT_ID_RED_MAIN			= 3,	// 赤モードのメインショット
		PLAYER_SHOT_ID_GREEN_BOMB		= 4,	// 緑モードのボム
	};

	class PlayerShot::Impl : public GameObjectImplBase
	{
	private:
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		int									m_ShotID;
		float								m_PosX;
		float								m_PosY;
		float								m_Angle;
		float								m_Speed;
		float								m_ColRadius;
		int									m_ShotPower;
		bool								m_Colided;			// 衝突したか？
		int									m_Counter;
		int									m_ConsAttr;
		Player*								m_pPlayer;
		Enemy*								m_pEnemy;
		StageData*							m_pStageData;
	public:
		Impl( std::shared_ptr < ResourceMap > pMap, int id );
		~Impl();
		void Draw();													// 描画
		bool Update();													// 更新
		void GetPos( float* pX, float* pY );
		void SetPos( const Player& player );
		void SetPos( float posX, float posY );
		void SetAngle( float angle );									// 角度を設定
		void SetSpeed( float speed );
		void SetShotPower( int power );
		void SetConsAttr( int attr );
		void ProcessCollision( Enemy* pEnemy );							// 衝突時の処理（敵）
		float GetCollisionRadius() const;
		int GetShotPower() const;
		int GetConsAttr() const;
		void SetPlayer( Player* pPlayer );
		void SetEnemy( Enemy* pEnemy );
		void SetStageData( StageData* pData );
		int GetID() const;
	};

	PlayerShot::Impl::Impl( std::shared_ptr < ResourceMap > pMap, int id ) :	GameObjectImplBase(),
																				m_pResourceMap( pMap ),
																				m_ShotID( id ),
																				m_Colided( false ),
																				m_pPlayer( NULL ),
																				m_pEnemy( NULL )
	{
		m_PosX = 0.0f;
		m_PosY = 0.0f;
		m_ColRadius = 8.0f;
		m_Counter = 0;
		m_Speed = 0.0f;
		m_Angle = 0.0f;
		m_ShotPower = 0;
	}

	PlayerShot::Impl::~Impl()
	{
	}

	void PlayerShot::Impl::Draw()
	{
		int rest = m_Counter % 6;
		if( ( rest ) >= 2 ){
			int alpha = ( rest ) * 50;
			if( m_ShotID == PLAYER_SHOT_ID_NORMAL_MAIN ){
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_PLAYER_SHOT_1_TEXTURE ],
									m_PosX, m_PosY, true, alpha << 24 | 0xFFFFFF );
			}
			else if( m_ShotID == PLAYER_SHOT_ID_GREEN_MAIN ){
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_PLAYER_SHOT_GREEN_MAIN ],
									m_PosX, m_PosY, m_Angle - MAPIL::DegToRad( 90.0f ), true, alpha << 24 | 0xFFFFFF );
			}
		}

		if( m_ShotID == PLAYER_SHOT_ID_RED_MAIN ){
			MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_PLAYER_SHOT_RED_MAIN ],
								m_PosX, m_PosY, m_Angle - static_cast < float > ( MAPIL::DegToRad( 90.0f ) ), true, 0x55FFFFFF );
		}
		else if( m_ShotID == PLAYER_SHOT_ID_BLUE_MAIN ){
			MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_PLAYER_SHOT_BLUE_MAIN ],
								m_PosX, m_PosY, 1.0f + ( m_ShotPower - 4 ) * 0.3f, 1.0f, true, 0x55FFFFFF );
		}
		else if( m_ShotID == PLAYER_SHOT_ID_GREEN_BOMB ){
			AddToSpriteBatch(	MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT, m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_3 ],
								m_PosX, m_PosY,
								0.0f,
								true, 0xFF33FF33 );
		}
	}

	bool PlayerShot::Impl::Update()
	{
		if( m_ShotID == PLAYER_SHOT_ID_NORMAL_MAIN || m_ShotID == PLAYER_SHOT_ID_BLUE_MAIN ){
			m_PosY -= 15.0f;
			if( m_pPlayer ){
				float x;
				float y;
				m_pPlayer->GetPos( &x, &y );
				m_PosX = x;
			}
			if( m_PosY <= -10.0f ){
				return false;
			}
		}
		else if( m_ShotID == PLAYER_SHOT_ID_GREEN_MAIN ){
			m_PosX += m_Speed * ::cos( m_Angle );
			m_PosY -= m_Speed * ::sin( m_Angle );
			if( m_PosY <= -10.0f ){
				return false;
			}
		}
		else if( m_ShotID == PLAYER_SHOT_ID_RED_MAIN ){
			m_PosX += m_Speed * ::cos( m_Angle );
			m_PosY -= m_Speed * ::sin( m_Angle );
			if( m_PosY <= -10.0f || m_PosY >= 500.0f || m_PosX <= 100.0f || m_PosX >= 550.0f ){
				return false;
			}
		}
		else if( m_ShotID == PLAYER_SHOT_ID_GREEN_BOMB ){
	
			if( m_Speed != 7.0f ){
				m_ColRadius = 30.0f;
				if( m_pStageData->m_EnemyList.size() > 0 ){
					EnemyList::iterator it = m_pStageData->m_EnemyList.begin();
					int random = m_pStageData->m_RandGen.GetRand() * 10 / m_pStageData->m_RandGen.GetRandMax();
					int offset = random % m_pStageData->m_EnemyList.size();
					for( int i = 0; i < offset; ++i ){
						++it;
					}
					m_Speed = 7.0f;
					float eX;
					float eY;
					( *it )->GetPos( &eX, &eY );
					m_Angle = ::atan2( eY - m_PosY, eX - m_PosX );
				}
				else if( m_pStageData->m_pBoss ){
					m_Speed = 7.0f;
					float eX;
					float eY;
					m_pStageData->m_pBoss->GetPos( &eX, &eY );
					m_Angle = ::atan2( eY - m_PosY, eX - m_PosX );
				}
			}

			m_PosX += m_Speed * ::cos( m_Angle );
			m_PosY += m_Speed * ::sin( m_Angle );
			if( m_PosY <= -10.0f || m_PosY >= 500.0f || m_PosX <= 100.0f || m_PosX >= 550.0f ){
				return false;
			}
		}

		

		if( m_Colided ){
			return false;
		}

		++m_Counter;

		return true;
	}

	inline void PlayerShot::Impl::GetPos( float* pX, float* pY )
	{
		*pX = m_PosX;
		*pY = m_PosY;
	}

	inline void PlayerShot::Impl::SetPos( const Player& player )
	{
	}

	inline void PlayerShot::Impl::SetPos( float posX, float posY )
	{
		m_PosX = posX;
		m_PosY = posY;
	}

	inline void PlayerShot::Impl::SetAngle( float angle )
	{
		m_Angle = angle;
	}

	inline void PlayerShot::Impl::SetSpeed( float speed )
	{
		m_Speed = speed;
	}

	inline void PlayerShot::Impl::SetShotPower( int power )
	{
		m_ShotPower = power;
	}

	inline void PlayerShot::Impl::SetConsAttr( int attr )
	{
		m_ConsAttr = attr;
	}

	inline void PlayerShot::Impl::ProcessCollision( Enemy* pEnemy )
	{
		if( !pEnemy->IsNonCollisionMode() ){
			m_Colided = true;
		}
	}

	inline float PlayerShot::Impl::GetCollisionRadius() const
	{
		return m_ColRadius;
	}

	inline int PlayerShot::Impl::GetShotPower() const
	{
		return m_ShotPower;
	}

	inline int PlayerShot::Impl::GetConsAttr() const
	{
		return m_ConsAttr;
	}

	inline void PlayerShot::Impl::SetPlayer( Player* pPlayer )
	{
		m_pPlayer = pPlayer;
	}

	inline void PlayerShot::Impl::SetEnemy( Enemy* pEnemy )
	{
		m_pEnemy = pEnemy;
	}

	inline int PlayerShot::Impl::GetID() const
	{
		return m_ShotID;
	}

	inline void PlayerShot::Impl::SetStageData( StageData* pData )
	{
		m_pStageData = pData;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	PlayerShot::PlayerShot( std::shared_ptr < ResourceMap > pMap, int id ) :	CollisionObject(),
																				m_pImpl( new PlayerShot::Impl( pMap, id ) )
	{
	}

	PlayerShot::~PlayerShot()
	{
	}

	void PlayerShot::Init( float posX, float posY )
	{
	}

	void PlayerShot::Draw()
	{
		m_pImpl->Draw();
	}

	bool PlayerShot::Update()
	{
		return m_pImpl->Update();
	}

	void PlayerShot::SetPos( const Player& player )
	{
		m_pImpl->SetPos( player );
	}

	void PlayerShot::SetPos( float posX, float posY )
	{
		m_pImpl->SetPos( posX, posY );
	}

	void PlayerShot::SetAngle( float angle )
	{
		m_pImpl->SetAngle( angle );
	}

	void PlayerShot::SetSpeed( float speed )
	{
		m_pImpl->SetSpeed( speed );
	}

	void PlayerShot::SetShotPower( int power )
	{
		m_pImpl->SetShotPower( power );
	}

	void PlayerShot::SetConsAttr( int attr )
	{
		m_pImpl->SetConsAttr( attr );
	}

	void PlayerShot::Colided( CollisionObject* pObject )
	{
		pObject->ProcessCollision( this );
	}

	void PlayerShot::ProcessCollision( Player* pPlayer )
	{
	}

	void PlayerShot::ProcessCollision( Enemy* pEnemy )
	{
		m_pImpl->ProcessCollision( pEnemy );
	}

	void PlayerShot::ProcessCollision( PlayerShot* pPlayerShot )
	{
	}

	void PlayerShot::ProcessCollision( EnemyShot* pEnemyShot )
	{
	}

	void PlayerShot::ProcessCollision( Item* pItem )
	{
	}

	void PlayerShot::GetPos( float* pPosX, float* pPosY )
	{
		m_pImpl->GetPos( pPosX, pPosY );
	}

	float PlayerShot::GetCollisionRadius()
	{
		return m_pImpl->GetCollisionRadius();
	}

	int PlayerShot::GetShotPower() const
	{
		return m_pImpl->GetShotPower();
	}

	int PlayerShot::GetConsAttr() const
	{
		return m_pImpl->GetConsAttr();
	}

	void PlayerShot::SetPlayer( Player* pPlayer )
	{
		m_pImpl->SetPlayer( pPlayer );
	}

	void PlayerShot::SetEnemy( Enemy* pEnemy )
	{
		m_pImpl->SetEnemy( pEnemy );
	}

	int PlayerShot::GetID() const
	{
		return m_pImpl->GetID();
	}

	void PlayerShot::SetStageData( StageData* pData )
	{
		m_pImpl->SetStageData( pData );
	}
}
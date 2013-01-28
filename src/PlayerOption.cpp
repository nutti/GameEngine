#include <MAPIL/MAPIL.h>

#include "PlayerOption.h"
#include "GameObjectImplBase.h"
#include "Player.h"
#include "PlayerShot.h"
#include "EnemyShot.h"
#include "Item.h"

#include "Stage.h"
#include "Util.h"

namespace GameEngine
{

	class PlayerOption::Impl : public GameObjectImplBase
	{
	private:

		const int ALIVAL_FRAME;		// 生存期間

		struct PrivateData
		{
			float			m_BasePosX;
			float			m_BasePosY;
			float			m_PosX;
			float			m_PosY;
			int				m_Counter;
			int				m_ID;
			int				m_ConsID;
			int				m_OptTotal;
		};

		ButtonStatusHolder					m_ButtonStatus;
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		StageData*							m_pStageData;
		PrivateData							m_PrivData;

		void NormalModeShot();
		
	public:
		Impl( std::shared_ptr < ResourceMap > pMap, StageData* pStageData, int id );
		~Impl();
		void AttachButtonState( const ButtonStatusHolder& holder );		// キー入力を設定
		void Init( float posX, float posY );							// 初期化
		void Draw();													// 描画
		bool Update();													// 更新
		void ProcessCollision( Player* pPlayer );						// 衝突時の処理（プレイヤー）
		void ProcessCollision( Enemy* pEnemy );							// 衝突時の処理（敵）
		void ProcessCollision( PlayerShot* pPlayerShot );				// 衝突時の処理（プレイヤーショット）
		void ProcessCollision( EnemyShot* pEnemyShot );					// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );							// 衝突時の処理（アイテム）
		void GetPos( float* pPosX, float* pPosY );						// 位置を取得
		void SetPos( float posX, float posY );
		void ChangeID( int id );
		void ChangeConsMode( int mode );
		void NotifyOptTotal( int total );
	};

	PlayerOption::Impl::Impl( std::shared_ptr < ResourceMap > pMap, StageData* pStageData, int id )	:	m_pStageData( pStageData ),
																										m_pResourceMap( pMap ),
																										ALIVAL_FRAME( 1500 )
	{
		m_PrivData.m_Counter = 0;
		m_PrivData.m_ID = id;
		m_PrivData.m_ConsID = PLAYER_CONS_MODE_NORMAL;
	}

	PlayerOption::Impl::~Impl()
	{
	}

	void PlayerOption::Impl::NormalModeShot()
	{
		if( ( m_PrivData.m_Counter % 6 ) == 0 ){
			PlayerShot* pNewShot = m_pStageData->m_ObjBuilder.CreatePlayerShot( 0 );
			pNewShot->SetPos( m_PrivData.m_PosX, m_PrivData.m_PosY );
			pNewShot->SetShotPower( 5 );
			m_pStageData->m_PlayerShotList.push_back( pNewShot );
		}
	}

	void PlayerOption::Impl::AttachButtonState( const ButtonStatusHolder& holder )
	{
		m_ButtonStatus = holder;
	}

	void PlayerOption::Impl::Init( float posX, float posY )
	{
		m_PrivData.m_PosX = posX;
		m_PrivData.m_PosY = posY;
	}

	void PlayerOption::Impl::Draw()
	{
		
		MAPIL::DrawString( m_PrivData.m_PosX, m_PrivData.m_PosY, "■ %d", ALIVAL_FRAME - m_PrivData.m_Counter );
	}

	bool PlayerOption::Impl::Update()
	{
		if( m_PrivData.m_Counter >= ALIVAL_FRAME ){
			return false;
		}

		if( m_PrivData.m_ConsID == PLAYER_CONS_MODE_NORMAL ){
			const float offsets[ 4 ] = { 30.0f, -30.0f, 60.0f, -60.0f };
			m_PrivData.m_PosX = m_PrivData.m_BasePosX + offsets[ m_PrivData.m_ID ];
			m_PrivData.m_PosY = m_PrivData.m_BasePosY;
		}
		else if( m_PrivData.m_ConsID == PLAYER_CONS_MODE_GREEN ){
			const float offsetsX[ 4 ] = { 15.0f, -15.0f, 30.0f, -30.0f };
			const float offsetsY[ 4 ] = { 30.0f, 30.0f, 15.0f, 15.0f };
			m_PrivData.m_PosX = m_PrivData.m_BasePosX + offsetsX[ m_PrivData.m_ID ];
			m_PrivData.m_PosY = m_PrivData.m_BasePosY + offsetsY[ m_PrivData.m_ID ];
		}
		else if( m_PrivData.m_ConsID == PLAYER_CONS_MODE_BLUE ){
			m_PrivData.m_PosX = m_PrivData.m_BasePosX;
			m_PrivData.m_PosY = m_PrivData.m_BasePosY - 30.0f;
		}
		else if( m_PrivData.m_ConsID == PLAYER_CONS_MODE_RED ){
			const float offsetAngle[ 4 ] = { 0.0f, 180.0f, 120.0f, 90.0f };
			m_PrivData.m_PosX = m_PrivData.m_BasePosX + 40.0f * sin( MAPIL::DegToRad( m_pStageData->m_Frame * 4.0f + m_PrivData.m_ID * offsetAngle[ m_PrivData.m_OptTotal - 1 ] ) );
			m_PrivData.m_PosY = m_PrivData.m_BasePosY + 40.0f * cos( MAPIL::DegToRad( m_pStageData->m_Frame * 4.0f + m_PrivData.m_ID * offsetAngle[ m_PrivData.m_OptTotal - 1 ] ) );
		}

		if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			NormalModeShot();
		}

		++m_PrivData.m_Counter;

		return true;
	}

	void PlayerOption::Impl::ProcessCollision( Player* pPlayer )
	{
	}

	void PlayerOption::Impl::ProcessCollision( Enemy* pEnemy )
	{
	}

	void PlayerOption::Impl::ProcessCollision( PlayerShot* pPlayerShot )
	{
	}

	void PlayerOption::Impl::ProcessCollision( EnemyShot* pEnemyShot )
	{
	}

	void PlayerOption::Impl::ProcessCollision( Item* pItem )
	{
	}

	void PlayerOption::Impl::GetPos( float* pPosX, float* pPosY )
	{
		*pPosX = m_PrivData.m_BasePosX;
		*pPosY = m_PrivData.m_BasePosY;
	}

	void PlayerOption::Impl::SetPos( float posX, float posY )
	{
		m_PrivData.m_BasePosX = posX;
		m_PrivData.m_BasePosY = posY;
	}

	void PlayerOption::Impl::ChangeID( int id )
	{
		m_PrivData.m_ID = id;
	}

	void PlayerOption::Impl::ChangeConsMode( int mode )
	{
		m_PrivData.m_ConsID = mode;
	}

	void PlayerOption::Impl::NotifyOptTotal( int total )
	{
		m_PrivData.m_OptTotal = total;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	PlayerOption::PlayerOption( std::shared_ptr < ResourceMap > pMap, StageData* pStageData, int id ) :	CollisionObject(),
																										m_pImpl( new PlayerOption::Impl( pMap, pStageData, id ) )
	{
	}

	PlayerOption::~PlayerOption()
	{
	}

	void PlayerOption::AttachButtonState( const ButtonStatusHolder& holder )
	{
		m_pImpl->AttachButtonState( holder );
	}

	void PlayerOption::Init( float posX, float posY )
	{
		m_pImpl->Init( posX, posY );
	}

	void PlayerOption::Draw()
	{
		m_pImpl->Draw();
	}

	bool PlayerOption::Update()
	{
		return m_pImpl->Update();
	}

	void PlayerOption::Colided( CollisionObject* pObject )
	{
		//pObject->ProcessCollision( this );
	}

	void PlayerOption::ProcessCollision( Player* pPlayer )
	{
		m_pImpl->ProcessCollision( pPlayer );
	}

	void PlayerOption::ProcessCollision( Enemy* pEnemy )
	{
		m_pImpl->ProcessCollision( pEnemy );
	}

	void PlayerOption::ProcessCollision( PlayerShot* pPlayerShot )
	{
		m_pImpl->ProcessCollision( pPlayerShot );
	}

	void PlayerOption::ProcessCollision( EnemyShot* pEnemyShot )
	{
		m_pImpl->ProcessCollision( pEnemyShot );
	}

	void PlayerOption::ProcessCollision( Item* pItem )
	{
		m_pImpl->ProcessCollision( pItem );
	}

	void PlayerOption::GetPos( float* pPosX, float* pPosY )
	{
		m_pImpl->GetPos( pPosX, pPosY );
	}

	void PlayerOption::SetPos( float posX, float posY )
	{
		m_pImpl->SetPos( posX, posY );
	}

	float PlayerOption::GetCollisionRadius()
	{
		return 0.0f;
	}

	void PlayerOption::ChangeID( int id )
	{
		m_pImpl->ChangeID( id );
	}

	void PlayerOption::ChangeConsMode( int mode )
	{
		m_pImpl->ChangeConsMode( mode );
	}

	void PlayerOption::NotifyOptTotal( int total )
	{
		m_pImpl->NotifyOptTotal( total );
	}
}
#include <MAPIL/MAPIL.h>

#include "Item.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"
#include "Effect.h"
#include "ResourceTypes.h"
#include "ResourceID.h"

namespace GameEngine
{
	Item::Item( std::shared_ptr < ResourceMap > pMap, StageData* pStageData, int id, int subID ) :	CollisionObject(),
																									m_pResourceMap( pMap )
	{
		m_ItemData.m_GUData.m_ColRadius = GameUnit( 3 );
		m_ItemData.m_GUData.m_PosX = GameUnit( 0 );
		m_ItemData.m_GUData.m_PosY = GameUnit( 0 );
		m_ItemData.m_GUData.m_Vel = GameUnit( -2 );
		m_ItemData.m_GUData.m_Angle = GameUnit( 0 );

		m_ItemData.m_ItemID = id;
		m_ItemData.m_ItemSubID = subID;
		m_ItemData.m_Near = false;
		m_ItemData.m_Counter = 0;
		m_ItemData.m_ConsumedCounter = 0;
		m_pPlayer = NULL;
		m_ItemData.m_StatusFlags.reset();
		m_ItemData.m_pStageData = pStageData;
	}

	Item::~Item()
	{
	}

	void Item::Draw()
	{
		float posX = m_ItemData.m_GUData.m_PosX.GetFloat();
		float posY = m_ItemData.m_GUData.m_PosY.GetFloat();

		if( m_ItemData.m_ItemID == ITEM_ID_POWER_UP ){
			float scaleX = 0.8f;
			float scaleY = 0.8f;
			if( m_ItemData.m_Counter <= 10 ){
				scaleY = 0.8f * m_ItemData.m_Counter / 10.0f;
				scaleX = 0.8f + ( 10 - m_ItemData.m_Counter ) * 0.1f;
			}
			if( ( m_ItemData.m_Counter % 100 ) > 50 ){
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_POWER_UP_ITEM_1_TEXTURE ],
									posX,
									posY,
									scaleX, scaleY );
			}
			else{
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_POWER_UP_ITEM_2_TEXTURE ],
									posX,
									posY,
									scaleX, scaleY );
			}
		}
		else if( m_ItemData.m_ItemID == ITEM_ID_CRYSTAL ){
			float scaleX = m_ItemData.m_ItemSubID * 0.03f + 0.1f;
			float scaleY = m_ItemData.m_ItemSubID * 0.03f + 0.1f;
			if( m_ItemData.m_Counter <= 10 ){
				scaleY += 0.3f * m_ItemData.m_Counter / 10.0f;
				scaleX += 0.3f + ( 10 - m_ItemData.m_Counter ) * 0.1f;
			}
			MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE ],
								posX,
								posY,
								scaleX, scaleY );
		}
		else if( m_ItemData.m_ItemID == ITEM_ID_RECOVER ){
				float scaleX = 0.8f;
				float scaleY = 0.8f;
				if( m_ItemData.m_Counter <= 10 ){
					scaleY = 0.8f * m_ItemData.m_Counter / 10.0f;
					scaleX = 0.8f + ( 10 - m_ItemData.m_Counter ) * 0.1f;
				}
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_ITEM_RECOVER_1 + ( m_ItemData.m_Counter / 60 ) % 2 ],
									posX,
									posY,
									scaleX, scaleY );
		}
		else if( m_ItemData.m_ItemID == ITEM_ID_CONS_LEVEL_RECOVER ){
			if( m_ItemData.m_ItemSubID == ITEM_SUB_ID_GREEN ){
				float scaleX = 0.5f;
				float scaleY = 0.5f;
				if( m_ItemData.m_Counter <= 10 ){
					scaleY = 0.5f * m_ItemData.m_Counter / 10.0f;
					scaleX = 0.5f + ( 10 - m_ItemData.m_Counter ) * 0.1f;
				}
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_ITEM_CONS_LEVEL_RECOVER_1 + ( m_ItemData.m_Counter / 10 ) % 5 ],
									posX,
									posY,
									scaleX, scaleY );
			}
		}
	}

	bool Item::Update()
	{
		if( m_ItemData.m_Near ){
			GameUnit px;
			GameUnit py;
			m_pPlayer->GetPos( &px, &py );
			m_ItemData.m_GUData.m_Angle = Atan2GU( py - m_ItemData.m_GUData.m_PosY, m_ItemData.m_GUData.m_PosX - px );
			m_ItemData.m_GUData.m_PosX -= ( GameUnit( 6 ) * CosGU( m_ItemData.m_GUData.m_Angle ) );
			m_ItemData.m_GUData.m_PosY += ( GameUnit( 6 ) * SinGU( m_ItemData.m_GUData.m_Angle ) );
		}
		else{
			if( m_ItemData.m_Counter < 100 ){
				m_ItemData.m_GUData.m_Vel += GameUnit( 0.04f );
			}
			m_ItemData.m_GUData.m_PosY += m_ItemData.m_GUData.m_Vel;
		}
		
		if(	m_ItemData.m_GUData.m_PosY > GameUnit( 500 ) ){
			return false;
		}

		if(	m_ItemData.m_StatusFlags[ STATUS_FLAG_CONSUMED ] ||
			m_ItemData.m_StatusFlags[ STATUS_FLAG_OBTAINED ] ){
			if( m_ItemData.m_StatusFlags[ STATUS_FLAG_OBTAINED ] ){
				if( m_ItemData.m_ItemID == ITEM_ID_CRYSTAL ){
					Effect* pNewEffect = m_ItemData.m_pStageData->m_ObjBuilder.CreateEffect( EFFECT_ID_OBTAIN_ITEM_CRYSTAL, m_ItemData.m_ItemSubID );
					pNewEffect->SetPos( m_ItemData.m_GUData.m_PosX.GetFloat(), m_ItemData.m_GUData.m_PosY.GetFloat() );
					m_ItemData.m_pStageData->m_EffectList.push_back( pNewEffect );
					// ステージにメッセージ送信
					/*StageMessage msg;
					msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_ITEM_OBTAINED;
					StageMessage::StageMessageData data;
					data.m_Integer = ITEM_ID_CRYSTAL;
					msg.m_MsgDataList.push_back( data );
					m_ItemData.m_pStageData->m_MsgQueue.push( msg );*/
				}
			}
			return false;
		}

		++m_ItemData.m_Counter;

		return true;
	}

	void Item::Colided( CollisionObject* pObject )
	{
		pObject->ProcessCollision( this );
	}

	void Item::ProcessCollision( Player* pPlayer )
	{
		m_ItemData.m_StatusFlags.set( STATUS_FLAG_OBTAINED );
	}

	void Item::ProcessCollision( Enemy* pEnemy )
	{
		if( pEnemy->IsNonCollisionMode() ){
			return;
		}

		if( m_ItemData.m_ConsumedCounter < 15 && m_ItemData.m_Counter > 120 ){
			++m_ItemData.m_ConsumedCounter;
			return;
		}
		
		if( !m_ItemData.m_StatusFlags[ STATUS_FLAG_CONSUMED ] ){
			return;
		}
	}

	void Item::ProcessCollision( EnemyShot* pEnemyShot )
	{
	}

	void Item::ProcessCollision( PlayerShot* pPlayerShot )
	{
	}

	void Item::ProcessCollision( Item* pItem )
	{
	}
	void Item::Init( const GameUnit& posX, const GameUnit& posY )
	{
		m_ItemData.m_GUData.m_PosX = posX;
		m_ItemData.m_GUData.m_PosY = posY;
	}

	void Item::SetPos( const GameUnit& posX, const GameUnit& posY )
	{
		m_ItemData.m_GUData.m_PosX = posX;
		m_ItemData.m_GUData.m_PosY = posY;
	}

	void Item::GetPos( GameUnit* pPosX, GameUnit* pPosY )
	{
		*pPosX = m_ItemData.m_GUData.m_PosX;
		*pPosY = m_ItemData.m_GUData.m_PosY;
	}

	GameUnit Item::GetCollisionRadius()
	{
		return m_ItemData.m_GUData.m_ColRadius;
	}

	int Item::GetItemID() const
	{
		return m_ItemData.m_ItemID;
	}

	int Item::GetItemSubID() const
	{
		return m_ItemData.m_ItemSubID;
	}

	void Item::PlayerIsNear( Player* pPlayer )
	{
		m_pPlayer = pPlayer;
		m_ItemData.m_Near = true;
	}

	bool Item::CanBeConsumed() const
	{
		if( m_ItemData.m_ConsumedCounter < 15 ){
			return false;
		}

		return true;
	}

	void Item::Consume()
	{
		m_ItemData.m_StatusFlags.set( STATUS_FLAG_CONSUMED );
	}
}
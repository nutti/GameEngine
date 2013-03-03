#include <MAPIL/MAPIL.h>

#include "Item.h"
#include "Player.h"
#include "ResourceTypes.h"
#include "ResourceID.h"

namespace GameEngine
{
	Item::Item( std::shared_ptr < ResourceMap > pMap, int id, int subID ) :	CollisionObject(),
																			m_pResourceMap( pMap )
	{
		m_ItemData.m_ItemID = id;
		m_ItemData.m_ItemSubID = subID;
		m_ItemData.m_Colided = false;
		m_ItemData.m_ColRadius = 3.0f;
		m_ItemData.m_PosX = 0.0f;
		m_ItemData.m_PosY = 0.0f;
		m_ItemData.m_Near = false;
		m_ItemData.m_Vel = -2.0f;
		m_ItemData.m_Angle = 0.0f;
		m_ItemData.m_Counter = 0;
		m_pPlayer = NULL;
		m_ItemData.m_StatusFlags.reset();
	}

	Item::~Item()
	{
	}

	void Item::Init( float posX, float posY )
	{
		m_ItemData.m_PosX = posX;
		m_ItemData.m_PosY = posY;
	}

	void Item::Draw()
	{
		

		if( m_ItemData.m_ItemID == ITEM_ID_POWER_UP ){
			float scaleX = 0.8f;
			float scaleY = 0.8f;
			if( m_ItemData.m_Counter <= 10 ){
				scaleY = 0.8f * m_ItemData.m_Counter / 10.0f;
				scaleX = 0.8f + ( 10 - m_ItemData.m_Counter ) * 0.1f;
			}
			if( ( m_ItemData.m_Counter % 100 ) > 50 ){
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_POWER_UP_ITEM_1_TEXTURE ],
									m_ItemData.m_PosX,
									m_ItemData.m_PosY,
									scaleX, scaleY );
			}
			else{
				MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_POWER_UP_ITEM_2_TEXTURE ],
									m_ItemData.m_PosX,
									m_ItemData.m_PosY,
									scaleX, scaleY );
			}
		}
		else if( m_ItemData.m_ItemID == ITEM_ID_CRYSTAL ){
			float scaleX = m_ItemData.m_ItemSubID * 0.025f;
			float scaleY = m_ItemData.m_ItemSubID * 0.025f;
			if( m_ItemData.m_Counter <= 10 ){
				scaleY += 0.3f * m_ItemData.m_Counter / 10.0f;
				scaleX += 0.3f + ( 10 - m_ItemData.m_Counter ) * 0.1f;
			}
			MAPIL::DrawTexture(	m_pResourceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE ],
								m_ItemData.m_PosX,
								m_ItemData.m_PosY,
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
									m_ItemData.m_PosX,
									m_ItemData.m_PosY,
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
									m_ItemData.m_PosX,
									m_ItemData.m_PosY,
									scaleX, scaleY );
			}
		}
	}

	bool Item::Update()
	{
		if( m_ItemData.m_Near ){
			float px;
			float py;
			m_pPlayer->GetPos( &px, &py );
			m_ItemData.m_Angle = ::atan2( py - m_ItemData.m_PosY, m_ItemData.m_PosX - px );
			m_ItemData.m_PosX -= 6.0f * ::cos( m_ItemData.m_Angle );
			m_ItemData.m_PosY += 6.0f * ::sin( m_ItemData.m_Angle );
		}
		else{
			if( m_ItemData.m_Counter < 100 ){
				m_ItemData.m_Vel += 0.04f;
			}
			m_ItemData.m_PosY += m_ItemData.m_Vel;
		}
		
		if(	m_ItemData.m_PosY > 500.0f ){
			return false;
		}

		if( m_ItemData.m_Colided ){
			return false;
		}

		++m_ItemData.m_Counter;

		return true;
	}

	
	void Item::SetPos( float posX, float posY )
	{
		m_ItemData.m_PosX = posX;
		m_ItemData.m_PosY = posY;
	}

	void Item::Colided( CollisionObject* pObject )
	{
		pObject->ProcessCollision( this );
	}

	void Item::ProcessCollision( Player* pPlayer )
	{
		m_ItemData.m_Colided = true;
	}

	void Item::ProcessCollision( Enemy* pEnemy )
	{
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

	void Item::GetPos( float* pPosX, float* pPosY )
	{
		*pPosX = m_ItemData.m_PosX;
		*pPosY = m_ItemData.m_PosY;
	}

	float Item::GetCollisionRadius()
	{
		return m_ItemData.m_ColRadius;
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
}
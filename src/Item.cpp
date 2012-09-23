#include <MAPIL/MAPIL.h>

#include "Item.h"

namespace GameEngine
{
	Item::Item( std::shared_ptr < ResourceMap > pMap, int id ) :	CollisionObject(),
																	m_pResourceMap( pMap )
	{
		m_ItemData.m_ItemID = id;
		m_ItemData.m_Colided = false;
		m_ItemData.m_ColRadius = 3.0f;
		m_ItemData.m_PosX = 0.0f;
		m_ItemData.m_PosY = 0.0f;
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
		MAPIL::DrawString( m_ItemData.m_PosX, m_ItemData.m_PosY, "~" );
	}

	bool Item::Update()
	{
		m_ItemData.m_PosY += 2.0f;
		
		if(	m_ItemData.m_PosY > 500.0f ||
			m_ItemData.m_PosY < -10.0f ||
			m_ItemData.m_PosX < 100.0f ||
			m_ItemData.m_PosX > 550.0f ){
			return false;
		}

		if( m_ItemData.m_Colided ){
			return false;
		}

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
}
#include <MAPIL/MAPIL.h>

#include <bitset>

#include "LaserShotImpl.h"
#include "../../ResourceTypes.h"
#include "../../EnemyShotGroup.h"
#include "../../GameObjectImplBase.h"

#include "../../SpriteBatch.h"

namespace GameEngine
{

	LaserShotImpl::LaserShotImpl( std::shared_ptr < ResourceMap > pMap, int id ) :	EnemyShotImpl( pMap, id )
	{
		m_AlphaBlendingMode = MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT;
		m_TexColor = 0;
	}

	LaserShotImpl::~LaserShotImpl()
	{
		if( m_ShotGroupData.m_pShotGroup ){
			m_ShotGroupData.m_pShotGroup->DeleteShot( m_ShotGroupData.m_ID );
		}
		MAPIL::ZeroObject( &m_ShotGroupData, sizeof( m_ShotGroupData ) );
	}


	void LaserShotImpl::Draw()
	{
		if( m_StatusFlags[ INVISIBLE ] ){
			return;
		}

		float posX = m_GUData.m_PosX.GetFloat();
		float posY = m_GUData.m_PosY.GetFloat();
		int color = 0xFFFFFFFF;
		int scale = m_ImgScale;
		float multiplicity = 1;

		if( m_StatusFlags[ DEAD ] ){
			color = ( ( 20 - m_DeadCounter ) * 5 ) << 24 | 0xFFFFFF;
			scale += m_DeadCounter * 0.05f;
		}

		if( m_Counter % 8 >= 4 ){
			multiplicity = 2;
		}

		float angle = ::atan2( m_Line.GetEndY() - m_Line.GetStartY(), -m_Line.GetEndX() + m_Line.GetStartX() ) - MAPIL::DegToRad( 90.0f );
		float length = std::sqrt(	( m_Line.GetEndX() - m_Line.GetStartX() ) * ( m_Line.GetEndX() - m_Line.GetStartX() ) + 
									( m_Line.GetEndY() - m_Line.GetStartY() ) * ( m_Line.GetEndY() - m_Line.GetStartY() ) );
		unsigned int texSizeX;
		unsigned int texSizeY;

		for( int i = 0; i < multiplicity; ++i ){
			if( m_AtlasImgID == -1 ){
				MAPIL::GetTextureSize( m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ], &texSizeX, &texSizeY );
				for( int i = 0; i < m_DrawingMultiplicity; ++i ){
					MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
										m_Line.GetStartX() - ( texSizeX * m_ImgScale / 2.0f ) * cos( angle ),
										m_Line.GetStartY() + ( texSizeX * m_ImgScale / 2.0f ) * sin( angle ),
										m_ImgScale,
										length / texSizeY,
										angle,
										false, color );
				}
			}
			else{
				ResourceMap::TextureAtlas atlas;
				atlas = m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ m_AtlasImgID ];
				texSizeX = atlas.m_Width;
				texSizeY = atlas.m_Height;

				for( int i = 0; i < m_DrawingMultiplicity; ++i ){
					AddToAtlasSpriteBatch(	m_AlphaBlendingMode,
											m_AtlasImgID,
											m_Line.GetStartX() - ( texSizeX * m_ImgScale / 2.0f ) * cos( angle ),
											m_Line.GetStartY() + ( texSizeX * m_ImgScale / 2.0f ) * sin( angle ),
											m_ImgScale,
											length / texSizeY, 
											angle, false, color );
				}
			}
		}
	}

	bool LaserShotImpl::Update()
	{
		if( m_StatusFlags[ PAUSED ] ){
			return true;
		}

		// メッセージ処理
		ProcessMessages();
		
		// 死亡判定処理
		if( m_StatusFlags[ DEAD ] ){
			++m_DeadCounter;
			if( m_DeadCounter >= 20 ){
				return false;
			}
		}
		else{
			m_GUData.m_PosX += m_GUData.m_Speed * CosGU( m_GUData.m_Angle );
			m_GUData.m_PosY -= m_GUData.m_Speed * SinGU( m_GUData.m_Angle );

			if(	m_GUData.m_PosX < GameUnit( 0 ) ||
				m_GUData.m_PosX > GameUnit( 640 ) ||
				m_GUData.m_PosY < GameUnit( -30 ) ||
				m_GUData.m_PosY > GameUnit( 500 ) ){
				return false;
			}

			float sx = m_GUData.m_PosX.GetFloat();
			float sy = m_GUData.m_PosY.GetFloat();
			float ex = m_GUData.m_PosX.GetFloat() + m_Length.GetFloat() * cos( m_GUData.m_Angle.GetFloat() );
			float ey = m_GUData.m_PosY.GetFloat() - m_Length.GetFloat() * sin( m_GUData.m_Angle.GetFloat() );
			
			m_Line.SetStartX( sx );
			m_Line.SetStartY( sy );
			m_Line.SetEndX( ex );
			m_Line.SetEndY( ey );
		}

		m_ImgRotAngle = MAPIL::DegToRad( m_GUData.m_Angle.GetFloat() + 90.0f );


		++m_Counter;

		return true;
	}


	void LaserShotImpl::ProcessCollision( Player* pPlayer )
	{
		if( !m_StatusFlags[ NOT_DELETE_BY_PLAYER_DAMAGE ] ){
			PrepDestroy();
		}
	}

	void LaserShotImpl::GetPos( GameUnit* pX, GameUnit* pY )
	{
		*pX = m_GUData.m_PosX;
		*pY = m_GUData.m_PosY;
	}


	void LaserShotImpl::SetPos( const GameUnit& posX, const GameUnit& posY )
	{
		m_GUData.m_PosX = posX;
		m_GUData.m_PosY = posY;
	}


	void LaserShotImpl::AddPos( const GameUnit& x, const GameUnit& y )
	{
		m_GUData.m_PosX += x;
		m_GUData.m_PosY += y;
	}

	bool LaserShotImpl::DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius )
	{
		Circle c;
		c.SetCenterX( x.GetFloat() );
		c.SetCenterY( y.GetFloat() );
		c.SetRadius( radius.GetFloat() );

		return m_Line.Colided( &c );
	}


	void LaserShotImpl::SetLinePos(	const GameUnit& x1,
										const GameUnit& y1,
										const GameUnit& x2,
										const GameUnit& y2,
										const GameUnit& thickness )
	{
		m_Line.SetStartX( x1.GetFloat() );
		m_Line.SetStartY( y1.GetFloat() );
		m_Line.SetEndX( x2.GetFloat() );
		m_Line.SetEndY( y2.GetFloat() );
		m_Line.SetThickness( thickness.GetFloat() );
	}

	void LaserShotImpl::SetLength( const GameUnit& length )
	{
		m_Length = length;
	}
}
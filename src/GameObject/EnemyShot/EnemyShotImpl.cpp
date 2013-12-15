#include <MAPIL/MAPIL.h>

#include <bitset>

#include "EnemyShotImpl.h"
#include "../../ResourceTypes.h"
#include "../../EnemyShotGroup.h"
#include "../../GameObjectImplBase.h"

#include "../../SpriteBatch.h"

namespace GameEngine
{

	EnemyShotImpl::EnemyShotImpl( std::shared_ptr < ResourceMap > pMap, int id ) :	m_pResourceMap( pMap ),
																			m_ShotID( id ),

																			m_ShotShape( SHOT_SHAPE_CIRCLE ),
																			m_Circle(),
																			m_Line()
	{
		m_Counter = 0;
		m_Attr = ENEMY_SHOT_ATTR_NORMAL;
		m_StatusFlags.reset();
		MAPIL::ZeroObject( &m_ShotGroupData, sizeof( m_ShotGroupData ) );
		m_Power = 1;
		m_AlphaBlendingMode = MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT;
		m_ImgRotAnglePerFrame = 0.0f;
		m_ImgRotMode = IMG_ROT_MODE_SYNC;
		m_ImgRotAngle = 0.0f;
		m_ImgRotAnglePerFrame = 0.0f;
		m_ImgScale = 1.0f;
		m_AtlasImgID = -1;
		m_DrawingMultiplicity = 1;
	}

	EnemyShotImpl::~EnemyShotImpl()
	{
		if( m_ShotGroupData.m_pShotGroup ){
			m_ShotGroupData.m_pShotGroup->DeleteShot( m_ShotGroupData.m_ID );
		}
		MAPIL::ZeroObject( &m_ShotGroupData, sizeof( m_ShotGroupData ) );
	}


	void EnemyShotImpl::Draw()
	{
		if( m_StatusFlags[ INVISIBLE ] ){
			return;
		}

		float posX = m_GUData.m_PosX.GetFloat();
		float posY = m_GUData.m_PosY.GetFloat();
		int color = 0xFFFFFFFF;
		int scale = m_ImgScale;

		if( m_StatusFlags[ DEAD ] ){
			color = ( ( 20 - m_DeadCounter ) * 5 ) << 24 | 0xFFFFFF;
			scale += m_DeadCounter * 0.05f;
		}

		if( m_ShotShape == SHOT_SHAPE_CIRCLE ){
			if( m_AlphaBlendingMode != MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT ){
				for( int i = 0; i < m_DrawingMultiplicity; ++i ){
					if( m_AtlasImgID == -1 ){
						AddToSpriteBatch(	m_AlphaBlendingMode,
											m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
											posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle, true, color );
					}
					else{
						AddToAtlasSpriteBatch(	false, m_AlphaBlendingMode,
												m_AtlasImgID,
												posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle, true, color );
					}
				}
			}
			else{
				for( int i = 0; i < m_DrawingMultiplicity; ++i ){
					if( m_AtlasImgID == -1 ){
						MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
											posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle, true, color );
					}
					else{
						ResourceMap::TextureAtlas atlas;
						atlas = m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ m_AtlasImgID ];
						MAPIL::DrawClipedTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ atlas.m_TexID ],
													posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle,
													atlas.m_X, atlas.m_Y, atlas.m_X + atlas.m_Width, atlas.m_Y + atlas.m_Height, true, color );
					}
				}
			}
		}
		else if( m_ShotShape == SHOT_SHAPE_LINE ){
			float angle = ::atan2( m_Line.GetEndY() - m_Line.GetStartY(), -m_Line.GetEndX() + m_Line.GetStartX() ) - MAPIL::DegToRad( 90.0f );
			float length = std::sqrt(	( m_Line.GetEndX() - m_Line.GetStartX() ) * ( m_Line.GetEndX() - m_Line.GetStartX() ) + 
										( m_Line.GetEndY() - m_Line.GetStartY() ) * ( m_Line.GetEndY() - m_Line.GetStartY() ) );
			unsigned int texSizeX;
			unsigned int texSizeY;
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
					AddToAtlasSpriteBatch(	false, m_AlphaBlendingMode,
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

	bool EnemyShotImpl::Update()
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

			if( m_ShotShape == SHOT_SHAPE_CIRCLE ){
				m_GUData.m_PosX += m_GUData.m_Speed * CosGU( m_GUData.m_Angle );
				m_GUData.m_PosY -= m_GUData.m_Speed * SinGU( m_GUData.m_Angle );
				m_Circle.SetCenterX( m_GUData.m_PosX.GetFloat() );
				m_Circle.SetCenterY( m_GUData.m_PosY.GetFloat() );
			}
			else if( m_ShotShape == SHOT_SHAPE_LINE ){
				m_GUData.m_PosX = m_Line.GetStartX();
				m_GUData.m_PosY = m_Line.GetStartY();
			}

			if(	m_GUData.m_PosX < GameUnit( 0 ) ||
				m_GUData.m_PosX > GameUnit( 640 ) ||
				m_GUData.m_PosY < GameUnit( -30 ) ||
				m_GUData.m_PosY > GameUnit( 500 ) ){
				return false;
			}
		}

		if( m_ImgRotMode == IMG_ROT_MODE_SYNC ){
			m_ImgRotAngle = MAPIL::DegToRad( m_GUData.m_Angle.GetFloat() + 90.0f );
		}
		else if( m_ImgRotMode == IMG_ROT_MODE_AUTO ){
			m_ImgRotAngle += m_ImgRotAnglePerFrame;
		}

		++m_Counter;

		return true;
	}
	

	int EnemyShotImpl::GetPower() const
	{
		return m_Power;
	}

	void EnemyShotImpl::SetPower( int power )
	{
		m_Power = power;
	}

	void EnemyShotImpl::SetImage( int id )
	{
		m_ImgID = id;
	}

	void EnemyShotImpl::SetAtlasImage( int id )
	{
		m_AtlasImgID = id;
	}

	void EnemyShotImpl::SetImageScale( float scale )
	{
		m_ImgScale = scale;
		m_StatusFlags.set( IMG_SCALE_CHANGED );
	}

	

	void EnemyShotImpl::SetConsAttr( int attr )
	{
		m_Attr = attr;
		m_StatusFlags.set( HAS_CONS_ATTR );
	}

	void EnemyShotImpl::ProcessCollision( Player* pPlayer )
	{
		if( !m_StatusFlags[ NOT_DELETE_BY_PLAYER_DAMAGE ] ){
			PrepDestroy();
		}
	}

	void EnemyShotImpl::JoinShotGroup( int id, EnemyShotGroup* pGroup )
	{
		m_ShotGroupData.m_ID = id;
		m_ShotGroupData.m_pShotGroup = pGroup;
	}

	int EnemyShotImpl::GetCounter() const
	{
		return m_Counter;
	}

	void EnemyShotImpl::GetPos( GameUnit* pX, GameUnit* pY )
	{
		*pX = m_GUData.m_PosX;
		*pY = m_GUData.m_PosY;
	}

	GameUnit EnemyShotImpl::GetAngle() const
	{
		return m_GUData.m_Angle;
	}

	void EnemyShotImpl::SetPos( const GameUnit& posX, const GameUnit& posY )
	{
		m_GUData.m_PosX = posX;
		m_GUData.m_PosY = posY;
	}

	void EnemyShotImpl::SetAngle( const GameUnit& angle )
	{
		m_GUData.m_Angle = angle;
	}

	void EnemyShotImpl::SetSpeed( const GameUnit& speed )
	{
		m_GUData.m_Speed = speed;
	}

	void EnemyShotImpl::SetCollisionRadius( const GameUnit& radius )
	{
		m_GUData.m_ColRadius = radius;
		if( m_ShotShape == SHOT_SHAPE_CIRCLE ){
			m_Circle.SetRadius( radius.GetFloat() );
		}
	}

	GameUnit EnemyShotImpl::GetCollisionRadius() const
	{
		return m_GUData.m_ColRadius;
	}

	void EnemyShotImpl::AddPos( const GameUnit& x, const GameUnit& y )
	{
		m_GUData.m_PosX += x;
		m_GUData.m_PosY += y;
	}

	void EnemyShotImpl::AddAngle( const GameUnit& angle )
	{
		m_GUData.m_Angle += angle;
	}

	void EnemyShotImpl::AddSpeed( const GameUnit& speed )
	{
		m_GUData.m_Speed += speed;
	}

	bool EnemyShotImpl::DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius )
	{
		Circle c;
		c.SetCenterX( x.GetFloat() );
		c.SetCenterY( y.GetFloat() );
		c.SetRadius( radius.GetFloat() );

		if( m_ShotShape == SHOT_SHAPE_CIRCLE ){
			return m_Circle.Colided( &c );
		}
		else if( m_ShotShape == SHOT_SHAPE_LINE ){
			return m_Line.Colided( &c );
		}

		return false;
	}
	
	void EnemyShotImpl::ProcessMessages()
	{
		while( !m_MsgQueue.empty() ){
			int msg = m_MsgQueue.front().m_MsgID;
			switch( msg ){
				case EnemyShotMessage::ENEMY_SHOT_MESSAGE_ID_PLAYER_DAMAGED:
					Delete( DELETE_BY_PLAYER_DAMAGE );
					break;
				case EnemyShotMessage::ENEMY_SHOT_MESSAGE_ID_PLAYER_BOMBED:
					Delete( DELETE_BY_PLAYER_SKILL );
					break;
				default:
					break;
			}
			m_MsgQueue.pop();
		}
	}

	void EnemyShotImpl::PostMessage( int msgID )
	{
		EnemyShotMessage msg;
		msg.m_MsgID = msgID;
		m_MsgQueue.push( msg );
	}

	void EnemyShotImpl::PrepDestroy()
	{
		if( !m_StatusFlags[ DEAD ] ){
			m_DeadCounter = 0;
			m_StatusFlags.set( DEAD );
		}
	}

	bool EnemyShotImpl::IsDead() const
	{
		return m_StatusFlags[ DEAD ];
	}

	int EnemyShotImpl::GetConsAttr() const
	{
		return m_Attr;
	}

	void EnemyShotImpl::Pause()
	{
		m_StatusFlags.set( PAUSED );
	}

	void EnemyShotImpl::Resume()
	{
		m_StatusFlags.reset( PAUSED );
	}

	void EnemyShotImpl::SetImgRotMode( int mode )
	{
		m_ImgRotMode = mode;
	}

	void EnemyShotImpl::SetImgRotAnglePerFrame( float angle )
	{
		m_ImgRotAnglePerFrame = angle;
	}

	void EnemyShotImpl::SetAlphaBlendingMode( int mode )
	{
		m_AlphaBlendingMode = mode;
	}

	void EnemyShotImpl::SetShape( int shape )
	{
		m_ShotShape = shape;
	}

	void EnemyShotImpl::SetLinePos(	const GameUnit& x1,
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

	void EnemyShotImpl::EnableInvincibleMode()
	{
		m_StatusFlags.set( INVINCIBLE );
	}

	void EnemyShotImpl::DisableInvincibleMode()
	{

		m_StatusFlags.reset( INVINCIBLE );
	}

	void EnemyShotImpl::EnableInvisibleMode()
	{
		m_StatusFlags.set( INVISIBLE );
	}

	void EnemyShotImpl::DisableInvisibleMode()
	{
		m_StatusFlags.reset( INVISIBLE );
	}

	GameUnit EnemyShotImpl::GetSpeed() const
	{
		return m_GUData.m_Speed;
	}

	std::string EnemyShotImpl::GetMasterEnemyName() const
	{
		if( m_ShotGroupData.m_pShotGroup != NULL ){
			return m_ShotGroupData.m_pShotGroup->GetMasterEnemyName();
		}
		else{
			MAPIL::Assert( 0, CURRENT_POSITION, TSTR( "shot group is null pointer!!" ), -1 );
			return "";
		}
	}

	void EnemyShotImpl::SetDrawingMultiplicity( int num )
	{
		m_DrawingMultiplicity = num;
	}

	void EnemyShotImpl::Delete( int by )
	{
		if( m_StatusFlags[ DEAD ] ){
			return;
		}

		if( by == DELETE_BY_PLAYER_DAMAGE && m_StatusFlags[ NOT_DELETE_BY_PLAYER_DAMAGE ] ){
			return;
		}

		if( by == DELETE_BY_PLAYER_SKILL && m_StatusFlags[ NOT_DELETE_BY_PLAYER_SKILL ] ){
			return;
		}

		m_DeadCounter = 0;
		m_StatusFlags.set( DEAD );
	}

	void EnemyShotImpl::DeleteWhen( int when )
	{
		switch( when ){
			case DELETE_BY_PLAYER_DAMAGE:
				m_StatusFlags.reset( NOT_DELETE_BY_PLAYER_DAMAGE );
				break;
			case DELETE_BY_PLAYER_SKILL:
				m_StatusFlags.reset( NOT_DELETE_BY_PLAYER_SKILL );
				break;
			default:
				break;
		}
	}

	void EnemyShotImpl::NotDeleteWhen( int when )
	{
		switch( when ){
			case DELETE_BY_PLAYER_DAMAGE:
				m_StatusFlags.set( NOT_DELETE_BY_PLAYER_DAMAGE );
				break;
			case DELETE_BY_PLAYER_SKILL:
				m_StatusFlags.set( NOT_DELETE_BY_PLAYER_SKILL );
				break;
			default:
				break;
		}
	}
}
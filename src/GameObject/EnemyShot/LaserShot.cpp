#include <MAPIL/MAPIL.h>

#include <bitset>

#include "LaserShot.h"

#include "../../ResourceTypes.h"
#include "../../ResourceID.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int LAUNCH_EFFECT_TEX_ID		= 1024;
	static const int LASER_MAKER_TEX_ID			= 1016;
	static const int LASER_SHOT_TEX_ID			= 121;
	static const int LAUNCH_EFFECT_SE_ID		= GLOBAL_RESOURCE_SE_ID_ENEMY_LASER_SHOT;

	LaserShot::LaserShot( std::shared_ptr < ResourceMap > pMap, int id ) :	EnemyShot( pMap, id )
	{
		m_AlphaBlendingMode = MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT;
		m_GUData.m_ColRadiusBase = GameUnit( 2 );
		m_GUData.m_ColRadius = m_GUData.m_ColRadiusBase;
		m_Line.SetThickness( m_GUData.m_ColRadius.GetFloat() );
	}

	LaserShot::~LaserShot()
	{
	}

	void LaserShot::DrawEffect()
	{
		if( m_Counter >= 30 ){
			return;
		}

		float sx = 3.0f;
		float sy = 3.0f;
		int color = 0xFFFFFFFF;

		if( m_Counter < 10 ){
			sx = m_Counter * 0.3f;
			color = ( m_Counter * 25 ) << 24 | 0xFFFFFF;
		}
		else if( m_Counter > 20 && m_Counter <= 25 ){
			sy = ( 25 - m_Counter ) * 0.6f;
			color = ( ( 30 - m_Counter ) * 25 ) << 24 | 0xFFFFFF;
		}
		else if( m_Counter > 25 && m_Counter <= 30 ){
			sx = ( 30 - m_Counter ) * 0.6f;
			sy = ( m_Counter - 25 ) * 0.6f;
			color = ( ( 30 - m_Counter ) * 25 ) << 24 | 0xFFFFFF;
		}

		ResourceMap::TextureAtlas atlas;
		atlas = m_pResourceMap->m_pGlobalResourceMap->m_TexAtlasMap[ LAUNCH_EFFECT_TEX_ID + m_TexColor ];
		AddToAtlasSpriteBatch(	true,
								MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT,
								atlas.m_TexID,
								m_BeginPosX.GetFloat(),
								m_BeginPosY.GetFloat(),
								sx, sy, 0.0f, true, color );
	}


	void LaserShot::Draw()
	{
		if( m_StatusFlags[ INVISIBLE ] ){
			return;
		}

		float posX = m_GUData.m_PosX.GetFloat();
		float posY = m_GUData.m_PosY.GetFloat();
		int color = 0xFFFFFFFF;
		int scale = m_ImgScale;
		float angle = ::atan2( m_Line.GetEndY() - m_Line.GetStartY(), -m_Line.GetEndX() + m_Line.GetStartX() ) - MAPIL::DegToRad( 90.0f );
		float length = std::sqrt(	( m_Line.GetEndX() - m_Line.GetStartX() ) * ( m_Line.GetEndX() - m_Line.GetStartX() ) + 
									( m_Line.GetEndY() - m_Line.GetStartY() ) * ( m_Line.GetEndY() - m_Line.GetStartY() ) );

		DrawEffect();

		// 消去されるときは徐々に薄くなっていく。
		if( m_StatusFlags[ DEAD ] ){
			color = ( ( 20 - m_DeadCounter ) * 5 ) << 24 | 0xFFFFFF;
			scale += m_DeadCounter * 0.05f;
		}

		// 点滅エフェクト
		if( m_Counter % 16 >= 12 ){
			m_DrawingMultiplicity = 2;
		}
		else{
			m_DrawingMultiplicity = 1;
		}

		// 描画
		for( int i = 0; i < m_DrawingMultiplicity; ++i ){
			unsigned int texSizeX;
			unsigned int texSizeY;
			MAPIL::Assert( m_AtlasImgID != -1, CURRENT_POSITION, TSTR( "Invalid image ID was input." ), -1 );
			ResourceMap::TextureAtlas atlas;
			atlas = m_pResourceMap->m_pGlobalResourceMap->m_TexAtlasMap[ m_AtlasImgID ];
			texSizeX = atlas.m_Width;
			texSizeY = atlas.m_Height;

			for( int i = 0; i < m_DrawingMultiplicity; ++i ){
				AddToAtlasSpriteBatch(	true,
										m_AlphaBlendingMode,
										m_AtlasImgID,
										m_Line.GetStartX() - ( texSizeX * m_ImgScale / 2.0f ) * cos( angle ),
										m_Line.GetStartY() + ( texSizeX * m_ImgScale / 2.0f ) * sin( angle ),
										m_ImgScale,
										length / texSizeY, 
										angle, false, color );
			}
		}
	}

	bool LaserShot::Update()
	{
		if( m_StatusFlags[ PAUSED ] ){
			return true;
		}

		// メッセージ処理
		ProcessMessages();

		// 最初の位置を保存
		if( m_Counter == 0 ){
			m_BeginPosX = m_GUData.m_PosX;
			m_BeginPosY = m_GUData.m_PosY;
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ LAUNCH_EFFECT_SE_ID ] );
		}
		
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
			float ex = m_GUData.m_PosX.GetFloat() + m_Length.GetFloat() * cos( MAPIL::DegToRad( m_GUData.m_Angle.GetFloat() ) );
			float ey = m_GUData.m_PosY.GetFloat() - m_Length.GetFloat() * sin( MAPIL::DegToRad( m_GUData.m_Angle.GetFloat() ) );
			
			m_Line.SetStartX( sx );
			m_Line.SetStartY( sy );
			m_Line.SetEndX( ex );
			m_Line.SetEndY( ey );
		}

		m_ImgRotAngle = MAPIL::DegToRad( m_GUData.m_Angle.GetFloat() + 90.0f );


		++m_Counter;

		return true;
	}

	void LaserShot::Init( const GameUnit& posX, const GameUnit& posY )
	{
	}


	bool LaserShot::DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius )
	{
		Circle c;
		c.SetCenterX( x.GetFloat() );
		c.SetCenterY( y.GetFloat() );
		c.SetRadius( radius.GetFloat() );

		return m_Line.Colided( &c );
	}


	void LaserShot::SetLinePos(	const GameUnit& x1,
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

	void LaserShot::SetLength( const GameUnit& length )
	{
		m_Length = length;
	}

	void LaserShot::SetTextureColor( int color )
	{
		m_TexColor = color;
		m_AtlasImgID = LASER_SHOT_TEX_ID + m_TexColor;
	}
}
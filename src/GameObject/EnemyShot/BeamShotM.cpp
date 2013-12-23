#include <MAPIL/MAPIL.h>

#include <bitset>

#include "BeamShotM.h"

#include "../../ResourceTypes.h"
#include "../../ResourceID.h"
#include "../../SpriteBatch.h"

namespace GameEngine
{
	static const int LAUNCH_EFFECT_TEX_ID		= 1024;
	static const int LASER_MAKER_TEX_ID			= 1016;
	static const int LASER_SHOT_TEX_ID			= 145;
	static const int LAUNCH_EFFECT_SE_ID		= GLOBAL_RESOURCE_SE_ID_ENEMY_LASER_SHOT;

	BeamShotM::BeamShotM( std::shared_ptr < ResourceMap > pMap, int id ) :	EnemyShot( pMap, id )
	{
		m_AlphaBlendingMode = MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT;
		m_GUData.m_ColRadius = GameUnit( 0 );
		m_Line.SetThickness( m_GUData.m_ColRadius.GetFloat() );
		m_Length = GameUnit( 750 );
		m_StatusFlags.set( NOT_DELETE_BY_PLAYER_DAMAGE );
		m_StatusFlags.set( NOT_DELETE_BY_PLAYER_SKILL );
	}

	BeamShotM::~BeamShotM()
	{
	}

	void BeamShotM::DrawEffect()
	{
		if( m_Counter < 20 ){
			return;
		}

		float scale = 0.0f;

		if( m_Counter >= 20 && m_Counter < 60 ){
			scale = ( m_Counter - 20 ) / 20.0f;
		}
		else if( m_Counter >= 60 && m_Counter < 140 ){
			scale = 2.0f;
		}
		else if( m_Counter >= 140 && m_Counter < 160 ){
			scale = ( 160 - m_Counter ) / 10.0f;
		}

		AddToAtlasSpriteBatch(	true,
								MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT,
								LAUNCH_EFFECT_TEX_ID + m_TexColor,
								m_GUData.m_PosX.GetFloat(),
								m_GUData.m_PosY.GetFloat(),
								scale, scale, 0.0f, true );
	}


	void BeamShotM::Draw()
	{
		if( m_StatusFlags[ INVISIBLE ] ){
			return;
		}

		float posX = m_GUData.m_PosX.GetFloat();
		float posY = m_GUData.m_PosY.GetFloat();
		int color = 0xFFFFFFFF;
		float scale = 0.0f;
		float angle = ::atan2( m_Line.GetEndY() - m_Line.GetStartY(), -m_Line.GetEndX() + m_Line.GetStartX() ) - MAPIL::DegToRad( 90.0f );
		float length = std::sqrt(	( m_Line.GetEndX() - m_Line.GetStartX() ) * ( m_Line.GetEndX() - m_Line.GetStartX() ) + 
									( m_Line.GetEndY() - m_Line.GetStartY() ) * ( m_Line.GetEndY() - m_Line.GetStartY() ) );

		DrawEffect();

		// 消去されるときは徐々に薄くなっていく。
		if( m_StatusFlags[ DEAD ] ){
			color = ( ( 20 - m_DeadCounter ) * 5 ) << 24 | 0xFFFFFF;
			scale = ( 20 - m_DeadCounter ) / 20.0f;
		}
		else{
			// 拡大率
			if( m_Counter < 20 ){
				scale = 1.0f;
			}
			else if( m_Counter >= 20 && m_Counter < 60 ){
				scale = ( m_Counter - 20 ) / 40.0f;
			}
			else if( m_Counter >= 60 ){
				scale = 1.0f;
			}
		}

		// 点滅エフェクト
		if( m_Counter > 20 ){
			if( m_Counter % 16 == 0 || m_Counter % 16 == 12 ){
				m_DrawingMultiplicity = 2;
			}
			if( m_Counter % 16 == 10 || m_Counter % 16 == 14 ){
				m_DrawingMultiplicity = 1;
			}
		}
		else{
			m_DrawingMultiplicity = 1;
		}

		// 描画
		unsigned int texSizeX;
		unsigned int texSizeY;
		int img;
		MAPIL::Assert( m_AtlasImgID != -1, CURRENT_POSITION, TSTR( "Invalid image ID was input." ), -1 );
		ResourceMap::TextureAtlas atlas;
		if( m_Counter < 20 ){
			img = LASER_MAKER_TEX_ID + m_TexColor;
		}
		else{
			img = m_AtlasImgID;
		}
		atlas = m_pResourceMap->m_pGlobalResourceMap->m_TexAtlasMap[ img ];
		texSizeX = atlas.m_Width;
		texSizeY = atlas.m_Height;

		for( int i = 0; i < m_DrawingMultiplicity; ++i ){
			AddToAtlasSpriteBatch(	true,
									m_AlphaBlendingMode,
									img,
									m_Line.GetStartX() - ( texSizeX * scale / 2.0f ) * cos( angle ),
									m_Line.GetStartY() + ( texSizeX * scale / 2.0f ) * sin( angle ),
									scale,
									length / texSizeY, 
									angle, false, color );
		}
	}

	bool BeamShotM::Update()
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
		}

		// 効果音
		if( m_Counter == 20 ){
			MAPIL::PlayStaticBuffer( m_pResourceMap->m_pGlobalResourceMap->m_SEMap[ LAUNCH_EFFECT_SE_ID ] );
		}

		// 衝突半径
		switch( m_Counter ){
			case 20:
				m_GUData.m_ColRadius = GameUnit( 0 );
				break;
			case 60:
				m_GUData.m_ColRadius = GameUnit( 2 );
				break;
		}
		
		// 死亡判定処理
		if( m_StatusFlags[ DEAD ] ){
			m_GUData.m_ColRadius = GameUnit( -5 );
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

	void BeamShotM::Init( const GameUnit& posX, const GameUnit& posY )
	{
	}


	bool BeamShotM::DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius )
	{
		Circle c;
		c.SetCenterX( x.GetFloat() );
		c.SetCenterY( y.GetFloat() );
		c.SetRadius( radius.GetFloat() );

		return m_Line.Colided( &c );
	}


	void BeamShotM::SetLinePos(	const GameUnit& x1,
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

	void BeamShotM::SetLength( const GameUnit& length )
	{
		m_Length = length;
	}

	void BeamShotM::SetTextureColor( int color )
	{
		m_TexColor = color;
		m_AtlasImgID = LASER_SHOT_TEX_ID + m_TexColor;
	}
}
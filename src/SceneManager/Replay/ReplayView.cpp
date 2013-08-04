#include "ReplayView.h"

#include <MAPIL/MAPIL.h>

#include "../../GameStateTypes.h"

#include "../../Util.h"

#include "../../ResourceID.h"

namespace GameEngine
{
	ReplayView::ReplayView()
	{
	}

	ReplayView::~ReplayView()
	{
	}

	void ReplayView::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	//-------------------------------------------
	// ReplayBackgroundView
	//-------------------------------------------

	ReplayBackgroundView::ReplayBackgroundView() : 	ReplayView(),
													m_CurMode( MODE_INVISIBLE ),
													m_TransitionCounter( 0 ),
													m_Counter( 0 )
	{
	}

	ReplayBackgroundView::~ReplayBackgroundView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ReplayBackgroundView::Draw() const
	{
		int alpha = 0x3C;
		

		if( m_CurMode == MODE_INITIALIZE ){
			if( m_TransitionCounter < 60 ){
				alpha = m_TransitionCounter & 0xFF;
			}
		}
		// メニュー画面へ移動時
		else if( m_CurMode == MODE_BACK_TO_MENU ){
			if( m_TransitionCounter <= 15 ){
				alpha = 0x3C - ( 15 - m_TransitionCounter ) * 4;
			}
		}
		// ステージへ移行
		else if( m_CurMode == MODE_FINALIZE )
		{
			if( m_TransitionCounter <= 20 ){
				alpha = 0x3C;		// 60
			}
			else if( m_TransitionCounter > 20 && m_TransitionCounter <= 40 ){
				alpha = 0x3C - ( m_TransitionCounter - 20 ) * 3;
			}
			else{
				alpha = 0x00;
			}
		}

		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
		for( int i = 0; i < 100; ++i ){
			int alpha2 = 0x78 + 0x60 * sin( m_Counter * 0.03f + i * 360 / 100 );
			if( m_CurMode == MODE_INITIALIZE ){
				if( m_TransitionCounter < 120 ){
					alpha2 = ( 0x78 + 0x60 * sin( m_Counter * 0.03f + i * 360 / 100 ) ) * ( m_TransitionCounter ) / 120;
				}
			}
			else if( m_CurMode == MODE_FINALIZE ){
				if( m_TransitionCounter <= 20 ){
					alpha2 = 0x78 + 0x60 * sin( m_Counter * 0.03f + i * 360 / 100 );
				}
				else if( m_TransitionCounter > 20 && m_TransitionCounter <= 40 ){
					alpha2 = ( 0x78 + 0x60 * sin( m_Counter * 0.03f + i * 360 / 100 ) ) * ( 40 - m_TransitionCounter ) / 20;
				}
				else{
					alpha2 = 0x00;
				}
			}
			// メニュー画面へ移動時
			else if( m_CurMode == MODE_BACK_TO_MENU ){
				if( m_TransitionCounter <= 15 ){
					alpha2 = ( 0x78 + 0x60 * sin( m_Counter * 0.03f + i * 360 / 100 ) ) * ( 15 - m_TransitionCounter ) / 15;
				}
			}

			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_CONS_RED_SYMBOL + i % 3 ],
								320.0f + ( i % 5 ) * 110.0f * cos( m_Counter * 0.005f + i * 123.1f ),
								240.0f + ( i % 4 ) * 90.0f * sin( m_Counter * 0.005f + i * 123.1f ),
								( i % 5 ) * 0.2f * cos( m_Counter * 0.005f + i * 123.1f ),
								( i % 5 ) * 0.2f * cos( m_Counter * 0.005f + i * 123.1f ),
								( i % 10 ) * m_Counter * 0.02f,
								true,
								alpha << 24 | alpha << 16 | alpha << 8 | alpha );
								
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_CONS_RED_SYMBOL + i % 3 ],
								320.0f + ( i % 5 ) * 110.0f * cos( m_Counter * 0.005f + i * 123.1f ),
								240.0f + ( i % 4 ) * 90.0f * sin( m_Counter * 0.005f + i * 123.1f ),
								( i % 5 ) * 0.2f * cos( m_Counter * 0.005f + i * 123.1f ),
								( i % 5 ) * 0.2f * cos( m_Counter * 0.005f + i * 123.1f ),
								( i % 10 ) * m_Counter * 0.02f,
								true,
								alpha2 << 24 | alpha2 << 16 | alpha2 << 8 | alpha2 );
								
		}
		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
	}

	void ReplayBackgroundView::Update()
	{
		switch( m_CurMode ){
				case MODE_BACK_TO_MENU:
				++m_TransitionCounter;
				if( m_TransitionCounter > 15 ){
					m_CurMode = MODE_INVISIBLE;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_INITIALIZE:
				++m_TransitionCounter;
				if( m_TransitionCounter > 120 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_FINALIZE:
				++m_TransitionCounter;
				break;
			default:
				break;
		}

		++m_Counter;
	}

	void ReplayBackgroundView::BackToMenu()
	{
		m_CurMode = MODE_BACK_TO_MENU;
		m_TransitionCounter = 0;
	}

	void ReplayBackgroundView::Init()
	{
		m_CurMode = MODE_INITIALIZE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ReplayBackgroundView::Finalize()
	{
		m_CurMode = MODE_FINALIZE;
		m_TransitionCounter = 0;
	}

	//-------------------------------------------
	// ReplayTitleView
	//-------------------------------------------

	ReplayTitleView::ReplayTitleView() :	ReplayView(),
											m_CurMode( MODE_INVISIBLE ),
											m_TransitionCounter( 0 ),
											m_Counter( 0 )
	{
	}

	ReplayTitleView::~ReplayTitleView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ReplayTitleView::Draw() const
	{
		int alpha = 0xFF;

		// 初期化時
		if( m_CurMode == MODE_INITIALIZE ){
			if( m_TransitionCounter < 60 ){
				alpha = ( m_TransitionCounter * 4 ) & 0xFF;
			}
		}
		// メニュー画面へ移動時
		else if( m_CurMode == MODE_BACK_TO_MENU ){
			if( m_TransitionCounter <= 15 ){
				alpha = ( 15 - m_TransitionCounter ) * 16;
			}
		}
		// ステージへ移行
		else if( m_CurMode == MODE_FINALIZE ){
			// フェードアウト
			if( m_TransitionCounter <= 20 ){
				alpha = 0xFF;
			}
			else if( m_TransitionCounter > 20 && m_TransitionCounter <= 40 ){
				alpha = ( 40 - m_TransitionCounter ) * 12;
			}
			else{
				alpha = 0x00;
			}
		}

		int color;
		color = alpha << 24 | 0xFFFFFF;

		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_REPLAY_TITLE ],
							5.0f, -5.0f, 0.7f, 0.7f, 0.0f, false, color );
	}

	void ReplayTitleView::Update()
	{
		switch( m_CurMode ){
				case MODE_BACK_TO_MENU:
				++m_TransitionCounter;
				if( m_TransitionCounter > 15 ){
					m_CurMode = MODE_INVISIBLE;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_INITIALIZE:
				++m_TransitionCounter;
				if( m_TransitionCounter > 120 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_FINALIZE:
				++m_TransitionCounter;
				break;
			default:
				break;
		}

		++m_Counter;
	}

	void ReplayTitleView::BackToMenu()
	{
		m_CurMode = MODE_BACK_TO_MENU;
		m_TransitionCounter = 0;
	}

	void ReplayTitleView::Init()
	{
		m_CurMode = MODE_INITIALIZE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ReplayTitleView::Finalize()
	{
		m_CurMode = MODE_FINALIZE;
		m_TransitionCounter = 0;
	}

}
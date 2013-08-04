#include "ScoreView.h"

#include <MAPIL/MAPIL.h>

#include "../../GameStateTypes.h"

#include "../../Util.h"

#include "../../ResourceID.h"

namespace GameEngine
{
	ScoreView::ScoreView()
	{
	}

	ScoreView::~ScoreView()
	{
	}

	void ScoreView::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	//-------------------------------------------
	// ScoreBackgroundView
	//-------------------------------------------

	ScoreBackgroundView::ScoreBackgroundView() : 	ScoreView(),
													m_CurMode( MODE_INVISIBLE ),
													m_TransitionCounter( 0 ),
													m_Counter( 0 )
	{
	}

	ScoreBackgroundView::~ScoreBackgroundView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ScoreBackgroundView::Draw() const
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

		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
		for( int i = 0; i < 100; ++i ){
			int alpha2 = 0x78 + 0x60 * sin( m_Counter * 0.03f + i * 360 / 100 );
			if( m_CurMode == MODE_INITIALIZE ){
				if( m_TransitionCounter < 120 ){
					alpha2 = ( 0x78 + 0x60 * sin( m_Counter * 0.03f + i * 360 / 100 ) ) * ( m_TransitionCounter ) / 120;
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

	void ScoreBackgroundView::Update()
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
			default:
				break;
		}

		++m_Counter;
	}

	void ScoreBackgroundView::BackToMenu()
	{
		m_CurMode = MODE_BACK_TO_MENU;
		m_TransitionCounter = 0;
	}

	void ScoreBackgroundView::Init()
	{
		m_CurMode = MODE_INITIALIZE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	//---------------------------------------
	// ScoreDifficultyView
	//---------------------------------------

	ScoreDifficultyView::ScoreDifficultyView() :	ScoreView(),
													m_CurMode( MODE_INVISIBLE ),
													m_TransitionCounter( 0 ),
													m_Counter( 0 ),
													m_SelectedDifficulty( GAME_DIFFICULTY_CALM ),
													m_PrevSelectedDifficulty( GAME_DIFFICULTY_CALM )
	{
	}

	ScoreDifficultyView::~ScoreDifficultyView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_TransitionCounter = 0;
		m_Counter = 0;
		m_SelectedDifficulty = GAME_DIFFICULTY_CALM;
		m_PrevSelectedDifficulty = GAME_DIFFICULTY_CALM;
	}

	void ScoreDifficultyView::Draw() const
	{
		int alpha = 0xFF;
		float SCALE_FACTOR = 1.0f;
		float scale = SCALE_FACTOR;

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
		// 難易度変更
		else if( m_CurMode == MODE_DIFFICULTY_CHANGED ){
			if( m_TransitionCounter < 10 ){
				scale = ::abs( SCALE_FACTOR * ( m_TransitionCounter ) / 10 );
			}
		}

		int color;
		color = alpha << 24 | 0xFFFFFF;

		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_SCORE_CALM + m_SelectedDifficulty ],
							320.0f, 50.0f, scale, scale, 0.0f, true, color );
	}

	void ScoreDifficultyView::Update()
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
			case MODE_DIFFICULTY_CHANGED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 20 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			default:
				break;
		}

		++m_Counter;
	}

	void ScoreDifficultyView::BackToMenu()
	{
		m_CurMode = MODE_BACK_TO_MENU;
		m_TransitionCounter = 0;
	}

	void ScoreDifficultyView::Init()
	{
		m_CurMode = MODE_INITIALIZE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ScoreDifficultyView::ChangeDifficulty( int difficulty )
	{
		m_CurMode = MODE_DIFFICULTY_CHANGED;
		m_PrevSelectedDifficulty = m_SelectedDifficulty;
		m_SelectedDifficulty = difficulty;
		m_Counter = 0;
		m_TransitionCounter = 0;
	}

	//-----------------------------------
	// ScoreTitleView
	//-----------------------------------

	ScoreTitleView::ScoreTitleView() :	ScoreView(),
										m_CurMode( MODE_INVISIBLE ),
										m_TransitionCounter( 0 ),
										m_Counter( 0 )
	{
	}

	ScoreTitleView::~ScoreTitleView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ScoreTitleView::Draw() const
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

		int color;
		color = alpha << 24 | 0xFFFFFF;

		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_SCORE_TITLE ],
							-5.0f, -5.0f, 0.7f, 0.7f, 0.0f, false, color );
	}

	void ScoreTitleView::Update()
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
			default:
				break;
		}

		++m_Counter;
	}

	void ScoreTitleView::BackToMenu()
	{
		m_CurMode = MODE_BACK_TO_MENU;
		m_TransitionCounter = 0;
	}

	void ScoreTitleView::Init()
	{
		m_CurMode = MODE_INITIALIZE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

}
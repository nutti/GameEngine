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

	//-------------------------------------------
	// ReplayStageView
	//-------------------------------------------


	ReplayStageView::ReplayStageView() :	ReplayView(),
											m_CurMode( MODE_INVISIBLE ),
											m_TransitionCounter( 0 ),
											m_Counter( 0 ),
											m_SelectedStage( 0 ),
											m_SelectedReplayNo( 0 ),
											m_pDispReplayInfo( NULL )
	{
	}

	ReplayStageView::~ReplayStageView()
	{
	}

	void ReplayStageView::Draw() const
	{
		if( m_CurMode == MODE_INVISIBLE ){
			return;
		}

		int alpha[ STAGE_TOTAL ] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
		float baseX = 270.0f;
		float baseY = 380.0f;
		float offsetX[ STAGE_TOTAL ] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		float offsetY[ STAGE_TOTAL ] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

		offsetX[ m_SelectedStage ] = -2.0f;
		

		if( m_CurMode == MODE_FINALIZE ){
			for( int i = 0; i < STAGE_TOTAL; ++i ){
				if( m_TransitionCounter == 0 ){
					alpha[ i ] = 0xFF;
				}
				else if( m_TransitionCounter > 0 && m_TransitionCounter <= 20 ){
					alpha[ i ] = ( 0xFF * ( 20 - m_TransitionCounter ) ) / 20;
				}
				else{
					alpha [ i ]= 0x00;
				}
			}
		}
		else if( m_CurMode == MODE_REPLAY_SELECTED ){
			const int INTERVAL = 10;
			const int STAGE_INTERVAL = 2;

			for( int i = 0; i < STAGE_TOTAL; ++i ){
				if( m_TransitionCounter > i * STAGE_INTERVAL && m_TransitionCounter < INTERVAL + i * STAGE_INTERVAL ){
					offsetX[ i ] = ( ( INTERVAL + i * STAGE_INTERVAL ) - m_TransitionCounter ) * 5.0f;
					offsetY[ i ] = 0.0f;
					alpha[ i ] = ( 0xFF * ( m_TransitionCounter - i * STAGE_INTERVAL ) ) / INTERVAL;
				}
				else if( m_TransitionCounter <= i * STAGE_INTERVAL ){
					offsetX[ i ] = 150.0f;
					offsetY[ i ] = 0.0f;
					alpha[ i ] = 0x00;
				}
			}
		}
		else if( m_CurMode == MODE_STAGE_CHANGED ){
			offsetX[ m_SelectedStage ] = -2.0f * sin( MAPIL::DegToRad( m_TransitionCounter * 45.0 ) );
		}
		else if( m_CurMode == MODE_STAGE_CANCELED ){
			const int INTERVAL = 10;
			const int STAGE_INTERVAL = 2;

			for( int i = 0; i < STAGE_TOTAL; ++i ){
				if( m_TransitionCounter > i * STAGE_INTERVAL && m_TransitionCounter < INTERVAL + i * STAGE_INTERVAL ){
					offsetX[ i ] = ( m_TransitionCounter - i * STAGE_INTERVAL ) * 5.0f;
					offsetY[ i ] = 0.0f;
					alpha[ i ] = ( 0xFF * ( i * STAGE_INTERVAL + INTERVAL - m_TransitionCounter ) ) / INTERVAL;
				}
				else if( m_TransitionCounter >= i * STAGE_INTERVAL + INTERVAL ){
					offsetX[ i ] = 500.0f;
					offsetY[ i ] = 0.0f;
					alpha[ i ] = 0x00;
				}
			}
		}

		int color1[ STAGE_TOTAL ];
		int color2[ STAGE_TOTAL ];
		int selColor[ STAGE_TOTAL ];
		int c[ STAGE_TOTAL ];
		
		for( int i = 0; i < STAGE_TOTAL; ++i ){
			color1[ i ] = alpha[ i ] << 24 | 0xAAFFAA;
			color2[ i ] = alpha[ i ] << 24 | 0xAAAAAA;
			selColor[ i ] = alpha[ i ] << 24 | 0xFFFFFF;
			if( i == m_SelectedStage ){
				c[ i ] = selColor[ i ];
			}
			else{
				c[ i ] = color2[ i ];
			}
		}
		
		// ステージ情報の表示
		for( int i = 0; i < STAGE_TOTAL; ++i ){
			std::string str;
			if( m_pDispReplayInfo->m_Entries[ m_SelectedReplayNo ].m_Progress > i ){
				str = "stage";
				str += i + '1';
			}
			else{
				str = "no entry";
			}
			DrawFontString( m_ResourceMap, baseX + offsetX[ i ], baseY + i * 17.0f + offsetY[ i ], 0.5f, c[ i ], str.c_str() );
		}

		// 詳細情報の表示
		float fontScale = 0.4f;

		float x[ 3 ];
		float y[ 3 ];
		float ox = 100.0f;
		float oy = 0.0f;
		x[ 0 ] = 415.0f;
		y[ 0 ] = 395.0f;
		for( int i = 0; i < 2; ++i ){
			//x[ i + 1 ] = x[ i ] + 15.0f;
			y[ i + 1 ] = y[ i ] + 17.0f;
			x[ i + 1 ] = x[ i ];
		}

		DrawFontString( m_ResourceMap, x[ 0 ], y[ 0 ], fontScale, color1[ 0 ], "score" );
		DrawFontString( m_ResourceMap, x[ 1 ], y[ 1 ], fontScale, color1[ 0 ], "killed" );
		DrawFontString( m_ResourceMap, x[ 2 ], y[ 2 ], fontScale, color1[ 0 ], "crystal" );

		int color3;

		if( m_CurMode == MODE_STAGE_CHANGED ){
			color3 = ( ( m_TransitionCounter ) * 25 ) << 24 | 0xFFFFFF;
		}
		else{
			color3 = selColor[ 0 ];
		}
		DrawFontString( m_ResourceMap, x[ 0 ] + ox, y[ 0 ] + oy, fontScale, color3, "%d", m_pDispReplayInfo->m_Entries[ m_SelectedReplayNo ].m_StageInfo[ m_SelectedStage ].m_Score );
		DrawFontString( m_ResourceMap, x[ 1 ] + ox, y[ 1 ] + oy, fontScale, color3, "%d", m_pDispReplayInfo->m_Entries[ m_SelectedReplayNo ].m_StageInfo[ m_SelectedStage ].m_Killed );
		DrawFontString( m_ResourceMap, x[ 2 ] + ox, y[ 2 ] + oy, fontScale, color3, "%d", m_pDispReplayInfo->m_Entries[ m_SelectedReplayNo ].m_StageInfo[ m_SelectedStage ].m_Crystal );
	}

	void ReplayStageView::Update()
	{
		switch( m_CurMode ){
				case MODE_REPLAY_SELECTED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 20 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_STAGE_CANCELED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 20 ){
					m_CurMode = MODE_INVISIBLE;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_STAGE_CHANGED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
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

	void ReplayStageView::AttachDisplayedReplayInfo( DisplayedReplayInfo* pInfo )
	{
		m_pDispReplayInfo = pInfo;
	}

	void ReplayStageView::Finalize()
	{
		m_CurMode = MODE_FINALIZE;
		m_TransitionCounter = 0;
	}

	void ReplayStageView::SelectReplay( int replayNo, int stage )
	{
		m_SelectedReplayNo = replayNo;
		m_SelectedStage = stage;
		m_CurMode = MODE_REPLAY_SELECTED;
		m_TransitionCounter = 0;
	}

	void ReplayStageView::CancelStage()
	{
		m_CurMode = MODE_STAGE_CANCELED;
		m_TransitionCounter = 0;
	}

	void ReplayStageView::ChangeStage( int stage )
	{
		m_CurMode = MODE_STAGE_CHANGED;
		m_TransitionCounter = 0;
		m_SelectedStage = stage;
	}
}
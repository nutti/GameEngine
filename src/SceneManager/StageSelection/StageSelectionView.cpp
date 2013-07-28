#include "StageSelectionView.h"

#include <MAPIL/MAPIL.h>

#include "../../GameStateTypes.h"

#include "../../Util.h"

#include "../../ResourceID.h"

namespace GameEngine
{
	StageSelectionView::StageSelectionView()
	{
	}

	StageSelectionView::~StageSelectionView()
	{
	}

	void StageSelectionView::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}



	StageView::StageView() :	StageSelectionView(),
								m_CurMode( MODE_INVISIBLE ),
								m_SelectedStage( 0 ),
								m_PrevSelectedStage( 0 ),
								m_PermittedStage( 0 ),
								m_TransitionCounter( 0 )
	{
	}

	StageView::~StageView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_SelectedStage = 0;
		m_PrevSelectedStage = 0;
		m_PermittedStage = 0;
		m_TransitionCounter = 0;
	}

	void StageView::SetPermittedStage( int stage )
	{
		m_PermittedStage = stage;
	}

	void StageView::Draw() const
	{
		if( m_CurMode == MODE_INVISIBLE ){
			return;
		}

		int color;
		int alpha = 0xFF;
		

		const float STAGE_BASE_POS_X = 200.0f;
		const float STAGE_BASE_POS_Y = 377.0f;
		const float STAGE_OFFSET_POS_Y = 20.0f;

		float offsetY = 0.0f;
		float offsetX = 0.0f;

		const float SCALE_FACTOR = 0.8f;

		float imgScale[ 2 ] = { 0.0f, SCALE_FACTOR };


		// 難易度選択時の処理
		if( m_CurMode == MODE_DIFFICULTY_SELECTED ){
			if( m_TransitionCounter > 30 ){
				imgScale[ 1 ] = 10 * SCALE_FACTOR / 10.0f;
				alpha = 0xFF;
			}
			else if( m_TransitionCounter > 20 && m_TransitionCounter <= 30 ){
				imgScale[ 1 ] = ( m_TransitionCounter - 20 ) * SCALE_FACTOR / 10.0f;
				alpha = ( m_TransitionCounter - 20 ) * 25;
			}
			else{
				imgScale[ 1 ] = 0.0f;
				alpha = 0;
			}
		}
		// キャンセル時の処理
		else if( m_CurMode == MODE_CANCELED ){
			if( m_TransitionCounter > 10 ){
				imgScale[ 1 ] = 0.0f;
				alpha = 0;
			}
			else{
				imgScale[ 1 ] = ( 10 - m_TransitionCounter ) * SCALE_FACTOR / 10.0f;
				alpha = ( 10 - m_TransitionCounter ) * 25;
			}
		}
		// 選択するステージを変更した時
		else if( m_CurMode == MODE_CHANGED ){
			if( m_TransitionCounter < 8 ){
				imgScale[ 0 ] = ( 8 - m_TransitionCounter ) * SCALE_FACTOR / 8.0f;
				imgScale[ 1 ] = 0.0f;
				alpha = ( 8 - m_TransitionCounter ) * 31;
			}
			else{
				imgScale[ 0 ] = 0.0f;
				imgScale[ 1 ] = ( m_TransitionCounter - 8 ) * SCALE_FACTOR / 8.0f;
				alpha = ( m_TransitionCounter - 8 ) * 31;
			}
		}
		// ステージを選択した時（未使用）
		else if( m_CurMode == MODE_STAGE_SELECTED ){
		}
		// ゲーム開始時
		else if( m_CurMode == MODE_FINALIZE ){
			if( m_TransitionCounter > 0 && m_TransitionCounter <= 15 ){
				alpha = 127 + static_cast < int > ( 127 * cos( MAPIL::DegToRad( m_TransitionCounter * 120.0f ) ) );
			}
			else if( m_TransitionCounter > 15 && m_TransitionCounter <= 20 ){
				alpha = 0xFF;
			}
			else if( m_TransitionCounter > 20 && m_TransitionCounter <= 40 ){
				alpha = ( 0xFF * ( 40 - m_TransitionCounter ) ) / 20;
			}
			else{
				alpha = 0x00;
			}

			if( m_TransitionCounter >= 20 && m_TransitionCounter < 60 ){
				offsetX = ( m_TransitionCounter - 20 ) * 25.0f;
			}
		}

		color = ( alpha << 24 ) | 0xFFFFFF;

		int texture[ 5 ] = {	GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_STAGE_1,
								GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_STAGE_2,
								GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_STAGE_3,
								GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_STAGE_4,
								GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_STAGE_5 };

		for( int i = 0; i < sizeof( texture ) / sizeof( texture[ 0 ] ); ++i ){

			if( i == m_SelectedStage ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ texture[ i ] ],
									STAGE_BASE_POS_X + offsetX, STAGE_BASE_POS_Y + offsetY, imgScale[ 1 ], imgScale[ 1 ], true, color );
			}
			else if( i == m_PrevSelectedStage ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ texture[ i ] ],
									STAGE_BASE_POS_X + offsetX, STAGE_BASE_POS_Y + offsetY, imgScale[ 0 ], imgScale[ 0 ], true, color );
			}

		}
	}

	void StageView::Update()
	{
		switch( m_CurMode ){
			case MODE_DIFFICULTY_SELECTED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 30 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_STAGE_SELECTED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_CANCELED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 30 ){
					m_CurMode = MODE_INVISIBLE;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_CHANGED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 16 ){
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
	}

	void StageView::CancelStage()
	{
		m_CurMode = MODE_CANCELED;
		m_TransitionCounter = 0;
	}

	void StageView::SelectStage()
	{
		m_CurMode = MODE_STAGE_SELECTED;
		m_TransitionCounter = 0;
	}

	void StageView::ChangeStage( int stage )
	{
		m_PrevSelectedStage = m_SelectedStage;
		m_SelectedStage = stage;
		m_CurMode = MODE_CHANGED;
		m_TransitionCounter = 0;
	}


	void StageView::SelectDifficulty()
	{
		m_CurMode = MODE_DIFFICULTY_SELECTED;
		m_TransitionCounter = 0;
	}

	void StageView::Finalize()
	{
		m_CurMode = MODE_FINALIZE;
		m_TransitionCounter = 0;
	}

	//-------------------------------------------
	// StageSelectionTitleView
	//-------------------------------------------


	StageSelectionTitleView::StageSelectionTitleView() :	StageSelectionView(),
															m_CurMode( MODE_INVISIBLE ),
															m_TransitionCounter( 0 ),
															m_IsStageSelectionMode( false )
	{
	}

	StageSelectionTitleView::~StageSelectionTitleView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_TransitionCounter = 0;
		m_IsStageSelectionMode = false;
	}

	void StageSelectionTitleView::Draw() const
	{
		int alpha = 0xFF;
		MAPIL::Matrix4x4 < float > mat;
		const float SCALE_FACTOR = 0.7f;
		const float BASE_X = 320.0f;
		const float BASE_Y = 90.0f;
		float scaleX = SCALE_FACTOR;

		// 開始時
		if( m_CurMode == MODE_INITIALIZE ){
			// フェードイン
			if( m_TransitionCounter < 60 ){
				alpha = m_TransitionCounter * 4;
			}
			else{
				alpha = 0xFF;
			}
		}
		// 終了時
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
		// 難易度選択時
		else if( m_CurMode == MODE_DIFFICULTY_SELECTED ){
			int counter = ( m_TransitionCounter - 20 ) > 0 ? ( m_TransitionCounter - 20 ) : 50;
			if( abs( 10 - counter ) <= 10 ){
				scaleX = abs( 10 - counter ) * SCALE_FACTOR / 10.0f;
			}
			else{
				scaleX = SCALE_FACTOR;
			}
		}
		// ステージキャンセル時
		else if( m_CurMode == MODE_STAGE_CANCELED ){
			if( abs( 10 - m_TransitionCounter ) <= 10 ){
				scaleX = abs( 10 - m_TransitionCounter ) * SCALE_FACTOR / 10.0f;
			}
			else{
				scaleX = SCALE_FACTOR;
			}
		}
		// 難易度キャンセル時
		else if( m_CurMode == MODE_DIFFICULTY_CANCELED ){
			if( m_TransitionCounter <= 15 ){
				alpha = ( 15 - m_TransitionCounter ) * 16;
			}
		}

		int color = 0xFFFFFF | alpha << 24;

		if( m_CurMode == MODE_DIFFICULTY_SELECTED ){
			if( m_TransitionCounter <= 30 ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_TITLE ],
									BASE_X, BASE_Y, scaleX, SCALE_FACTOR, true, color );
			}
			else{
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_TITLE ],
									BASE_X, BASE_Y, scaleX, SCALE_FACTOR, true, color );
			}
		}
		else if( m_CurMode == MODE_STAGE_CANCELED ){
			if( m_TransitionCounter <= 10 ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_TITLE ],
									BASE_X, BASE_Y, scaleX, SCALE_FACTOR, true, color );
			}
			else{
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_TITLE ],
									BASE_X, BASE_Y, scaleX, SCALE_FACTOR, true, color );
			}
		}
		else{
			if( m_IsStageSelectionMode ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_TITLE ],
									BASE_X, BASE_Y, SCALE_FACTOR, SCALE_FACTOR, true, color );
			}
			else{
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_TITLE ],
									BASE_X, BASE_Y, SCALE_FACTOR, SCALE_FACTOR, true, color );
			}
		}
	}

	void StageSelectionTitleView::Update()
	{
		switch( m_CurMode ){
			case MODE_INITIALIZE:
				++m_TransitionCounter;
				if( m_TransitionCounter > 60 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_DIFFICULTY_SELECTED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 40 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_DIFFICULTY_CANCELED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 15 ){
					m_CurMode = MODE_INVISIBLE;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_STAGE_CANCELED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 30 ){
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
	}

	void StageSelectionTitleView::CancelStage()
	{
		m_CurMode = MODE_STAGE_CANCELED;
		m_TransitionCounter = 0;
		m_IsStageSelectionMode = false;
	}

	void StageSelectionTitleView::SelectDifficulty()
	{
		m_CurMode = MODE_DIFFICULTY_SELECTED;
		m_TransitionCounter = 0;
		m_IsStageSelectionMode = true;
	}

	void StageSelectionTitleView::CancelDifficulty()
	{
		m_CurMode = MODE_DIFFICULTY_CANCELED;
		m_TransitionCounter = 0;
	}

	void StageSelectionTitleView::Init()
	{
		m_CurMode = MODE_INITIALIZE;
		m_TransitionCounter = 0;
	}

	void StageSelectionTitleView::Finalize()
	{
		m_CurMode = MODE_FINALIZE;
		m_TransitionCounter = 0;
	}


	//-------------------------------------------
	// StageSelectionBackgroundView
	//-------------------------------------------

	StageSelectionBackgroundView::StageSelectionBackgroundView() : 	StageSelectionView(),
																	m_CurMode( MODE_INVISIBLE ),
																	m_TransitionCounter( 0 ),
																	m_Counter( 0 )
	{
	}

	StageSelectionBackgroundView::~StageSelectionBackgroundView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void StageSelectionBackgroundView::Draw() const
	{
		int alpha = 0x3C;
		

		if( m_CurMode == MODE_INITIALIZE ){
			if( m_TransitionCounter < 60 ){
				alpha = m_TransitionCounter & 0xFF;
			}
		}
		else if( m_CurMode == MODE_FINALIZE ){
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
		// 難易度選択キャンセル時
		else if( m_CurMode == MODE_DIFFICULTY_CANCELED ){
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
			// 難易度選択キャンセル時
			else if( m_CurMode == MODE_DIFFICULTY_CANCELED ){
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

	void StageSelectionBackgroundView::Update()
	{
		switch( m_CurMode ){
			case MODE_DIFFICULTY_CANCELED:
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

	void StageSelectionBackgroundView::CancelDifficulty()
	{
		m_CurMode = MODE_DIFFICULTY_CANCELED;
		m_TransitionCounter = 0;
	}

	void StageSelectionBackgroundView::Init()
	{
		m_CurMode = MODE_INITIALIZE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void StageSelectionBackgroundView::Finalize()
	{
		m_CurMode = MODE_FINALIZE;
		m_TransitionCounter = 0;
	}

	//-------------------------------------------
	// StageSelectionStatView
	//-------------------------------------------

	StageSelectionStatView::StageSelectionStatView() :	StageSelectionView(),
														m_CurMode( MODE_INVISIBLE ),
														m_TransitionCounter( 0 ),
														m_SelectedDifficulty( GAME_DIFFICULTY_CALM ),
														m_SelectedStage( 0 ),
														m_pDispStagePlayStat( NULL )
	{
	}

	StageSelectionStatView::~StageSelectionStatView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_TransitionCounter = 0;
		m_SelectedDifficulty = GAME_DIFFICULTY_CALM;
		m_SelectedStage = 0;
	}

	void StageSelectionStatView::AttachDisplayedStagePlayStat(  DisplayedStageSelectionPlayStat* pStat )
	{
		m_pDispStagePlayStat = pStat;
	}

	void StageSelectionStatView::Draw() const
	{
		// 見えない状態
		if( m_CurMode == MODE_INVISIBLE ){
			return;
		}

		int alpha = 0xFF;
		int alpha2 = 0xFF;

		// ステージ選択後の処理
		if( m_CurMode == MODE_FINALIZE ){
			// 通常表示
			if( m_TransitionCounter <= 20 ){
				alpha = ( 0xFF * ( 20 - m_TransitionCounter ) ) / 20;
				alpha2 = alpha;
			}
			// 消滅
			else{
				alpha = 0x00;
				alpha2 = alpha;
			}
		}
		// 難易度選択時の処理
		else if( m_CurMode == MODE_DIFFICULTY_SELECTED ){
			// 非表示
			if( m_TransitionCounter < 20 ){
				alpha = 0x00;
				alpha2 = alpha;
			}
			// フェードイン
			else if( m_TransitionCounter >= 20 && m_TransitionCounter <= 30 ){
				alpha = ( 0xFF * ( m_TransitionCounter - 20 ) ) / 15;
				alpha2 = alpha;
			}
		}
		// ステージキャンセル時の処理
		else if( m_CurMode == MODE_STAGE_CANCELED ){
			// フェードアウト
			if( m_TransitionCounter < 10 ){
				alpha = ( 0xFF * ( 10 - m_TransitionCounter ) ) / 10;
				alpha2 = alpha;
			}
			// 消滅
			else{
				alpha = 0x00;
				alpha2 = alpha;
			}
		}
		// ステージ変更時
		else if( m_CurMode == MODE_STAGE_CHANGED ){
			if( m_TransitionCounter < 8 ){
				alpha = ( 0xFF * ( 8 - m_TransitionCounter ) ) / 8;
			}
			else{
				alpha = ( 0xFF * ( m_TransitionCounter - 8 ) ) / 8;
			}
		}

		int color1 = alpha2 << 24 | 0xAAFFAA;
		int color2 = alpha << 24 | 0xFFFFFF;

		float fontScale = 0.50f;

		float x[ 4 ];
		float y[ 4 ];
		float offsetX = 40.0f;
		float offsetY = 20.0f;
		x[ 0 ] = 415.0f;
		y[ 0 ] = 305.0f;
		for( int i = 0; i < 3; ++i ){
			x[ i + 1 ] = x[ i ] + 17.0f;
			y[ i + 1 ] = y[ i ] + 40.0f;
		}

		DrawFontString( m_ResourceMap, x[ 0 ], y[ 0 ], fontScale, color1, "hi score" );
		DrawFontString( m_ResourceMap, x[ 1 ], y[ 1 ], fontScale, color1, "play" );
		DrawFontString( m_ResourceMap, x[ 2 ], y[ 2 ], fontScale, color1, "clear" );
		DrawFontString( m_ResourceMap, x[ 3 ], y[ 3 ], fontScale, color1, "play time" );

		DrawFontString( m_ResourceMap, x[ 0 ] + offsetX, y[ 0 ] + offsetY, fontScale, color2, "%d", m_pDispStagePlayStat->m_Stat[ m_SelectedDifficulty ].m_StageStat[ m_SelectedStage ].m_HIScore );
		DrawFontString( m_ResourceMap, x[ 1 ] + offsetX, y[ 1 ] + offsetY, fontScale, color2, "%d", m_pDispStagePlayStat->m_Stat[ m_SelectedDifficulty ].m_StageStat[ m_SelectedStage ].m_Play );
		DrawFontString( m_ResourceMap, x[ 2 ] + offsetX, y[ 2 ] + offsetY, fontScale, color2, "%d", m_pDispStagePlayStat->m_Stat[ m_SelectedDifficulty ].m_StageStat[ m_SelectedStage ].m_Clear );
		DrawFontString(	m_ResourceMap, x[ 3 ] + offsetX, y[ 3 ] + offsetY, fontScale, color2, "%d:%02d:%02d",
						m_pDispStagePlayStat->m_Stat[ m_SelectedDifficulty ].m_StageStat[ m_SelectedStage ].m_PlayTime / ( 60 * 60 ),
						( m_pDispStagePlayStat->m_Stat[ m_SelectedDifficulty ].m_StageStat[ m_SelectedStage ].m_PlayTime / 60 ) % 60,
						m_pDispStagePlayStat->m_Stat[ m_SelectedDifficulty ].m_StageStat[ m_SelectedStage ].m_PlayTime % 60 );
	}

	void StageSelectionStatView::Update()
	{
		switch( m_CurMode ){
			case MODE_INITIALIZE:
				m_CurMode = MODE_INVISIBLE;
				m_TransitionCounter = 0;
				break;
			case MODE_DIFFICULTY_SELECTED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 40 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_STAGE_CANCELED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 30 ){
					m_CurMode = MODE_INVISIBLE;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_STAGE_CHANGED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 16 ){
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
	}

	void StageSelectionStatView::Init()
	{
		m_CurMode = MODE_INITIALIZE;
		m_TransitionCounter = 0;
	}

	void StageSelectionStatView::CancelStage()
	{
		m_CurMode = MODE_STAGE_CANCELED;
		m_TransitionCounter = 0;
	}

	void StageSelectionStatView::ChangeStage( int stage )
	{
		m_CurMode = MODE_STAGE_CHANGED;
		m_SelectedStage = stage;
		m_TransitionCounter = 0;
	}

	void StageSelectionStatView::ChangeDifficulty( int difficulty )
	{
		m_SelectedDifficulty = difficulty;
	}

	void StageSelectionStatView::SelectDifficulty()
	{
		m_CurMode = MODE_DIFFICULTY_SELECTED;
		m_TransitionCounter = 0;
	}

	void StageSelectionStatView::Finalize()
	{
		m_CurMode = MODE_FINALIZE;
		m_TransitionCounter = 0;
	}
}
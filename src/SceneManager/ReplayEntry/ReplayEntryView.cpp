#include "ReplayEntryView.h"

#include <MAPIL/MAPIL.h>

#include "../../GameStateTypes.h"

#include "../../Util.h"

#include "../../ResourceID.h"


namespace GameEngine
{
	ReplayEntryView::ReplayEntryView()
	{
	}

	ReplayEntryView::~ReplayEntryView()
	{
	}

	void ReplayEntryView::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	//-------------------------------------------
	// ReplayEntryView
	//-------------------------------------------

	ReplayEntryBackgroundView::ReplayEntryBackgroundView() : 	ReplayEntryView(),
																m_CurMode( MODE_INVISIBLE ),
																m_TransitionCounter( 0 ),
																m_Counter( 0 )
	{
	}

	ReplayEntryBackgroundView::~ReplayEntryBackgroundView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ReplayEntryBackgroundView::Draw() const
	{
		int alpha = 0x3C;
		

		if( m_CurMode == MODE_INITIALIZE ){
			if( m_TransitionCounter < 60 ){
				alpha = m_TransitionCounter & 0xFF;
			}
		}
		// 次の画面へ移行時
		else if( m_CurMode == MODE_FINALIZE ){
			if( m_TransitionCounter <= 15 ){
				alpha = 0x3C - ( 15 - m_TransitionCounter ) * 4;
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
			// 次の画面移行時
			else if( m_CurMode == MODE_FINALIZE ){
				if( m_TransitionCounter <= 15 ){
					alpha2 = ( 0x78 + 0x60 * sin( m_Counter * 0.03f + i * 360 / 100 ) ) * ( 15 - m_TransitionCounter ) / 15;
				}
				else{
					alpha2 = 0x00;
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

	void ReplayEntryBackgroundView::Update()
	{
		switch( m_CurMode ){
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

	void ReplayEntryBackgroundView::Init()
	{
		m_CurMode = MODE_INITIALIZE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ReplayEntryBackgroundView::Finalize()
	{
		m_CurMode = MODE_FINALIZE;
		m_TransitionCounter = 0;
	}

	//-------------------------------------------
	// ReplayEntryTitleView
	//-------------------------------------------

	ReplayEntryTitleView::ReplayEntryTitleView() :	ReplayEntryView(),
													m_CurMode( MODE_INVISIBLE ),
													m_TransitionCounter( 0 ),
													m_Counter( 0 )
	{
	}

	ReplayEntryTitleView::~ReplayEntryTitleView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ReplayEntryTitleView::Draw() const
	{
		int alpha = 0xFF;

		// 初期化時
		if( m_CurMode == MODE_INITIALIZE ){
			if( m_TransitionCounter < 60 ){
				alpha = ( m_TransitionCounter * 4 ) & 0xFF;
			}
		}
		// メニュー画面へ移動時
		else if( m_CurMode == MODE_FINALIZE ){
			if( m_TransitionCounter <= 20 ){
				alpha = ( 20 - m_TransitionCounter ) * 12;
			}
			else{
				alpha = 0x00;
			}
		}

		int color;
		color = alpha << 24 | 0xFFFFFF;

		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_REPLAY_ENTRY_TITLE ],
							140.0f, -5.0f, 0.7f, 0.7f, 0.0f, false, color );
	}

	void ReplayEntryTitleView::Update()
	{
		switch( m_CurMode ){
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

	void ReplayEntryTitleView::Init()
	{
		m_CurMode = MODE_INITIALIZE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ReplayEntryTitleView::Finalize()
	{
		m_CurMode = MODE_FINALIZE;
		m_TransitionCounter = 0;
	}

	//-------------------------------------------
	// ReplayEntryInfoView
	//-------------------------------------------

	ReplayEntryInfoView::ReplayEntryInfoView() :	ReplayEntryView(),
													m_CurMode( MODE_INVISIBLE ),
													m_TransitionCounter( 0 ),
													m_Counter( 0 ),
													m_ReplayNo( 0 )
	{
	}

	ReplayEntryInfoView::~ReplayEntryInfoView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ReplayEntryInfoView::Draw() const
	{
		if( m_CurMode == MODE_INVISIBLE ){
			return;
		}

		int alpha = 0xFF;
		int alpha2 = 0xFF;
		
		if( m_CurMode == MODE_INITIALIZE ){
			if( m_TransitionCounter < 20 ){
				alpha = 0x00;
				alpha2 = alpha;
			}
			else if( m_TransitionCounter >= 20 && m_TransitionCounter < 60 ){
				alpha = ( ( m_Counter - 20 ) * 6 + 10 ) & 0xFF;
				alpha2 = alpha;
			}
		}
		else if( m_CurMode == MODE_FINALIZE ){
			if( m_TransitionCounter <= 20 ){
				alpha = ( 0xFF * ( 20 - m_TransitionCounter ) ) / 20;
				alpha2 = alpha;
			}
			else{
				alpha = 0x00;
				alpha2 = alpha;
			}
		}
		else if( m_CurMode == MODE_REPLAY_SELECTED ){
			if( m_TransitionCounter <= 10 ){
				alpha = ( ( 10 - m_TransitionCounter ) * 25 );
				alpha2 = alpha;
			}
		}
		else if( m_CurMode == MODE_REPLAY_CANCELED ){
			if( m_TransitionCounter <= 10 ){
				alpha = ( m_TransitionCounter * 25 );
				alpha2 = alpha;
			}
		}
		else if( m_CurMode == MODE_REPLAY_CHANGED ){
			if( m_TransitionCounter <= 10 ){
				alpha = ( m_TransitionCounter * 25 );
			}
		}
		else if( m_CurMode == MODE_REPLAY_SAVED ){
			if( m_TransitionCounter <= 10 ){
				alpha = ( m_TransitionCounter * 25 );
				alpha2 = alpha;
			}
		}

		int color1 = alpha2 << 24 | 0xAAFFAA;
		int selColor = alpha << 24 | 0xFFFFFF;

		float startY = 110.0f;
		float titleFont = 0.4f;
		float itemFont = 0.38f;

		DrawFontString( m_ResourceMap, 30.0f, startY + 300.0f, titleFont, color1, "killed" );
		DrawFontString( m_ResourceMap, 30.0f, startY + 320.0f, titleFont, color1, "crystal" );
		DrawFontString( m_ResourceMap, 160.0f, startY + 300.0f, itemFont, selColor,
						"%d", m_pDispReplayInfo->m_Entries[ m_ReplayNo ].m_Killed );
		DrawFontString( m_ResourceMap, 160.0f, startY + 320.0f, itemFont, selColor,
						"%d", m_pDispReplayInfo->m_Entries[ m_ReplayNo ].m_Crystal );
	}

	void ReplayEntryInfoView::Update()
	{
		switch( m_CurMode ){
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
			case MODE_REPLAY_SELECTED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_INVISIBLE;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_REPLAY_CANCELED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_REPLAY_CHANGED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_REPLAY_SAVED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			default:
				break;
		}

		++m_Counter;
	}

	void ReplayEntryInfoView::Init()
	{
		m_CurMode = MODE_INITIALIZE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ReplayEntryInfoView::Finalize()
	{
		m_CurMode = MODE_FINALIZE;
		m_TransitionCounter = 0;
	}

	void ReplayEntryInfoView::SelectReplay( int replayNo )
	{
		m_CurMode = MODE_REPLAY_SELECTED;
		m_ReplayNo = replayNo;
		m_TransitionCounter = 0;
	}

	void ReplayEntryInfoView::AttachDisplayReplayInfo( DisplayedReplayInfo* pInfo )
	{
		m_pDispReplayInfo = pInfo;
	}

	void ReplayEntryInfoView::CancelReplay()
	{
		m_CurMode = MODE_REPLAY_CANCELED;
		m_TransitionCounter = 0;
	}

	void ReplayEntryInfoView::ChangeReplay( int replayNo )
	{
		m_CurMode = MODE_REPLAY_CHANGED;
		m_TransitionCounter = 0;
		m_ReplayNo = replayNo;
	}

	void ReplayEntryInfoView::SaveReplay()
	{
		m_CurMode = MODE_REPLAY_SAVED;
		m_TransitionCounter = 0;
	}

	//-------------------------------------------
	// ReplayEntryKeySelectionView
	//-------------------------------------------

	const int NAME_CHARS_X = 12;		// 横並び数
	const int NAME_CHARS_Y = 3;			// 縦並び数
	const char NAME_CHARS[ NAME_CHARS_Y ][ NAME_CHARS_X + 1 ] =	{	"abcdefghijkl",
																	"mnopqrstuvwx",
																	"yz0123456789" };

	ReplayEntryKeySelectionView::ReplayEntryKeySelectionView() :	ReplayEntryView(),
																	m_CurMode( MODE_INVISIBLE ),
																	m_TransitionCounter( 0 ),
																	m_Counter( 0 ),
																	m_SelectedCharX( 0 ),
																	m_SelectedCharY( 0 )
	{
	}

	ReplayEntryKeySelectionView::~ReplayEntryKeySelectionView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_TransitionCounter = 0;
		m_Counter = 0;
	}

	void ReplayEntryKeySelectionView::Draw() const
	{
		if( m_CurMode == MODE_INVISIBLE ){
			return;
		}

		int alpha = 0xFF;
		int alpha2 = 0xFF;
		
		if( m_CurMode == MODE_REPLAY_SELECTED ){
			if( m_TransitionCounter <= 10 ){
				alpha = ( m_TransitionCounter * 25 );
				alpha2 = alpha;
			}
		}
		else if( m_CurMode == MODE_REPLAY_CANCELED ){
			if( m_TransitionCounter <= 10 ){
				alpha = ( ( 10 - m_TransitionCounter ) * 25 );
				alpha2 = alpha;
			}
		}
		else if( m_CurMode == MODE_REPLAY_SAVED ){
			if( m_TransitionCounter <= 10 ){
				alpha = ( ( 10 - m_TransitionCounter ) * 25 );
				alpha2 = alpha;
			}
		}
		else if( m_CurMode == MODE_NAME_INPUT ){
			if( m_TransitionCounter <= 10 ){
				
			}
		}
		else if( m_CurMode == MODE_NAME_KEY_CHANGED ){
			if( m_TransitionCounter <= 10 ){
			}
		}

		int color1 = alpha2 << 24 | 0xAAFFAA;
		int selColor = alpha << 24 | 0xFFFFFF;

		float startY = 110.0f;
		float titleFont = 0.4f;
		float itemFont = 0.38f;

		float baseX = 300.0f;
		float baseY = 400.0f;
		float fontScale = 0.55f;
		for( int i = 0; i < NAME_CHARS_Y; ++i ){
			for( int j = 0; j < NAME_CHARS_X; ++j ){
				std::string s;
				s = NAME_CHARS[ i ][ j ];
				if( i == m_SelectedCharY && j == m_SelectedCharX ){
					DrawFontString( m_ResourceMap, baseX + j * 25.0f, baseY + i * 25.0f, fontScale, selColor, s.c_str() );
				}
				else{
					DrawFontString( m_ResourceMap, baseX + j * 25.0f, baseY + i * 25.0f, fontScale, color1, s.c_str() );
				}
			}
		}
	}

	void ReplayEntryKeySelectionView::Update()
	{
		switch( m_CurMode ){
			case MODE_REPLAY_SELECTED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_REPLAY_CANCELED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_INVISIBLE;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_REPLAY_SAVED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_INVISIBLE;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_NAME_INPUT:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_NAME_KEY_CHANGED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			default:
				break;
		}

		++m_Counter;
	}

	void ReplayEntryKeySelectionView::SelectReplay( int replayNo )
	{
		m_CurMode = MODE_REPLAY_SELECTED;
		m_TransitionCounter = 0;
	}

	void ReplayEntryKeySelectionView::CancelReplay()
	{
		m_CurMode = MODE_REPLAY_CANCELED;
		m_TransitionCounter = 0;
	}

	void ReplayEntryKeySelectionView::SaveReplay()
	{
		m_CurMode = MODE_REPLAY_SAVED;
		m_TransitionCounter = 0;
	}

	void ReplayEntryKeySelectionView::NameInput( std::string name, char c )
	{
		m_CurMode = MODE_NAME_INPUT;
		m_TransitionCounter = 0;
	}

	void ReplayEntryKeySelectionView::ChangeNameKey( int x, int y, char c )
	{
		m_CurMode = MODE_NAME_KEY_CHANGED;
		m_TransitionCounter = 0;
		m_SelectedCharX = x;
		m_SelectedCharY = y;
	}

	//-------------------------------------------
	// ReplayEntryNameView
	//-------------------------------------------

	ReplayEntryNameView::ReplayEntryNameView() :	ReplayEntryView(),
													m_CurMode( MODE_INVISIBLE ),
													m_TransitionCounter( 0 ),
													m_Counter( 0 ),
													m_Name(),
													m_SelectedCharX( 0 ),
													m_SelectedCharY( 0 )
	{
	}

	ReplayEntryNameView::~ReplayEntryNameView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_TransitionCounter = 0;
		m_Counter = 0;
		m_Name.clear();
		m_SelectedCharX = 0;
		m_SelectedCharY = 0;
	}

	void ReplayEntryNameView::Draw() const
	{
		if( m_CurMode == MODE_INVISIBLE ){
			return;
		}

		float fontScale = 0.7f;
		int alpha = 0xFF;

		if( m_CurMode == MODE_REPLAY_SELECTED ){
			if( m_TransitionCounter <= 10 ){
				alpha = ( m_TransitionCounter * 25 );
			}
		}
		else if( m_CurMode == MODE_REPLAY_CANCELED ){
			if( m_TransitionCounter <= 10 ){
				alpha = ( ( 10 - m_TransitionCounter ) * 25 );
			}
		}
		else if( m_CurMode == MODE_REPLAY_SAVED ){
			if( m_TransitionCounter <= 10 ){
				alpha = ( ( 10 - m_TransitionCounter ) * 25 );
			}
		}
		else if( m_CurMode == MODE_NAME_INPUT ){
			
		}
		else if( m_CurMode == MODE_NAME_DELETED ){
		}

		int selColor = alpha << 24 | 0xFFFFFF;

		DrawFontString( m_ResourceMap, 60.0f, 420.0f, fontScale, selColor, m_Name.c_str() );
	}

	void ReplayEntryNameView::Update()
	{
		switch( m_CurMode ){
			case MODE_REPLAY_SELECTED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_REPLAY_CANCELED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_INVISIBLE;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_REPLAY_SAVED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_INVISIBLE;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_NAME_INPUT:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_NAME_DELETED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			case MODE_NAME_KEY_CHANGED:
				++m_TransitionCounter;
				if( m_TransitionCounter > 10 ){
					m_CurMode = MODE_NORMAL;
					m_TransitionCounter = 0;
				}
				break;
			default:
				break;
		}

		++m_Counter;
	}

	void ReplayEntryNameView::SelectReplay( int replayNo )
	{
		m_CurMode = MODE_REPLAY_SELECTED;
		m_TransitionCounter = 0;
	}

	void ReplayEntryNameView::CancelReplay()
	{
		m_CurMode = MODE_REPLAY_CANCELED;
		m_TransitionCounter = 0;
	}

	void ReplayEntryNameView::SaveReplay()
	{
		m_CurMode = MODE_REPLAY_SAVED;
		m_TransitionCounter = 0;
	}

	void ReplayEntryNameView::NameInput( std::string name, char c )
	{
		m_CurMode = MODE_NAME_INPUT;
		m_TransitionCounter = 0;
		m_Name = name;
	}

	void ReplayEntryNameView::NameDelete( std::string name, char c )
	{
		m_CurMode = MODE_NAME_DELETED;
		m_TransitionCounter = 0;
		m_Name = name;
	}

	void ReplayEntryNameView::ChangeNameKey( int x, int y, char c )
	{
		m_CurMode = MODE_NAME_KEY_CHANGED;
		m_TransitionCounter = 0;
		m_SelectedCharX = x;
		m_SelectedCharY = y;
	}
}
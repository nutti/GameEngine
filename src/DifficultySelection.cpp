#include <MAPIL/MAPIL.h>

#include "DifficultySelection.h"

#include "ResourceID.h"
#include "GameStateTypes.h"

#include "Util.h"

namespace GameEngine
{

	const int SELECTION_INTERVAL		= 20;

	class DifficultySelection::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;
		ResourceMap					m_ResourceMap;
		DisplayedNormalPlayStat		m_DispNormalPlayStat;
		int							m_Counter;
		int							m_MenuPointed;
		bool						m_PlayBGM;
		int							m_GameMode;
		int							m_PrepareCounter;

		int							m_SelectCounter;

		void DrawBackGround() const;
		void DrawDifficultySelection() const;
		void DrawDifficultyInfo() const;
	public:
		Impl();
		~Impl(){}
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachNormalPlayStat( const DisplayedNormalPlayStat& stat );
		int GetDifficulty() const;
		void SetGameMode( int mode );
	};

	DifficultySelection::Impl::Impl()
	{
		m_Counter = 0;
		m_PrepareCounter = 0;
		m_MenuPointed = GAME_DIFFICULTY_CALM;
		m_SelectCounter = 0;
	}

	SceneType DifficultySelection::Impl::Update()
	{
		// 最初の時のカウンタ
		if( m_Counter < 60 ){
			++m_Counter;
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( m_PrepareCounter > 0 ){
			++m_PrepareCounter;
			if( m_PrepareCounter == 20 ){
				return SCENE_TYPE_STAGE;
			}
			else{
				return SCENE_TYPE_NOT_CHANGE;
			}
		}

		if( m_SelectCounter > 0 ){
			--m_SelectCounter;
			return SCENE_TYPE_NOT_CHANGE;
		}
		if( m_SelectCounter < 0 ){
			++m_SelectCounter;
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			if( m_GameMode == GAME_MODE_NORMAL ){
				MAPIL::PlayStaticBuffer( m_ResourceMap.m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_MENU_SELECTED ] );
				MAPIL::StopStreamingBuffer( GLOBAL_RESOURCE_BGM_ID_MENU );
				m_PrepareCounter = 1;
			}
			else{
				return SCENE_TYPE_STAGE_SELECTION;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
			return SCENE_TYPE_MENU;
		}

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_RIGHT ) ){
			++m_MenuPointed;
			if( m_MenuPointed > GAME_DIFFICULTY_HAZARD ){
				m_MenuPointed = GAME_DIFFICULTY_CALM;
			}
			m_SelectCounter = SELECTION_INTERVAL;
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_LEFT ) ){
			--m_MenuPointed;
			if( m_MenuPointed < GAME_DIFFICULTY_CALM ){
				m_MenuPointed = GAME_DIFFICULTY_HAZARD;
			}
			m_SelectCounter = -SELECTION_INTERVAL;
		}


		++m_Counter;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void DifficultySelection::Impl::DrawBackGround() const
	{
		// 背景画像
		int weight;
		if( m_Counter < 20 ){
			weight = ( m_Counter * 6 ) & 0xFF;
		}
		else{
			if( m_PrepareCounter == 0 ){
				weight = 0x78;		// 120
			}
			else if( m_PrepareCounter > 0 && m_PrepareCounter <= 20 ){
				weight = 0x78 + 10 + m_PrepareCounter * 6;
			}
			else{
				weight = 0xFF;
			}
		}
		int color = weight << 16 | weight << 8 | weight;
		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
							0.0f, 0.0f, false, 0xFF << 24 | color );
	}

	void DifficultySelection::Impl::DrawDifficultySelection() const
	{
		const int texture[] ={	GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_CALM,
								GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_EASY,
								GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_NORMAL,
								GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_HARD,
								GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_HAZARD };

		const float BASE_OFFSET_X = 450.0f;
		float offsetX = -m_MenuPointed * BASE_OFFSET_X;
		float x[ 5 ];
		x[ 0 ] = -560.0f;
		x[ 1 ] = -120.0f;
		x[ 2 ] = 320.0f;
		x[ 3 ] = 760.0f;
		x[ 4 ] = 1200.0f;
		float y[ 5 ] = { 250.0f, 100.0f, 100.0f, 100.0f, 100.0f };
		float scale[ 5 ] = { 0.8f, 0.8f, 0.8f, 0.8f, 0.8f };

		int alpha = 0xFF;
		if( m_Counter < 60 ){
			alpha = m_Counter * 4;
		}
		else{
			if( m_PrepareCounter == 0 ){
				alpha = 0xFF;
			}
			else if( m_PrepareCounter > 0 && m_PrepareCounter <= 20 ){
				alpha = ( 0xFF * ( 20 - m_PrepareCounter ) ) / 20;
			}
			else{
				alpha = 0x00;
			}
		}
		int color = 0xAAAAAA | alpha << 24;
		int selColor = 0xFFFFFF | alpha << 24;
		if( m_SelectCounter > 0 ){
			selColor = 0xAAAAAA | alpha << 24;
		}
		int color2 = 0xFFFFFF | alpha << 24;

		for( int i = 0; i < sizeof( texture ) / sizeof( texture[ 0 ] ); ++i ){
			int prev2;
			int prev;
			int next;
			int next2;
			prev2 = m_MenuPointed - 2;
			if( prev2 < GAME_DIFFICULTY_CALM ){
				prev2 += GAME_DIFFICULTY_TOTAL;
			}
			prev = m_MenuPointed - 1;
			if( prev < GAME_DIFFICULTY_CALM ){
				prev += GAME_DIFFICULTY_TOTAL;
			}
			next = m_MenuPointed + 1;
			if( next >= GAME_DIFFICULTY_TOTAL ){
				next -= GAME_DIFFICULTY_TOTAL;
			}
			next2 = m_MenuPointed + 2;
			if( next2 >= GAME_DIFFICULTY_TOTAL ){
				next2 -= GAME_DIFFICULTY_TOTAL;
			}
			

			const float MOVE_X = 440.0f / SELECTION_INTERVAL;
			const float SCALE_FACTOR = 0.01f;

			scale[ 0 ] = 0.4f + SCALE_FACTOR * m_SelectCounter;
			scale[ 1 ] = 0.6f + SCALE_FACTOR * m_SelectCounter;
			if( m_SelectCounter > 0 ){
				scale[ 2 ] = 0.8f - SCALE_FACTOR * m_SelectCounter;
			}
			else{
				scale[ 2 ] = 0.8f + SCALE_FACTOR * m_SelectCounter;
			}
			scale[ 3 ] = 0.6f - SCALE_FACTOR * m_SelectCounter;
			scale[ 4 ] = 0.4f - SCALE_FACTOR * m_SelectCounter;

			if( i == m_MenuPointed ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ texture[ i ] ],
									x[ 2 ] + m_SelectCounter * MOVE_X, y[ 0 ], scale[ 2 ], scale[ 2 ], true, selColor );
			}
			else if( i == prev2 ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ texture[ i ] ],
									x[ 0 ] + m_SelectCounter * MOVE_X, y[ 0 ], scale[ 0 ], scale[ 0 ], true, color );
			}
			else if( i == prev ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ texture[ i ] ],
									x[ 1 ] + m_SelectCounter * MOVE_X, y[ 0 ], scale[ 1 ], scale[ 1 ], true, color );
			}
			else if( i == next ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ texture[ i ] ],
									x[ 3 ] + m_SelectCounter * MOVE_X, y[ 0 ], scale[ 3 ], scale[ 3 ], true, color );
			}
			else if( i == next2 ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ texture[ i ] ],
									x[ 4 ] + m_SelectCounter * MOVE_X, y[ 0 ], scale[ 4 ], scale[ 4 ], true, color );
			}
		}

		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_TITLE ],
							320.0f, 100.0f, 0.7f, 0.7f, true, color2 );
	}

	void DifficultySelection::Impl::DrawDifficultyInfo() const
	{
		int alpha = 0;
		if( m_Counter >= 20 && m_Counter < 60 ){
			alpha = ( ( m_Counter - 20 ) * 6 + 10 ) & 0xFF;
		}
		else if( m_Counter >= 60 ){
			if( m_PrepareCounter == 0 ){
				alpha = 0xFF;
			}
			else if( m_PrepareCounter > 0 && m_PrepareCounter <= 20 ){
				alpha = ( 0xFF * ( 20 - m_PrepareCounter ) ) / 20;
			}
			else{
				alpha = 0x00;
			}
		}

		int color1 = alpha << 24 | 0xAAFFAA;
		int color2 = alpha << 24 | 0xFFFFFF;
		if( ::abs( m_SelectCounter ) > 0 ){
			color2 = ( alpha * ( SELECTION_INTERVAL - ::abs( m_SelectCounter ) ) / SELECTION_INTERVAL ) << 24 | 0xFFFFFF; 
		}

		float fontScale = 0.6f;

		float x = 155.0f;
		float y[ 4 ];
		float offsetX = 210.0f;
		y[ 0 ] = 380.0f;
		for( int i = 0; i < 3; ++i ){
			y[ i + 1 ] = y[ i ] + 25.0f;
		}

		DrawFontString( m_ResourceMap, x, y[ 0 ], fontScale, color1, "hi score" );
		DrawFontString( m_ResourceMap, x, y[ 1 ], fontScale, color1, "play" );
		DrawFontString( m_ResourceMap, x, y[ 2 ], fontScale, color1, "clear" );
		DrawFontString( m_ResourceMap, x, y[ 3 ], fontScale, color1, "play time" );

		DrawFontString( m_ResourceMap, x + offsetX, y[ 0 ], fontScale, color2, "%d", m_DispNormalPlayStat.m_HIScore[ m_MenuPointed ] );
		DrawFontString( m_ResourceMap, x + offsetX, y[ 1 ], fontScale, color2, "%d", m_DispNormalPlayStat.m_Stat[ m_MenuPointed ].m_Play );
		DrawFontString( m_ResourceMap, x + offsetX, y[ 2 ], fontScale, color2, "%d", m_DispNormalPlayStat.m_Stat[ m_MenuPointed ].m_AllClear );
		DrawFontString(	m_ResourceMap, x + offsetX, y[ 3 ], fontScale, color2, "%d:%02d:%02d",
						m_DispNormalPlayStat.m_Stat[ m_MenuPointed ].m_PlayTime / ( 60 * 60 ),
						( m_DispNormalPlayStat.m_Stat[ m_MenuPointed ].m_PlayTime / 60 ) % 60,
						m_DispNormalPlayStat.m_Stat[ m_MenuPointed ].m_PlayTime % 60 );
	}

	void DifficultySelection::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();

		DrawBackGround();
		DrawDifficultySelection();
		DrawDifficultyInfo();


		MAPIL::EndRendering2DGraphics();
	}

	void DifficultySelection::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_ButtonStatus = *pHolder;
	}

	void DifficultySelection::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	void DifficultySelection::Impl::AttachNormalPlayStat( const DisplayedNormalPlayStat& stat )
	{
		m_DispNormalPlayStat = stat;
	}

	int DifficultySelection::Impl::GetDifficulty() const
	{
		return m_MenuPointed;
	}

	void DifficultySelection::Impl::SetGameMode( int mode )
	{
		m_GameMode = mode;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	DifficultySelection::DifficultySelection() : Scene(), m_pImpl( new DifficultySelection::Impl )
	{
	}

	DifficultySelection::~DifficultySelection()
	{
	}

	void DifficultySelection::Init()
	{
	}

	SceneType DifficultySelection::Update()
	{
		return m_pImpl->Update();
	}

	void DifficultySelection::Draw()
	{
		m_pImpl->Draw();
	}

	void DifficultySelection::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_pImpl->AttachButtonState( pHolder );
	}

	void DifficultySelection::AttachResourceMap( const ResourceMap& map )
	{
		m_pImpl->AttachResourceMap( map );
	}

	void DifficultySelection::AttachNormalPlayStat( const DisplayedNormalPlayStat& stat )
	{
		m_pImpl->AttachNormalPlayStat( stat );
	}

	int DifficultySelection::GetDifficulty() const
	{
		return m_pImpl->GetDifficulty();
	}

	void DifficultySelection::SetGameMode( int mode )
	{
		m_pImpl->SetGameMode( mode );
	}
}
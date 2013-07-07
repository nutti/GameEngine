#include <MAPIL/MAPIL.h>

#include "StageSelection.h"

#include "ResourceID.h"
#include "GameStateTypes.h"

#include "SceneManager/StageSelection/StageSelectionView.h"

#include "Util.h"

namespace GameEngine
{
	static const int PERMITTED_STAGE_TOTAL	= 4;
	static const int SELECTION_INTERVAL		= 20;

	enum SelectionMode
	{
		SELECTION_MODE_DIFFICULTY	= 0,
		SELECTION_MODE_STAGE		= 1,
		SELECTION_MODE_TOTAL,
	};

	class StageSelection::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;
		ResourceMap					m_ResourceMap;
		int							m_Counter;
		int							m_SelectedDifficulty;
		int							m_SelectedStage;
		bool						m_PlayBGM;
		int							m_PrepareCounter;

		int							m_SelectionMode;

		int							m_TransitionCounter;

		int							m_DifficultySelectCounter;

		std::list < std::shared_ptr < StageSelectionView > >		m_ViewList;

		void DrawBackGround() const;
		void DrawDifficultySelection() const;
		void DrawStageSelection() const;
	public:
		Impl();
		~Impl();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		int GetStageNo() const;
	};

	StageSelection::Impl::Impl() : m_ViewList()
	{
		m_Counter = 0;
		m_SelectedStage = STAGE_ID_STAGE_1;
		m_SelectedDifficulty = GAME_DIFFICULTY_CALM;
		m_PrepareCounter = 0;
		m_SelectionMode = SELECTION_MODE_DIFFICULTY;
		m_DifficultySelectCounter = 0;
		m_TransitionCounter = 0;

		m_ViewList.clear();
		StageView* pStageView = new StageView();
		pStageView->SetPermittedStage( PERMITTED_STAGE_TOTAL );
		m_ViewList.push_back( std::shared_ptr < StageSelectionView > ( pStageView ) );
	}

	StageSelection::Impl::~Impl()
	{
		m_ViewList.clear();
	}

	SceneType StageSelection::Impl::Update()
	{

		++m_Counter;

		std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < StageSelectionView > view ){ view->Update(); } );

		if( m_PrepareCounter > 0 ){
			++m_PrepareCounter;
			if( m_PrepareCounter == 20 ){
				return SCENE_TYPE_STAGE;
			}
			else{
				return SCENE_TYPE_NOT_CHANGE;
			}
		}

		if( m_DifficultySelectCounter > 0 ){
			--m_DifficultySelectCounter;
			return SCENE_TYPE_NOT_CHANGE;
		}
		if( m_DifficultySelectCounter < 0 ){
			++m_DifficultySelectCounter;
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( m_TransitionCounter < 30 ){
			if( m_SelectionMode == SELECTION_MODE_STAGE ){
				++m_TransitionCounter;
				return SCENE_TYPE_NOT_CHANGE;
			}
		}
		if( m_TransitionCounter > 0 ){
			if( m_SelectionMode == SELECTION_MODE_DIFFICULTY ){
				--m_TransitionCounter;
				if( m_TransitionCounter == 20 ){
					m_TransitionCounter = 0;
				}
				return SCENE_TYPE_NOT_CHANGE;
			}
		}

		if( m_SelectionMode == SELECTION_MODE_STAGE ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
				MAPIL::PlayStaticBuffer( m_ResourceMap.m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_MENU_SELECTED ] );
				MAPIL::StopStreamingBuffer( GLOBAL_RESOURCE_BGM_ID_MENU );
				m_PrepareCounter = 1;
				std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < StageSelectionView > view ){ view->Finalize(); } );
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
				m_SelectionMode = SELECTION_MODE_DIFFICULTY;
				std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < StageSelectionView > view ){ view->CancelStage(); } );
			}
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
				++m_SelectedStage;
				if( m_SelectedStage > PERMITTED_STAGE_TOTAL - 1 ){
					m_SelectedStage = 0;
				}
				std::for_each( m_ViewList.begin(), m_ViewList.end(), [this]( std::shared_ptr < StageSelectionView > view ){ view->ChangeStage( m_SelectedStage ); } );
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				--m_SelectedStage;
				if( m_SelectedStage < 0 ){
					m_SelectedStage = PERMITTED_STAGE_TOTAL - 1;
				}
				std::for_each( m_ViewList.begin(), m_ViewList.end(), [this]( std::shared_ptr < StageSelectionView > view ){ view->ChangeStage( m_SelectedStage ); } );
			}
		}
		else if( m_SelectionMode == SELECTION_MODE_DIFFICULTY ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
				MAPIL::PlayStaticBuffer( m_ResourceMap.m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_MENU_SELECTED ] );
				m_SelectionMode = SELECTION_MODE_STAGE;
				// ビュークラス全体に通知
				std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < StageSelectionView > view ){ view->SelectDifficulty(); } );
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
				return SCENE_TYPE_DIFFICULTY_SELECTION_IN_STAGE;
			}
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_RIGHT ) ){
				++m_SelectedDifficulty;
				if( m_SelectedDifficulty > GAME_DIFFICULTY_TOTAL - 1 ){
					m_SelectedDifficulty = GAME_DIFFICULTY_CALM;
				}
				m_DifficultySelectCounter = SELECTION_INTERVAL;
				std::for_each( m_ViewList.begin(), m_ViewList.end(), [this]( std::shared_ptr < StageSelectionView > view ){ view->ChangeDifficulty( m_DifficultySelectCounter ); } );
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_LEFT ) ){
				--m_SelectedDifficulty;
				if( m_SelectedDifficulty < GAME_DIFFICULTY_CALM ){
					m_SelectedDifficulty = GAME_DIFFICULTY_TOTAL - 1;
				}
				m_DifficultySelectCounter = -SELECTION_INTERVAL;
				std::for_each( m_ViewList.begin(), m_ViewList.end(), [this]( std::shared_ptr < StageSelectionView > view ){ view->ChangeDifficulty( m_DifficultySelectCounter ); } );
			}
		}

		return SCENE_TYPE_NOT_CHANGE;
	}

	void StageSelection::Impl::DrawBackGround() const
	{
		// 背景画像
		int weight;
		if( m_Counter < 20 ){
			weight = ( m_Counter * 6 ) & 0xFF;
		}
		else{
			if( m_PrepareCounter <= 40 ){
				weight = 0x78;		// 120
			}
			else if( m_PrepareCounter > 40 && m_PrepareCounter <= 60 ){
				weight = 0x78 + 10 + ( m_PrepareCounter - 40 ) * 6;
			}
			else{
				weight = 0xFF;
			}
		}
		int color = weight << 16 | weight << 8 | weight;
		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
							0.0f, 0.0f, false, 0xFF << 24 | color );



		// 背景エフェクト
		int alpha;
		//int alpha2;
		if( m_Counter < 60 ){
			alpha = ( m_Counter ) & 0xFF;
		}
		else{
			if( m_PrepareCounter <= 20 ){
				alpha = 0x3C;		// 60
			}
			else if( m_PrepareCounter > 20 && m_PrepareCounter <= 40 ){
				alpha = 0x3C - ( m_PrepareCounter - 20 ) * 3;
			}
			else{
				alpha = 0x00;
			}
		}

		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
		for( int i = 0; i < 100; ++i ){
			int alpha2;
			if( m_Counter < 120 ){
				alpha2 = ( 0x78 + 0x60 * sin( m_Counter * 0.03f + i * 360 / 100 ) ) * ( m_Counter ) / 120;
			}
			else{
				if( m_PrepareCounter <= 20 ){
					alpha2 = 0x78 + 0x60 * sin( m_Counter * 0.03f + i * 360 / 100 );
				}
				else if( m_PrepareCounter > 20 && m_PrepareCounter <= 40 ){
					alpha2 = ( 0x78 + 0x60 * sin( m_Counter * 0.03f + i * 360 / 100 ) ) * ( 40 - m_PrepareCounter ) / 20;
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

	void StageSelection::Impl::DrawDifficultySelection() const
	{
		const int texture[] ={	GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_CALM,
								GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_EASY,
								GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_NORMAL,
								GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_HARD,
								GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_HAZARD };

		const float BASE_OFFSET_X = 450.0f;
		float offsetX = -m_SelectedDifficulty * BASE_OFFSET_X;
		float offsetY = 0.0f;
		if( m_TransitionCounter > 20 ){
			offsetY = ( m_TransitionCounter - 20 ) * ( -10.0f );
		}
		float x[ 5 ];
		x[ 0 ] = -560.0f;
		x[ 1 ] = -120.0f;
		x[ 2 ] = 320.0f;
		x[ 3 ] = 760.0f;
		x[ 4 ] = 1200.0f;
		float y[ 5 ] = { 250.0f, 100.0f, 100.0f, 100.0f, 100.0f };
		float scale[ 5 ] = { 0.8f, 0.8f, 0.8f, 0.8f, 0.8f };

		int alpha = 0xFF;
		int alpha2 = 0xFF;
		int alpha3 = 0xFF;
		if( m_Counter < 60 ){
			alpha = m_Counter * 4;
			alpha2 = alpha;
			alpha3 = alpha;
		}
		else{
			if( m_TransitionCounter <= 0 ){
				alpha = 0xFF;
				alpha2 = alpha;
				alpha3 = alpha;
			}
			else if( m_TransitionCounter > 0 && m_TransitionCounter <= 15 ){
				alpha = 127 + static_cast < int > ( 127 * cos( MAPIL::DegToRad( m_TransitionCounter * 120.0f ) ) );
				alpha2 = 0xFF;
				alpha3 = 0xFF;
			}
			else if( m_TransitionCounter > 15 && m_TransitionCounter <= 20 ){
				alpha = 0xFF;
				alpha2 = 0xFF;
			}
			else if( m_TransitionCounter > 20 && m_TransitionCounter <= 30 ){
				alpha2 = ( 0xFF * ( 30 - m_TransitionCounter ) ) / 10;
				alpha3 = ( 0xFF * ( 30 - m_TransitionCounter ) ) / 10;
			}
			else{
				alpha = 0x00;
				alpha2 = alpha;
			}
			
		}
		int color = 0xAAAAAA | alpha3 << 24;
		int selColor = 0xFFFFFF | alpha << 24;
		if( m_DifficultySelectCounter > 0 ){
			selColor = 0xAAAAAA | alpha << 24;
		}
		int color2 = 0xFFFFFF | alpha << 24;
		int color3 = 0xFFFFFF | alpha2 << 24;

		for( int i = 0; i < sizeof( texture ) / sizeof( texture[ 0 ] ); ++i ){
			int prev2;
			int prev;
			int next;
			int next2;
			prev2 = m_SelectedDifficulty - 2;
			if( prev2 < GAME_DIFFICULTY_CALM ){
				prev2 += GAME_DIFFICULTY_TOTAL;
			}
			prev = m_SelectedDifficulty - 1;
			if( prev < GAME_DIFFICULTY_CALM ){
				prev += GAME_DIFFICULTY_TOTAL;
			}
			next = m_SelectedDifficulty + 1;
			if( next >= GAME_DIFFICULTY_TOTAL ){
				next -= GAME_DIFFICULTY_TOTAL;
			}
			next2 = m_SelectedDifficulty + 2;
			if( next2 >= GAME_DIFFICULTY_TOTAL ){
				next2 -= GAME_DIFFICULTY_TOTAL;
			}
			

			const float MOVE_X = 440.0f / SELECTION_INTERVAL;
			const float SCALE_FACTOR = 0.01f;

			scale[ 0 ] = 0.4f + SCALE_FACTOR * m_DifficultySelectCounter;
			scale[ 1 ] = 0.6f + SCALE_FACTOR * m_DifficultySelectCounter;
			if( m_DifficultySelectCounter > 0 ){
				scale[ 2 ] = 0.8f - SCALE_FACTOR * m_DifficultySelectCounter;
			}
			else{
				scale[ 2 ] = 0.8f + SCALE_FACTOR * m_DifficultySelectCounter;
			}
			scale[ 3 ] = 0.6f - SCALE_FACTOR * m_DifficultySelectCounter;
			scale[ 4 ] = 0.4f - SCALE_FACTOR * m_DifficultySelectCounter;

			float offsetX = 0.0f;

			if( m_PrepareCounter >= 20 && m_PrepareCounter < 60 ){
				offsetX = ( m_PrepareCounter - 20 ) * 20.0f;
			}

			if( m_Counter < 60 ){
				offsetX = ( 60 - m_Counter ) * 40.0f;
			}

			if( i == m_SelectedDifficulty ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ texture[ i ] ],
									x[ 2 ] + m_DifficultySelectCounter * MOVE_X - offsetX, y[ 0 ] + offsetY, scale[ 2 ], scale[ 2 ], true, selColor );
			}
			else if( i == prev2 ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ texture[ i ] ],
									x[ 0 ] + m_DifficultySelectCounter * MOVE_X, y[ 0 ] + offsetY, scale[ 0 ], scale[ 0 ], true, color );
			}
			else if( i == prev ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ texture[ i ] ],
									x[ 1 ] + m_DifficultySelectCounter * MOVE_X - offsetX, y[ 0 ] + offsetY, scale[ 1 ], scale[ 1 ], true, color );
			}
			else if( i == next ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ texture[ i ] ],
									x[ 3 ] + m_DifficultySelectCounter * MOVE_X + offsetX, y[ 0 ] + offsetY, scale[ 3 ], scale[ 3 ], true, color );
			}
			else if( i == next2 ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ texture[ i ] ],
									x[ 4 ] + m_DifficultySelectCounter * MOVE_X, y[ 0 ] + offsetY, scale[ 4 ], scale[ 4 ], true, color );
			}
		}

		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_TITLE ],
							320.0f, 100.0f, 0.7f, 0.7f, true, color3 );
	}

	void StageSelection::Impl::DrawStageSelection() const
	{
		

		std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < StageSelectionView > view ){ view->Draw(); } );

		/*int alpha = 0xFF;
		int alpha2 = 0xFF;
		int alpha3 = 0xFF;
		if( m_TransitionCounter > 20 && m_TransitionCounter <= 30 ){
			alpha = 0xFF * ( m_TransitionCounter - 20 ) / 10;
		}
		else{
			if( m_PrepareCounter <= 0 ){
				alpha = 0xFF;
				alpha2 = alpha;
				alpha3 = alpha;
			}
			else if( m_PrepareCounter > 0 && m_PrepareCounter <= 15 ){
				alpha = 127 + static_cast < int > ( 127 * cos( MAPIL::DegToRad( m_PrepareCounter * 120.0f ) ) );
				alpha2 = 0xFF;
				alpha3 = 0xFF;
			}
			else if( m_PrepareCounter > 15 && m_PrepareCounter <= 20 ){
				alpha = 0xFF;
				alpha2 = 0xFF;
			}
			else if( m_PrepareCounter > 20 && m_PrepareCounter <= 30 ){
				alpha2 = ( 0xFF * ( 30 - m_PrepareCounter ) ) / 10;
				alpha3 = ( 0xFF * ( 30 - m_PrepareCounter ) ) / 10;
			}
			else{
				alpha = 0x00;
				alpha2 = alpha;
				alpha3 = alpha;
			}
			
		}

		for( int i = 0; i < STAGE_TOTAL; ++i ){
			std::string str = "stage";
			str += ( i + 1 ) + '0';
			if( i == m_SelectedStage ){
				DrawFontString( m_ResourceMap, 20.0f, 250.0f + m_SelectedStage * 30.0f, 0.5f, 0xFFFFFF00, str.c_str() );
			}
			else{
				if( i <= PERMITTED_STAGE_TOTAL - 1 ){
					DrawFontString( m_ResourceMap, 20.0f, 250.0f + i * 30.0f, 0.5f, 0xFFFFFFFF, str.c_str() );
				}
				else{
					DrawFontString( m_ResourceMap, 20.0f, 250.0f + i * 30.0f, 0.5f, 0xFFAAAAAA, str.c_str() );
				}
			}
		}*/
	}

	void StageSelection::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();

		DrawBackGround();
		DrawDifficultySelection();
		DrawStageSelection();

		MAPIL::EndRendering2DGraphics();
	}

	void StageSelection::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_ButtonStatus = *pHolder;
	}

	void StageSelection::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
		std::for_each( m_ViewList.begin(), m_ViewList.end(), [map]( std::shared_ptr < StageSelectionView > view ){ view->AttachResourceMap( map ); } );
	}

	int StageSelection::Impl::GetStageNo() const
	{
		return m_SelectedStage + 1;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	StageSelection::StageSelection() : Scene(), m_pImpl( new StageSelection::Impl )
	{
	}

	StageSelection::~StageSelection()
	{
	}

	void StageSelection::Init()
	{
	}

	SceneType StageSelection::Update()
	{
		return m_pImpl->Update();
	}

	void StageSelection::Draw()
	{
		m_pImpl->Draw();
	}

	void StageSelection::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_pImpl->AttachButtonState( pHolder );
	}

	void StageSelection::AttachResourceMap( const ResourceMap& map )
	{
		m_pImpl->AttachResourceMap( map );
	}

	int StageSelection::GetStageNo() const
	{
		return m_pImpl->GetStageNo();
	}
}
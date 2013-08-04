#include <MAPIL/MAPIL.h>

#include "StageSelection.h"

#include "ResourceID.h"
#include "GameStateTypes.h"

#include "SceneManager/StageSelection/StageSelectionView.h"

#include "Util.h"

namespace GameEngine
{
	static const int PERMITTED_STAGE_TOTAL	= 5;
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
		ButtonStatusHolder					m_ButtonStatus;
		ResourceMap							m_ResourceMap;
		DisplayedStageSelectionPlayStat		m_DispStagePlayStat;
		int									m_Counter;
		int									m_SelectedDifficulty;
		int									m_SelectedStage;
		bool								m_PlayBGM;
		int									m_PrepareCounter;

		int									m_SelectionMode;
		
		int									m_TransitionCounter;

		int									m_ReturnCounter;

		int									m_DifficultySelectCounter;

		std::list < std::shared_ptr < StageSelectionView > >		m_ViewList;

		void DrawBackGround() const;
		void DrawDifficultySelection() const;
		void DrawStageSelection() const;
	public:
		Impl();
		~Impl();
		SceneType Update();
		void Init();
		void Reflesh();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachStageSelectionStat( const DisplayedStageSelectionPlayStat& stat );
		int GetStageNo() const;
		int GetDifficulty() const;
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
		m_ReturnCounter = 0;

		m_ViewList.clear();
	}

	StageSelection::Impl::~Impl()
	{
		m_ViewList.clear();
	}

	void StageSelection::Impl::Init()
	{
		// 背景エフェクトの追加
		StageSelectionBackgroundView* pStageSelectionBGView = new StageSelectionBackgroundView();
		pStageSelectionBGView->AttachResourceMap( m_ResourceMap );
		pStageSelectionBGView->Init();
		m_ViewList.push_back( std::shared_ptr < StageSelectionBackgroundView > ( pStageSelectionBGView ) );

		// ステージ選択画像の追加
		StageView* pStageView = new StageView();
		pStageView->SetPermittedStage( PERMITTED_STAGE_TOTAL );
		pStageView->AttachResourceMap( m_ResourceMap );
		pStageView->Init();
		m_ViewList.push_back( std::shared_ptr < StageSelectionView > ( pStageView ) );

		// ステージ選択タイトル画像の追加
		StageSelectionTitleView* pStageSelectionTitleView = new StageSelectionTitleView();
		pStageSelectionTitleView->AttachResourceMap( m_ResourceMap );
		pStageSelectionTitleView->Init();
		m_ViewList.push_back( std::shared_ptr < StageSelectionTitleView > ( pStageSelectionTitleView ) );

		// 統計情報の追加
		StageSelectionStatView* pStageSelectionStatView = new StageSelectionStatView();
		pStageSelectionStatView->AttachResourceMap( m_ResourceMap );
		pStageSelectionStatView->AttachDisplayedStagePlayStat( &m_DispStagePlayStat );
		pStageSelectionBGView->Init();
		m_ViewList.push_back( std::shared_ptr < StageSelectionStatView > ( pStageSelectionStatView ) );
	}

	void StageSelection::Impl::Reflesh()
	{
		std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < StageSelectionView > view ){ view->Init(); } );
		m_Counter = 0;
		m_PrepareCounter = 0;
		m_DifficultySelectCounter = 0;
		m_TransitionCounter = 0;
		m_ReturnCounter = 0;
		m_SelectionMode = SELECTION_MODE_DIFFICULTY;
	}

	SceneType StageSelection::Impl::Update()
	{

		++m_Counter;

		std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < StageSelectionView > view ){ view->Update(); } );

		if( m_Counter < 60 ){
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( m_PrepareCounter > 0 ){
			++m_PrepareCounter;
			if( m_PrepareCounter == 60 ){
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

		if( m_ReturnCounter > 0 ){
			if( m_ReturnCounter < 15 ){
				++m_ReturnCounter;
			}
			if( m_ReturnCounter == 15 ){
				return SCENE_TYPE_MENU;
			}
			else{
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
				if( m_SelectedStage < PERMITTED_STAGE_TOTAL - 1 ){
					++m_SelectedStage;
					std::for_each( m_ViewList.begin(), m_ViewList.end(), [this]( std::shared_ptr < StageSelectionView > view ){ view->ChangeStage( m_SelectedStage ); } );
				}
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				if( m_SelectedStage > 0 ){
					--m_SelectedStage;
					std::for_each( m_ViewList.begin(), m_ViewList.end(), [this]( std::shared_ptr < StageSelectionView > view ){ view->ChangeStage( m_SelectedStage ); } );
				}
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
				m_ReturnCounter = 1;
				std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < StageSelectionView > view ){ view->CancelDifficulty(); } );
				//return SCENE_TYPE_MENU;
			}
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_RIGHT ) ){
				++m_SelectedDifficulty;
				if( m_SelectedDifficulty > GAME_DIFFICULTY_TOTAL - 1 ){
					m_SelectedDifficulty = GAME_DIFFICULTY_CALM;
				}
				m_DifficultySelectCounter = SELECTION_INTERVAL;
				std::for_each( m_ViewList.begin(), m_ViewList.end(), [this]( std::shared_ptr < StageSelectionView > view ){ view->ChangeDifficulty( m_SelectedDifficulty ); } );
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_LEFT ) ){
				--m_SelectedDifficulty;
				if( m_SelectedDifficulty < GAME_DIFFICULTY_CALM ){
					m_SelectedDifficulty = GAME_DIFFICULTY_TOTAL - 1;
				}
				m_DifficultySelectCounter = -SELECTION_INTERVAL;
				std::for_each( m_ViewList.begin(), m_ViewList.end(), [this]( std::shared_ptr < StageSelectionView > view ){ view->ChangeDifficulty( m_SelectedDifficulty ); } );
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
			if( m_ReturnCounter > 0 ){
				weight = ( ( 15 - m_ReturnCounter ) * 8 ) & 0xFF;
			}
		}
		int color = weight << 16 | weight << 8 | weight;
		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
							0.0f, 0.0f, false, 0xFF << 24 | color );
	}

	void StageSelection::Impl::DrawDifficultySelection() const
	{
		const int texture[] ={	GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_CALM,
								GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_EASY,
								GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_NORMAL,
								GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_HARD,
								GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_HAZARD };

		const float BASE_OFFSET_X = 450.0f;
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		float x[ 5 ] = { -560.0f, -120.0f, 320.0f, 760.0f, 1200.0f };
		float y[ 5 ] = { 250.0f, 100.0f, 100.0f, 100.0f, 100.0f };
		float scale[ 5 ] = { 0.8f, 0.8f, 0.8f, 0.8f, 0.8f };
		int alpha = 0xFF;
		int alpha2 = 0xFF;
		int alpha3 = 0xFF;
		const float MOVE_X = 440.0f / SELECTION_INTERVAL;
		const float SCALE_FACTOR = 0.01f;

		// 拡大率の設定
		float scaleOffset = 1.0f;		// ステージ選択時の拡大率
		if( m_TransitionCounter >= 20 && m_TransitionCounter < 30 ){
			scaleOffset = 0.7f + ( 30 - m_TransitionCounter ) * 0.3f / 10.0f;
		}
		else if( m_TransitionCounter >= 30 ){
			scaleOffset = 0.7f;
		}
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
		for( int i = 0; i < sizeof( scale ) / sizeof( scale[ 0 ] ); ++i ){
			scale[ i ] *= scaleOffset;
		}


		// オフセットX座標の決定
		if( m_Counter < 60 ){
			offsetX = ( 60 - m_Counter ) * 40.0f;
		}
		else{
			if( m_PrepareCounter >= 20 /*&& m_PrepareCounter < 60*/ ){
				offsetX = ( m_PrepareCounter - 20 ) * 20.0f;
			}
			if( m_ReturnCounter > 0 ){
				offsetX = m_ReturnCounter * 80.0f;
			}
		}

		// オフセットY座標の決定
		if( m_TransitionCounter > 20 && m_TransitionCounter <= 30 ){
			offsetY = ( m_TransitionCounter - 20 ) * ( -5.0f );
		}

		// α値の設定
		if( m_Counter < 60 ){
			alpha = m_Counter * 4;
			alpha2 = alpha;
			alpha3 = alpha;
		}
		else{
			// 通常時
			if( m_TransitionCounter <= 0 ){
				alpha = 0xFF;
				alpha2 = alpha;
				alpha3 = alpha;
			}
			// 選択時の点滅処理
			else if( m_TransitionCounter > 0 && m_TransitionCounter <= 15 ){
				alpha = 127 + static_cast < int > ( 127 * cos( MAPIL::DegToRad( m_TransitionCounter * 120.0f ) ) );
				alpha2 = 0xFF;
				alpha3 = 0xFF;
			}
			else if( m_TransitionCounter > 15 && m_TransitionCounter <= 20 ){
				alpha = 0xFF;
				alpha2 = 0xFF;
			}
			// 選択時の上に上がるときの処理
			else if( m_TransitionCounter > 20 && m_TransitionCounter <= 30 ){
				alpha2 = ( 0xFF * ( 30 - m_TransitionCounter ) ) / 10;
				alpha3 = ( 0xFF * ( 30 - m_TransitionCounter ) ) / 10;
			}
			else{
				alpha2 = alpha;
				alpha3 = alpha;
			}

			// ステージへ移行する時
			if( m_PrepareCounter > 20 && m_PrepareCounter <= 40 ){
				alpha = ( 0xFF * ( 40 - m_TransitionCounter ) ) / 20;
				alpha2 = alpha;
				alpha3 = alpha;
			}
			else if( m_PrepareCounter > 40 ){
				alpha = 0.0f;
				alpha2 = alpha;
				alpha3 = alpha;
			}

			// メニュー画面に戻る時
			if( m_ReturnCounter > 0 ){
				alpha = ( 0xFF * ( 20 - m_ReturnCounter ) );
				alpha2 = alpha;
				alpha3 = alpha;
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
	}

	void StageSelection::Impl::DrawStageSelection() const
	{
		std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < StageSelectionView > view ){ view->Draw(); } );
	}

	void StageSelection::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();

		DrawBackGround();
		DrawStageSelection();
		DrawDifficultySelection();

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

	void StageSelection::Impl::AttachStageSelectionStat( const DisplayedStageSelectionPlayStat& stat )
	{
		m_DispStagePlayStat = stat;
	}

	int StageSelection::Impl::GetStageNo() const
	{
		return m_SelectedStage + 1;
	}

	int StageSelection::Impl::GetDifficulty() const
	{
		return m_SelectedDifficulty;
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

	void StageSelection::InitImpl()
	{
		m_pImpl->Init();
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

	void StageSelection::AttachStageSelectionStat( const DisplayedStageSelectionPlayStat& stat )
	{
		m_pImpl->AttachStageSelectionStat( stat );
	}

	int StageSelection::GetStageNo() const
	{
		return m_pImpl->GetStageNo();
	}

	int StageSelection::GetDifficulty() const
	{
		return m_pImpl->GetDifficulty();
	}

	void StageSelection::Reflesh()
	{
		m_pImpl->Reflesh();
	}
}
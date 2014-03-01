#include <MAPIL/MAPIL.h>

#include "Score.h"

#include "SceneManager/Score/ScoreView.h"

#include "ResourceID.h"

#include "Util.h"

namespace GameEngine
{

	class Score::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;
		ResourceMap					m_ResourceMap;
		int							m_Counter;
		DisplayedSaveData			m_DisplayedSaveData;
		int							m_Difficulty;
		int							m_SelectedRecord;
		int							m_PrevDifficulty;

		int							m_SelectCounter;				// �I�����̏����p�J�E���^
		int							m_DifficultySelectCounter;		// ��Փx�I�����̏����p�J�E���^
		int							m_ReturnCounter;

		std::list < std::shared_ptr < ScoreView > >		m_ViewList;

	public:
		Impl();
		~Impl();
		SceneType Update();
		void Init();
		void Reflesh();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachDisplayedSaveData( const DisplayedSaveData& data );
	};

	Score::Impl::Impl()
	{
		m_Counter = 0;
		m_Difficulty = 0;
		m_SelectedRecord = 0;
		m_SelectCounter = 0;
		m_DifficultySelectCounter = 0;
		m_ReturnCounter = 0;

		m_ViewList.clear();
	}

	Score::Impl::~Impl()
	{
		m_ViewList.clear();
	}

	void Score::Impl::Init()
	{
		// �w�i�G�t�F�N�g�̒ǉ�
		ScoreBackgroundView* pScoreBGView = new ScoreBackgroundView();
		pScoreBGView->AttachResourceMap( m_ResourceMap );
		pScoreBGView->Init();
		m_ViewList.push_back( std::shared_ptr < ScoreView > ( pScoreBGView ) );

		// ��Փx�摜�̒ǉ�
		ScoreDifficultyView* pScoreDiffView = new ScoreDifficultyView();
		pScoreDiffView->AttachResourceMap( m_ResourceMap );
		pScoreDiffView->Init();
		m_ViewList.push_back( std::shared_ptr < ScoreDifficultyView > ( pScoreDiffView ) );

		// �^�C�g���摜�̒ǉ�
		ScoreTitleView* pTitleView = new ScoreTitleView();
		pTitleView->AttachResourceMap( m_ResourceMap );
		pTitleView->Init();
		m_ViewList.push_back( std::shared_ptr < ScoreTitleView > ( pTitleView ) );
	}

	void Score::Impl::Reflesh()
	{
		std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < ScoreView > view ){ view->Init(); } );
		m_Counter = 0;
		m_SelectCounter = 0;
		m_DifficultySelectCounter = 0;
		m_ReturnCounter = 0;
	}

	SceneType Score::Impl::Update()
	{
		std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < ScoreView > view ){ view->Update(); } );

		// �ŏ��̎��̃J�E���^
		if( m_Counter < 60 ){
			++m_Counter;
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( m_SelectCounter > 0 ){
			--m_SelectCounter;
		}

		if( m_DifficultySelectCounter > 0 ){
			--m_DifficultySelectCounter;
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


		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
			m_ReturnCounter = 1;
			std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < ScoreView > view ){ view->BackToMenu(); } );
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_RIGHT ) ){
			m_PrevDifficulty = m_Difficulty++;
			if( m_Difficulty > GAME_DIFFICULTY_TOTAL - 1 ){
				m_Difficulty = GAME_DIFFICULTY_CALM;
			}
			m_DifficultySelectCounter = 20;
			std::for_each( m_ViewList.begin(), m_ViewList.end(), [this]( std::shared_ptr < ScoreView > view ){ view->ChangeDifficulty( m_Difficulty ); } );
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_LEFT ) ){
			m_PrevDifficulty = m_Difficulty--;
			if( m_Difficulty < GAME_DIFFICULTY_CALM ){
				m_Difficulty = GAME_DIFFICULTY_TOTAL - 1;
			}
			m_DifficultySelectCounter = 20;
			std::for_each( m_ViewList.begin(), m_ViewList.end(), [this]( std::shared_ptr < ScoreView > view ){ view->ChangeDifficulty( m_Difficulty ); } );
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
			++m_SelectedRecord;
			if( m_SelectedRecord > MAX_SCORE_ENTRY - 1 ){
				m_SelectedRecord = 0;
			}
			m_SelectCounter = 10;
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
			--m_SelectedRecord;
			if( m_SelectedRecord < 0 ){
				m_SelectedRecord = MAX_SCORE_ENTRY - 1;
			}
			m_SelectCounter = 10;
		}

		++m_Counter;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void Score::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();

		// �w�i�摜
		int weight;
		if( m_Counter < 20 ){
			weight = ( m_Counter * 6 ) & 0xFF;
		}
		else{
			if( m_ReturnCounter > 0 ){
				weight = 0x78 * ( 15 - m_ReturnCounter ) / 15;
			}
			else{
				weight = 0x78;		// 120
			}
		}
		int color = weight << 16 | weight << 8 | weight;
		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
							0.0f, 0.0f, false, 0xFF << 24 | color );

		std::for_each( m_ViewList.begin(), m_ViewList.end(), []( std::shared_ptr < ScoreView > view ){ view->Draw(); } );

		// �X�R�A�f�[�^�ꗗ�\��
		char* progStr[] = { "", "st 1", "st 2", "st 3", "st 4", "st 5", "all" };
		int alpha1 = 0;
		int alpha2 = 0;
		int dispDifficulty = GAME_DIFFICULTY_CALM;
		if( m_Counter >= 20 && m_Counter < 60 ){
			alpha1 = ( ( m_Counter - 20 ) * 6 + 10 ) & 0xFF;
			alpha2 = alpha1;
		}
		else if( m_Counter >= 60 ){
			alpha1 = 0xFF;
			if( m_DifficultySelectCounter == 0 ){
				alpha2 = alpha1;
				dispDifficulty = m_Difficulty;
			}
			else if( m_DifficultySelectCounter > 0 && m_DifficultySelectCounter <= 10 ){
				alpha2 = ( ( 10 - m_DifficultySelectCounter ) * 25 ) & 0xFF;
				dispDifficulty = m_Difficulty;
			}
			else if( m_DifficultySelectCounter > 10 && m_DifficultySelectCounter <= 20 ){
				alpha2 = ( ( m_DifficultySelectCounter - 10 ) * 25 + 5 ) & 0xFF;
				dispDifficulty = m_PrevDifficulty;
			}
			if( m_ReturnCounter > 0 ){
				alpha1 = 0xFF * ( 15 - m_ReturnCounter ) / 15;
				alpha2 = alpha1;
			}
		}
		int color1 = alpha1 << 24 | 0xAAFFAA;
		int color2 = alpha2 << 24 | 0xAAAAAA;
		int selColor = alpha2 << 24 | 0xFFFFFF;

		float titleFont = 1.0f;
		float itemFont = 1.0f;

		enum ItemID
		{
			ITEM_ID_RANK	= 0,
			ITEM_ID_NAME	= 1,
			ITEM_ID_PROG	= 2,
			ITEM_ID_SCORE	= 3,
			ITEM_ID_DATE	= 4,
			ITEM_ID_TOTAL,
		};

		float startX[ ITEM_ID_TOTAL ];
		startX[ ITEM_ID_RANK ] = 50.0f;
		startX[ ITEM_ID_NAME ] = startX[ ITEM_ID_RANK ] + 60.0f;
		startX[ ITEM_ID_PROG ] = startX[ ITEM_ID_NAME ] + 120.0f;
		startX[ ITEM_ID_SCORE ] = startX[ ITEM_ID_PROG ] + 70.0f;
		startX[ ITEM_ID_DATE ] = startX[ ITEM_ID_SCORE ] + 130.0f;

		DrawFontString( m_ResourceMap, startX[ ITEM_ID_RANK ], 85.0f, titleFont, color1, "rank" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_NAME ], 85.0f, titleFont, color1, "name" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_PROG ], 85.0f, titleFont, color1, "prog" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_SCORE ], 85.0f, titleFont, color1, "score" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_DATE ], 85.0f, titleFont, color1, "date" );
		for( int i = 0; i < MAX_SCORE_ENTRY; ++i ){
			int c = color2;
			float offsetX = 0.0f;
			float offsetY = 0.0f;
			if( i == m_SelectedRecord ){
				if( m_SelectCounter > 0 ){
					offsetX = -2.0f * sin( MAPIL::DegToRad( m_SelectCounter * 45.0 ) );
				}
				else{
					offsetX = -2.0f;
				}
				offsetY = 0.0f;
				c = selColor;
			}
			DrawFontString( m_ResourceMap, startX[ ITEM_ID_RANK ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, titleFont, color1, "%d", i + 1 );
			if( m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ i ].m_Date.m_Year != 0 ){
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_NAME ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ i ].m_Name );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_PROG ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, progStr[ m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ i ].m_Progress ] );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_SCORE ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, "%d", m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ i ].m_Score );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_DATE ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, "%02d/%02d/%02d %02d:%02d",
								m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ i ].m_Date.m_Year % 100,
								m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ i ].m_Date.m_Month,
								m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ i ].m_Date.m_Day,
								m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ i ].m_Date.m_Hour,
								m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ i ].m_Date.m_Min );
			}
			else{
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_NAME ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, "no entry" );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_PROG ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, progStr[ 1 ] );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_SCORE ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, "%d", 0 );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_DATE ] + offsetX, 90.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, "%02d/%02d/%02d %02d:%02d",
								0,
								1,
								1,
								0,
								0 );
			}
		}

		DrawFontString( m_ResourceMap, 30.0f, 383.0f, titleFont, color1, "killed" );
		DrawFontString( m_ResourceMap, 30.0f, 403.0f, titleFont, color1, "crystal" );

		// �ڍ׏��̕\��
		int color3 = 0;
		if( m_Counter < 60 ){
			if( m_Counter > 20 ){
				color3 = ( ( ( m_Counter - 20 ) * 6 + 10 ) & 0xFF ) << 24 | 0xFFFFFF;
			}
			else{
				color3 = 0;
			}
		}
		else{
			if( m_DifficultySelectCounter > 0 ){
				color3 = color2;
			}
			else if( m_SelectCounter > 0 ){
				color3 = ( ( 10 - m_SelectCounter ) * 25 ) << 24 | 0xFFFFFF;
			}
			else{
				color3 = 0xFFFFFFFF;
			}
			if( m_ReturnCounter > 0 ){
				color3 = color2;
			}
		}
		float dispOffsetY = 400.0f;
		float dispOffsetX[ 5 ];
		dispOffsetX[ 0 ] = 260.0f;
		dispOffsetX[ 1 ] = dispOffsetX[ 0 ] + 30.0f;
		dispOffsetX[ 2 ] = dispOffsetX[ 1 ] + 120.0f;
		dispOffsetX[ 3 ] = dispOffsetX[ 2 ] + 60.0f;
		dispOffsetX[ 4 ] = dispOffsetX[ 3 ] + 85.0f;
		DrawFontString( m_ResourceMap, dispOffsetX[ 1 ], dispOffsetY - 17.0f, titleFont, color1, "Score" );
		DrawFontString( m_ResourceMap, dispOffsetX[ 2 ], dispOffsetY - 17.0f, titleFont, color1, "Kill" );
		DrawFontString( m_ResourceMap, dispOffsetX[ 3 ], dispOffsetY - 17.0f, titleFont, color1, "Crys" );
		DrawFontString( m_ResourceMap, dispOffsetX[ 4 ], dispOffsetY - 17.0f, titleFont, color1, "Prog" );

		if( m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ m_SelectedRecord ].m_Date.m_Year == 0 ){
			DrawFontString( m_ResourceMap, 160.0f, 383.0f, itemFont, color3, "%d", 0 );
			DrawFontString( m_ResourceMap, 160.0f, 403.0f, itemFont, color3, "%d", 0 );
		}
		else{
			DrawFontString( m_ResourceMap, 160.0f, 383.0f, itemFont, color3,
							"%d", m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ m_SelectedRecord ].m_Killed );
			DrawFontString( m_ResourceMap, 160.0f, 403.0f, itemFont, color3,
							"%d", m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ m_SelectedRecord ].m_Crystal );
		}

		for( int i = 0; i < STAGE_TOTAL; ++i ){
			if( i != STAGE_TOTAL + 1 ){
				DrawFontString( m_ResourceMap, dispOffsetX[ 0 ], dispOffsetY + i * 17.0f, titleFont, color1, "%d", i + 1 );
				if( m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ m_SelectedRecord ].m_StageData[ i ].m_Progress <= 0 ){
					DrawFontString( m_ResourceMap, dispOffsetX[ 1 ], dispOffsetY + i * 17.0f, itemFont, color3, "%d", 0 );
					DrawFontString( m_ResourceMap, dispOffsetX[ 2 ], dispOffsetY + i * 17.0f, itemFont, color3, "%d", 0 );
					DrawFontString( m_ResourceMap, dispOffsetX[ 3 ], dispOffsetY + i * 17.0f, itemFont, color3, "%d", 0 );
					DrawFontString( m_ResourceMap, dispOffsetX[ 4 ], dispOffsetY + i * 17.0f, itemFont, color3, "%d", 0 );
				}
				else{
					DrawFontString( m_ResourceMap, dispOffsetX[ 1 ], dispOffsetY + i * 17.0f, itemFont, color3, "%d",
									m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ m_SelectedRecord ].m_StageData[ i ].m_Score );
					DrawFontString( m_ResourceMap, dispOffsetX[ 2 ], dispOffsetY + i * 17.0f, itemFont, color3, "%d",
									m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ m_SelectedRecord ].m_StageData[ i ].m_Killed );
					DrawFontString( m_ResourceMap, dispOffsetX[ 3 ], dispOffsetY + i * 17.0f, itemFont, color3, "%d",
									m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ m_SelectedRecord ].m_StageData[ i ].m_Crystal );
					DrawFontString( m_ResourceMap, dispOffsetX[ 4 ], dispOffsetY + i * 17.0f, itemFont, color3, "%d",
									m_DisplayedSaveData.m_Difficulty[ dispDifficulty ].m_Record[ m_SelectedRecord ].m_StageData[ i ].m_Progress );
				}
			}
		}

		MAPIL::EndRendering2DGraphics();
	}

	void Score::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_ButtonStatus = *pHolder;
	}

	void Score::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	void Score::Impl::AttachDisplayedSaveData( const DisplayedSaveData& data )
	{
		m_DisplayedSaveData = data;
	}

	// ----------------------------------
	// �����N���X�̌Ăяo��
	// ----------------------------------

	Score::Score() : Scene(), m_pImpl( new Score::Impl )
	{
	}

	Score::~Score()
	{
	}

	void Score::InitImpl()
	{
		m_pImpl->Init();
	}

	SceneType Score::Update()
	{
		return m_pImpl->Update();
	}

	void Score::Draw()
	{
		m_pImpl->Draw();
	}

	void Score::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_pImpl->AttachButtonState( pHolder );
	}

	void Score::AttachResourceMap( const ResourceMap& map )
	{
		m_pImpl->AttachResourceMap( map );
	}

	void Score::AttachDisplayedSaveData( const DisplayedSaveData& data )
	{
		m_pImpl->AttachDisplayedSaveData( data );
	}

	void Score::Reflesh()
	{
		m_pImpl->Reflesh();
	}
}
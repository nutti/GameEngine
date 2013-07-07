#include "StageSelectionView.h"

#include "../../GameStateTypes.h"

#include "../../Util.h"

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
								m_CancelCounter( 0 ),
								m_ChangeCounter( 0 ),
								m_FinalizeCounter( 0 ),
								m_SelectCounter( 0 ),
								m_DifficultySelectCounter( 0 ),
								m_SelectedStage( 0 ),
								m_PermittedStage( 0 ),
								m_TransitionCounter( 0 )
	{
	}

	StageView::~StageView()
	{
		m_CurMode = MODE_INVISIBLE;
		m_CancelCounter = 0;
		m_ChangeCounter = 0;
		m_FinalizeCounter = 0;
		m_SelectCounter = 0;
		m_DifficultySelectCounter = 0;
		m_SelectedStage = 0;
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

		enum Color
		{
			SELECTED		= 0,
			NOT_PERMITTED	= 1,
			PERMITTED		= 2,
			COLOR_TOTAL,
		};

		int color[ COLOR_TOTAL ];
		int alpha = 0xFF;
		

		const float STAGE_BASE_POS_X = 70.0f;
		const float STAGE_BASE_POS_Y = 350.0f;
		const float STAGE_OFFSET_POS_Y = 20.0f;

		float offsetY = 0.0f;
		float offsetX = 0.0f;

		if( m_CurMode == MODE_DIFFICULTY_SELECTED ){
			offsetX = ( m_TransitionCounter - 30 ) * 3.0f;
			alpha = ( m_TransitionCounter - 15 ) * 17 < 0 ? 0 : ( m_TransitionCounter - 15 ) * 17;
		}
		else if( m_CurMode == MODE_CANCELED ){
			offsetX = -m_TransitionCounter * 3.0f;
			alpha = ( 15 - m_TransitionCounter ) * 17 < 0 ? 0 : ( 15 - m_TransitionCounter ) * 17;
		}

		color[ SELECTED ] = ( alpha << 24 ) | 0xFFFF00;
		color[ NOT_PERMITTED ] = ( alpha << 24 ) | 0xAAAAAA;
		color[ PERMITTED ] = ( alpha << 24 ) | 0xFFFFFF;


		for( int i = 0; i < STAGE_TOTAL; ++i ){
			std::string str = "stage";
			str += ( i + 1 ) + '0';
			if( i == m_SelectedStage ){
				DrawFontString(	m_ResourceMap,
								STAGE_BASE_POS_X + offsetX,
								STAGE_BASE_POS_Y + m_SelectedStage * STAGE_OFFSET_POS_Y,
								0.5f, color[ SELECTED ], str.c_str() );
			}
			else{
				if( i <= m_PermittedStage - 1 ){
					DrawFontString(	m_ResourceMap,
									STAGE_BASE_POS_X + offsetX,
									STAGE_BASE_POS_Y + i * STAGE_OFFSET_POS_Y,
									0.5f, color[ PERMITTED ], str.c_str() );
				}
				else{
					DrawFontString(	m_ResourceMap,
									STAGE_BASE_POS_X + offsetX,
									STAGE_BASE_POS_Y + i * STAGE_OFFSET_POS_Y,
									0.5f, color[ NOT_PERMITTED ], str.c_str() );
				}
			}
		}
	}

	void StageView::Update()
	{
		switch( m_CurMode ){
			case MODE_DIFFICULTY_SELECTED:
				//++m_DifficultySelectCounter;
				++m_TransitionCounter;
				if( m_TransitionCounter > 30 ){
					m_CurMode = MODE_STAGE_SELECTED;
					m_TransitionCounter = 10;
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
			default:
				break;
		}
	}

	void StageView::CancelStage()
	{
		m_CurMode = MODE_CANCELED;
		m_CancelCounter = 0;
		m_TransitionCounter = 0;
	}

	void StageView::SelectStage()
	{
		m_CurMode = MODE_STAGE_SELECTED;
		m_SelectCounter = 0;
	}

	void StageView::ChangeStage( int stage )
	{
		m_SelectedStage = stage;
		m_CurMode = MODE_CHANGED;
		m_ChangeCounter = 0;
	}


	void StageView::SelectDifficulty()
	{
		m_CurMode = MODE_DIFFICULTY_SELECTED;
		m_DifficultySelectCounter = 0;
		m_TransitionCounter = 0;
	}

	void StageView::Finalize()
	{
		m_CurMode = MODE_FINALIZE;
		m_FinalizeCounter = 0;
	}
}
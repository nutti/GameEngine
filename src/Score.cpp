#include <MAPIL/MAPIL.h>

#include "Score.h"

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
		int							m_MovementCounter;
		int							m_PrevDifficulty;
		int							m_ScrollDirection;
		int							m_SelectedRecord;
	public:
		Impl();
		~Impl(){}
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachDisplayedSaveData( const DisplayedSaveData& data );
	};

	Score::Impl::Impl()
	{
		m_Counter = 0;
		m_Difficulty = 0;
		m_MovementCounter = 100;
		m_ScrollDirection = 0;
		m_SelectedRecord = 0;
	}

	SceneType Score::Impl::Update()
	{
		
		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
			return SCENE_TYPE_MENU;
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_RIGHT ) ){
			m_PrevDifficulty = m_Difficulty++;
			m_MovementCounter = 0;
			m_ScrollDirection = 1;		// 左側にスクロール
			if( m_Difficulty > 3 ){
				m_Difficulty = 0;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_LEFT ) ){
			m_PrevDifficulty = m_Difficulty--;
			m_MovementCounter = 0;
			m_ScrollDirection = 2;		// 右側にスクロール
			if( m_Difficulty < 0 ){
				m_Difficulty = 3;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
			++m_SelectedRecord;
			if( m_SelectedRecord > 14 ){
				m_SelectedRecord = 0;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
			--m_SelectedRecord;
			if( m_SelectedRecord < 0 ){
				m_SelectedRecord = 14;
			}
		}

		++m_Counter;
		++m_MovementCounter;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void Score::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();

		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
							0.0f, 0.0f, 640.0f, 480.0f, 0.0f, false, 0xFF999999 );

		char* difStr[] = { "easy", "normal", "hard", "hazard" };
		char* progStr[] = { "", "stage 1", "stage 2", "stage 3", "stage 4", "stage 5", "all clear" };

		DrawFontString( m_ResourceMap, 200.0f, 30.0f, 1.0f, 0xFFFFFF00, difStr[ m_Difficulty ] );

		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
							0.0f, 99.0f + m_SelectedRecord * 17.0f, 50.0f, 1.0f, false, 0xDD5577FF );

		// 縦にスクロールした場合
		if( m_Counter < 20 ){
			DrawFontString( m_ResourceMap, 30.0f, 505.0f - m_Counter * 21.0f, 0.5f, 0xFFAAFFAA, "rank" );
			DrawFontString( m_ResourceMap, 100.0f, 505.0f - m_Counter * 21.0f, 0.5f, 0xFFAAFFAA, "name" );
			DrawFontString( m_ResourceMap, 200.0f, 505.0f - m_Counter * 21.0f, 0.5f, 0xFFAAFFAA, "progress" );
			DrawFontString( m_ResourceMap, 350.0f, 505.0f - m_Counter * 21.0f, 0.5f, 0xFFAAFFAA, "score" );
			DrawFontString( m_ResourceMap, 500.0f, 505.0f - m_Counter * 21.0f, 0.5f, 0xFFAAFFAA, "date" );
			for( int i = 0; i < 15; ++i ){
				DrawFontString( m_ResourceMap, 30.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", i + 1 );
				DrawFontString( m_ResourceMap, 100.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f, 0.45f, m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Name );
				DrawFontString( m_ResourceMap, 200.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f, 0.45f, progStr[ m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Progress ] );
				DrawFontString( m_ResourceMap, 350.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Score );
				DrawFontString( m_ResourceMap, 500.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f, 0.45f, "%04d%02d%02d%02d%02d",
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Year,
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Month,
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Day,
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Hour,
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Min );
			}
		}
		// 横にスクロールした場合
		else if( m_MovementCounter <= 30 ){
			float dir = 0.0f;
			// 左側にスクロール
			if( m_ScrollDirection == 1 ){
				dir = -1.0f;
			}
			else if( m_ScrollDirection == 2 ){
				dir = 1.0f;
			}
			DrawFontString( m_ResourceMap, 30.0f + dir * m_MovementCounter * 21.0f, 85.0f, 0.5f, 0xFFAAFFAA, "rank" );
			DrawFontString( m_ResourceMap, 100.0f + dir * m_MovementCounter * 21.0f, 85.0f, 0.5f, 0xFFAAFFAA, "name" );
			DrawFontString( m_ResourceMap, 200.0f + dir * m_MovementCounter * 21.0f, 85.0f, 0.5f, 0xFFAAFFAA, "progress" );
			DrawFontString( m_ResourceMap, 350.0f + dir * m_MovementCounter * 21.0f, 85.0f, 0.5f, 0xFFAAFFAA, "score" );
			DrawFontString( m_ResourceMap, 500.0f + dir * m_MovementCounter * 21.0f, 85.0f, 0.5f, 0xFFAAFFAA, "date" );

			DrawFontString( m_ResourceMap, 30.0f - 640.0f * dir + dir * m_MovementCounter * 21.0f, 85.0f, 0.5f, 0xFFAAFFAA, "rank" );
			DrawFontString( m_ResourceMap, 100.0f - 640.0f * dir + dir * m_MovementCounter * 21.0f, 85.0f, 0.5f, 0xFFAAFFAA, "name" );
			DrawFontString( m_ResourceMap, 200.0f - 640.0f * dir + dir * m_MovementCounter * 21.0f, 85.0f, 0.5f, 0xFFAAFFAA, "progress" );
			DrawFontString( m_ResourceMap, 350.0f - 640.0f * dir + dir * m_MovementCounter * 21.0f, 85.0f, 0.5f, 0xFFAAFFAA, "score" );
			DrawFontString( m_ResourceMap, 500.0f - 640.0f * dir + dir * m_MovementCounter * 21.0f, 85.0f, 0.5f, 0xFFAAFFAA, "date" );

			for( int i = 0; i < 15; ++i ){
				DrawFontString( m_ResourceMap, 30.0f + dir * m_MovementCounter * 21.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", i + 1 );
				DrawFontString( m_ResourceMap, 100.0f + dir * m_MovementCounter * 21.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, m_DisplayedSaveData.m_Difficulty[ m_PrevDifficulty ].m_Record[ i ].m_Name );
				DrawFontString( m_ResourceMap, 200.0f + dir * m_MovementCounter * 21.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, progStr[ m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Progress ] );
				DrawFontString( m_ResourceMap, 350.0f + dir * m_MovementCounter * 21.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Score );
				DrawFontString( m_ResourceMap, 500.0f + dir * m_MovementCounter * 21.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%04d%02d%02d%02d%02d",
								m_DisplayedSaveData.m_Difficulty[ m_PrevDifficulty ].m_Record[ i ].m_Date.m_Year,
								m_DisplayedSaveData.m_Difficulty[ m_PrevDifficulty ].m_Record[ i ].m_Date.m_Month,
								m_DisplayedSaveData.m_Difficulty[ m_PrevDifficulty ].m_Record[ i ].m_Date.m_Day,
								m_DisplayedSaveData.m_Difficulty[ m_PrevDifficulty ].m_Record[ i ].m_Date.m_Hour,
								m_DisplayedSaveData.m_Difficulty[ m_PrevDifficulty ].m_Record[ i ].m_Date.m_Min );
				DrawFontString( m_ResourceMap, 30.0f - 640.0f * dir + dir * m_MovementCounter * 21.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", i + 1 );
				DrawFontString( m_ResourceMap, 100.0f - 640.0f * dir + dir * m_MovementCounter * 21.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Name );
				DrawFontString( m_ResourceMap, 200.0f - 640.0f * dir + dir * m_MovementCounter * 21.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, progStr[ m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Progress ] );
				DrawFontString( m_ResourceMap, 350.0f - 640.0f * dir + dir * m_MovementCounter * 21.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Score );
				DrawFontString( m_ResourceMap, 500.0f - 640.0f * dir + dir * m_MovementCounter * 21.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%04d%02d%02d%02d%02d",
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Year,
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Month,
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Day,
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Hour,
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Min );
			}
		}
		// 通常の表示
		else{
			DrawFontString( m_ResourceMap, 30.0f, 85.0f, 0.5f, 0xFFAAFFAA, "rank" );
			DrawFontString( m_ResourceMap, 100.0f, 85.0f, 0.5f, 0xFFAAFFAA, "name" );
			DrawFontString( m_ResourceMap, 200.0f, 85.0f, 0.5f, 0xFFAAFFAA, "progress" );
			DrawFontString( m_ResourceMap, 350.0f, 85.0f, 0.5f, 0xFFAAFFAA, "score" );
			DrawFontString( m_ResourceMap, 500.0f, 85.0f, 0.5f, 0xFFAAFFAA, "date" );
			for( int i = 0; i < 15; ++i ){
				if( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Year != 0 ){
					DrawFontString( m_ResourceMap, 30.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", i + 1 );
					DrawFontString( m_ResourceMap, 100.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Name );
					DrawFontString( m_ResourceMap, 200.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, progStr[ m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Progress ] );
					DrawFontString( m_ResourceMap, 350.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Score );
					DrawFontString( m_ResourceMap, 500.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%04d%02d%02d%02d%02d",
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Year,
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Month,
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Day,
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Hour,
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Min );
				}
			}
			DrawFontString( m_ResourceMap, 30.0f, 380.0f, 0.5f, 0xFFAAFFAA, "killed" );
			DrawFontString( m_ResourceMap, 30.0f, 400.0f, 0.5f, 0xFFAAFFAA, "crystal" );
			DrawFontString( m_ResourceMap, 160.0f, 380.0f, 0.45f,
							"%d", m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ m_SelectedRecord ].m_Killed );
			DrawFontString( m_ResourceMap, 160.0f, 400.0f, 0.45f,
							"%d", m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ m_SelectedRecord ].m_Crystal );
			for( int i = 0; i < m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ m_SelectedRecord ].m_Progress + 5; ++i ){
				if( i != 6 ){
					DrawFontString( m_ResourceMap, 280.0f, 380.0f + i * 20.0f, 0.5f, 0xFFAAFFAA, "%d", i + 1 );
					DrawFontString( m_ResourceMap, 320.0f, 380.0f + i * 20.0f, 0.5f, "%d",
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ m_SelectedRecord ].m_StageData[ i ].m_Score );
					DrawFontString( m_ResourceMap, 440.0f, 380.0f + i * 20.0f, 0.5f, "%d",
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ m_SelectedRecord ].m_StageData[ i ].m_Killed );
					DrawFontString( m_ResourceMap, 520.0f, 380.0f + i * 20.0f, 0.5f, "%d",
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ m_SelectedRecord ].m_StageData[ i ].m_Crystal );
					DrawFontString( m_ResourceMap, 600.0f, 380.0f + i * 20.0f, 0.5f, "%d",
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ m_SelectedRecord ].m_StageData[ i ].m_Progress );
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
	// 実装クラスの呼び出し
	// ----------------------------------

	Score::Score() : Scene(), m_pImpl( new Score::Impl )
	{
	}

	Score::~Score()
	{
	}

	void Score::Init()
	{
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
}
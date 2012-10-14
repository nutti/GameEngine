#include <MAPIL/MAPIL.h>

#include "ScoreEntry.h"

#include "ResourceID.h"

#include "Util.h"

namespace GameEngine
{

	class ScoreEntry::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;
		ResourceMap					m_ResourceMap;
		int							m_Counter;
		DisplayedSaveData			m_DisplayedSaveData;
		int							m_Difficulty;
		int							m_NewEntryRank;			// 新しくエントリされるランク（ランク外の場合は-1）
		int							m_EntryName[ 10 ];		// エントリ名（9文字まで）
	public:
		Impl();
		~Impl(){}
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachDisplayedSaveData( const DisplayedSaveData& data );
		void AttachRecord( const SaveDataRecord& record );
		void SetDifficulty( int difficulty );
	};

	ScoreEntry::Impl::Impl()
	{
		m_Counter = 0;
		m_Difficulty = GAME_DIFFICULTY_EASY;
		m_NewEntryRank = -1;
	}

	SceneType ScoreEntry::Impl::Update()
	{
		if( m_NewEntryRank == -1 ){
			return SCENE_TYPE_MENU;
		}

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
			return SCENE_TYPE_MENU;
		}

		++m_Counter;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void ScoreEntry::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();

		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
							0.0f, 0.0f, 640.0f, 480.0f, 0.0f, false, 0xFF999999 );

		char* progStr[] = { "", "stage 1", "stage 2", "stage 3", "stage 4", "stage 5", "all clear" };

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
		else{
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								0.0f, 99.0f + m_NewEntryRank * 17.0f, 50.0f, 1.0f, false, 0xDD5577FF );

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
		}


		MAPIL::EndRendering2DGraphics();
	}

	void ScoreEntry::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_ButtonStatus = *pHolder;
	}

	void ScoreEntry::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	void ScoreEntry::Impl::AttachDisplayedSaveData( const DisplayedSaveData& data )
	{
		m_DisplayedSaveData = data;
	}

	void ScoreEntry::Impl::AttachRecord( const SaveDataRecord& record )
	{
		int newScore = record.m_Score;
		for( int i = 0; i < sizeof( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record ) / sizeof( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ 0 ] ); ++i ){
			if( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Score < newScore ){
				m_NewEntryRank = i;
				// 消去されるものの全コピー
				for( int j = sizeof( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record ) / sizeof( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ 0 ] ) - 1; j > i; --j ){
					m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ j ] = m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ j - 1 ];
				}
				break;
			}
		}
	}

	void ScoreEntry::Impl::SetDifficulty( int difficulty )
	{
		m_Difficulty = difficulty;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	ScoreEntry::ScoreEntry() : Scene(), m_pImpl( new ScoreEntry::Impl )
	{
	}

	ScoreEntry::~ScoreEntry()
	{
	}

	void ScoreEntry::Init()
	{
	}

	SceneType ScoreEntry::Update()
	{
		return m_pImpl->Update();
	}

	void ScoreEntry::Draw()
	{
		m_pImpl->Draw();
	}

	void ScoreEntry::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_pImpl->AttachButtonState( pHolder );
	}

	void ScoreEntry::AttachResourceMap( const ResourceMap& map )
	{
		m_pImpl->AttachResourceMap( map );
	}

	void ScoreEntry::AttachDisplayedSaveData( const DisplayedSaveData& data )
	{
		m_pImpl->AttachDisplayedSaveData( data );
	}

	void ScoreEntry::AttachRecord( const SaveDataRecord& record )
	{
		m_pImpl->AttachRecord( record );
	}

	void ScoreEntry::SetDifficulty( int difficulty )
	{
		m_pImpl->SetDifficulty( difficulty );
	}
}
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
		SaveDataRecord				m_NewRecord;			// 新しいゲームデータ
		int							m_Difficulty;
		int							m_NewEntryRank;			// 新しくエントリされるランク（ランク外の場合は-1）
		int							m_EntryName[ 10 ];		// エントリ名（9文字まで）
		int							m_NameInputPos;			// 名前入力の場所
		int							m_NameSelectPos;		// 名前選択の場所
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
		const DisplayedSaveData& GetDisplayedSaveData() const;
		void SetRecordRank( int rank );
		const SaveDataRecord& GetRecord() const;
	};

	ScoreEntry::Impl::Impl()
	{
		m_Counter = 0;
		m_Difficulty = GAME_DIFFICULTY_EASY;
		m_NewEntryRank = -1;
		MAPIL::ZeroObject( m_EntryName, sizeof( m_EntryName ) );
		m_NameInputPos = 0;
		m_NameSelectPos = 0;
	}

	SceneType ScoreEntry::Impl::Update()
	{
		if( m_NewEntryRank == -1 ){
			return SCENE_TYPE_MENU;
		}

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_CHANGE_MODE ) ){
			return SCENE_TYPE_REPLAY_ENTRY;
		}

		const int NAME_CHARS_TOTAL = 36;
		const char* NAME_CHARS = "0123456789abcdefghijklmnopqrstuvwxyz";

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			m_NewRecord.m_Name[ m_NameInputPos ] = NAME_CHARS[ m_NameSelectPos ];
			if( m_NameInputPos < 8 ){
				++m_NameInputPos;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
			m_NewRecord.m_Name[ m_NameInputPos ] = '\0';
			if( m_NameInputPos > 0 ){
				--m_NameInputPos;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_LEFT ) ){
			--m_NameSelectPos;
			if( m_NameSelectPos < 0 ){
				m_NameSelectPos = NAME_CHARS_TOTAL - 1;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_RIGHT ) ){
			++m_NameSelectPos;
			if( m_NameSelectPos >= NAME_CHARS_TOTAL ){
				m_NameSelectPos = 0;
			}
		}

		m_NewRecord.m_Name[ m_NameInputPos ] = NAME_CHARS[ m_NameSelectPos ];


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
				float offset = 0.0f;
				int rank = 0;
				if( m_NewEntryRank <= i ){
					offset = 17.0f;
					rank = 1;
				}
				if( i == m_NewEntryRank ){
					DrawFontString( m_ResourceMap, 30.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", i + 1 );
					DrawFontString( m_ResourceMap, 100.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, m_NewRecord.m_Name );
					DrawFontString( m_ResourceMap, 200.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, progStr[ m_NewRecord.m_Progress ] );
					DrawFontString( m_ResourceMap, 350.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", m_NewRecord.m_Score );
					DrawFontString( m_ResourceMap, 500.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%04d%02d%02d%02d%02d",
									m_NewRecord.m_Date.m_Year,
									m_NewRecord.m_Date.m_Month,
									m_NewRecord.m_Date.m_Day,
									m_NewRecord.m_Date.m_Hour,
									m_NewRecord.m_Date.m_Min );
				}
				if( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Year != 0 ){
					DrawFontString( m_ResourceMap, 30.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f + offset, 0.45f, "%d", i + 1 + rank );
					DrawFontString( m_ResourceMap, 100.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f + offset, 0.45f, m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Name );
					DrawFontString( m_ResourceMap, 200.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f + offset, 0.45f, progStr[ m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Progress ] );
					DrawFontString( m_ResourceMap, 350.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f + offset, 0.45f, "%d", m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Score );
					DrawFontString( m_ResourceMap, 500.0f, 505.0f - m_Counter * 21.0f + ( i + 1 ) * 17.0f + offset, 0.45f, "%04d%02d%02d%02d%02d",
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Year,
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Month,
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Day,
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Hour,
									m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Min );
				}
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
				float offset = 0.0f;
				int rank = 0;
				if( m_NewEntryRank <= i ){
					offset = 17.0f;
					rank = 1;
				}
				if( i == m_NewEntryRank ){
					MAPIL::DrawTexture( m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
										101.0f + m_NameInputPos * 32.0f * 0.45f, 90.0f + ( i + 1 ) * 17.0f, 0.95f, 1.0f, true, 0xFF55FF55 );
					DrawFontString( m_ResourceMap, 30.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", i + 1 );
					DrawFontString( m_ResourceMap, 100.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, m_NewRecord.m_Name );
					DrawFontString( m_ResourceMap, 200.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, progStr[ m_NewRecord.m_Progress ] );
					DrawFontString( m_ResourceMap, 350.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", m_NewRecord.m_Score );
					DrawFontString( m_ResourceMap, 500.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%04d%02d%02d%02d%02d",
									m_NewRecord.m_Date.m_Year,
									m_NewRecord.m_Date.m_Month,
									m_NewRecord.m_Date.m_Day,
									m_NewRecord.m_Date.m_Hour,
									m_NewRecord.m_Date.m_Min );
				}
				if( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Year != 0 ){
					DrawFontString( m_ResourceMap, 30.0f, 90.0f + ( i + 1 ) * 17.0f + offset, 0.45f, "%d", i + 1 + rank );
					DrawFontString( m_ResourceMap, 100.0f, 90.0f + ( i + 1 ) * 17.0f + offset, 0.45f, m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Name );
					DrawFontString( m_ResourceMap, 200.0f, 90.0f + ( i + 1 ) * 17.0f + offset, 0.45f, progStr[ m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Progress ] );
					DrawFontString( m_ResourceMap, 350.0f, 90.0f + ( i + 1 ) * 17.0f + offset, 0.45f, "%d", m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Score );
					DrawFontString( m_ResourceMap, 500.0f, 90.0f + ( i + 1 ) * 17.0f + offset, 0.45f, "%04d%02d%02d%02d%02d",
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
		m_NewRecord = record;
		//int newScore = record.m_Score;
		//for( int i = 0; i < sizeof( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record ) / sizeof( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ 0 ] ); ++i ){
		//	if( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Score < newScore ){
		//		m_NewEntryRank = i;
		//		// 消去されるものの全コピー
		//		for( int j = sizeof( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record ) / sizeof( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ 0 ] ) - 1; j > i; --j ){
		//			m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ j ] = m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ j - 1 ];
		//		}
		//		m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ m_NewEntryRank ] = record;
		//		break;
		//	}
		//}
	}

	void ScoreEntry::Impl::SetDifficulty( int difficulty )
	{
		m_Difficulty = difficulty;
	}

	const DisplayedSaveData& ScoreEntry::Impl::GetDisplayedSaveData() const
	{
		return m_DisplayedSaveData;
	}

	void ScoreEntry::Impl::SetRecordRank( int rank )
	{
		m_NewEntryRank = rank;
	}

	const SaveDataRecord& ScoreEntry::Impl::GetRecord() const
	{
		return m_NewRecord;
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

	const DisplayedSaveData& ScoreEntry::GetDisplayedSaveData() const
	{
		return m_pImpl->GetDisplayedSaveData();
	}

	void ScoreEntry::SetRecordRank( int rank )
	{
		m_pImpl->SetRecordRank( rank );
	}

	const SaveDataRecord& ScoreEntry::GetRecord() const
	{
		return m_pImpl->GetRecord();
	}
}
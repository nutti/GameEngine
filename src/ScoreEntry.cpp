#include <MAPIL/MAPIL.h>

#include "ScoreEntry.h"

#include "ResourceID.h"

#include "Util.h"

namespace GameEngine
{
	const int NAME_CHARS_X = 12;		// 横並び数
	const int NAME_CHARS_Y = 3;			// 縦並び数
	const char NAME_CHARS[ NAME_CHARS_Y ][ NAME_CHARS_X + 1 ] =	{	"abcdefghijkl",
																	"mnopqrstuvwx",
																	"yz0123456789" };


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
		//int							m_EntryName[ 10 ];		// エントリ名（9文字まで）
		int							m_NameInputPos;			// 名前入力の場所
		int							m_NameSelectPosX;		// 名前選択の場所
		int							m_NameSelectPosY;

		int							m_SelectCounter;		// 選択時のカウンタ
		int							m_SceneChangeCounter;	// 画面遷移時のカウンタ
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
		//MAPIL::ZeroObject( m_EntryName, sizeof( m_EntryName ) );
		m_NameInputPos = -1;
		m_NameSelectPosX = 0;
		m_NameSelectPosY = 0;

		m_SelectCounter = 0;
		m_SceneChangeCounter = 0;
	}

	SceneType ScoreEntry::Impl::Update()
	{
		if( m_Counter < 20 ){
			++m_Counter;
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( m_SelectCounter > 0 ){
			--m_SelectCounter;
		}

		if( m_NewEntryRank == -1 ){
			return SCENE_TYPE_REPLAY_ENTRY;
		}


		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
			if( m_NameInputPos != -1 ){
				m_NewRecord.m_Name[ m_NameInputPos ] = '\0';
				--m_NameInputPos;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_GREEN ) ){
			m_NewRecord.m_Name[ m_NameInputPos + 1 ] = '\0';
			return SCENE_TYPE_REPLAY_ENTRY;
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			if( m_NameInputPos < 7 ){
				++m_NameInputPos;
			}
			m_NewRecord.m_Name[ m_NameInputPos ] = NAME_CHARS[ m_NameSelectPosY ][ m_NameSelectPosX ];
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_LEFT ) ){
			--m_NameSelectPosX;
			if( m_NameSelectPosX < 0 ){
				m_NameSelectPosX = NAME_CHARS_X - 1;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_RIGHT ) ){
			++m_NameSelectPosX;
			if( m_NameSelectPosX >= NAME_CHARS_X ){
				m_NameSelectPosX = 0;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
			--m_NameSelectPosY;
			if( m_NameSelectPosY < 0 ){
				m_NameSelectPosY = NAME_CHARS_Y - 1;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
			++m_NameSelectPosY;
			if( m_NameSelectPosY >= NAME_CHARS_Y ){
				m_NameSelectPosY = 0;
			}
		}


		++m_Counter;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void ScoreEntry::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();

		
		// 背景画像
		int weight;
		if( m_Counter < 20 ){
			weight = ( m_Counter * 6 ) & 0xFF;
		}
		else{
			weight = 0x78;		// 120
		}
		int color = weight << 16 | weight << 8 | weight;
		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
							0.0f, 0.0f, false, 0xFF << 24 | color );

		char* rankStr[] = { "calm", "easy", "normal", "hard", "hazard" };
		char* progStr[] = { "", "st 1", "st 2", "st 3", "st 4", "st 5", "all" };


		
		// リプレイデータ一覧表示
		int alpha = 0;
		if( m_Counter >= 20 && m_Counter < 60 ){
			alpha = ( ( m_Counter - 20 ) * 6 + 10 ) & 0xFF;
		}
		else if( m_Counter >= 60 ){
			if( m_SceneChangeCounter == 0 ){
				alpha = 0xFF;
			}
			else if( m_SceneChangeCounter > 0 && m_SceneChangeCounter <= 20 ){
				alpha = ( 0xFF * ( 20 - m_SceneChangeCounter ) ) / 20;
			}
			else{
				alpha = 0x00;
			}
		}
		int color1 = alpha << 24 | 0xAAFFAA;
		int color2 = alpha << 24 | 0xAAAAAA;
		int selColor = alpha << 24 | 0xFFFFFF;


		float titleFont = 0.4f;
		float itemFont = 0.38f;

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
		startX[ ITEM_ID_RANK ] = 70.0f;
		startX[ ITEM_ID_NAME ] = startX[ ITEM_ID_RANK ] + 60.0f;
		startX[ ITEM_ID_PROG ] = startX[ ITEM_ID_NAME ] + 100.0f;
		startX[ ITEM_ID_SCORE ] = startX[ ITEM_ID_PROG ] + 70.0f;
		startX[ ITEM_ID_DATE ] = startX[ ITEM_ID_SCORE ] + 110.0f;
		float startY = 90.0f;

		DrawFontString( m_ResourceMap, 30.0f, 10.0f, 0.7f, color1, "Records past conscious" );

		DrawFontString( m_ResourceMap, startX[ ITEM_ID_RANK ], startY, titleFont, color1, "rank" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_NAME ], startY, titleFont, color1, "name" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_PROG ], startY, titleFont, color1, "prog" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_SCORE ], startY, titleFont, color1, "score" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_DATE ], startY, titleFont, color1, "date" );
		startY += 15.0f;
		for( int i = 0; i < MAX_SCORE_ENTRY; ++i ){;
			int rank = 0;
			if( m_NewEntryRank <= i ){
				rank = 1;
			}

			int c = color2;
			float offsetX = 0.0f;
			if( m_NewEntryRank == i ){
				offsetX = -2.0f;
			}
			float offsetY = 0.0f;
			DrawFontString( m_ResourceMap, startX[ ITEM_ID_RANK ] + offsetX, startY + ( i ) * 17.0f + offsetY, titleFont, color1, "%d", i + 1 );
			if( m_NewEntryRank == i ){
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_NAME ] + offsetX, startY + ( i ) * 17.0f + offsetY, itemFont, selColor, m_NewRecord.m_Name );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_PROG ] + offsetX, startY + ( i ) * 17.0f + offsetY, itemFont, selColor, progStr[ m_NewRecord.m_Progress ] );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_SCORE ] + offsetX, startY + ( i ) * 17.0f + offsetY, itemFont, selColor, "%d", m_NewRecord.m_Score );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_DATE ] + offsetX, startY + ( i ) * 17.0f + offsetY, itemFont, selColor, "%02d/%02d/%02d %02d:%02d",
								m_NewRecord.m_Date.m_Year % 100,
								m_NewRecord.m_Date.m_Month,
								m_NewRecord.m_Date.m_Day,
								m_NewRecord.m_Date.m_Hour,
								m_NewRecord.m_Date.m_Min );
			}
			if( m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Year != 0 ){
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_NAME ] + offsetX, startY + ( i + rank ) * 17.0f + offsetY, itemFont, color2, m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Name );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_PROG ] + offsetX, startY + ( i + rank ) * 17.0f + offsetY, itemFont, color2, progStr[ m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Progress ] );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_SCORE ] + offsetX, startY + ( i + rank ) * 17.0f + offsetY, itemFont, color2, "%d", m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Score );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_DATE ] + offsetX, startY + ( i + rank ) * 17.0f + offsetY, itemFont, color2, "%02d/%02d/%02d %02d:%02d",
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Year % 100,
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Month,
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Day,
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Hour,
								m_DisplayedSaveData.m_Difficulty[ m_Difficulty ].m_Record[ i ].m_Date.m_Min );
			}
		}

		float baseX = 300.0f;
		float baseY = 400.0f;
		float fontScale = 0.55f;
		for( int i = 0; i < NAME_CHARS_Y; ++i ){
			for( int j = 0; j < NAME_CHARS_X; ++j ){
				std::string s;
				s = NAME_CHARS[ i ][ j ];
				if( i == m_NameSelectPosY && j == m_NameSelectPosX ){
					DrawFontString( m_ResourceMap, baseX + j * 25.0f, baseY + i * 25.0f, fontScale, color1, s.c_str() );
				}
				else{
					DrawFontString( m_ResourceMap, baseX + j * 25.0f, baseY + i * 25.0f, fontScale, color2, s.c_str() );
				}
			}
		}
		float fontScale2 = 0.7f;
		DrawFontString( m_ResourceMap, 60.0f, 420.0f, fontScale2, m_NewRecord.m_Name );

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

	void ScoreEntry::InitImpl()
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
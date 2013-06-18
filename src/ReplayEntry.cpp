#include <MAPIL/MAPIL.h>

#include "ReplayEntry.h"

#include "ResourceID.h"

#include "Util.h"

namespace GameEngine
{
	const int NAME_CHARS_X = 12;		// 横並び数
	const int NAME_CHARS_Y = 3;	// 縦並び数
	const char NAME_CHARS[ NAME_CHARS_Y ][ NAME_CHARS_X + 1 ] =	{	"abcdefghijkl",
																	"mnopqrstuvwx",
																	"yz0123456789" };

	enum ReplayEntrySelectState
	{
		REPLAY_ENTRY_SELECT_STATE_NO		= 0,		// エントリ番号を選択
		REPLAY_ENTRY_SELECT_STATE_NAME		= 1,		// 名前を入力
	};

	class ReplayEntry::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;
		ResourceMap					m_ResourceMap;
		int							m_Counter;
		DisplayedReplayInfo			m_DispReplayInfo;
		ReplayDataRecord			m_ReplayDataRecord;
		int							m_CurSelectState;		// 現在の選択状態
		int							m_NameInputPos;			// 名前入力の場所
		int							m_NameSelectPosX;		// 名前選択の場所
		int							m_NameSelectPosY;		// 
		char						m_EntryName[ 10 ];

		int							m_SelectCounter;		// 選択時の準備用カウンタ
		int							m_SceneChangeCounter;	// 次のシーンへ向かう時のカウンタ
	public:
		Impl();
		~Impl(){}
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachDisplayedReplayInfo( const DisplayedReplayInfo& info );
		void AttachReplayDataRecord( const ReplayDataRecord& record );
		const ReplayDataRecord& GetReplayDataRecord() const;
	};

	ReplayEntry::Impl::Impl()
	{
		m_Counter = 20;
		m_CurSelectState = REPLAY_ENTRY_SELECT_STATE_NO;
		m_NameInputPos = -1;
		m_NameSelectPosX = 0;
		m_NameSelectPosY = 0;
		MAPIL::ZeroObject( m_EntryName, sizeof( m_EntryName ) );

		m_SelectCounter = 0;
		m_SceneChangeCounter = 0;
	}

	SceneType ReplayEntry::Impl::Update()
	{
		

		if( m_Counter < 60 ){
			++m_Counter;
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( m_SelectCounter > 0 ){
			--m_SelectCounter;
		}

		if( m_CurSelectState == REPLAY_ENTRY_SELECT_STATE_NO ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
				return SCENE_TYPE_MENU;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
				m_CurSelectState = REPLAY_ENTRY_SELECT_STATE_NAME;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
				++m_ReplayDataRecord.m_EntryNo;
				if( m_ReplayDataRecord.m_EntryNo > MAX_REPLAY_ENTRY - 1 ){
					m_ReplayDataRecord.m_EntryNo = 0;
				}
				m_SelectCounter = 10;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				--m_ReplayDataRecord.m_EntryNo;
				if( m_ReplayDataRecord.m_EntryNo < 0 ){
					m_ReplayDataRecord.m_EntryNo = MAX_REPLAY_ENTRY - 1;
				}
				m_SelectCounter = 10;
			}
		}
		else if( m_CurSelectState == REPLAY_ENTRY_SELECT_STATE_NAME ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
				if( m_NameInputPos == -1 ){
					m_CurSelectState = REPLAY_ENTRY_SELECT_STATE_NO;
				}
				else{
					m_EntryName[ m_NameInputPos ] = '\0';
					--m_NameInputPos;
				}
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_CHANGE_MODE ) ){
				m_EntryName[ m_NameInputPos + 1 ] = '\0';
				::memcpy( m_ReplayDataRecord.m_Name, m_EntryName, sizeof( m_EntryName ) );
				m_CurSelectState = REPLAY_ENTRY_SELECT_STATE_NO;
				return SCENE_TYPE_REPLAY_ENTRY;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
				if( m_NameInputPos < 7 ){
					++m_NameInputPos;
				}
				m_EntryName[ m_NameInputPos ] = NAME_CHARS[ m_NameSelectPosY ][ m_NameSelectPosX ];
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
		}

		++m_Counter;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void ReplayEntry::Impl::Draw()
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
			ITEM_ID_NO		= 0,
			ITEM_ID_NAME	= 1,
			ITEM_ID_RANK	= 2,
			ITEM_ID_PROG	= 3,
			ITEM_ID_SCORE	= 4,
			ITEM_ID_DATE	= 5,
			ITEM_ID_TOTAL,
		};

		float startX[ ITEM_ID_TOTAL ];
		startX[ ITEM_ID_NO ] = 30.0f;
		startX[ ITEM_ID_NAME ] = startX[ ITEM_ID_NO ] + 40.0f;
		startX[ ITEM_ID_RANK ] = startX[ ITEM_ID_NAME ] + 100.0f;
		startX[ ITEM_ID_PROG ] = startX[ ITEM_ID_RANK ] + 100.0f;
		startX[ ITEM_ID_SCORE ] = startX[ ITEM_ID_PROG ] + 70.0f;
		startX[ ITEM_ID_DATE ] = startX[ ITEM_ID_SCORE ] + 110.0f;
		float startY = 110.0f;

		DrawFontString( m_ResourceMap, startX[ ITEM_ID_NO ], 10.0f, 0.7f, color1, "Input conscious" );

		DrawFontString( m_ResourceMap, startX[ ITEM_ID_NO ], startY, titleFont, color1, "no" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_NAME ], startY, titleFont, color1, "name" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_RANK ], startY, titleFont, color1, "rank" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_PROG ], startY, titleFont, color1, "prog" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_SCORE ], startY, titleFont, color1, "score" );
		DrawFontString( m_ResourceMap, startX[ ITEM_ID_DATE ], startY, titleFont, color1, "date" );
		for( int i = 0; i < MAX_REPLAY_ENTRY; ++i ){
			if( m_DispReplayInfo.m_Entries[ i ].m_Progress != -1 ){
				int c = color2;
				float offsetX = 0.0f;
				float offsetY = 0.0f;
				if( i == m_ReplayDataRecord.m_EntryNo ){
					if( m_SelectCounter > 0 ){
						offsetX = -2.0f * sin( MAPIL::DegToRad( m_SelectCounter * 45.0 ) );
					}
					else{
						offsetX = -2.0f;
					}
					offsetY = 0.0f;
					c = selColor;
				}

				DrawFontString( m_ResourceMap, startX[ ITEM_ID_NO ] + offsetX, startY + 5.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, "%d", i + 1 );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_NAME ] + offsetX, startY + 5.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, m_DispReplayInfo.m_Entries[ i ].m_Name );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_RANK ] + offsetX, startY + 5.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, rankStr[ m_DispReplayInfo.m_Entries[ i ].m_Difficulty ] );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_PROG ] + offsetX, startY + 5.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, progStr[ m_DispReplayInfo.m_Entries[ i ].m_Progress ] );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_SCORE ] + offsetX, startY + 5.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, "%d", m_DispReplayInfo.m_Entries[ i ].m_Score );
				DrawFontString( m_ResourceMap, startX[ ITEM_ID_DATE ] + offsetX, startY + 5.0f + ( i + 1 ) * 17.0f + offsetY, itemFont, c, "%02d/%02d/%02d %02d:%02d",
								m_DispReplayInfo.m_Entries[ i ].m_Date.m_Year % 100,
								m_DispReplayInfo.m_Entries[ i ].m_Date.m_Month,
								m_DispReplayInfo.m_Entries[ i ].m_Date.m_Day,
								m_DispReplayInfo.m_Entries[ i ].m_Date.m_Hour,
								m_DispReplayInfo.m_Entries[ i ].m_Date.m_Min );
			}
		}

		{
			float newTitleFont = 0.45f;
			float newItemFont = 0.45f;

			// スコアの桁数を取得する。
			int digit = 0;
			int val = m_ReplayDataRecord.m_Score;
			for( int i = 0; i < 20; ++i ){
				val /= 10;
				if( val == 0 ){
					digit = i;
					break;
				}
			}

			float offsetX[ 7 ];
			offsetX[ 0 ] = 105.0f - 7.0f * digit;		// Rank title.
			offsetX[ 1 ] = offsetX[ 0 ] + 75.0f;		// Rank data.
			offsetX[ 2 ] = offsetX[ 1 ] + 110.0f;		// Prog title.
			offsetX[ 3 ] = offsetX[ 2 ] + 75.0f;		// Prog data.
			offsetX[ 4 ] = offsetX[ 3 ] + 80.0f;		// Score title.
			offsetX[ 5 ] = offsetX[ 4 ] + 90.0f;		// Score data.
			offsetX[ 6 ] = offsetX[ 5 ] + 110.0f;
			float offsetY = 80.0f;
			DrawFontString( m_ResourceMap, offsetX[ 0 ], offsetY, newTitleFont, color1, "rank" );
			DrawFontString( m_ResourceMap, offsetX[ 2 ], offsetY, newTitleFont, color1, "prog" );
			DrawFontString( m_ResourceMap, offsetX[ 4 ], offsetY, newTitleFont, color1, "score" );
			DrawFontString( m_ResourceMap, offsetX[ 1 ], offsetY, newItemFont, rankStr[ m_ReplayDataRecord.m_Difficulty ] );
			DrawFontString( m_ResourceMap, offsetX[ 3 ], offsetY, newItemFont, progStr[ m_ReplayDataRecord.m_Progress ] );
			DrawFontString( m_ResourceMap, offsetX[ 5 ], offsetY, newItemFont, "%d", m_ReplayDataRecord.m_Score );
		}

		// 詳細情報の表示
		if( m_CurSelectState == REPLAY_ENTRY_SELECT_STATE_NO ){
			int color3 = 0;
			if( m_Counter > 60 ){
				if( m_SceneChangeCounter == 0 ){
					if( m_SelectCounter > 0 ){
						color3 = ( ( 10 - m_SelectCounter ) * 25 ) << 24 | 0xFFFFFF;
					}
					else{
						color3 = 0xFFFFFFFF;
					}
				}
				else{
					color3 = selColor;
				}
			}
			DrawFontString( m_ResourceMap, 30.0f, startY + 300.0f, titleFont, color1, "killed" );
			DrawFontString( m_ResourceMap, 30.0f, startY + 320.0f, titleFont, color1, "crystal" );
			DrawFontString( m_ResourceMap, 160.0f, startY + 300.0f, itemFont, color3,
							"%d", m_DispReplayInfo.m_Entries[ m_ReplayDataRecord.m_EntryNo ].m_Killed );
			DrawFontString( m_ResourceMap, 160.0f, startY + 320.0f, itemFont, color3,
							"%d", m_DispReplayInfo.m_Entries[ m_ReplayDataRecord.m_EntryNo ].m_Crystal );
		}
		else if( m_CurSelectState == REPLAY_ENTRY_SELECT_STATE_NAME ){
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
			DrawFontString( m_ResourceMap, 60.0f, 420.0f, fontScale2, m_EntryName );
		}

		MAPIL::EndRendering2DGraphics();
	}

	void ReplayEntry::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_ButtonStatus = *pHolder;
	}

	void ReplayEntry::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	void ReplayEntry::Impl::AttachDisplayedReplayInfo( const DisplayedReplayInfo& info )
	{
		m_DispReplayInfo = info;
	}

	void ReplayEntry::Impl::AttachReplayDataRecord( const ReplayDataRecord& record )
	{
		m_ReplayDataRecord = record;
	}

	const ReplayDataRecord& ReplayEntry::Impl::GetReplayDataRecord() const
	{
		return m_ReplayDataRecord;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	ReplayEntry::ReplayEntry() : Scene(), m_pImpl( new ReplayEntry::Impl )
	{
	}

	ReplayEntry::~ReplayEntry()
	{
	}

	void ReplayEntry::Init()
	{
	}

	SceneType ReplayEntry::Update()
	{
		return m_pImpl->Update();
	}

	void ReplayEntry::Draw()
	{
		m_pImpl->Draw();
	}

	void ReplayEntry::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_pImpl->AttachButtonState( pHolder );
	}

	void ReplayEntry::AttachResourceMap( const ResourceMap& map )
	{
		m_pImpl->AttachResourceMap( map );
	}

	void ReplayEntry::AttachDisplayedReplayInfo( const DisplayedReplayInfo& info )
	{
		m_pImpl->AttachDisplayedReplayInfo( info );
	}

	void ReplayEntry::AttachReplayDataRecord( const ReplayDataRecord& record )
	{
		m_pImpl->AttachReplayDataRecord( record );
	}

	const ReplayDataRecord& ReplayEntry::GetReplayDataRecord() const
	{
		return m_pImpl->GetReplayDataRecord();
	}

}
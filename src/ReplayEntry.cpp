#include <MAPIL/MAPIL.h>

#include "ReplayEntry.h"

#include "ResourceID.h"

#include "Util.h"

namespace GameEngine
{
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
		DisplayedReplayInfo::Entry	m_ReplayInfo;
		int							m_CurSelectState;		// 現在の選択状態
		int							m_NameInputPos;			// 名前入力の場所
		int							m_NameSelectPos;		// 名前選択の場所
		char						m_EntryName[ 10 ];
	public:
		Impl();
		~Impl(){}
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachDisplayedReplayInfo( const DisplayedReplayInfo& info );
		void AttachReplayInfo( const DisplayedReplayInfo::Entry& info );
		const DisplayedReplayInfo::Entry& GetReplayInfo() const;
	};

	ReplayEntry::Impl::Impl()
	{
		m_Counter = 0;
		m_CurSelectState = REPLAY_ENTRY_SELECT_STATE_NO;
		m_NameInputPos = 0;
		m_NameSelectPos = 0;
		MAPIL::ZeroObject( m_EntryName, sizeof( m_EntryName ) );
	}

	SceneType ReplayEntry::Impl::Update()
	{
		const int NAME_CHARS_TOTAL = 36;
		const char* NAME_CHARS = "0123456789abcdefghijklmnopqrstuvwxyz";

		if( m_CurSelectState == REPLAY_ENTRY_SELECT_STATE_NO ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
				return SCENE_TYPE_MENU;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
				m_CurSelectState = REPLAY_ENTRY_SELECT_STATE_NAME;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
				++m_ReplayInfo.m_EntryNo;
				if( m_ReplayInfo.m_EntryNo >= 15 ){
					m_ReplayInfo.m_EntryNo = 0;
				}
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				--m_ReplayInfo.m_EntryNo;
				if( m_ReplayInfo.m_EntryNo < 0 ){
					m_ReplayInfo.m_EntryNo = 14;
				}
			}
		}
		else if( m_CurSelectState == REPLAY_ENTRY_SELECT_STATE_NAME ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
				m_CurSelectState = REPLAY_ENTRY_SELECT_STATE_NO;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_CHANGE_MODE ) ){
				m_EntryName[ m_NameInputPos ] = '\0';
				::memcpy( m_ReplayInfo.m_Name, m_EntryName, sizeof( m_EntryName ) );
				return SCENE_TYPE_REPLAY_ENTRY;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
				m_EntryName[ m_NameInputPos ] = NAME_CHARS[ m_NameSelectPos ];
				if( m_NameInputPos < 8 ){
					++m_NameInputPos;
				}
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
				m_EntryName[ m_NameInputPos ] = '\0';
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

			m_EntryName[ m_NameInputPos ] = NAME_CHARS[ m_NameSelectPos ];
		}

		++m_Counter;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void ReplayEntry::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();

		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
							0.0f, 0.0f, 640.0f, 480.0f, 0.0f, false, 0xFF999999 );

		char* progStr[] = { "", "stage 1", "stage 2", "stage 3", "stage 4", "stage 5", "all clear" };

		

		if( m_CurSelectState == REPLAY_ENTRY_SELECT_STATE_NO ){
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								0.0f, 99.0f + m_ReplayInfo.m_EntryNo * 17.0f, 50.0f, 1.0f, false, 0xDD5577FF );
			DrawFontString( m_ResourceMap, 30.0f, 85.0f, 0.5f, 0xFFAAFFAA, "rank" );
			DrawFontString( m_ResourceMap, 100.0f, 85.0f, 0.5f, 0xFFAAFFAA, "name" );
			DrawFontString( m_ResourceMap, 200.0f, 85.0f, 0.5f, 0xFFAAFFAA, "progress" );
			DrawFontString( m_ResourceMap, 350.0f, 85.0f, 0.5f, 0xFFAAFFAA, "score" );
			DrawFontString( m_ResourceMap, 500.0f, 85.0f, 0.5f, 0xFFAAFFAA, "date" );
			for( int i = 0; i < 15; ++i ){
				DrawFontString( m_ResourceMap, 30.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", i + 1 );
				if( m_DispReplayInfo.m_Entries[ i ].m_Date.m_Year != 0 ){
					DrawFontString( m_ResourceMap, 100.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, m_DispReplayInfo.m_Entries[ i ].m_Name );
					DrawFontString( m_ResourceMap, 200.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, progStr[ m_DispReplayInfo.m_Entries[ i ].m_Progress ] );
					DrawFontString( m_ResourceMap, 350.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", m_DispReplayInfo.m_Entries[ i ].m_Score );
					DrawFontString( m_ResourceMap, 500.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%04d%02d%02d%02d%02d",
									m_DispReplayInfo.m_Entries[ i ].m_Date.m_Year,
									m_DispReplayInfo.m_Entries[ i ].m_Date.m_Month,
									m_DispReplayInfo.m_Entries[ i ].m_Date.m_Day,
									m_DispReplayInfo.m_Entries[ i ].m_Date.m_Hour,
									m_DispReplayInfo.m_Entries[ i ].m_Date.m_Min );
				}
			}
		}
		else if( m_CurSelectState == REPLAY_ENTRY_SELECT_STATE_NAME ){
			DrawFontString( m_ResourceMap, 30.0f, 85.0f, 0.5f, 0xFFAAFFAA, "rank" );
			DrawFontString( m_ResourceMap, 100.0f, 85.0f, 0.5f, 0xFFAAFFAA, "name" );
			DrawFontString( m_ResourceMap, 200.0f, 85.0f, 0.5f, 0xFFAAFFAA, "progress" );
			DrawFontString( m_ResourceMap, 350.0f, 85.0f, 0.5f, 0xFFAAFFAA, "score" );
			DrawFontString( m_ResourceMap, 500.0f, 85.0f, 0.5f, 0xFFAAFFAA, "date" );
			DrawFontString( m_ResourceMap, 30.0f, 107.0f, 0.45f, "%d", m_ReplayInfo.m_EntryNo + 1 );

			DrawFontString( m_ResourceMap, 100.0f, 107.0f, 0.45f, m_EntryName );
			DrawFontString( m_ResourceMap, 200.0f, 107.0f, 0.45f, progStr[ m_ReplayInfo.m_Progress ] );
			DrawFontString( m_ResourceMap, 350.0f, 107.0f, 0.45f, "%d", m_ReplayInfo.m_Score );
			DrawFontString( m_ResourceMap, 500.0f, 107.0f, 0.45f, "%04d%02d%02d%02d%02d",
							m_ReplayInfo.m_Date.m_Year,
							m_ReplayInfo.m_Date.m_Month,
							m_ReplayInfo.m_Date.m_Day,
							m_ReplayInfo.m_Date.m_Hour,
							m_ReplayInfo.m_Date.m_Min );
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

	void ReplayEntry::Impl::AttachReplayInfo( const DisplayedReplayInfo::Entry& info )
	{
		m_ReplayInfo = info;
	}

	const DisplayedReplayInfo::Entry& ReplayEntry::Impl::GetReplayInfo() const
	{
		return m_ReplayInfo;
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

	void ReplayEntry::AttachReplayInfo( const DisplayedReplayInfo::Entry& info )
	{
		m_pImpl->AttachReplayInfo( info );
	}

	const DisplayedReplayInfo::Entry& ReplayEntry::GetReplayInfo() const
	{
		return m_pImpl->GetReplayInfo();
	}
}
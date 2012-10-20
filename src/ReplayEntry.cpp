#include <MAPIL/MAPIL.h>

#include "ReplayEntry.h"

#include "ResourceID.h"

#include "Util.h"

namespace GameEngine
{

	class ReplayEntry::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;
		ResourceMap					m_ResourceMap;
		int							m_Counter;
		DisplayedReplayInfo			m_ReplayInfo;
	public:
		Impl();
		~Impl(){}
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachDisplayedReplayInfo( const DisplayedReplayInfo& info );
	};

	ReplayEntry::Impl::Impl()
	{
		m_Counter = 0;
	}

	SceneType ReplayEntry::Impl::Update()
	{

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
			return SCENE_TYPE_MENU;
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

		//MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
		//					0.0f, 99.0f + m_NewEntryRank * 17.0f, 50.0f, 1.0f, false, 0xDD5577FF );

		DrawFontString( m_ResourceMap, 30.0f, 85.0f, 0.5f, 0xFFAAFFAA, "rank" );
		DrawFontString( m_ResourceMap, 100.0f, 85.0f, 0.5f, 0xFFAAFFAA, "name" );
		DrawFontString( m_ResourceMap, 200.0f, 85.0f, 0.5f, 0xFFAAFFAA, "progress" );
		DrawFontString( m_ResourceMap, 350.0f, 85.0f, 0.5f, 0xFFAAFFAA, "score" );
		DrawFontString( m_ResourceMap, 500.0f, 85.0f, 0.5f, 0xFFAAFFAA, "date" );
		for( int i = 0; i < 15; ++i ){
			if( m_ReplayInfo.m_Entries[ i ].m_Date.m_Year != 0 ){
				DrawFontString( m_ResourceMap, 30.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", i + 1 );
				DrawFontString( m_ResourceMap, 100.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, m_ReplayInfo.m_Entries[ i ].m_Name );
				DrawFontString( m_ResourceMap, 200.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, progStr[ m_ReplayInfo.m_Entries[ i ].m_Progress ] );
				DrawFontString( m_ResourceMap, 350.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%d", m_ReplayInfo.m_Entries[ i ].m_Score );
				DrawFontString( m_ResourceMap, 500.0f, 90.0f + ( i + 1 ) * 17.0f, 0.45f, "%04d%02d%02d%02d%02d",
								m_ReplayInfo.m_Entries[ i ].m_Date.m_Year,
								m_ReplayInfo.m_Entries[ i ].m_Date.m_Month,
								m_ReplayInfo.m_Entries[ i ].m_Date.m_Day,
								m_ReplayInfo.m_Entries[ i ].m_Date.m_Hour,
								m_ReplayInfo.m_Entries[ i ].m_Date.m_Min );
			}
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
		m_ReplayInfo = info;
	}

	// ----------------------------------
	// ŽÀ‘•ƒNƒ‰ƒX‚ÌŒÄ‚Ño‚µ
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

}
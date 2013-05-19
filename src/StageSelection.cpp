#include <MAPIL/MAPIL.h>

#include "StageSelection.h"

#include "ResourceID.h"
#include "GameStateTypes.h"

#include "Util.h"

namespace GameEngine
{
	static const int PERMITTED_STAGE_TOTAL		= 3;

	class StageSelection::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;
		ResourceMap					m_ResourceMap;
		int							m_Counter;
		int							m_MenuPointed;
		bool						m_PlayBGM;
		int							m_PrepareCounter;
	public:
		Impl();
		~Impl(){}
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		int GetStageNo() const;
	};

	StageSelection::Impl::Impl()
	{
		m_Counter = 0;
		m_MenuPointed = 0;
		m_PrepareCounter = 0;
	}

	SceneType StageSelection::Impl::Update()
	{
		if( m_PrepareCounter > 0 ){
			++m_PrepareCounter;
			if( m_PrepareCounter == 20 ){
				return SCENE_TYPE_STAGE;
			}
			else{
				return SCENE_TYPE_NOT_CHANGE;
			}
		}

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			MAPIL::PlayStaticBuffer( m_ResourceMap.m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_MENU_SELECTED ] );
			MAPIL::StopStreamingBuffer( GLOBAL_RESOURCE_BGM_ID_MENU );
			m_PrepareCounter = 1;
			//return SCENE_TYPE_STAGE;
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
			return SCENE_TYPE_DIFFICULTY_SELECTION_IN_STAGE;
		}

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
			++m_MenuPointed;
			if( m_MenuPointed > PERMITTED_STAGE_TOTAL - 1 ){
				m_MenuPointed = 0;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
			--m_MenuPointed;
			if( m_MenuPointed < 0 ){
				m_MenuPointed = PERMITTED_STAGE_TOTAL - 1;
			}
		}


		++m_Counter;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void StageSelection::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();

		for( int i = 0; i < STAGE_TOTAL; ++i ){
			std::string str = "stage";
			str += ( i + 1 ) + '0';
			if( i == m_MenuPointed ){
				DrawFontString( m_ResourceMap, 20.0f, 250.0f + m_MenuPointed * 30.0f, 0.5f, 0xFFFFFF00, str.c_str() );
			}
			else{
				if( i <= PERMITTED_STAGE_TOTAL - 1 ){
					DrawFontString( m_ResourceMap, 20.0f, 250.0f + i * 30.0f, 0.5f, 0xFFFFFFFF, str.c_str() );
				}
				else{
					DrawFontString( m_ResourceMap, 20.0f, 250.0f + i * 30.0f, 0.5f, 0xFFAAAAAA, str.c_str() );
				}
			}
		}

		if( m_PrepareCounter > 0 && m_PrepareCounter <= 20 ){
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
								0.0f, ( m_PrepareCounter - 20 ) * 24.0f, false );
		}
		else if( m_PrepareCounter > 20 ){
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
								0.0f, 0.0f, false );
		}

		MAPIL::EndRendering2DGraphics();
	}

	void StageSelection::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_ButtonStatus = *pHolder;
	}

	void StageSelection::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	int StageSelection::Impl::GetStageNo() const
	{
		return m_MenuPointed + 1;
	}

	// ----------------------------------
	// ŽÀ‘•ƒNƒ‰ƒX‚ÌŒÄ‚Ño‚µ
	// ----------------------------------

	StageSelection::StageSelection() : Scene(), m_pImpl( new StageSelection::Impl )
	{
	}

	StageSelection::~StageSelection()
	{
	}

	void StageSelection::Init()
	{
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

	int StageSelection::GetStageNo() const
	{
		return m_pImpl->GetStageNo();
	}
}
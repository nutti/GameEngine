#include <MAPIL/MAPIL.h>

#include "DifficultySelection.h"

#include "ResourceID.h"
#include "GameStateTypes.h"

#include "Util.h"

namespace GameEngine
{

	class DifficultySelection::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;
		ResourceMap					m_ResourceMap;
		int							m_Counter;
		int							m_MenuPointed;
		bool						m_PlayBGM;
	public:
		Impl();
		~Impl(){}
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		int GetDifficulty() const;
	};

	DifficultySelection::Impl::Impl()
	{
		m_Counter = 0;
		m_MenuPointed = GAME_DIFFICULTY_CALM;
	}

	SceneType DifficultySelection::Impl::Update()
	{
		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			MAPIL::PlayStaticBuffer( m_ResourceMap.m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_MENU_SELECTED ] );
			MAPIL::StopStreamingBuffer( GLOBAL_RESOURCE_BGM_ID_MENU );
			return SCENE_TYPE_STAGE;
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
			return SCENE_TYPE_MENU;
		}

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
			++m_MenuPointed;
			if( m_MenuPointed > GAME_DIFFICULTY_HAZARD ){
				m_MenuPointed = GAME_DIFFICULTY_CALM;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
			--m_MenuPointed;
			if( m_MenuPointed < GAME_DIFFICULTY_CALM ){
				m_MenuPointed = GAME_DIFFICULTY_HAZARD;
			}
		}


		++m_Counter;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void DifficultySelection::Impl::Draw()
	{

		const char* pDispStr[] = { "calm", "easy", "normal", "hard", "hazard" };

		MAPIL::BeginRendering2DGraphics();


		for( int i = 0; i < sizeof( pDispStr ) / sizeof( pDispStr[ 0 ] ); ++i ){
			if( i == m_MenuPointed ){
				continue;
			}
			DrawFontString( m_ResourceMap, 20.0f, 250.0f + i * 30.0f, 0.5f, 0xFFFFFFFF, pDispStr[ i ] );
		}

		DrawFontString( m_ResourceMap, 20.0f, 250.0f + m_MenuPointed * 30.0f, 0.5f, 0xFFFFFF00, pDispStr[ m_MenuPointed ] );
		

		MAPIL::EndRendering2DGraphics();
	}

	void DifficultySelection::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_ButtonStatus = *pHolder;
	}

	void DifficultySelection::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	int DifficultySelection::Impl::GetDifficulty() const
	{
		return m_MenuPointed;
	}

	// ----------------------------------
	// ŽÀ‘•ƒNƒ‰ƒX‚ÌŒÄ‚Ño‚µ
	// ----------------------------------

	DifficultySelection::DifficultySelection() : Scene(), m_pImpl( new DifficultySelection::Impl )
	{
	}

	DifficultySelection::~DifficultySelection()
	{
	}

	void DifficultySelection::Init()
	{
	}

	SceneType DifficultySelection::Update()
	{
		return m_pImpl->Update();
	}

	void DifficultySelection::Draw()
	{
		m_pImpl->Draw();
	}

	void DifficultySelection::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_pImpl->AttachButtonState( pHolder );
	}

	void DifficultySelection::AttachResourceMap( const ResourceMap& map )
	{
		m_pImpl->AttachResourceMap( map );
	}

	int DifficultySelection::GetDifficulty() const
	{
		return m_pImpl->GetDifficulty();
	}
}
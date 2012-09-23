#include <MAPIL/MAPIL.h>

#include "Menu.h"

namespace GameEngine
{
	class Menu::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;
	public:
		Impl();
		~Impl(){}
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
	};

	Menu::Impl::Impl()
	{
	}

	SceneType Menu::Impl::Update()
	{
		if( m_ButtonStatus.m_Status[ 0 ] == BUTTON_STATUS_KEEP ){
			MAPIL::BeginRendering2DGraphics();
			MAPIL::DrawString( 20.0f, 20.0f, "aiueo" );
			MAPIL::EndRendering2DGraphics();
		}
		else if( m_ButtonStatus.m_Status[ 1 ] == BUTTON_STATUS_PUSHED ){
			return SCENE_TYPE_STAGE;
		}

		return SCENE_TYPE_NOT_CHANGE;
	}

	void Menu::Impl::Draw()
	{
	}

	void Menu::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_ButtonStatus = *pHolder;
	}


	// ----------------------------------
	// ŽÀ‘•ƒNƒ‰ƒX‚ÌŒÄ‚Ño‚µ
	// ----------------------------------

	Menu::Menu() : Scene(), m_pImpl( new Menu::Impl )
	{
	}

	Menu::~Menu()
	{
	}

	void Menu::Init()
	{
	}

	SceneType Menu::Update()
	{
		return m_pImpl->Update();
	}

	void Menu::Draw()
	{
		m_pImpl->Draw();
	}

	void Menu::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_pImpl->AttachButtonState( pHolder );
	}

	void Menu::AttachResourceMap( const ResourceMap& map )
	{
	}
}
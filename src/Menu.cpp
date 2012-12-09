#include <MAPIL/MAPIL.h>

#include "Menu.h"

#include "ResourceID.h"

#include "Util.h"

namespace GameEngine
{
	enum MenuID
	{
		MENU_ID_GAME_START		= 0,	// ゲーム開始
		MENU_ID_SCORE			= 1,	// スコア
		MENU_ID_REPLAY			= 2,	// リプレイ
		MENU_ID_CONFIG			= 3,	// 設定
		MENU_ID_EXIT			= 4,	// ゲーム終了
	};

	class Menu::Impl
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
	};

	Menu::Impl::Impl()
	{
		m_Counter = 0;
		m_MenuPointed = MENU_ID_GAME_START;
		m_PlayBGM = true;
	}

	SceneType Menu::Impl::Update()
	{
		// BGM再生
		if( m_PlayBGM ){
			MAPIL::PlayStreamingBuffer( GLOBAL_RESOURCE_BGM_ID_MENU );
		}

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			MAPIL::PlayStaticBuffer( m_ResourceMap.m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_MENU_SELECTED ] );
			switch( m_MenuPointed ){
				case MENU_ID_GAME_START:
					MAPIL::StopStreamingBuffer( GLOBAL_RESOURCE_BGM_ID_MENU );
					m_PlayBGM = false;
					return SCENE_TYPE_STAGE;
				case MENU_ID_SCORE:
					return SCENE_TYPE_SCORE;
				case MENU_ID_REPLAY:
					return SCENE_TYPE_REPLAY;
				case MENU_ID_EXIT:
					return SCENE_TYPE_GAME_TERM;
				default:
					break;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
			if( m_MenuPointed == MENU_ID_EXIT ){
				return SCENE_TYPE_GAME_TERM;
			}
			m_MenuPointed = MENU_ID_EXIT;
		}

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
			++m_MenuPointed;
			if( m_MenuPointed > MENU_ID_EXIT ){
				m_MenuPointed = MENU_ID_GAME_START;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
			--m_MenuPointed;
			if( m_MenuPointed < MENU_ID_GAME_START ){
				m_MenuPointed = MENU_ID_EXIT;
			}
		}

		++m_Counter;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void Menu::Impl::Draw()
	{

		MAPIL::BeginRendering2DGraphics();


		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_GAME_START ],
							20.0f, 250.0f, 0.8f, 0.8f, false );
		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_SCORE ],
							20.0f, 280.0f, 0.8f, 0.8f, false );
		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_REPLAY ],
							20.0f, 310.0f, 0.8f, 0.8f, false );
		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_CONFIG ],
							20.0f, 340.0f, 0.8f, 0.8f, false );
		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_EXIT ],
							20.0f, 370.0f, 0.8f, 0.8f, false );

		unsigned char alpha = 255 - ( m_Counter % 20 ) * 5;
		if( m_MenuPointed == MENU_ID_GAME_START ){
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_GAME_START_SELECTED ],
								20.0f, 250.0f, 0.8f, 0.8f, false, 0xFFFFFF | ( alpha << 24 ) );
		}
		else if( m_MenuPointed == MENU_ID_SCORE ){
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_SCORE_SELECTED ],
								20.0f, 280.0f, 0.8f, 0.8f, false, 0xFFFFFF | ( alpha << 24 ) );
		}
		else if( m_MenuPointed == MENU_ID_REPLAY ){
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_REPLAY_SELECTED ],
								20.0f, 310.0f, 0.8f, 0.8f, false, 0xFFFFFF | ( alpha << 24 ) );
		}
		else if( m_MenuPointed == MENU_ID_CONFIG ){
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_CONFIG_SELECTED ],
								20.0f, 340.0f, 0.8f, 0.8f, false, 0xFFFFFF | ( alpha << 24 ) );
		}
		else if( m_MenuPointed == MENU_ID_EXIT ){
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_EXIT_SELECTED ],
								20.0f, 370.0f, 0.8f, 0.8f, false, 0xFFFFFF | ( alpha << 24 ) );
		}
		

		MAPIL::EndRendering2DGraphics();
	}

	void Menu::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_ButtonStatus = *pHolder;
	}

	void Menu::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
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
		m_pImpl->AttachResourceMap( map );
	}
}
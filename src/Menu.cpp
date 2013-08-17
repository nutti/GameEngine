#include <MAPIL/MAPIL.h>

#include "Menu.h"

#include "ResourceID.h"

#include "Util.h"

namespace GameEngine
{
	enum MenuID
	{
		MENU_ID_GAME_START		= 0,	// ゲーム開始
		MENU_ID_STAGE_SELECT	= 1,	// ステージセレクト
		MENU_ID_SCORE			= 2,	// スコア
		MENU_ID_REPLAY			= 3,	// リプレイ
		MENU_ID_CONFIG			= 4,	// 設定
		MENU_ID_EXIT			= 5,	// ゲーム終了
	};

	enum InitialAnimPhaseID
	{
		INI_ANIM_PHASE_ID_START				= 0,	// アニメーション開始
		INI_ANIM_PHASE_ID_TITLE				= 1,	// タイトルのアニメーション
		INI_ANIM_PHASE_ID_FLASH				= 2,	// スクリーンフラッシュのアニメーション
		INI_ANIM_PHASE_ID_MENU				= 3,	// メニューのアニメーション
		INI_ANIM_PHASE_ID_PREPARE_TERM		= 4,	// アニメーション終了準備
		INI_ANIM_PHASE_ID_TERM				= 5,	// アニメーション終了
		INI_ANIM_PHASE_ID_TOTAL,
	};

	const int INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_TOTAL ] = { 0, 30, 90, 180, 210, 230 };

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
		void Reflesh();
	};

	Menu::Impl::Impl()
	{
		m_Counter = 0;
		m_MenuPointed = MENU_ID_GAME_START;
		m_PlayBGM = true;
	}

	SceneType Menu::Impl::Update()
	{
		++m_Counter;

		// BGM再生
		if( m_PlayBGM ){
			MAPIL::PlayStreamingBuffer( GLOBAL_RESOURCE_BGM_ID_MENU );
		}

		if( m_Counter < INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_PREPARE_TERM ] ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
				m_Counter = INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_PREPARE_TERM ];
			}
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			MAPIL::PlayStaticBuffer( m_ResourceMap.m_pGlobalResourceMap->m_SEMap[ GLOBAL_RESOURCE_SE_ID_MENU_SELECTED ] );
			switch( m_MenuPointed ){
				case MENU_ID_GAME_START:
					return SCENE_TYPE_DIFFICULTY_SELECTION;
				case MENU_ID_STAGE_SELECT:
					return SCENE_TYPE_STAGE_SELECTION;
				case MENU_ID_CONFIG:
					return SCENE_TYPE_CONFIG;
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

		return SCENE_TYPE_NOT_CHANGE;
	}

	void Menu::Impl::Draw()
	{

		MAPIL::BeginRendering2DGraphics();

		const float CHAR_INTERVAL = 96.0f;
		const float INI_CHAR_POS = 8.0f;
		const float DISP_Y = 234.0f;
		const float DISP_X_OFFSET = 123.0f;

		// 背景画面アニメーション
		if( m_Counter <= INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_TITLE ] ){
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_INITIALIZE ],
								0.0f, 0.0f, false,
								0xFFFFFF | ( ( INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_TITLE ] - m_Counter ) * 8 ) << 24 );
		}

		// タイトルアニメーション
		if( m_Counter <= INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_TITLE ] ){
			for( int i = 0; i < 5; ++i ){
				MAPIL::DrawClipedTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_TITLE_2 ],
											DISP_X_OFFSET + CHAR_INTERVAL * i, DISP_Y, 1.0f, 1.0f,
											0.0f, INI_CHAR_POS + CHAR_INTERVAL * i, 0.0f, INI_CHAR_POS + CHAR_INTERVAL * ( i + 1 ),
											104.0f );
			}
		}
		else if(	m_Counter > INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_TITLE ] &&
					m_Counter <= INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_FLASH ] ){
			int phase = m_Counter - INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_TITLE ];
			for( int i = 0; i < 5; ++i ){
				float angle = 0.0f;
				float scale = 1.0f;
				float x = DISP_X_OFFSET + CHAR_INTERVAL * i;
				float y = DISP_Y;
				if( phase > i * 10 && phase < i * 10 + 60 ){
					angle = MAPIL::DegToRad( ( phase -  i * 10 ) * 18.0f );
					scale = 1.0f - ( phase - i * 10 ) * 0.4f / 60.0f;
					x = DISP_X_OFFSET - ( phase - i * 10 ) * ( ( i + 1 ) * 1.2f ) + ( 1.0f - ( -phase + 60 + i * 10 ) / 200.0f ) * CHAR_INTERVAL * i;
					y = DISP_Y - ( phase - i * 10 ) * 2.5f * ( 1.0f + i * 0.23f );
				}
				MAPIL::DrawClipedTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_TITLE_2 ],
											x,
											y,
											scale, scale,
											angle,
											INI_CHAR_POS + CHAR_INTERVAL * i, 0.0f, INI_CHAR_POS + CHAR_INTERVAL * ( i + 1 ),
											104.0f );
			}
		}
		else if(	m_Counter > INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_MENU ] &&
					m_Counter <= INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_PREPARE_TERM ] ){
			int phase = m_Counter - INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_MENU ];
			if( ( phase >= 5 && phase <= 8 ) || ( phase >= 15 && phase <= 18 ) ){
				MAPIL::DrawTexture( m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_TITLE ],
									20.0f, 20.0f, 0.6f, 0.6f, false );
			}
			MAPIL::DrawTexture( m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_TITLE ],
								20.0f, 20.0f, 0.6f, 0.6f, false );
		}
		else{
			int phase = ( m_Counter % 480 );
			if( ( phase >= 5 && phase <= 8 ) || ( phase >= 15 && phase <= 18 ) ){
				MAPIL::DrawTexture( m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_TITLE ],
									20.0f, 20.0f, 0.6f, 0.6f, false );
			}
			MAPIL::DrawTexture( m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_TITLE ],
								20.0f, 20.0f, 0.6f, 0.6f, false );
		}

		// 画面のフラッシュ処理
		if( m_Counter >= INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_FLASH ] && m_Counter < INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_MENU ] ){
			int phase = m_Counter - INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_FLASH ];
			if( phase < 10 ){
				MAPIL::DrawTexture( m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									0.0f, 0.0f, 40.0f, 40.0f, false,
									0xFFFFFFFF );
			}
			else if( phase >= 10 && phase < 90 ){
				MAPIL::DrawTexture( m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									0.0f, 0.0f, 40.0f, 40.0f, false,
									0xFFFFFF | ( ( 90 - phase ) * 3 ) << 24 );
			}
		}

		// メニュー画面出現処理
		if( m_Counter > INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_MENU ] && m_Counter <= INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_PREPARE_TERM ] ){
			int interval = INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_PREPARE_TERM ] - INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_MENU ];
			int phase = m_Counter - INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_MENU ];
			int texID[ 6 ] = {	GLOBAL_RESOURCE_TEXTURE_ID_MENU_GAME_START,
								GLOBAL_RESOURCE_TEXTURE_ID_MENU_GAME_START,
								GLOBAL_RESOURCE_TEXTURE_ID_MENU_SCORE,
								GLOBAL_RESOURCE_TEXTURE_ID_MENU_REPLAY,
								GLOBAL_RESOURCE_TEXTURE_ID_MENU_CONFIG,
								GLOBAL_RESOURCE_TEXTURE_ID_MENU_EXIT };
			for( int i = 0; i < sizeof( texID ) / sizeof( texID[ 0 ] ); ++i ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ texID[ i ] ],
									20.0f, 250.0f + i * 30.0f * phase / interval,
									0.8f, 0.8f, false, 0xFFFFFF | ( phase * 8 ) << 24 );
			}
		}

		if( m_Counter > INITIAL_ANIM_PHASE_COUNTS[ INI_ANIM_PHASE_ID_PREPARE_TERM ] ){
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_GAME_START ],
								20.0f, 250.0f, 0.8f, 0.8f, false );
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_GAME_START ],
								20.0f, 280.0f, 0.8f, 0.8f, false );
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_SCORE ],
								20.0f, 310.0f, 0.8f, 0.8f, false );
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_REPLAY ],
								20.0f, 340.0f, 0.8f, 0.8f, false );
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_CONFIG ],
								20.0f, 370.0f, 0.8f, 0.8f, false );
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_EXIT ],
								20.0f, 410.0f, 0.8f, 0.8f, false );

			unsigned char alpha = 255 - ( m_Counter % 20 ) * 5;
			if( m_MenuPointed == MENU_ID_GAME_START ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_GAME_START_SELECTED ],
									20.0f, 250.0f, 0.8f, 0.8f, false, 0xFFFFFF | ( alpha << 24 ) );
			}
			else if( m_MenuPointed == MENU_ID_STAGE_SELECT ){
				DrawFontString( m_ResourceMap, 20.0f, 290.0f, 0.6f, 0xFFFFFF00, "Stage Selection" );
			}
			else if( m_MenuPointed == MENU_ID_SCORE ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_SCORE_SELECTED ],
									20.0f, 310.0f, 0.8f, 0.8f, false, 0xFFFFFF | ( alpha << 24 ) );
			}
			else if( m_MenuPointed == MENU_ID_REPLAY ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_REPLAY_SELECTED ],
									20.0f, 340.0f, 0.8f, 0.8f, false, 0xFFFFFF | ( alpha << 24 ) );
			}
			else if( m_MenuPointed == MENU_ID_CONFIG ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_CONFIG_SELECTED ],
									20.0f, 370.0f, 0.8f, 0.8f, false, 0xFFFFFF | ( alpha << 24 ) );
			}
			else if( m_MenuPointed == MENU_ID_EXIT ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_MENU_EXIT_SELECTED ],
									20.0f, 410.0f, 0.8f, 0.8f, false, 0xFFFFFF | ( alpha << 24 ) );
			}
		}
		
		ResourceMap::TextureAtlas atlas;
		for( int i = 0; i < 12; i = i + 1 ){
		atlas = m_ResourceMap.m_pGlobalResourceMap->m_TexAtlasMap[ GLOBAL_TEX_ATLAS_ID_FONT_FIRST + 'A' + i - 'A' + 10 ];
		int id = m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ atlas.m_TexID ];
		MAPIL::DrawClipedTexture( m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ atlas.m_TexID ],
			120.0f+ 20.0f * i, 120.0f, 0.5f, 0.5f, 0.0f, atlas.m_X, atlas.m_Y, atlas.m_X + atlas.m_Width, atlas.m_Y + atlas.m_Height );
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

	void Menu::Impl::Reflesh()
	{
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

	void Menu::InitImpl()
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

	void Menu::Reflesh()
	{
		m_pImpl->Reflesh();
	}
}
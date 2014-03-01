#include <MAPIL/MAPIL.h>

#include "Config.h"
#include "EventMediator.h"

#include "ResourceID.h"
#include "EventTypes.h"

#include "Util.h"

namespace GameEngine
{

	enum ConfigItem
	{
		CONFIG_ITEM_BGM				= 0,
		CONFIG_ITEM_SE				= 1,
		CONFIG_ITEM_PLAY_SPEED		= 2,
		CONFIG_ITEM_INPUT			= 3,
		CONFIG_ITEM_EXIT			= 4,
		CONFIG_ITEM_TOTAL,
	};

	enum ConfigMode
	{
		CONFIG_MODE_MENU			= 0,
		CONFIG_MODE_INPUT			= 1,
		CONFIG_MODE_INPUT_KEY		= 2,
	};

	// 許可されたボタン
	const int ALLOWED_BUTTONS[] = {
#define BUTTON_CODE_STR
#include "ButtonCode.h"
#undef BUTTON_CODE_STR
	};

	class Config::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;
		ResourceMap					m_ResourceMap;
		int							m_Counter;
		GameConfigData				m_ConfigData;

		int							m_Mode;			// 現在の設定画面状態

		int							m_SelectedInputItem;	// 入力で選択されている項目
		int							m_SelectedItem;			// 選択されている項目

		int							m_SelectCounter;		// 項目選択時のカウンタ

		int							m_InputAccpetCount;		// 次に入力ボタン変更を受け付けるまでの時間

		std::weak_ptr < EventMediator >			m_pEventMediator;		// イベント仲介役クラス


		void DrawOnMenuMode() const;
		void DrawOnInputMode() const;
	public:
		Impl();
		~Impl(){}
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachConfigData( const GameConfigData& data );
		void SetEventMediator( std::weak_ptr < EventMediator > pMediator );
		const GameConfigData& GetConfigData() const;
	};

	Config::Impl::Impl()
	{
		m_Counter = 0;
		m_SelectCounter = 0;
		m_SelectedItem = CONFIG_ITEM_BGM;
		MAPIL::ZeroObject( &m_ConfigData, sizeof( m_ConfigData ) );
		m_Mode = CONFIG_MODE_MENU;
		m_SelectedInputItem = GENERAL_BUTTON_MOVE_UP;
		m_InputAccpetCount = 0;
	}

	SceneType Config::Impl::Update()
	{
		// 最初の時のカウンタ
		if( m_Counter < 60 ){
			++m_Counter;
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( m_SelectCounter > 0 ){
			--m_SelectCounter;
		}

		if( m_InputAccpetCount > 0 ){
			--m_InputAccpetCount;
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( m_Mode == CONFIG_MODE_MENU ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
				return SCENE_TYPE_MENU;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
				++m_SelectedItem;
				if( m_SelectedItem > CONFIG_ITEM_TOTAL - 1 ){
					m_SelectedItem = 0;
				}
				m_SelectCounter = 10;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				--m_SelectedItem;
				if( m_SelectedItem < 0 ){
					m_SelectedItem = CONFIG_ITEM_TOTAL - 1;
				}
				m_SelectCounter = 10;
			}

		
			if( m_SelectedItem == CONFIG_ITEM_BGM ){
				if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_RIGHT ) ){
					++m_ConfigData.m_BGMVolume;
					if( m_ConfigData.m_BGMVolume > MAX_BGM_VOLUME ){
						m_ConfigData.m_BGMVolume = MAX_BGM_VOLUME;
					}
					if( std::shared_ptr < EventMediator > p = m_pEventMediator.lock() ){
						p->SendEvent( EVENT_TYPE_BGM_VOLUME_CHANGED, &m_ConfigData );
					}
				}
				else if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_LEFT ) ){
					--m_ConfigData.m_BGMVolume;
					if( m_ConfigData.m_BGMVolume < 0 ){
						m_ConfigData.m_BGMVolume = 0;
					}
					if( std::shared_ptr < EventMediator > p = m_pEventMediator.lock() ){
						p->SendEvent( EVENT_TYPE_BGM_VOLUME_CHANGED, &m_ConfigData );
					}
				}
			}
			else if( m_SelectedItem == CONFIG_ITEM_SE ){
				if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_RIGHT ) ){
					++m_ConfigData.m_SEVolume;
					if( m_ConfigData.m_SEVolume > MAX_BGM_VOLUME ){
						m_ConfigData.m_SEVolume = MAX_BGM_VOLUME;
					}
					if( std::shared_ptr < EventMediator > p = m_pEventMediator.lock() ){
						p->SendEvent( EVENT_TYPE_SE_VOLUME_CHANGED, &m_ConfigData );
					}
				}
				else if( IsKeepPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_LEFT ) ){
					--m_ConfigData.m_SEVolume;
					if( m_ConfigData.m_SEVolume < 0 ){
						m_ConfigData.m_SEVolume = 0;
					}
					if( std::shared_ptr < EventMediator > p = m_pEventMediator.lock() ){
						p->SendEvent( EVENT_TYPE_SE_VOLUME_CHANGED, &m_ConfigData );
					}
				}
			}
			else if( m_SelectedItem == CONFIG_ITEM_PLAY_SPEED ){
				if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_RIGHT ) ){
					++m_ConfigData.m_PlaySpeed;
					if( m_ConfigData.m_PlaySpeed > GAME_PLAY_SPEED_X4 ){
						m_ConfigData.m_PlaySpeed = GAME_PLAY_SPEED_X1_4;
					}
					if( std::shared_ptr < EventMediator > p = m_pEventMediator.lock() ){
						p->SendEvent( EVENT_TYPE_PLAY_SPEED_CHANGED, &m_ConfigData );
					}
				}
				else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_LEFT ) ){
					--m_ConfigData.m_PlaySpeed;
					if( m_ConfigData.m_PlaySpeed < GAME_PLAY_SPEED_X1_4 ){
						m_ConfigData.m_PlaySpeed = GAME_PLAY_SPEED_X4;
					}
					if( std::shared_ptr < EventMediator > p = m_pEventMediator.lock() ){
						p->SendEvent( EVENT_TYPE_PLAY_SPEED_CHANGED, &m_ConfigData );
					}
				}
			}
			else if( m_SelectedItem == CONFIG_ITEM_INPUT ){
				if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
					m_Mode = CONFIG_MODE_INPUT;
				}
			}
			else if( m_SelectedItem == CONFIG_ITEM_EXIT ){
				if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
					return SCENE_TYPE_MENU;
				}
			}
		}
		else if( m_Mode == CONFIG_MODE_INPUT ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_BOMB ) ){
				m_Mode = CONFIG_MODE_MENU;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
				--m_SelectedInputItem;
				if( m_SelectedInputItem < 0 ){
					m_SelectedInputItem = GENERAL_BUTTON_TOTAL - 1;
				}
				m_SelectCounter = 10;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
				++m_SelectedInputItem;
				if( m_SelectedInputItem > GENERAL_BUTTON_TOTAL - 1 ){
					m_SelectedInputItem = 0;
				}
				m_SelectCounter = 10;
			}
			else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
				m_Mode = CONFIG_MODE_INPUT_KEY;
				m_InputAccpetCount = 10;
			}
		}
		else if( m_Mode == CONFIG_MODE_INPUT_KEY ){
			for( int i = 0; i < sizeof( ALLOWED_BUTTONS ) / sizeof( ALLOWED_BUTTONS[ 0 ] ); ++i ){
				if( MAPIL::IsKeyboardKeyPushed( MAPIL::GetKeyboardKeyCode( ALLOWED_BUTTONS[ i ] ) ) ){
					m_ConfigData.m_KeyboardCaps[ m_SelectedInputItem ] = ALLOWED_BUTTONS[ i ];
					m_Mode = CONFIG_MODE_INPUT;
					m_InputAccpetCount = 10;
					break;
				}
			}
		}


		++m_Counter;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void Config::Impl::DrawOnMenuMode() const
	{
		int alpha1 = 0;
		if( m_Counter >= 20 && m_Counter < 60 ){
			alpha1 = ( ( m_Counter - 20 ) * 6 + 10 ) & 0xFF;
		}
		else if( m_Counter >= 60 ){
			alpha1 = 0xFF;
		}
		int color1 = alpha1 << 24 | 0xAAAAAA;
		int selColor = alpha1 << 24 | 0xFFFFFF;

		float fontScale = 1.0f;
		float startX = 120.0f;
		float startY[ CONFIG_ITEM_TOTAL ];
		startY[ CONFIG_ITEM_BGM ] = 100.0f;
		startY[ CONFIG_ITEM_SE ] = startY[ CONFIG_ITEM_BGM ] + 50.0f;
		startY[ CONFIG_ITEM_PLAY_SPEED ] = startY[ CONFIG_ITEM_SE ] + 50.0f;
		startY[ CONFIG_ITEM_INPUT ] = startY[ CONFIG_ITEM_PLAY_SPEED ] + 50.0f;
		startY[ CONFIG_ITEM_EXIT ] = startY[ CONFIG_ITEM_PLAY_SPEED ] + 150.0f;

		int c[ CONFIG_ITEM_TOTAL ];
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		for( int i = 0; i < CONFIG_ITEM_TOTAL; ++i ){
			if( i == m_SelectedItem ){
				if( m_SelectCounter > 0 ){
					offsetX = -2.0f * sin( MAPIL::DegToRad( m_SelectCounter * 45.0 ) );
				}
				else{
					offsetX = -2.0f;
				}
				offsetY = 0.0f;
				c[ i ] = selColor;
			}
			else{
				c[ i ] = color1;
			}
		}

		DrawFontString( m_ResourceMap, startX, startY[ CONFIG_ITEM_BGM ], fontScale, c[ CONFIG_ITEM_BGM ], "BGM : %d", m_ConfigData.m_BGMVolume );
		DrawFontString( m_ResourceMap, startX, startY[ CONFIG_ITEM_SE ], fontScale, c[ CONFIG_ITEM_SE ], "SE : %d", m_ConfigData.m_SEVolume );
		DrawFontString( m_ResourceMap, startX, startY[ CONFIG_ITEM_PLAY_SPEED ], fontScale, c[ CONFIG_ITEM_PLAY_SPEED ], "Play Speed : %d", m_ConfigData.m_PlaySpeed );
		DrawFontString( m_ResourceMap, startX, startY[ CONFIG_ITEM_INPUT ], fontScale, c[ CONFIG_ITEM_INPUT ], "Configure Input" );
		DrawFontString( m_ResourceMap, startX, startY[ CONFIG_ITEM_EXIT ], fontScale, c[ CONFIG_ITEM_EXIT ], "Exit" );
	}

	void Config::Impl::DrawOnInputMode() const
	{
		int alpha1 = 0;
		if( m_Counter >= 20 && m_Counter < 60 ){
			alpha1 = ( ( m_Counter - 20 ) * 6 + 10 ) & 0xFF;
		}
		else if( m_Counter >= 60 ){
			alpha1 = 0xFF;
		}
		int color1 = alpha1 << 24 | 0xAAAAAA;
		int selColor = alpha1 << 24 | 0xFFFFFF;
		if( m_Mode == CONFIG_MODE_INPUT_KEY ){
			selColor = alpha1 << 24 | 0xFFFF00;
		}

		float fontScale = 1.0f;
		float startX = 120.0f;
		float startY = 100.0f;

		char* pStr[ 9 ] = { "up", "down", "right", "left", "shot", "bomb", "green", "blue", "red" };

		for( int i = 0; i < GENERAL_BUTTON_TOTAL; ++i ){
			int c = color1;
			if( i == m_SelectedInputItem ){
				c = selColor;
			}
			DrawFontString( m_ResourceMap, startX, startY + 30.0f * i, fontScale, c, pStr[ i ] );
			DrawFontString( m_ResourceMap, startX + 150.0f, startY + 30.0f * i, fontScale, c, GetStringFromButton( m_ConfigData.m_KeyboardCaps[ i ] ) );
		}
	}

	void Config::Impl::Draw()
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
		
		if( m_Mode == CONFIG_MODE_MENU ){
			DrawOnMenuMode();
		}
		else if( m_Mode == CONFIG_MODE_INPUT || m_Mode == CONFIG_MODE_INPUT_KEY ){
			DrawOnInputMode();
		}


		MAPIL::EndRendering2DGraphics();
	}

	void Config::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_ButtonStatus = *pHolder;
	}

	void Config::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	void Config::Impl::AttachConfigData( const GameConfigData& data )
	{
		m_ConfigData = data;
	}

	void Config::Impl::SetEventMediator( std::weak_ptr < EventMediator > pMediator )
	{
		m_pEventMediator = pMediator;
	}

	const GameConfigData& Config::Impl::GetConfigData() const
	{
		return m_ConfigData;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	Config::Config() : Scene(), m_pImpl( new Config::Impl )
	{
	}

	Config::~Config()
	{
	}

	void Config::InitImpl()
	{
	}

	SceneType Config::Update()
	{
		return m_pImpl->Update();
	}

	void Config::Draw()
	{
		m_pImpl->Draw();
	}

	void Config::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_pImpl->AttachButtonState( pHolder );
	}

	void Config::AttachResourceMap( const ResourceMap& map )
	{
		m_pImpl->AttachResourceMap( map );
	}

	void Config::AttachConfigData( const GameConfigData& data )
	{
		m_pImpl->AttachConfigData( data );
	}

	void Config::SetEventMediator( std::weak_ptr < EventMediator > pMediator )
	{
		m_pImpl->SetEventMediator( pMediator );
	}

	const GameConfigData& Config::GetConfigData() const
	{
		return m_pImpl->GetConfigData();
	}
}
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
		CONFIG_ITEM_EXIT			= 3,
		CONFIG_ITEM_TOTAL,
	};

	class Config::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;
		ResourceMap					m_ResourceMap;
		int							m_Counter;
		GameConfigData				m_ConfigData;

		int							m_SelectedItem;			// 選択されている項目

		int							m_SelectCounter;		// 項目選択時のカウンタ

		std::weak_ptr < EventMediator >			m_pEventMediator;		// イベント仲介役クラス
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
		else if( m_SelectedItem == CONFIG_ITEM_EXIT ){
			if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
				return SCENE_TYPE_MENU;
			}
		}


		++m_Counter;

		return SCENE_TYPE_NOT_CHANGE;
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

		int alpha1 = 0;
		int dispDifficulty = GAME_DIFFICULTY_CALM;
		if( m_Counter >= 20 && m_Counter < 60 ){
			alpha1 = ( ( m_Counter - 20 ) * 6 + 10 ) & 0xFF;
		}
		else if( m_Counter >= 60 ){
			alpha1 = 0xFF;
		}
		int color1 = alpha1 << 24 | 0xAAAAAA;
		int selColor = alpha1 << 24 | 0xFFFFFF;

		float fontScale = 0.6f;
		float startX = 120.0f;
		float startY[ CONFIG_ITEM_TOTAL ];
		startY[ CONFIG_ITEM_BGM ] = 100.0f;
		startY[ CONFIG_ITEM_SE ] = startY[ CONFIG_ITEM_BGM ] + 50.0f;
		startY[ CONFIG_ITEM_PLAY_SPEED ] = startY[ CONFIG_ITEM_SE ] + 50.0f;
		startY[ CONFIG_ITEM_EXIT ] = startY[ CONFIG_ITEM_PLAY_SPEED ] + 100.0f;

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
		DrawFontString( m_ResourceMap, startX, startY[ CONFIG_ITEM_EXIT ], fontScale, c[ CONFIG_ITEM_EXIT ], "Exit" );



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

	void Config::Init()
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
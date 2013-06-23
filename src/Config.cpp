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
	};

	// �����ꂽ�{�^��
	const int ALLOWED_BUTTONS[] = {
#define BUTTON_CODE_STR
#include "ButtonCode.h"
#undef BUTTON_CODE_STR
/*		MAPIL::KEYBOARD_KEY_BACK,		///< Backspace.
		MAPIL::KEYBOARD_KEY_RETURN,	///< Enter.
		MAPIL::KEYBOARD_KEY_ESCAPE,	///< Esc.
		MAPIL::KEYBOARD_KEY_SPACE,		///< Space.
		MAPIL::KEYBOARD_KEY_LEFT,		///< Left.
		MAPIL::KEYBOARD_KEY_UP,		///< Up.
		MAPIL::KEYBOARD_KEY_RIGHT,		///< Right.
		MAPIL::KEYBOARD_KEY_DOWN,		///< Down.
		MAPIL::KEYBOARD_KEY_SNAPSHOT,	///< Print Screen.
		MAPIL::KEYBOARD_KEY_DELETE,	///< Delete.
		MAPIL::KEYBOARD_KEY_0,			///< 0.
		MAPIL::KEYBOARD_KEY_1,			///< 1.
		MAPIL::KEYBOARD_KEY_2,		///< 2.
		MAPIL::KEYBOARD_KEY_3,		///< 3.
		MAPIL::KEYBOARD_KEY_4,		///< 4.
		MAPIL::KEYBOARD_KEY_5,		///< 5.
		MAPIL::KEYBOARD_KEY_6,		///< 6.
		MAPIL::KEYBOARD_KEY_7,		///< 7.
		MAPIL::KEYBOARD_KEY_8,		///< 8.
		MAPIL::KEYBOARD_KEY_9,		///< 9.
		MAPIL::KEYBOARD_KEY_A,		///< A.
		MAPIL::KEYBOARD_KEY_B,		///< B.
		MAPIL::KEYBOARD_KEY_C,		///< C.
		MAPIL::KEYBOARD_KEY_D,		///< D.
		MAPIL::KEYBOARD_KEY_E,		///< E.
		MAPIL::KEYBOARD_KEY_F,		///< F.
		MAPIL::KEYBOARD_KEY_G,		///< G.
		MAPIL::KEYBOARD_KEY_H,		///< H.
		MAPIL::KEYBOARD_KEY_I,		///< I.
		MAPIL::KEYBOARD_KEY_J,		///< J.
		MAPIL::KEYBOARD_KEY_K,		///< K.
		MAPIL::KEYBOARD_KEY_L,		///< L.
		MAPIL::KEYBOARD_KEY_M,		///< M.
		MAPIL::KEYBOARD_KEY_N,		///< N.
		MAPIL::KEYBOARD_KEY_O,		///< O.
		MAPIL::KEYBOARD_KEY_P,		///< P.
		MAPIL::KEYBOARD_KEY_Q,		///< Q.
		MAPIL::KEYBOARD_KEY_R,			///< R.
		MAPIL::KEYBOARD_KEY_S,			///< S.
		MAPIL::KEYBOARD_KEY_T,			///< T.
		MAPIL::KEYBOARD_KEY_U,			///< U.
		MAPIL::KEYBOARD_KEY_V,			///< V.
		MAPIL::KEYBOARD_KEY_W,			///< W.
		MAPIL::KEYBOARD_KEY_X,			///< X.
		MAPIL::KEYBOARD_KEY_Y,			///< Y.
		MAPIL::KEYBOARD_KEY_Z,			///< Z.
		MAPIL::KEYBOARD_KEY_F1,		///< F1.
		MAPIL::KEYBOARD_KEY_F2,		///< F2.
		MAPIL::KEYBOARD_KEY_F3,		///< F3.
		MAPIL::KEYBOARD_KEY_F4,		///< F4.
		MAPIL::KEYBOARD_KEY_F5,		///< F5.
		MAPIL::KEYBOARD_KEY_F6,		///< F6.
		MAPIL::KEYBOARD_KEY_F7,		///< F7.
		MAPIL::KEYBOARD_KEY_F8,		///< F8.
		MAPIL::KEYBOARD_KEY_F9,		///< F9.
		MAPIL::KEYBOARD_KEY_F10,		///< F10.
		MAPIL::KEYBOARD_KEY_LSHIFT,		///< Left Shift key.
		MAPIL::KEYBOARD_KEY_RSHIFT,		///< Right Shift key.*/
	};

	class Config::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;
		ResourceMap					m_ResourceMap;
		int							m_Counter;
		GameConfigData				m_ConfigData;

		int							m_Mode;			// ���݂̐ݒ��ʏ��

		int							m_SelectedInputItem;	// ���͂őI������Ă��鍀��
		int							m_SelectedItem;			// �I������Ă��鍀��

		int							m_SelectCounter;		// ���ڑI�����̃J�E���^

		std::weak_ptr < EventMediator >			m_pEventMediator;		// �C�x���g������N���X


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
	}

	SceneType Config::Impl::Update()
	{
		// �ŏ��̎��̃J�E���^
		if( m_Counter < 60 ){
			++m_Counter;
			return SCENE_TYPE_NOT_CHANGE;
		}

		if( m_SelectCounter > 0 ){
			--m_SelectCounter;
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

		float fontScale = 0.6f;
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

		float fontScale = 0.6f;
		float startX = 120.0f;
		float startY = 100.0f;

		char* pStr[ 9 ] = { "up", "down", "right", "left", "shot", "bomb", "green", "blue", "red" };

		for( int i = 0; i < GENERAL_BUTTON_TOTAL; ++i ){
			int c = color1;
			if( i == m_SelectedInputItem ){
				c = selColor;
			}
			DrawFontString( m_ResourceMap, startX, startY + 30.0f * i, fontScale, c, pStr[ i ] );
		}
	}

	void Config::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();

		// �w�i�摜
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
		else if( m_Mode == CONFIG_MODE_INPUT ){
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
	// �����N���X�̌Ăяo��
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
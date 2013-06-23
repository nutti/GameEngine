#include <MAPIL/MAPIL.h>

#include "KeyboardStateHolder.h"
#include "InputTypes.h"

namespace GameEngine
{
	class KeyboardStateHolder::Impl
	{
	private:
		unsigned char				m_KeyMaps[ GENERAL_BUTTON_TOTAL ];
		ButtonPushedStatus	m_ButtonStatus;
	public:
		Impl();
		~Impl(){}
		void Init();
		void Update();
		ButtonPushedStatus GetButtonStatus();
	};


	KeyboardStateHolder::Impl::Impl() : m_ButtonStatus( 0 )
	{
		MAPIL::ZeroObject( m_KeyMaps, sizeof( m_KeyMaps ) );
	}

	void KeyboardStateHolder::Impl::Init()
	{
		m_KeyMaps[ GENERAL_BUTTON_SHOT ] = MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_LSHIFT );
		m_KeyMaps[ GENERAL_BUTTON_BOMB ] = MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_SPACE );
		m_KeyMaps[ GENERAL_BUTTON_GREEN ] = MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_Z );
		m_KeyMaps[ GENERAL_BUTTON_BLUE ] = MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_X );
		m_KeyMaps[ GENERAL_BUTTON_RED ] = MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_C );
		m_KeyMaps[ GENERAL_BUTTON_MOVE_UP ] = MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_UP );
		m_KeyMaps[ GENERAL_BUTTON_MOVE_DOWN ] = MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_DOWN );
		m_KeyMaps[ GENERAL_BUTTON_MOVE_LEFT ] = MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_LEFT );
		m_KeyMaps[ GENERAL_BUTTON_MOVE_RIGHT ] = MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_RIGHT );
	}

	void KeyboardStateHolder::Impl::Update()
	{
		unsigned char keys[ 256 ];		// 取得キー一覧
		
		m_ButtonStatus = 0;
		MAPIL::GetKeyboardState( keys );
		for( int button = 0; button < GENERAL_BUTTON_TOTAL; ++button ){
			if( MAPIL::HasKeyPushedStatus( keys[ m_KeyMaps[ button ] ] ) ){
				m_ButtonStatus |= 1 << button;
			}
		}
	}

	ButtonPushedStatus KeyboardStateHolder::Impl::GetButtonStatus()
	{
		return m_ButtonStatus;
	}


	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	KeyboardStateHolder::KeyboardStateHolder() : m_pImpl( new KeyboardStateHolder::Impl )
	{
	}

	KeyboardStateHolder::~KeyboardStateHolder()
	{
	}

	void KeyboardStateHolder::Init()
	{
		m_pImpl->Init();
	}

	void KeyboardStateHolder::Update()
	{
		m_pImpl->Update();
	}

	ButtonPushedStatus KeyboardStateHolder::GetButtonState()
	{
		return m_pImpl->GetButtonStatus();
	}
}
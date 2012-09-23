#include <MAPIL/MAPIL.h>

#include "GeneralButtonManager.h"
#include "KeyboardStateHolder.h"

namespace GameEngine
{
	static bool IsPushed( int status, int button )
	{
		return status & ( 1 << button ) ? true : false;
	}

	// GeneralButtonManager実装クラス
	class GeneralButtonManager::Impl
	{
	private:
		std::auto_ptr < InputStateHolder >		m_pInputStateHolder;	// 入力デバイス
		int										m_PrevButtonStatus;		// 前回のキーの状態
		int										m_CurButtonStatus;		// 現在のキーの状態
		ButtonStatusHolder						m_ButtonStatus;			// ボタンの状態 ( ButtonStatus )
	public:
		Impl();
		~Impl(){}
		void Update();
		void GetButtonStatus( ButtonStatusHolder* pHolder );
		void ChangeDevice( InputDevice device );
	};

	GeneralButtonManager::Impl::Impl() : m_pInputStateHolder()
	{
		MAPIL::ZeroObject( &m_ButtonStatus, sizeof( m_ButtonStatus ) );
	}

	void GeneralButtonManager::Impl::Update()
	{
		m_pInputStateHolder->Update();
		int state = m_pInputStateHolder->GetButtonState();

		for( int i = 0; i < 8 ; ++i ){
			if( IsPushed( state, i ) ){
				// 連続して押されている場合
				if( m_ButtonStatus.m_Status[ i ] == BUTTON_STATUS_PUSHED ){
					m_ButtonStatus.m_Status[ i ] = BUTTON_STATUS_KEEP;
				}
				// 最初に押された場合
				else if( m_ButtonStatus.m_Status[ i ] == BUTTON_STATUS_NO_EVENT ){
					m_ButtonStatus.m_Status[ i ] = BUTTON_STATUS_PUSHED;
				}
			}
			// 押されていない時
			else{
				m_ButtonStatus.m_Status[ i ] = BUTTON_STATUS_NO_EVENT;
			}
		}
	}

	void GeneralButtonManager::Impl::GetButtonStatus( ButtonStatusHolder* pHolder )
	{
		*pHolder = m_ButtonStatus;
	}

	void GeneralButtonManager::Impl::ChangeDevice( InputDevice device )
	{
		switch( device ){
			case INPUT_DEVICE_KEYBOARD:
				m_pInputStateHolder.reset( new KeyboardStateHolder );
				break;
			default:
				break;
		}
		m_pInputStateHolder->Init();
	}



	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------


	GeneralButtonManager::GeneralButtonManager() : m_pImpl( new GeneralButtonManager::Impl )
	{
	}

	GeneralButtonManager::~GeneralButtonManager()
	{
	}

	void GeneralButtonManager::Update()
	{
		m_pImpl->Update();
	}

	void GeneralButtonManager::GetButtonStatus( ButtonStatusHolder* pHolder )
	{
		m_pImpl->GetButtonStatus( pHolder );
	}

	void GeneralButtonManager::ChangeDevice( InputDevice device )
	{
		m_pImpl->ChangeDevice( device );
	}
}
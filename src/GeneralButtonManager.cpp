#include <MAPIL/MAPIL.h>

#include "GeneralButtonManager.h"
#include "KeyboardStateHolder.h"
#include "FileInputStateHolder.h"

namespace GameEngine
{
	static bool IsPushed( int status, int button )
	{
		return status & ( 1 << button ) ? true : false;
	}

	// GeneralButtonManager�����N���X
	class GeneralButtonManager::Impl
	{
	private:
		std::auto_ptr < InputStateHolder >		m_pInputStateHolder;		// ���̓f�o�C�X
		int										m_PrevButtonStatus;			// �O��̃L�[�̏��
		int										m_CurButtonStatus;			// ���݂̃L�[�̏��
		ButtonStatusHolder						m_ButtonStatus;				// �{�^���̏�� ( ButtonStatus )
		int										m_SpecialKeyStatus[ 2 ];	// ����L�[�̏��
	public:
		Impl();
		~Impl(){}
		void Update();
		void GetButtonStatus( ButtonStatusHolder* pHolder );
		bool IsSpecialKeyPushed( int key );
		void ChangeDevice( InputDevice device );
		ButtonPushedStatus GetRawButtonStatus() const;
		void SetReplayNo( int entryNo );
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
				// �A�����ĉ�����Ă���ꍇ
				if( m_ButtonStatus.m_Status[ i ] == BUTTON_STATUS_PUSHED ){
					m_ButtonStatus.m_Status[ i ] = BUTTON_STATUS_KEEP;
				}
				// �ŏ��ɉ����ꂽ�ꍇ
				else if( m_ButtonStatus.m_Status[ i ] == BUTTON_STATUS_NO_EVENT ){
					m_ButtonStatus.m_Status[ i ] = BUTTON_STATUS_PUSHED;
				}
			}
			// ������Ă��Ȃ���
			else{
				m_ButtonStatus.m_Status[ i ] = BUTTON_STATUS_NO_EVENT;
			}
		}

		
		
		if( MAPIL::IsKeyboardKeyPushed( MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_F12 ) ) ){
			if( m_SpecialKeyStatus[ 0 ] == BUTTON_STATUS_NO_EVENT ){
				m_SpecialKeyStatus[ 0 ] = BUTTON_STATUS_PUSHED;
			}
			else if( m_SpecialKeyStatus[ 0 ] == BUTTON_STATUS_PUSHED ){
				m_SpecialKeyStatus[ 0 ] = BUTTON_STATUS_KEEP;
			}
		}
		else{
			m_SpecialKeyStatus[ 0 ] = BUTTON_STATUS_NO_EVENT;
		}
	}

	void GeneralButtonManager::Impl::GetButtonStatus( ButtonStatusHolder* pHolder )
	{
		*pHolder = m_ButtonStatus;
	}

	bool GeneralButtonManager::Impl::IsSpecialKeyPushed( int key )
	{
		return m_SpecialKeyStatus[ key ] == BUTTON_STATUS_PUSHED;
	}

	void GeneralButtonManager::Impl::ChangeDevice( InputDevice device )
	{
		switch( device ){
			case INPUT_DEVICE_KEYBOARD:
				m_pInputStateHolder.reset( new KeyboardStateHolder );
				break;
			case INPUT_DEVICE_FILE:
				m_pInputStateHolder.reset( new FileInputStateHolder );
				break;
			default:
				break;
		}
		m_pInputStateHolder->Init();
	}

	ButtonPushedStatus GeneralButtonManager::Impl::GetRawButtonStatus() const
	{
		return m_pInputStateHolder->GetButtonState();
	}

	void GeneralButtonManager::Impl::SetReplayNo( int entryNo )
	{
		FileInputStateHolder* p = dynamic_cast < FileInputStateHolder* > ( m_pInputStateHolder.get() );
		if( p ){
			p->LoadFile( entryNo );
		}
		else{
			exit( 0 );
		}
	}

	// ----------------------------------
	// �����N���X�̌Ăяo��
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

	bool GeneralButtonManager::IsSpecialKeyPushed( int key )
	{
		return m_pImpl->IsSpecialKeyPushed( key );
	}

	void GeneralButtonManager::ChangeDevice( InputDevice device )
	{
		m_pImpl->ChangeDevice( device );
	}

	ButtonPushedStatus GeneralButtonManager::GetRawButtonStatus() const
	{
		return m_pImpl->GetRawButtonStatus();
	}

	void GeneralButtonManager::SetReplayNo( int entryNo )
	{
		m_pImpl->SetReplayNo( entryNo );
	}
}
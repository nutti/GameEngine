#include <MAPIL/MAPIL.h>

#include "GeneralButtonManager.h"
#include "KeyboardStateHolder.h"

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
		std::auto_ptr < InputStateHolder >		m_pInputStateHolder;	// ���̓f�o�C�X
		int										m_PrevButtonStatus;		// �O��̃L�[�̏��
		int										m_CurButtonStatus;		// ���݂̃L�[�̏��
		ButtonStatusHolder						m_ButtonStatus;			// �{�^���̏�� ( ButtonStatus )
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

	void GeneralButtonManager::ChangeDevice( InputDevice device )
	{
		m_pImpl->ChangeDevice( device );
	}
}
#include <MAPIL/MAPIL.h>

#include "GameConfigurationHolder.h"

namespace GameEngine
{
	// GeneralButtonManager�����N���X
	class GameConfigurationHolder::Impl
	{
	private:
		int			m_SEVolume;			// ���ʉ��̉���
		int			m_BGMVolume;		// BGM�̉���
	public:
		Impl();
		~Impl(){}
		void Load( const std::string& fileName );
		void Save( const std::string& fileName );
	};

	GameConfigurationHolder::Impl::Impl()
	{
	}

	void GameConfigurationHolder::Impl::Load( const std::string& fileName )
	{
	}

	void GameConfigurationHolder::Impl::Save( const std::string& fileName )
	{
	}



	// ----------------------------------
	// �����N���X�̌Ăяo��
	// ----------------------------------


	GameConfigurationHolder::GameConfigurationHolder() : m_pImpl( new GameConfigurationHolder::Impl )
	{
	}

	GameConfigurationHolder::~GameConfigurationHolder()
	{
	}

	void GameConfigurationHolder::Load( const std::string& fileName )
	{
		m_pImpl->Load( fileName );
	}

	void GameConfigurationHolder::Save( const std::string& fileName )
	{
		m_pImpl->Save( fileName );
	}
}
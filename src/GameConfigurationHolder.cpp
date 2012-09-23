#include <MAPIL/MAPIL.h>

#include "GameConfigurationHolder.h"

namespace GameEngine
{
	// GeneralButtonManager実装クラス
	class GameConfigurationHolder::Impl
	{
	private:
		int			m_SEVolume;			// 効果音の音量
		int			m_BGMVolume;		// BGMの音量
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
	// 実装クラスの呼び出し
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
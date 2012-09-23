#ifndef INCLUDED_GAMEENGINE_GAMECONFIGURATIONHOLDER_H
#define INCLUDED_GAMEENGINE_GAMECONFIGURATIONHOLDER_H

#include <memory>
#include <string>

namespace GameEngine
{
	class GameConfigurationHolder
	{
		class Impl;
		std::auto_ptr < GameConfigurationHolder::Impl >		m_pImpl;
	public:
		GameConfigurationHolder();
		~GameConfigurationHolder();
		void Load( const std::string& fileName );
		void Save( const std::string& fileName );
	};
}

#endif
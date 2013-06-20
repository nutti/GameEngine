#ifndef INCLUDED_GAMEENGINE_CONFIGDATAHOLDER_H
#define INCLUDED_GAMEENGINE_CONFIGDATAHOLDER_H

#include <memory>

#include "GameStateTypes.h"

namespace GameEngine
{
	class ConfigDataHolder
	{
	private:
		class Impl;
		std::auto_ptr < ConfigDataHolder::Impl >	m_pImpl;
	public:
		ConfigDataHolder();
		~ConfigDataHolder();
		void Save( const std::string& fileName );
		void Load( const std::string& fileName );
		int GetBGMVolume() const;
		int GetSEVolume() const;
		int GetPlaySpeed() const;
		void SetBGMVolume( int volume );
		void SetSEVolume( int volume );
		void SetPlaySpeed( int speed );
	};
}

#endif
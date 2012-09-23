#ifndef INCLUDED_GAMEENGINE_REPLAYDATABUILDER_H
#define INCLUDED_GAMEENGINE_REPLAYDATABUILDER_H

#include <memory>
#include <string>

#include "InputTypes.h"

namespace GameEngine
{
	class ReplayDataBuilder
	{
	private:
		class Impl;
		std::auto_ptr < ReplayDataBuilder::Impl >	m_pImpl;
	public:
		ReplayDataBuilder();
		~ReplayDataBuilder();
		void AddButtonState( ButtonPushedStatus status );
		void Save( const std::string& fileName );
		void Cleanup();
	};
}

#endif
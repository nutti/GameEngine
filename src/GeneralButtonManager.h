#ifndef INCLUDED_GAMEENGINE_GENERALBUTTONMANAGER_H
#define INCLUDED_GAMEENGINE_GENERALBUTTONMANAGER_H

#include <memory>

#include "InputTypes.h"

namespace GameEngine
{
	class GeneralButtonManager
	{
		class Impl;
		std::auto_ptr < GeneralButtonManager::Impl >		m_pImpl;
	public:
		GeneralButtonManager();
		~GeneralButtonManager();
		void Update();
		void GetButtonStatus( ButtonStatusHolder* pHolder );
		void ChangeDevice( InputDevice device );
	};
}

#endif
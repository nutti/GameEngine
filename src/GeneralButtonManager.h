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
		bool IsSpecialKeyPushed( int key );
		//bool IsSubDeviceKeyPushed( int key );
		void ChangeDevice( InputDevice device );
		//void ChangeSubDevice( InputDevice device );
		ButtonPushedStatus GetRawButtonStatus() const;
		void SetReplayNo( int entryNo );
		void SetStageNo( int stageNo );
	};
}

#endif
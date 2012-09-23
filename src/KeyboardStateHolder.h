#ifndef INCLUDED_GAMEENGINE_KEYBOARDSTATEHOLDER_H
#define INCLUDED_GAMEENGINE_KEYBOARDSTATEHOLDER_H

#include <memory>

#include "InputStateHolder.h"


namespace GameEngine
{
	class KeyboardStateHolder : public InputStateHolder
	{
	private:
		class Impl;
		std::auto_ptr < KeyboardStateHolder::Impl >		m_pImpl;
	public:
		KeyboardStateHolder();
		~KeyboardStateHolder();
		void Init();
		void Update();
		ButtonPushedStatus GetButtonState();
	};
}

#endif
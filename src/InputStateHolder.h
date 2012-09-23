#ifndef INCLUDED_GAMEENGINE_INPUTSTATEHOLDER_H
#define INCLUDED_GAMEENGINE_INPUTSTATEHOLDER_H

#include "InputTypes.h"

namespace GameEngine
{
	class InputStateHolder
	{
	private:
	public:
		InputStateHolder();
		virtual ~InputStateHolder();
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual ButtonPushedStatus GetButtonState() = 0;
	};
}

#endif
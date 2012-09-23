#ifndef INCLUDED_GAMEENGINE_UTIL_H
#define INCLUDED_GAMEENGINE_UTIL_H

#include "InputTypes.h"

namespace GameEngine
{
	bool IsKeepPushed( const ButtonStatusHolder& holder, GeneralButton button );

	bool IsPushed( const ButtonStatusHolder& holder, GeneralButton button );
}

#endif
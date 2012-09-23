#include "Util.h"

namespace GameEngine
{
	bool IsKeepPushed( const ButtonStatusHolder& holder, GeneralButton button )
	{
		return holder.m_Status[ button ] == BUTTON_STATUS_KEEP;
	}

	bool IsPushed( const ButtonStatusHolder& holder, GeneralButton button )
	{
		return holder.m_Status[ button ] == BUTTON_STATUS_PUSHED;
	}
}
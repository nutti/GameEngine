#ifndef INCLUDED_GAMEENGINE_FILEINPUTSTATEHOLDER_H
#define INCLUDED_GAMEENGINE_FILEINPUTSTATEHOLDER_H

#include <memory>

#include "InputStateHolder.h"


namespace GameEngine
{
	class FileInputStateHolder : public InputStateHolder
	{
	private:
		class Impl;
		std::auto_ptr < FileInputStateHolder::Impl >		m_pImpl;
	public:
		FileInputStateHolder();
		~FileInputStateHolder();
		void Init();
		void Update();
		ButtonPushedStatus GetButtonState();
		void LoadFile( int entryNo );
	};
}

#endif
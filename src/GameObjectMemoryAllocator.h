#ifndef INCLUDED_GAMEENGINE_GAMEOBJECTMEMORYALLOCATOR_H
#define INCLUDED_GAMEENGINE_GAMEOBJECTMEMORYALLOCATOR_H

#include <memory>

namespace GameEngine
{
	class GameObjectMemoryAllocator
	{
	private:
	public:
		GameObjectMemoryAllocator();
		~GameObjectMemoryAllocator();
		static void* Allocate();
		static void Free( void* p );
	};
}

#endif
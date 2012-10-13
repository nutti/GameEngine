#ifndef INCLUDED_GAMEENGINE_GAMEOBJECTIMPLMEMORYALLOCATOR_H
#define INCLUDED_GAMEENGINE_GAMEOBJECTIMPLMEMORYALLOCATOR_H

#include <memory>

namespace GameEngine
{
	class GameObjectImplMemoryAllocator
	{
	private:
	public:
		GameObjectImplMemoryAllocator();
		~GameObjectImplMemoryAllocator();
		static void* Allocate();
		static void Free( void* p );
	};
}

#endif
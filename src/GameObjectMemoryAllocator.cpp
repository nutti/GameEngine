#include <MAPIL/MAPIL.h>

#include "GameObjectMemoryAllocator.h"

namespace GameEngine
{
	typedef MAPIL::Allocator < 6000, 1000 >		Allocator;
	static Allocator	g_Allocator;

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	GameObjectMemoryAllocator::GameObjectMemoryAllocator()
	{
	}

	GameObjectMemoryAllocator::~GameObjectMemoryAllocator()
	{
	}

	void* GameObjectMemoryAllocator::Allocate()
	{
		return g_Allocator.Alloc();
	}

	void GameObjectMemoryAllocator::Free( void* p )
	{
		g_Allocator.Free( p );
	}

}
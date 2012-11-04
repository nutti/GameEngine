#include <MAPIL/MAPIL.h>

#include "GameObjectMemoryAllocator.h"

namespace GameEngine
{
	typedef MAPIL::HeapAllocator Allocator;
	static Allocator	g_Allocator( 8000, 2000 );

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
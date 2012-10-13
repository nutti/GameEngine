#include <MAPIL/MAPIL.h>

#include "GameObjectImplMemoryAllocator.h"

namespace GameEngine
{
	typedef MAPIL::HeapAllocator Allocator;
	static Allocator	g_ImplAllocator( 6000, 3000 );

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	GameObjectImplMemoryAllocator::GameObjectImplMemoryAllocator()
	{
	}

	GameObjectImplMemoryAllocator::~GameObjectImplMemoryAllocator()
	{
	}

	void* GameObjectImplMemoryAllocator::Allocate()
	{
		return g_ImplAllocator.Alloc();
	}

	void GameObjectImplMemoryAllocator::Free( void* p )
	{
		g_ImplAllocator.Free( p );
	}

}
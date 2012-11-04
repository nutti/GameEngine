#include <MAPIL/MAPIL.h>

#include "GameObjectImplMemoryAllocator.h"

namespace GameEngine
{
	typedef MAPIL::HeapAllocator Allocator;
	static Allocator	g_ImplAllocator( 8000, 3000 );

	// ----------------------------------
	// é¿ëïÉNÉâÉXÇÃåƒÇ—èoÇµ
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
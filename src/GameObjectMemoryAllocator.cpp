#include <MAPIL/MAPIL.h>

#include "GameObjectMemoryAllocator.h"

namespace GameEngine
{
	typedef MAPIL::HeapAllocator Allocator;
	static Allocator	g_Allocator( 8000, 3000 );

	// ----------------------------------
	// é¿ëïÉNÉâÉXÇÃåƒÇ—èoÇµ
	// ----------------------------------

	GameObjectMemoryAllocator::GameObjectMemoryAllocator()
	{
	}

	GameObjectMemoryAllocator::~GameObjectMemoryAllocator()
	{
	}

	void* GameObjectMemoryAllocator::Allocate()
	{
		void* pNewMem = g_Allocator.Alloc();
		MAPIL::ZeroObject( pNewMem, 8000 );
		return pNewMem;
	}

	void GameObjectMemoryAllocator::Free( void* p )
	{
		g_Allocator.Free( p );
	}

}
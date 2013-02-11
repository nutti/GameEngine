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
		void* pNewMem = g_ImplAllocator.Alloc();
		MAPIL::ZeroObject( pNewMem, 8000 );
		return pNewMem;
	}

	void GameObjectImplMemoryAllocator::Free( void* p )
	{
		g_ImplAllocator.Free( p );
	}

}
#include "GameObjectImplBase.h"

#include "GameObjectImplMemoryAllocator.h"

#include <Windows.h>

namespace GameEngine
{
	GameObjectImplBase::GameObjectImplBase()
	{
	}

	GameObjectImplBase::~GameObjectImplBase()
	{
	}

	void* GameObjectImplBase::operator new( ::size_t size )
	{
		if( size > 8000 ){
			OutputDebugStringA( "Size over.\n" );
			exit( 1 );
		}
		return GameObjectImplMemoryAllocator::Allocate();
	}

	void GameObjectImplBase::operator delete( void* p )
	{
		GameObjectImplMemoryAllocator::Free( p );
	}
}
#include "GameObjectImplBase.h"

#include "GameObjectImplMemoryAllocator.h"

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
		return GameObjectImplMemoryAllocator::Allocate();
	}

	void GameObjectImplBase::operator delete( void* p )
	{
		GameObjectImplMemoryAllocator::Free( p );
	}
}
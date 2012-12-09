#include "GameObject.h"

#include "GameObjectMemoryAllocator.h"

#include <Windows.h>

namespace GameEngine
{
	GameObject::GameObject()
	{
	}

	GameObject::~GameObject()
	{
	}

	void* GameObject::operator new( ::size_t size )
	{
		if( size > 8000 ){
			OutputDebugStringA( "Size over." );
			exit( 1 );
		}
		return GameObjectMemoryAllocator::Allocate();
	}

	void GameObject::operator delete( void* p )
	{
		GameObjectMemoryAllocator::Free( p );
	}
}
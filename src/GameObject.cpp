#include "GameObject.h"

#include "GameObjectMemoryAllocator.h"

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
		return GameObjectMemoryAllocator::Allocate();
	}

	void GameObject::operator delete( void* p )
	{
		GameObjectMemoryAllocator::Free( p );
	}
}
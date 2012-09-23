#ifndef INCLUDED_GAMEENGINE_GAMEOBJECT_H
#define INCLUDED_GAMEENGINE_GAMEOBJECT_H

namespace GameEngine
{
	class GameObject
	{
	private:
	public:
		GameObject();
		virtual ~GameObject();
		void* operator new( ::size_t size );
		void operator delete( void* p );
	};
}

#endif
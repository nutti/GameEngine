#ifndef INCLUDED_GAMEENGINE_GAMEOBJECTIMPLBASE_H
#define INCLUDED_GAMEENGINE_GAMEOBJECTIMPLBASE_H

namespace GameEngine
{
	class GameObjectImplBase
	{
	private:
	public:
		GameObjectImplBase();
		virtual ~GameObjectImplBase();
		void* operator new( ::size_t size );
		void operator delete( void* p );
	};
}

#endif
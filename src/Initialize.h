#ifndef INCLUDED_GAMEENGINE_INITIALIZE_H
#define INCLUDED_GAMEENGINE_INITIALIZE_H

#include <memory>
#include "Scene.h"

namespace GameEngine
{
	class Initialize : public Scene
	{
	private:
		class Impl;
		std::auto_ptr < Initialize::Impl >		m_pImpl;
	public:
		Initialize();
		~Initialize();
		SceneType Update();
		void Init();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_SCENEBUILDER_H
#define INCLUDED_GAMEENGINE_SCENEBUILDER_H

#include <memory>

namespace GameEngine
{
	class Scene;
	class SceneBuilder
	{
		class Impl;
		std::auto_ptr < SceneBuilder::Impl >		m_pImpl;
	public:
		SceneBuilder();
		~SceneBuilder();
		Scene* CreateNextScene( int scene );
		void SaveScene( int type, Scene* scene );		// ÉVÅ[ÉìÇÃï€ë∂
	};
}

#endif
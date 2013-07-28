#ifndef INCLUDED_GAMEENGINE_MENU_H
#define INCLUDED_GAMEENGINE_MENU_H

#include <memory>
#include "Scene.h"

namespace GameEngine
{
	class Menu : public Scene
	{
	private:
		class Impl;
		std::auto_ptr < Menu::Impl >		m_pImpl;
		void InitImpl();
	public:
		Menu();
		~Menu();
		//void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void Reflesh();
	};
}

#endif
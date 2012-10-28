#ifndef INCLUDED_GAMEENGINE_APPLICATION_H
#define INCLUDED_GAMEENGINE_APPLICATION_H

#include <memory>

namespace GameEngine
{
	class Application
	{
		class Impl;
		std::auto_ptr < Application::Impl >		m_pImpl;
	public:
		Application();
		~Application();
		void Init( bool wndMode );
		void Run();
	};
}

#endif
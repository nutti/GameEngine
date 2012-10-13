#ifndef INCLUDED_GAMEENGINE_EVENTMEDIATOR_H
#define INCLUDED_GAMEENGINE_EVENTMEDIATOR_H

#include <memory>

namespace GameEngine
{
	class SceneManager;
	class GeneralButtonManager;
	class GameStateManager;
	class ResourceManager;
	class ScriptManager;
	class EventMediator
	{
		class Impl;
		std::auto_ptr < EventMediator::Impl >		m_pImpl;
	public:
		EventMediator();
		~EventMediator();
		void SendEvent( int type );
		void SendEvent( int type, void* pArg );
		void SetSceneManager( std::shared_ptr < SceneManager > pSceneManager );
		void SetButtonManager( std::shared_ptr < GeneralButtonManager > pButtonManager );
		void SetGameStateManager( std::shared_ptr < GameStateManager > pGameStateManager );
		void SetResourceManager( std::shared_ptr < ResourceManager > pResourceManager );
		void SetScriptManager( std::shared_ptr < ScriptManager > pScriptManager );
		bool HasTermSig() const;
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_SCENEMANAGER_H
#define INCLUDED_GAMEENGINE_SCENEMANAGER_H

#include <memory>

#include "SceneTypes.h"
#include "InputTypes.h"
#include "ScriptTypes.h"
#include "ResourceTypes.h"
#include "GameStateTypes.h"

namespace GameEngine
{
	class EventMediator;
	class SceneManager
	{
		class Impl;
		std::auto_ptr < SceneManager::Impl >		m_pImpl;
	public:
		SceneManager( std::shared_ptr < EventMediator > pEventMediator );
		~SceneManager();
		void Draw();
		SceneType Update();
		void AttachSceneResourceMap( const ResourceMap& map );
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachScriptData( const ScriptData& data );
		void AttachGameData( const GameDataMsg& msg );
		GameDataMsg GetFrameScoreData() const;
		void ChangeScene( SceneType scene );
		SceneType GetCurSceneType() const;
	};
}

#endif
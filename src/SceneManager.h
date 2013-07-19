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
		void AttachDisplayedSaveData( const DisplayedSaveData& data );
		void AttachDisplayedReplayInfo( const DisplayedReplayInfo& info );
		void AttachConfigData( const GameConfigData& data );
		void AttachInitialGameData( const InitialGameData& data );
		void AttachDisplayedNormalPlayStat( const DisplayedNormalPlayStat& stat );
		void AttachDisplayedStageSelectionPlayStat( const DisplayedStageSelectionPlayStat& stat );
		void AttachGameStat( const GameStat& stat );
		const DisplayedSaveData& GetDisplayedSaveData() const;
		void ChangeScene( SceneType scene );
		SceneType GetCurSceneType() const;
		void SetGameDifficulty( int difficulty );
		void SetRecordRank( int rank );
		void SetHIScore( int score );
		const SaveDataRecord& GetSaveDataRecord() const;
		const ReplayDataRecord& GetReplayDataRecord() const;
		const GameConfigData& GetConfigData() const;
		const GameStat& GetGameStat() const;
		int GetGameDifficulty() const;
		void ClearGameData();
		const DisplayedReplayInfo::Entry& GetReplayInfo() const;
		int GetGameMode() const;
		int GetReplayNo() const;
		void SwitchToNextScene();
		bool NeedToSwitch() const;
		bool Paused() const;
	};
}

#endif
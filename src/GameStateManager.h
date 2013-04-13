#ifndef INCLUDED_GAMEENGINE_GAMESTATEMANAGER_H
#define INCLUDED_GAMEENGINE_GAMESTATEMANAGER_H

#include <memory>
#include <string>

#include "InputTypes.h"
#include "GameStateTypes.h"

namespace GameEngine
{
	class GameStateManager
	{
	private:
		class Impl;
		std::auto_ptr < GameStateManager::Impl >	m_pImpl;
	public:
		GameStateManager();
		~GameStateManager();
		//void RecordButtonState( ButtonPushedStatus status );
		//void SaveReplayFile( const DisplayedReplayInfo::Entry& entry );
		//void StartReplayRecording();
		//void EndReplayRecording();
		//void StartGameDataRecording();
		//void EndGameDataRecording();
		void FlushGameData();			// ファイルへ現在のゲームデータを保存
		int GetPlayTime() const;
		void UpdatePlayTime();
		const SaveDataRecord& GetRecord( int difficulty, int rank ) const;
		int GetAllClearCount( int difficulty ) const;
		int GetProgress( int difficulty ) const;
		int GetProgress() const;
		int GetPlayTime( int difficulty ) const;
		void SetRecord( int difficulty, const SaveDataRecord& record );
		int GetRank( int difficulty, const SaveDataRecord& record ) const;
		int GetHIScore( int difficulty ) const;
		DisplayedReplayInfo GetDisplayedReplayInfo() const;
		void SaveReplay( const ReplayDataRecord& record );
		void LoadGameData();
		InitialGameData GetReplayIniData( int replayNo, int stageNo );
	};
}

#endif
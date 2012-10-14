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
		void RecordButtonState( ButtonPushedStatus status );
		void SaveReplayFile( const std::string& fileName );
		void StartReplayRecording();
		void EndReplayRecording();
		void StartGameDataRecording();
		void EndGameDataRecording();
		//GameDataMsg GetGameData() const;
		//void AddGameData( const GameDataMsg& data );
		//void UpdateGameData();
		void FlushGameData();			// ファイルへ現在のゲームデータを保存
		int GetPlayTime() const;
		void UpdatePlayTime();
		const SaveDataRecord& GetRecord( int difficulty, int rank ) const;
		int GetAllClearCount( int difficulty ) const;
		int GetProgress( int difficulty ) const;
		int GetProgress() const;
		int GetPlayTime( int difficulty ) const;
	};
}

#endif
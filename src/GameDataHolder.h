#ifndef INCLUDED_GAMEENGINE_GAMEDATAHOLDER_H
#define INCLUDED_GAMEENGINE_GAMEDATAHOLDER_H

#include <memory>

#include "GameStateTypes.h"

namespace GameEngine
{
	class GameDataHolder
	{
	private:
		class Impl;
		std::auto_ptr < GameDataHolder::Impl >	m_pImpl;
	public:
		GameDataHolder();
		~GameDataHolder();
		void StartRecording();
		void EndRecording();
		void Flush();
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
		void Load( const std::string& fileName );

		int GetPlayCount( int difficulty ) const;

		const NormalPlayStat& GetNormalPlayStat( int difficulty ) const;
		const StageSelectionPlayStat& GetStageSelectionPlayStat( int difficulty ) const;

		void SetNormalPlayStat( int difficulty, const NormalPlayStat& stat );
		void SetStageSelectionPlayStat( int difficulty, const StageSelectionPlayStat& stat );
	};
}

#endif
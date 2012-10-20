#include "GameStateManager.h"

#include <MAPIL/MAPIL.h>

#include "ReplayDataBuilder.h"
#include "GameDataHolder.h"
#include "ReplayDataLoader.h"

namespace GameEngine
{
	class GameStateManager::Impl
	{
	private:
		ReplayDataBuilder			m_ReplayBuilder;
		GameDataHolder				m_GameDataHolder;
		ReplayDataLoader			m_ReplayLoader;
	public:
		Impl();
		~Impl(){}
		void RecordButtonState( ButtonPushedStatus status );
		void SaveReplayFile( int entryNo );
		void StartReplayRecording();
		void EndReplayRecording();
		void StartGameDataRecording();
		void EndGameDataRecording();
		void FlushGameData();
		int GetPlayTime() const;
		void UpdatePlayTime();
		const SaveDataRecord& GetRecord( int difficulty, int rank ) const;
		int GetAllClearCount( int difficulty ) const;
		int GetProgress( int difficulty ) const;
		int GetProgress() const;
		int GetPlayTime( int difficulty ) const;
		void SetRecord( int difficulty, const SaveDataRecord& record );
		int GetRank( int difficulty, const SaveDataRecord& record ) const;
		DisplayedReplayInfo GetDisplayedReplayInfo() const;
	};

	GameStateManager::Impl::Impl() :	m_ReplayBuilder(),
										m_GameDataHolder(),
										m_ReplayLoader()
	{
	}

	void GameStateManager::Impl::RecordButtonState( ButtonPushedStatus status )
	{
		m_ReplayBuilder.AddButtonState( status );
	}

	void GameStateManager::Impl::SaveReplayFile( int entryNo )
	{
		std::string fileName = REPLAY_FILE_NAME_PREFIX;
		fileName += ( entryNo / 10 ) + '0';
		fileName += ( entryNo % 10 ) + '0';
		fileName += REPLAY_FILE_NAME_SUFFIX;
		m_ReplayBuilder.Save( fileName );
	}

	void GameStateManager::Impl::StartReplayRecording()
	{
		m_ReplayBuilder.Cleanup();
	}

	void GameStateManager::Impl::EndReplayRecording()
	{
		m_ReplayBuilder.Cleanup();
	}

	void GameStateManager::Impl::StartGameDataRecording()
	{
		m_GameDataHolder.StartRecording();
	}

	void GameStateManager::Impl::EndGameDataRecording()
	{
		m_GameDataHolder.EndRecording();
	}

	void GameStateManager::Impl::FlushGameData()
	{
		m_GameDataHolder.Flush();
	}

	int GameStateManager::Impl::GetPlayTime() const
	{
		return m_GameDataHolder.GetPlayTime();
	}

	void GameStateManager::Impl::UpdatePlayTime()
	{
		m_GameDataHolder.UpdatePlayTime();
	}

	const SaveDataRecord& GameStateManager::Impl::GetRecord( int difficulty, int rank ) const
	{
		return m_GameDataHolder.GetRecord( difficulty, rank );
	}

	int GameStateManager::Impl::GetAllClearCount( int difficulty ) const
	{
		return m_GameDataHolder.GetAllClearCount( difficulty );
	}

	int GameStateManager::Impl::GetProgress( int difficulty ) const
	{
		return m_GameDataHolder.GetProgress( difficulty );
	}

	int GameStateManager::Impl::GetProgress() const
	{
		return m_GameDataHolder.GetProgress();
	}

	int GameStateManager::Impl::GetPlayTime( int difficulty ) const
	{
		return m_GameDataHolder.GetPlayTime( difficulty );
	}

	void GameStateManager::Impl::SetRecord( int difficulty, const SaveDataRecord& record )
	{
		m_GameDataHolder.SetRecord( difficulty, record );
	}

	int GameStateManager::Impl::GetRank( int difficulty, const SaveDataRecord& record ) const
	{
		return m_GameDataHolder.GetRank( difficulty, record );
	}

	DisplayedReplayInfo GameStateManager::Impl::GetDisplayedReplayInfo() const
	{
		DisplayedReplayInfo info;

		for( int i = 0; i < 25; ++i ){
			std::string fileName = REPLAY_FILE_NAME_PREFIX;
			fileName += ( i / 10 ) + '0';
			fileName += ( i % 10 ) + '0';
			fileName += REPLAY_FILE_NAME_SUFFIX;
			info.m_Entries[ i ] = m_ReplayLoader.GetDisplayedInfo( fileName );
		}

		return info;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	GameStateManager::GameStateManager() : m_pImpl( new GameStateManager::Impl )
	{
	}

	GameStateManager::~GameStateManager()
	{
	}

	void GameStateManager::RecordButtonState( ButtonPushedStatus status )
	{
		m_pImpl->RecordButtonState( status );
	}

	void GameStateManager::SaveReplayFile( int entryNo )
	{
		m_pImpl->SaveReplayFile( entryNo );
	}

	void GameStateManager::StartReplayRecording()
	{
		m_pImpl->StartReplayRecording();
	}

	void GameStateManager::EndReplayRecording()
	{
		m_pImpl->EndReplayRecording();
	}

	void GameStateManager::StartGameDataRecording()
	{
		m_pImpl->StartGameDataRecording();
	}

	void GameStateManager::EndGameDataRecording()
	{
		m_pImpl->EndGameDataRecording();
	}

	void GameStateManager::FlushGameData()
	{
		m_pImpl->FlushGameData();
	}

	int GameStateManager::GetPlayTime() const
	{
		return m_pImpl->GetPlayTime();
	}

	void GameStateManager::UpdatePlayTime()
	{
		m_pImpl->UpdatePlayTime();
	}

	const SaveDataRecord& GameStateManager::GetRecord( int difficulty, int rank ) const
	{
		return m_pImpl->GetRecord( difficulty, rank );
	}

	int GameStateManager::GetAllClearCount( int difficulty ) const
	{
		return m_pImpl->GetAllClearCount( difficulty );
	}

	int GameStateManager::GetProgress( int difficulty ) const
	{
		return m_pImpl->GetProgress( difficulty );
	}

	int GameStateManager::GetProgress() const
	{
		return m_pImpl->GetProgress();
	}

	int GameStateManager::GetPlayTime( int difficulty ) const
	{
		return m_pImpl->GetPlayTime( difficulty );
	}

	void GameStateManager::SetRecord( int difficulty, const SaveDataRecord& record )
	{
		m_pImpl->SetRecord( difficulty, record );
	}

	int GameStateManager::GetRank( int difficulty, const SaveDataRecord& record ) const
	{
		return m_pImpl->GetRank( difficulty, record );
	}

	DisplayedReplayInfo GameStateManager::GetDisplayedReplayInfo() const
	{
		return m_pImpl->GetDisplayedReplayInfo();
	}
}
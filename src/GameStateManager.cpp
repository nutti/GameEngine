#include "GameStateManager.h"

#include <MAPIL/MAPIL.h>

#include "ReplayDataBuilder.h"
#include "GameDataHolder.h"

namespace GameEngine
{
	class GameStateManager::Impl
	{
	private:
		ReplayDataBuilder			m_ReplayBuilder;
		GameDataHolder				m_GameDataHolder;
	public:
		Impl();
		~Impl(){}
		void RecordButtonState( ButtonPushedStatus status );
		void SaveReplayFile( const std::string& fileName );
		void StartReplayRecording();
		void EndReplayRecording();
		void StartGameDataRecording();
		void EndGameDataRecording();
		GameDataMsg GetGameData() const;
		void AddGameData( const GameDataMsg& data );
		void UpdateGameData();
	};

	GameStateManager::Impl::Impl() : m_ReplayBuilder()
	{
	}

	void GameStateManager::Impl::RecordButtonState( ButtonPushedStatus status )
	{
		m_ReplayBuilder.AddButtonState( status );
	}

	void GameStateManager::Impl::SaveReplayFile( const std::string& fileName )
	{
		m_ReplayBuilder.Save( fileName );
	}

	void GameStateManager::Impl::StartReplayRecording()
	{
		m_ReplayBuilder.Cleanup();
	}

	void GameStateManager::Impl::EndReplayRecording()
	{
	}

	void GameStateManager::Impl::StartGameDataRecording()
	{
		m_GameDataHolder.StartRecording();
	}

	void GameStateManager::Impl::EndGameDataRecording()
	{
		m_GameDataHolder.EndRecording();
	}

	GameDataMsg GameStateManager::Impl::GetGameData() const
	{
		return m_GameDataHolder.GetScoreData();
	}

	void GameStateManager::Impl::AddGameData( const GameDataMsg& data )
	{
		m_GameDataHolder.Add( data );
	}

	void GameStateManager::Impl::UpdateGameData()
	{
		m_GameDataHolder.Update();
	}

	// ----------------------------------
	// ŽÀ‘•ƒNƒ‰ƒX‚ÌŒÄ‚Ño‚µ
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

	void GameStateManager::SaveReplayFile( const std::string& fileName )
	{
		m_pImpl->SaveReplayFile( fileName );
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

	GameDataMsg GameStateManager::GetGameData() const
	{
		return m_pImpl->GetGameData();
	}

	void GameStateManager::AddGameData( const GameDataMsg& data )
	{
		m_pImpl->AddGameData( data );
	}

	void GameStateManager::UpdateGameData()
	{
		m_pImpl->UpdateGameData();
	}

}
#include "GameStateManager.h"

#include <MAPIL/MAPIL.h>

#include "ReplayDataBuilder.h"
#include "GameDataHolder.h"
#include "ConfigDataHolder.h"
#include "ReplayDataLoader.h"
#include "Util.h"

namespace GameEngine
{
	const char* CONFIG_FILE_DIR		= "config";
	const char* CONFIG_FILE_NAME	= "eriKs.ini";

	class GameStateManager::Impl
	{
	private:
		ReplayDataBuilder			m_ReplayBuilder;
		GameDataHolder				m_GameDataHolder;
		ConfigDataHolder			m_ConfigDataHolder;
		ReplayDataLoader			m_ReplayLoader;
	public:
		Impl();
		~Impl(){}
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
		int GetHIScore( int difficulty ) const;
		DisplayedReplayInfo GetDisplayedReplayInfo() const;
		void SaveReplay( const ReplayDataRecord& record );
		void LoadGameData();
		InitialGameData GetReplayIniData( int replayNo, int stageNo );
		int GetReplayGameDifficulty( int replayNo );
		void SaveConfigData( const GameConfigData& data );
		GameConfigData GetConfigData() const;
		void LoadConfigData();
		DisplayedNormalPlayStat GetDisplayedNormalPlayStat() const;
		DisplayedStageSelectionPlayStat GetDisplayedStageSelectionPlayStat() const;
		GameStat GetGameStat() const;
		void SetGameStat( const GameStat& stat );
	};

	GameStateManager::Impl::Impl() :	m_ReplayBuilder(),
										m_GameDataHolder(),
										m_ConfigDataHolder(),
										m_ReplayLoader()
	{
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

	int GameStateManager::Impl::GetHIScore( int difficulty ) const
	{
		return m_GameDataHolder.GetHIScore( difficulty );
	}

	DisplayedReplayInfo GameStateManager::Impl::GetDisplayedReplayInfo() const
	{
		DisplayedReplayInfo info;

		for( int i = 0; i < MAX_REPLAY_ENTRY; ++i ){
			std::string fileName = REPLAY_FILE_DIR;
			fileName += '/';
			fileName += REPLAY_FILE_NAME_PREFIX;
			fileName += ( i / 10 ) + '0';
			fileName += ( i % 10 ) + '0';
			fileName += REPLAY_FILE_NAME_SUFFIX;
			info.m_Entries[ i ] = m_ReplayLoader.GetDisplayedInfo( fileName );
		}

		return info;
	}

	void GameStateManager::Impl::SaveReplay( const ReplayDataRecord& record )
	{
		if( !FileExist( REPLAY_FILE_DIR ) ){
			CreateDirectory( REPLAY_FILE_DIR );
		}

		std::string fileName = REPLAY_FILE_DIR;
		fileName += '/';
		fileName += REPLAY_FILE_NAME_PREFIX;
		fileName += ( record.m_EntryNo / 10 ) + '0';
		fileName += ( record.m_EntryNo % 10 ) + '0';
		fileName += REPLAY_FILE_NAME_SUFFIX;

		m_ReplayBuilder.Save( fileName, record );
	}

	void GameStateManager::Impl::LoadGameData()
	{
		m_GameDataHolder.Load( "save/save.dat" );
	}

	InitialGameData GameStateManager::Impl::GetReplayIniData( int replayNo, int stageNo )
	{
		std::string fileName = REPLAY_FILE_DIR;
		fileName += '/';
		fileName += REPLAY_FILE_NAME_PREFIX;
		fileName += ( replayNo / 10 ) + '0';
		fileName += ( replayNo % 10 ) + '0';
		fileName += REPLAY_FILE_NAME_SUFFIX;

		ReplayDataLoader loader;
		InitialGameData iniData;
		MAPIL::ZeroObject( &iniData, sizeof( iniData ) );
		loader.Load( fileName );
		iniData = loader.GetIniGameData( stageNo );

		return iniData;
	}

	int GameStateManager::Impl::GetReplayGameDifficulty( int replayNo )
	{
		std::string fileName = REPLAY_FILE_DIR;
		fileName += '/';
		fileName += REPLAY_FILE_NAME_PREFIX;
		fileName += ( replayNo / 10 ) + '0';
		fileName += ( replayNo % 10 ) + '0';
		fileName += REPLAY_FILE_NAME_SUFFIX;

		ReplayDataLoader loader;
		InitialGameData iniData;
		MAPIL::ZeroObject( &iniData, sizeof( iniData ) );
		loader.Load( fileName );

		return loader.GetGameDifficulty();
	}

	void GameStateManager::Impl::SaveConfigData( const GameConfigData& data )
	{
		std::string fileName = CONFIG_FILE_DIR;
		fileName += '/';
		fileName += CONFIG_FILE_NAME;

		m_ConfigDataHolder.SetBGMVolume( data.m_BGMVolume );
		m_ConfigDataHolder.SetSEVolume( data.m_SEVolume );
		m_ConfigDataHolder.SetPlaySpeed( data.m_PlaySpeed );
		for( int i = 0; i < GENERAL_BUTTON_TOTAL; ++i ){
			m_ConfigDataHolder.SetKeyboardCaps( i , data.m_KeyboardCaps[ i ] );
		}
		m_ConfigDataHolder.Save( fileName );
	}

	GameConfigData GameStateManager::Impl::GetConfigData() const
	{
		GameConfigData data;
		
		data.m_BGMVolume = m_ConfigDataHolder.GetBGMVolume();
		data.m_SEVolume = m_ConfigDataHolder.GetSEVolume();
		data.m_PlaySpeed = m_ConfigDataHolder.GetPlaySpeed();
		for( int i = 0; i < GENERAL_BUTTON_TOTAL; ++i ){
			data.m_KeyboardCaps[ i ] = m_ConfigDataHolder.GetKeyboradCaps( i );
		}

		return data;
	}

	void GameStateManager::Impl::LoadConfigData()
	{
		std::string fileName = CONFIG_FILE_DIR;
		fileName += '/';
		fileName += CONFIG_FILE_NAME;
		
		m_ConfigDataHolder.Load( fileName );
	}

	DisplayedNormalPlayStat GameStateManager::Impl::GetDisplayedNormalPlayStat() const
	{
		DisplayedNormalPlayStat stat;
		for( int i = 0; i < GAME_DIFFICULTY_TOTAL; ++i ){
			stat.m_HIScore[ i ] = m_GameDataHolder.GetHIScore( i );
			stat.m_Stat[ i ] = m_GameDataHolder.GetNormalPlayStat( i );
			//stat.m_Stat[ i ].m_Play = m_GameDataHolder.GetPlayCount( i );
			//stat.m_Stat[ i ].m_AllClear = m_GameDataHolder.GetAllClearCount( i );
			//stat.m_Stat[ i ].m_PlayTime = m_GameDataHolder.GetPlayTime( i );
			//stat.m_Stat[ i ].m_Progress = m_GameDataHolder.GetProgress( i );
		}

		return stat;
	}

	DisplayedStageSelectionPlayStat GameStateManager::Impl::GetDisplayedStageSelectionPlayStat() const
	{
		DisplayedStageSelectionPlayStat stat;

		for( int i = 0; i < GAME_DIFFICULTY_TOTAL; ++i ){
			stat.m_Stat[ i ] = m_GameDataHolder.GetStageSelectionPlayStat( i );
		}

		return stat;
	}

	GameStat GameStateManager::Impl::GetGameStat() const
	{
		GameStat stat;
		for( int i = 0; i < GAME_DIFFICULTY_TOTAL; ++i ){
			stat.m_Difficulty[ i ].m_NormalPlayStat = m_GameDataHolder.GetNormalPlayStat( i );
			stat.m_Difficulty[ i ].m_StageSelPlayStat = m_GameDataHolder.GetStageSelectionPlayStat( i );
		}

		return stat;
	}

	void GameStateManager::Impl::SetGameStat( const GameStat& stat )
	{
		for( int i = 0; i < GAME_DIFFICULTY_TOTAL; ++i ){
			m_GameDataHolder.SetNormalPlayStat( i, stat.m_Difficulty[ i ].m_NormalPlayStat );
			m_GameDataHolder.SetStageSelectionPlayStat( i, stat.m_Difficulty[ i ].m_StageSelPlayStat );
		}
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

	int GameStateManager::GetHIScore( int difficulty ) const
	{
		return m_pImpl->GetHIScore( difficulty );
	}

	DisplayedReplayInfo GameStateManager::GetDisplayedReplayInfo() const
	{
		return m_pImpl->GetDisplayedReplayInfo();
	}

	void GameStateManager::SaveReplay(  const ReplayDataRecord& record )
	{
		m_pImpl->SaveReplay( record );
	}

	void GameStateManager::LoadGameData()
	{
		m_pImpl->LoadGameData();
	}

	InitialGameData GameStateManager::GetReplayIniData( int replayNo, int stageNo )
	{
		return m_pImpl->GetReplayIniData( replayNo, stageNo );
	}

	int GameStateManager::GetReplayGameDifficulty( int replayNo )
	{
		return m_pImpl->GetReplayGameDifficulty( replayNo );
	}

	void GameStateManager::SaveConfigData( const GameConfigData& data )
	{
		m_pImpl->SaveConfigData( data );
	}

	GameConfigData GameStateManager::GetConfigData() const
	{
		return m_pImpl->GetConfigData();
	}

	void GameStateManager::LoadConfigData()
	{
		m_pImpl->LoadConfigData();
	}

	DisplayedNormalPlayStat GameStateManager::GetDisplayedNormalPlayStat() const
	{
		return m_pImpl->GetDisplayedNormalPlayStat();
	}

	DisplayedStageSelectionPlayStat GameStateManager::GetDisplayedStageSelectionPlayStat() const
	{
		return m_pImpl->GetDisplayedStageSelectionPlayStat();
	}

	GameStat GameStateManager::GetGameStat() const
	{
		return m_pImpl->GetGameStat();
	}

	void GameStateManager::SetGameStat( const GameStat& stat )
	{
		m_pImpl->SetGameStat( stat );
	}
}
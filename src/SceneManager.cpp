#include "SceneManager.h"

#include <time.h>

#include "SceneBuilder.h"
#include "Scene.h"
#include "Stage.h"
#include "Config.h"
#include "Score.h"
#include "Replay.h"
#include "DifficultySelection.h"
#include "StageSelection.h"
#include "Menu.h"
#include "Initialize.h"
#include "ScoreEntry.h"
#include "ReplayEntry.h"

#include "SceneTypes.h"

#include "EventMediator.h"
#include "EventTypes.h"

#include "ResourceID.h"

#include "Timer.h"

#include "Util.h"

namespace GameEngine
{
	// SceneManager実装クラス
	class SceneManager::Impl
	{
		std::tr1::shared_ptr < SceneBuilder >	m_pSceneBuilder;		// シーン構築クラス
		std::auto_ptr < Scene >					m_pCurScene;			// 現在のシーン
		std::auto_ptr < Scene >					m_pNextScene;			// 次のシーン
		std::weak_ptr < EventMediator >			m_pEventMediator;		// イベント仲介役クラス
		ResourceMap								m_ResourceMap;			// リソース一覧
		ScriptData								m_ScriptData;			// スクリプトデータ
		SceneType								m_CurSceneType;			// 現在のシーン
		

		// 現時点のゲームプレイに関する情報
		SaveDataRecord							m_SaveDataRecord;		// 現在プレイ中の記録
		ReplayDataRecord						m_ReplayDataRecord;		// 現在プレイ中のリプレイデータ
		GameStat								m_GameStat;				// ゲームの統計情報

		// シーン表示に必要な情報
		DisplayedSaveData						m_SaveData;				// セーブデータ
		DisplayedReplayInfo						m_DisplayedReplayInfo;	// 表示用リプレイ情報
		DisplayedReplayInfo::Entry				m_ReplayInfo;			// 現在プレイ中の情報
		DisplayedNormalPlayStat					m_DispNormalPlayStat;	// 表示用の通常プレイ情報
		DisplayedStageSelectionPlayStat			m_DispStagePlayStat;	// 表示用のステージ選択プレイ情報

		// コンフィグデータ
		GameConfigData							m_ConfigData;			// 設定データ

		int										m_GameDifficulty;		// 難易度
		int										m_CurStage;				// 現在のステージ
		int										m_RecordRank;			// スコアのランク
		int										m_GameMode;				// ゲームモード
		int										m_ReplayNo;				// リプレイエントリ番号

		int										m_Counter;				// カウンタ

		InitialGameData							m_IniGameData;			// 初期のゲームデータ
		int										m_HIScore;				// ハイスコア

		bool									m_Paused;				// ポーズ状態か？

		bool									m_IsFirstStage;			// 最初のステージか？

		Timer									m_PlayTime;				// プレイ時間

		void SaveStageScoreData( int stageNo, const Stage& stage );	// ステージのスコアを保存
		void SaveStageReplayData( int stageNo, const Stage& stage );	// ステージのリプレイ情報を保存
		void SaveStageReplayData( int stageNo, const InitialGameData& data );
		void SetupInitialData( const Stage& stage );
		void PrepareScoreEntry();											// スコアエントリ状態遷移のための準備
		void PrepareReplayEntry();											// リプレイエントリ状態遷移のための準備
		void DrawLoading();													// ローディング状態の描画
		void InitializeIniGameData();										// 初期ゲームデータを初期化
	public:
		Impl( std::shared_ptr < EventMediator > pEventMediator );
		~Impl(){}
		void Draw();
		SceneType Update();
		void AttachSceneResourceMap( const ResourceMap& map );
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachScriptData( const ScriptData& data );
		void AttachDisplayedSaveData( const DisplayedSaveData& data );
		void AttachDisplayedReplayInfo( const DisplayedReplayInfo& info );
		void AttachDisplayedNormalPlayStat( const DisplayedNormalPlayStat& stat );
		void AttachDisplayedStageSelectionPlayStat( const DisplayedStageSelectionPlayStat& stat );
		void AttachGameStat( const GameStat& stat );
		void AttachConfigData( const GameConfigData& data );
		void AttachInitialGameData( const InitialGameData& data );
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

	SceneManager::Impl::Impl( std::shared_ptr < EventMediator > pEventMediator ) :	m_pSceneBuilder( new SceneBuilder ),
																					m_pEventMediator( pEventMediator ),
																					m_PlayTime()
	{
		m_pCurScene.reset( m_pSceneBuilder->CreateNextScene( SCENE_TYPE_UNKNOWN ) );
		m_pNextScene.reset();
		m_CurSceneType = SCENE_TYPE_UNKNOWN;
		m_GameDifficulty = GAME_DIFFICULTY_EASY;
		m_CurStage = 0;
		MAPIL::ZeroObject( &m_SaveDataRecord, sizeof( m_SaveDataRecord ) );

		MAPIL::ZeroObject( &m_ReplayDataRecord.m_StageDataInfo, sizeof( m_ReplayDataRecord.m_StageDataInfo ) );
		for( int i = 0; i < STAGE_TOTAL; ++i ){
			m_ReplayDataRecord.m_StageKeyStatusList[ i ].m_StatusList.clear();
		}
		m_ReplayDataRecord.m_Crystal = 0;
		MAPIL::ZeroObject( &m_ReplayDataRecord.m_Date, sizeof( m_ReplayDataRecord.m_Date ) );
		m_ReplayDataRecord.m_Difficulty = 0;
		m_ReplayDataRecord.m_Killed = 0;
		MAPIL::ZeroObject( &m_ReplayDataRecord.m_Name, sizeof( m_ReplayDataRecord.m_Name ) );
		m_ReplayDataRecord.m_Progress = 0;
		m_ReplayDataRecord.m_Score = 0;
		m_ReplayDataRecord.m_EntryNo = 0;
		
		MAPIL::ZeroObject( &m_DisplayedReplayInfo, sizeof( m_DisplayedReplayInfo ) );
		MAPIL::ZeroObject( &m_ReplayInfo, sizeof( m_ReplayInfo ) );
		MAPIL::ZeroObject( &m_ConfigData, sizeof( m_ConfigData ) );
		m_GameMode = GAME_MODE_NORMAL;
		m_Counter = 0;
		m_ReplayNo = 0;

		m_Paused = false;
		m_IsFirstStage = false;
	}

	void SceneManager::Impl::PrepareScoreEntry()
	{
		if( m_CurSceneType == SCENE_TYPE_STAGE ){
			// ゲーム到達度の保存
			m_SaveDataRecord.m_Progress = m_CurStage;
			// 最終的なスコアを算出
			for( int i = 0; i < m_CurStage; ++i ){
				m_SaveDataRecord.m_Crystal += m_SaveDataRecord.m_StageData[ i ].m_Crystal;
				m_SaveDataRecord.m_Killed += m_SaveDataRecord.m_StageData[ i ].m_Killed;
				m_SaveDataRecord.m_Score += m_SaveDataRecord.m_StageData[ i ].m_Score;
				m_SaveDataRecord.m_CrystalUsed += m_SaveDataRecord.m_StageData[ i ].m_CrystalUsed;
				::time_t t;
				::time( &t );
				::tm* time = localtime( &t );
				m_SaveDataRecord.m_Date.m_Year = time->tm_year + 1900;
				m_SaveDataRecord.m_Date.m_Month = time->tm_mon + 1;
				m_SaveDataRecord.m_Date.m_Day = time->tm_mday;
				m_SaveDataRecord.m_Date.m_Hour = time->tm_hour;
				m_SaveDataRecord.m_Date.m_Min = time->tm_min;
				m_SaveDataRecord.m_Date.m_Sec = time->tm_sec;
			}
		}
	}

	void SceneManager::Impl::SetupInitialData( const Stage& stage )
	{
		// 次のステージのために初期データを取得
		m_IniGameData.m_Score = stage.GetScore();
		m_IniGameData.m_Killed = stage.GetKilled();
		m_IniGameData.m_Crystal = stage.GetCrystal();
		m_IniGameData.m_CrystalUsed = stage.GetCrystalUsed();
		m_IniGameData.m_Progress = stage.GetProgress();
		for( int i = 0; i < m_CurStage - 1; ++i ){
			m_IniGameData.m_Score += m_SaveDataRecord.m_StageData[ i ].m_Score;
			m_IniGameData.m_Killed += m_SaveDataRecord.m_StageData[ i ].m_Killed;
			m_IniGameData.m_Crystal += m_SaveDataRecord.m_StageData[ i ].m_Crystal;
			m_IniGameData.m_CrystalUsed += m_SaveDataRecord.m_StageData[ i ].m_CrystalUsed;
		}
		stage.GetPlayerPos( &m_IniGameData.m_PosX, &m_IniGameData.m_PosY );
		m_IniGameData.m_HP = stage.GetPlayerHP();
		m_IniGameData.m_ShotPower = stage.GetPlayerShotPower();
		m_IniGameData.m_Cons = stage.GetPlayerCons();
		stage.GetPlayerConsGauge( m_IniGameData.m_ConsGauge );
		stage.GetPlayerConsLevel( m_IniGameData.m_ConsLevel );
	}

	void SceneManager::Impl::PrepareReplayEntry()
	{
		// ゲーム到達度の保存
		m_ReplayDataRecord.m_Progress = m_SaveDataRecord.m_Progress;
		// 総合スコアの記録
		m_ReplayDataRecord.m_Crystal = m_SaveDataRecord.m_Crystal;
		m_ReplayDataRecord.m_CrystalUsed = m_SaveDataRecord.m_CrystalUsed;
		m_ReplayDataRecord.m_Date = m_SaveDataRecord.m_Date;
		m_ReplayDataRecord.m_Difficulty = m_GameDifficulty;
		m_ReplayDataRecord.m_Killed = m_SaveDataRecord.m_Killed;
		m_ReplayDataRecord.m_Score = m_SaveDataRecord.m_Score;
	}

	void SceneManager::Impl::SaveStageScoreData( int stageNo, const Stage& stage )
	{
		m_SaveDataRecord.m_StageData[ stageNo - 1 ].m_Crystal = stage.GetCrystal();
		m_SaveDataRecord.m_StageData[ stageNo - 1 ].m_CrystalUsed = stage.GetCrystalUsed();
		m_SaveDataRecord.m_StageData[ stageNo - 1 ].m_Killed = stage.GetKilled();
		m_SaveDataRecord.m_StageData[ stageNo - 1 ].m_Score = stage.GetScore();
		m_SaveDataRecord.m_StageData[ stageNo - 1 ].m_Progress = stage.GetProgress();
	}

	void SceneManager::Impl::SaveStageReplayData( int stageNo, const Stage& stage )
	{
		// キーの状態のみ、ステージ番号-1に設定。
		m_ReplayDataRecord.m_StageKeyStatusList[ stageNo - 1 ] = stage.GetKeyStates();
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniCons = stage.GetPlayerCons();
		stage.GetPlayerConsGauge( m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniConsGauge );
		stage.GetPlayerConsLevel( m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniConsLevel );
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniCrystal = stage.GetCrystal();
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniCrystalUsed = stage.GetCrystalUsed();
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniHP = stage.GetPlayerHP();
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniKilled = stage.GetKilled();
		stage.GetPlayerPos(	&m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniPosX,
							&m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniPosY );
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniScore = stage.GetScore();
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniShotPower = stage.GetPlayerShotPower();
	}

	void SceneManager::Impl::SaveStageReplayData( int stageNo, const InitialGameData& data )
	{
		if( stageNo != 0 ){
			throw new MAPIL::MapilException( CURRENT_POSITION, TSTR( "Invalid stage no." ), -1 );
		}

		// 初期データの保存
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniCons = data.m_Cons;
		::memcpy( &m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniConsGauge, &data.m_ConsGauge, sizeof( data.m_ConsGauge ) );
		::memcpy( &m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniConsLevel, &data.m_ConsLevel, sizeof( data.m_ConsLevel ) );
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniCrystal = data.m_Crystal;
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniCrystalUsed = data.m_CrystalUsed;
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniHP = data.m_HP;
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniKilled = data.m_Killed;
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniPosX = data.m_PosX;
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniPosY = data.m_PosY;
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniScore = data.m_Score;
		m_ReplayDataRecord.m_StageDataInfo[ stageNo ].m_IniShotPower = data.m_Score;
	}

	void SceneManager::Impl::InitializeIniGameData()
	{
		MAPIL::ZeroObject( &m_IniGameData, sizeof( m_IniGameData ) );
		
		m_IniGameData.m_PosX = 300;
		m_IniGameData.m_PosY = 400;
		m_IniGameData.m_HP = 10;
		//m_IniGameData.m_HP = 30;
		for( int i = 0; i < 3; ++i ){
			m_IniGameData.m_ConsGauge[ i ] = 1000;
			m_IniGameData.m_ConsLevel[ i ] = 1000;
		};
		m_IniGameData.m_ShotPower = 0;
		//m_IniGameData.m_ShotPower = 40;
		/*m_IniGameData.m_ShotPower = 40;
		m_IniGameData.m_Crystal = 54000;
		m_IniGameData.m_CrystalUsed = 3000;
		m_IniGameData.m_Killed = 2500;
		m_IniGameData.m_Score = 23456578;*/

		m_IniGameData.m_Cons = 0;			// PLAYER_CONS_NORMAL
#if defined ( MAKE_MODE_DEBUG )
		m_IniGameData.m_HP = 1;
		//m_IniGameData.m_ShotPower = 40;
		/*m_IniGameData.m_Crystal = 54000;
		m_IniGameData.m_CrystalUsed = 3000;
		m_IniGameData.m_Killed = 2500;
		m_IniGameData.m_Score = 23456578;*/
#endif
	}

	void SceneManager::Impl::DrawLoading()
	{
		MAPIL::BeginRendering2DGraphics();
		if( ( m_Counter % 30 ) < 15 ){
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_LOADING_1 ],
								400.0f, 300.0f, 0.7f, 0.7f, false, ( ( m_Counter % 30 ) * 4 + 130 ) << 24 | 0xFFFFFF );
		}
		else{
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_LOADING_1 ],
								400.0f, 300.0f, 0.7f, 0.7f, false, ( ( 30 - m_Counter % 30 ) * 4 + 130 ) << 24 | 0xFFFFFF );
		}
		if( ( m_Counter % 40 ) < 10 ){
			MAPIL::DrawTexture( m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_LOADING_2 ],
								400.0f + ( m_Counter % 20 ) * 10.0f, 420.0f, 1.0f, 0.5f,
								true, 0xFF0000FF | ( ( m_Counter % 10 ) * 255 / 10 ) << 16 | ( ( m_Counter % 10 ) * 255 / 10 ) << 8 );
			MAPIL::DrawTexture( m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_LOADING_2 ],
								600.0f - ( m_Counter % 20 ) * 10.0f, 350.0f, 1.0f, 0.5f,
								true, 0xFF0000FF | ( ( m_Counter % 10 ) * 255 / 10 ) << 16 | ( ( m_Counter % 10 ) * 255 / 10 ) << 8 );
		}
		else if( ( m_Counter % 40 ) < 20 ){
			MAPIL::DrawTexture( m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_LOADING_2 ],
								400.0f + ( m_Counter % 20 ) * 10.0f, 420.0f, 1.0f, 0.5f,
								true, 0xFF0000FF | ( ( 10 - m_Counter % 10 ) * 255 / 10 ) << 16 | ( ( 10 - m_Counter % 10 ) * 255 / 10 ) << 8 );
			MAPIL::DrawTexture( m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_LOADING_2 ],
								600.0f - ( m_Counter % 20 ) * 10.0f, 350.0f, 1.0f, 0.5f,
								true, 0xFF0000FF | ( ( 10 - m_Counter % 10 ) * 255 / 10 ) << 16 | ( ( 10 - m_Counter % 10 ) * 255 / 10 ) << 8 );
		}
		MAPIL::EndRendering2DGraphics();
	}

	void SceneManager::Impl::Draw()
	{
		m_pCurScene->Draw();
		if( m_CurSceneType == SCENE_TYPE_LOADING ){
			DrawLoading();	
		}

		++m_Counter;
	}

	SceneType SceneManager::Impl::Update()
	{
		static bool prevPaused = false;

		// シーン更新
		SceneType next = m_pCurScene->Update();
		if( next == SCENE_TYPE_PAUSED ){
			m_Paused = true;
		}
		else{
			m_Paused = false;
		}

		if( prevPaused != m_Paused ){
			if( m_Paused ){
				m_PlayTime.Pause();
			}
			else{
				m_PlayTime.Resume();
			}
		}

		prevPaused = m_Paused;


		// シーン遷移
		if( next != SCENE_TYPE_NOT_CHANGE && m_CurSceneType != SCENE_TYPE_LOADING && next != SCENE_TYPE_PAUSED ){
			if( std::shared_ptr < EventMediator > p = m_pEventMediator.lock() ){
				if( next == SCENE_TYPE_MENU ){
					m_GameMode = GAME_MODE_NORMAL;
					if( m_CurSceneType == SCENE_TYPE_REPLAY_ENTRY ){
						p->SendEvent( EVENT_TYPE_MOVE_TO_MENU_FROM_REPLAY_ENTRY );
					}
					else if( m_CurSceneType == SCENE_TYPE_CONFIG ){
						Config* pScene = dynamic_cast < Config* > ( m_pCurScene.get() );
						if( pScene ){
							m_ConfigData = pScene->GetConfigData();
						}
						else{
							throw MAPIL::MapilException( CURRENT_POSITION, TSTR( "Called from invalid scene." ), -1 );
						}
						p->SendEvent( EVENT_TYPE_MOVE_TO_MENU_FROM_CONFIG );
					}
					else if( m_CurSceneType == SCENE_TYPE_INITIALIZE ){
						p->SendEvent( EVENT_TYPE_MOVE_TO_MENU_FROM_INITIALIZE );
					}
					else{
						p->SendEvent( EVENT_TYPE_MOVE_TO_MENU );
					}
				}
				else if( next == SCENE_TYPE_STAGE_SELECTION ){
					m_GameMode = GAME_MODE_ONE_STAGE;
					p->SendEvent( EVENT_TYPE_MOVE_TO_STAGE_SELECTION );
				}
				else if( next == SCENE_TYPE_DIFFICULTY_SELECTION ){
					m_GameMode = GAME_MODE_NORMAL;
					p->SendEvent( EVENT_TYPE_MOVE_TO_DIFFICULTY_SELECTION );
				}
				else if( next == SCENE_TYPE_STAGE ){
					if( m_CurSceneType == SCENE_TYPE_REPLAY ){
						m_GameMode = GAME_MODE_REPLAY;
						m_IsFirstStage = true;
						Replay* pp = dynamic_cast < Replay* > ( m_pCurScene.get() );
						if( pp ){
							m_CurStage = pp->GetReplayStage() + 1;
							m_ReplayNo = pp->GetReplayNo();
							// 初期データの設定
							InitializeIniGameData();
							p->SendEvent( EVENT_TYPE_MOVE_TO_STAGE, &m_CurStage );
						}
						else{
							throw MAPIL::MapilException( CURRENT_POSITION, TSTR( "Called from invalid scene." ), -1 );
						}
					}
					else{
						if( m_CurSceneType == SCENE_TYPE_DIFFICULTY_SELECTION ){
							DifficultySelection* pScene = dynamic_cast < DifficultySelection* > ( m_pCurScene.get() );
							if( pScene ){
								m_GameDifficulty = pScene->GetDifficulty();
							}
							m_GameMode = GAME_MODE_NORMAL;
							m_IsFirstStage = true;
							m_CurStage = 1;
							// プレイ時間の記録を開始
							m_PlayTime.Init();
							m_PlayTime.Start();
							// 統計情報の更新
							m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_Play += 1;
							m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_Play += 1;
							// 初期データの設定
							InitializeIniGameData();
							SaveStageReplayData( m_CurStage - 1, m_IniGameData );
							p->SendEvent( EVENT_TYPE_MOVE_TO_STAGE, &m_CurStage );
						}
						else if( m_CurSceneType == SCENE_TYPE_STAGE_SELECTION ){
							m_GameMode = GAME_MODE_ONE_STAGE;
							m_IsFirstStage = true;
							StageSelection* pScene = dynamic_cast < StageSelection* > ( m_pCurScene.get() );
							if( pScene ){
								m_CurStage = pScene->GetStageNo();
								m_GameDifficulty = pScene->GetDifficulty();
							}
							else{
								throw MAPIL::MapilException( CURRENT_POSITION, TSTR( "Called from invalid scene." ), -1 );
							}
							// プレイ時間の記録を開始
							m_PlayTime.Init();
							m_PlayTime.Start();
							// 統計情報の更新
							m_GameStat.m_Difficulty[ m_GameDifficulty ].m_StageSelPlayStat.m_Play += 1;
							m_GameStat.m_Difficulty[ m_GameDifficulty ].m_StageSelPlayStat.m_StageStat[ m_CurStage - 1 ].m_Play += 1;
							// 初期データの設定
							InitializeIniGameData();
							// ※ステージごとにリプレイを保存する場合、バグが生じる↓
							//SaveStageReplayData( m_CurStage - 1, m_IniGameData );
							p->SendEvent( EVENT_TYPE_MOVE_TO_STAGE, &m_CurStage );
						}
						else if( m_CurSceneType == SCENE_TYPE_STAGE ){
							m_IsFirstStage = false;
							Stage* pStage = dynamic_cast < Stage* > ( m_pCurScene.get() );
							if( pStage == NULL ){
								throw new MAPIL::MapilException( CURRENT_POSITION, TSTR( "Called from invalid scene." ), -1 );
							}

							//-------------------------------
							// 統計情報の更新
							//-------------------------------
							// プレイ時間の更新
							m_PlayTime.Stop();
							if( m_GameMode == GAME_MODE_NORMAL ){
								m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_PlayTime += m_PlayTime.GetInterval();
								m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_Clear += 1;
							}
							// プレイ時間の記録
							if( m_CurStage == 1 ){
								m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_PlayTime += m_PlayTime.GetInterval();
							}
							else{
								m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_PlayTime +=
									m_PlayTime.GetInterval() -
									m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 2 ].m_PlayTime;
							}
							// 意識時間の取得
							for( int i = 0; i < NUM_OF( m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_ConsTime ); ++i ){
								m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_ConsTime[ i ] += pStage->GetStageStat().m_ConsTime[ i ];
							}
							// 敵情報の更新
							StageStat& stat = pStage->GetStageStat();
							for(	auto it = stat.m_EnemyStat.begin();
									it != stat.m_EnemyStat.end();
									++it ){
								if( m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_EnemyStat.find( it->first )
									== m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_EnemyStat.end() ){
									EnemyStat s;
									m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_EnemyStat[ it->first ] = s;
								}
								m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_EnemyStat[ it->first ] += stat.m_EnemyStat[ it->first ];
							}


							// 初期データの構築
							SetupInitialData( *pStage );
							// ステージ別のスコア情報を保存
							SaveStageScoreData( m_CurStage, *pStage );
							// ステージ別のリプレイ情報を保存
							SaveStageReplayData( m_CurStage, *pStage );
							// 次のステージ番号を取得し、遷移する
							m_CurStage = pStage->GetNextStageNo();
							// 統計情報の更新
							m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_Play += 1;
							if( m_GameMode == GAME_MODE_NORMAL || m_GameMode == GAME_MODE_REPLAY ){
								p->SendEvent( EVENT_TYPE_MOVE_TO_NEXT_STAGE, &m_CurStage );
							}
							else{
								p->SendEvent( EVENT_TYPE_MOVE_TO_MENU );
							}
						}
					}
				}
				else if( next == SCENE_TYPE_CONFIG ){
					p->SendEvent( EVENT_TYPE_MOVE_TO_CONFIG );
				}
				else if( next == SCENE_TYPE_SCORE ){
					p->SendEvent( EVENT_TYPE_MOVE_TO_SCORE );
				}
				else if( next == SCENE_TYPE_REPLAY ){
					p->SendEvent( EVENT_TYPE_MOVE_TO_REPLAY );
				}
				else if( next == SCENE_TYPE_SCORE_ENTRY ){
					// 通常プレイ時
					if( m_GameMode == GAME_MODE_NORMAL ){
						// ステージからの遷移であることを確認
						Stage* pStage = dynamic_cast < Stage* > ( m_pCurScene.get() );
						if( pStage == NULL ){
							throw MAPIL::MapilException( CURRENT_POSITION, TSTR( "Called from invalid scene." ), -1 );
						}

						//-------------------------------
						// 統計情報の更新
						//-------------------------------
						// 全体情報の更新
						// プレイ時間の記録
						m_PlayTime.Stop();
						m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_PlayTime += m_PlayTime.GetInterval();
						// ステージ個別情報の更新
						// プレイ時間の記録
						if( m_CurStage == 1 ){
							m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_PlayTime += m_PlayTime.GetInterval();
						}
						else{
							m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_PlayTime +=
								m_PlayTime.GetInterval() -
								m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 2 ].m_PlayTime;
						}
						// 意識時間の取得
						for( int i = 0; i < NUM_OF( m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_ConsTime ); ++i ){
							m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_ConsTime[ i ] += pStage->GetStageStat().m_ConsTime[ i ];
						}
						// 敵情報の更新
						StageStat& stat = pStage->GetStageStat();
						for(	auto it = stat.m_EnemyStat.begin();
								it != stat.m_EnemyStat.end();
								++it ){
							if( m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_EnemyStat.find( it->first )
								== m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_EnemyStat.end() ){
								EnemyStat s;
								m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_EnemyStat[ it->first ] = s;
							}
							m_GameStat.m_Difficulty[ m_GameDifficulty ].m_NormalPlayStat.m_StageStat[ m_CurStage - 1 ].m_EnemyStat[ it->first ] += stat.m_EnemyStat[ it->first ];
						}
						
						//-------------------------------
						// 最終ステージの結果を更新
						//-------------------------------
						// 初期データの構築
						SetupInitialData( *pStage );
						// ステージ別のスコア情報を保存
						SaveStageScoreData( m_CurStage, *pStage );
						PrepareScoreEntry();
						// ステージ別のリプレイ情報を保存
						SaveStageReplayData( m_CurStage, *pStage );
						PrepareReplayEntry();
						p->SendEvent( EVENT_TYPE_MOVE_TO_SCORE_ENTRY );
					}
					else if( m_GameMode == GAME_MODE_ONE_STAGE ){
						// ステージからの遷移であることを確認
						Stage* pStage = dynamic_cast < Stage* > ( m_pCurScene.get() );
						if( pStage == NULL ){
							throw MAPIL::MapilException( CURRENT_POSITION, TSTR( "Called from invalid scene." ), -1 );
						}

						//-------------------------------
						// 統計情報の更新
						//-------------------------------
						// 全体情報の更新
						// プレイ時間の記録
						m_PlayTime.Stop();
						m_GameStat.m_Difficulty[ m_GameDifficulty ].m_StageSelPlayStat.m_PlayTime += m_PlayTime.GetInterval();
						// ステージ個別情報の更新
						// プレイ時間の記録
						m_GameStat.m_Difficulty[ m_GameDifficulty ].m_StageSelPlayStat.m_StageStat[ m_CurStage - 1 ].m_PlayTime += m_PlayTime.GetInterval();
						// 意識時間の取得
						for( int i = 0; i < NUM_OF( m_GameStat.m_Difficulty[ m_GameDifficulty ].m_StageSelPlayStat.m_StageStat[ m_CurStage - 1 ].m_ConsTime ); ++i ){
							m_GameStat.m_Difficulty[ m_GameDifficulty ].m_StageSelPlayStat.m_StageStat[ m_CurStage - 1 ].m_ConsTime[ i ] += pStage->GetStageStat().m_ConsTime[ i ];
						}
						// 敵情報の更新
						StageStat& stat = pStage->GetStageStat();
						for(	auto it = stat.m_EnemyStat.begin();
								it != stat.m_EnemyStat.end();
								++it ){
							if( m_GameStat.m_Difficulty[ m_GameDifficulty ].m_StageSelPlayStat.m_StageStat[ m_CurStage - 1 ].m_EnemyStat.find( it->first )
								== m_GameStat.m_Difficulty[ m_GameDifficulty ].m_StageSelPlayStat.m_StageStat[ m_CurStage - 1 ].m_EnemyStat.end() ){
								EnemyStat s;
								m_GameStat.m_Difficulty[ m_GameDifficulty ].m_StageSelPlayStat.m_StageStat[ m_CurStage - 1 ].m_EnemyStat[ it->first ] = s;
							}
							m_GameStat.m_Difficulty[ m_GameDifficulty ].m_StageSelPlayStat.m_StageStat[ m_CurStage - 1 ].m_EnemyStat[ it->first ] += stat.m_EnemyStat[ it->first ];
						}


						// イベント送信
						p->SendEvent( EVENT_TYPE_MOVE_TO_MENU_FROM_SCORE_ENTRY );
					}
					else if( m_GameMode == GAME_MODE_REPLAY ){
						p->SendEvent( EVENT_TYPE_MOVE_TO_MENU );
					}
				}
				else if( next == SCENE_TYPE_REPLAY_ENTRY ){
					if( typeid( *m_pCurScene.get() ) == typeid( ScoreEntry ) ){
						m_SaveDataRecord = ( (ScoreEntry*) m_pCurScene.get() )->GetRecord();
						p->SendEvent( EVENT_TYPE_MOVE_TO_REPLAY_ENTRY_FROM_SCORE_ENTRY );
					}
					else if( typeid( *m_pCurScene.get() ) == typeid( ReplayEntry ) ){
						m_ReplayDataRecord = ( (ReplayEntry*) m_pCurScene.get() )->GetReplayDataRecord();
						p->SendEvent( EVENT_TYPE_MOVE_TO_REPLAY_ENTRY_FROM_SELF );
						if( typeid( *m_pCurScene.get() ) == typeid( ReplayEntry ) ){
							( (ReplayEntry*) m_pCurScene.get() )->AttachDisplayedReplayInfo( m_DisplayedReplayInfo );
							( (ReplayEntry*) m_pCurScene.get() )->AttachReplayDataRecord( m_ReplayDataRecord );
							m_CurSceneType = SCENE_TYPE_REPLAY;
						}
					}
					else{
						exit( 1 );
					}
					
					
				}
				else if( next == SCENE_TYPE_GAME_TERM ){
					p->SendEvent( EVENT_TYPE_GAME_TERM );
				}
			}
			else{
				exit( 1 );
			}
		}

		return SCENE_TYPE_NOT_CHANGE;
	}

	void SceneManager::Impl::AttachSceneResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	void SceneManager::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_pCurScene->AttachButtonState( pHolder );
	}

	void SceneManager::Impl::AttachDisplayedReplayInfo( const DisplayedReplayInfo& info )
	{
		m_DisplayedReplayInfo = info;
	}

	void SceneManager::Impl::AttachConfigData( const GameConfigData& data )
	{
		m_ConfigData = data;
	}

	void SceneManager::Impl::AttachInitialGameData( const InitialGameData& data )
	{
		m_IniGameData = data;
	}

	void SceneManager::Impl::ChangeScene( SceneType scene )
	{
		m_pNextScene.reset( m_pSceneBuilder->CreateNextScene( scene ) );
		m_CurSceneType = SCENE_TYPE_LOADING;
	}

	void SceneManager::Impl::AttachScriptData( const ScriptData& data )
	{
		m_ScriptData = data;
	}

	void SceneManager::Impl::AttachDisplayedSaveData( const DisplayedSaveData& data )
	{
		m_SaveData = data;
	}

	void SceneManager::Impl::AttachDisplayedNormalPlayStat( const DisplayedNormalPlayStat& stat )
	{
		m_DispNormalPlayStat = stat;
	}

	void SceneManager::Impl::AttachDisplayedStageSelectionPlayStat( const DisplayedStageSelectionPlayStat& stat )
	{
		m_DispStagePlayStat = stat;
	}

	void SceneManager::Impl::AttachGameStat( const GameStat& stat )
	{
		m_GameStat = stat;
	}

	SceneType SceneManager::Impl::GetCurSceneType() const
	{
		return m_CurSceneType;
	}

	void SceneManager::Impl::SetGameDifficulty( int difficulty )
	{
		m_GameDifficulty = difficulty;
	}

	const DisplayedSaveData& SceneManager::Impl::GetDisplayedSaveData() const
	{
		return m_SaveData;
	}

	void SceneManager::Impl::SetRecordRank( int rank )
	{
		m_RecordRank = rank;
	}

	void SceneManager::Impl::SetHIScore( int score )
	{
		m_IniGameData.m_HIScore = score;
	}

	const SaveDataRecord& SceneManager::Impl::GetSaveDataRecord() const
	{
		return m_SaveDataRecord;
	}

	const ReplayDataRecord& SceneManager::Impl::GetReplayDataRecord() const
	{
		return m_ReplayDataRecord;
	}

	const GameConfigData& SceneManager::Impl::GetConfigData() const
	{
		return m_ConfigData;
	}

	const GameStat& SceneManager::Impl::GetGameStat() const
	{
		return m_GameStat;
	}

	int SceneManager::Impl::GetGameDifficulty() const
	{
		return m_GameDifficulty;
	}

	void SceneManager::Impl::ClearGameData()
	{
		MAPIL::ZeroObject( &m_SaveData, sizeof( m_SaveData ) );
		MAPIL::ZeroObject( &m_SaveDataRecord, sizeof( m_SaveDataRecord ) );
		m_RecordRank = -1;
	}

	const DisplayedReplayInfo::Entry& SceneManager::Impl::GetReplayInfo() const
	{
		return m_ReplayInfo;
	}

	int SceneManager::Impl::GetGameMode() const
	{
		return m_GameMode;
	}

	int SceneManager::Impl::GetReplayNo() const
	{
		return m_ReplayNo;
	}

	void SceneManager::Impl::SwitchToNextScene()
	{
		// シーンの保存
		try{
			if( typeid( *m_pCurScene.get() ) == typeid( Menu ) ){
				m_pSceneBuilder->SaveScene( SCENE_TYPE_MENU, m_pCurScene.release() );
			}
			else if( typeid( *m_pCurScene.get() ) == typeid( StageSelection ) ){
				m_pSceneBuilder->SaveScene( SCENE_TYPE_STAGE_SELECTION, m_pCurScene.release() );
			}
			else if( typeid( *m_pCurScene.get() ) == typeid( DifficultySelection ) ){
				m_pSceneBuilder->SaveScene( SCENE_TYPE_DIFFICULTY_SELECTION, m_pCurScene.release() );
			}
		}
		// m_pCurSceneがぬるぽだったら、無視する。
		catch( const std::bad_typeid& e ){
			// 何もしない
			std::string s = e.what();
			s += "\n";
			::OutputDebugStringA( s.c_str() );
		}
		
		// シーン遷移
		m_pCurScene.reset( m_pNextScene.release() );

		if( typeid( *m_pCurScene.get() ) == typeid( Initialize ) ){
			m_CurSceneType = SCENE_TYPE_INITIALIZE;
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( Menu ) ){
			m_CurSceneType = SCENE_TYPE_MENU;
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( Stage ) ){
			( (Stage*) m_pCurScene.get() )->AttachScriptData( m_ScriptData );
			( (Stage*) m_pCurScene.get() )->SetInitialData( m_IniGameData );
			( (Stage*) m_pCurScene.get() )->SetDifficulty( m_GameDifficulty );
			if( m_IsFirstStage ){
				( (Stage*) m_pCurScene.get() )->MarkFirstTime();
			}
			m_CurSceneType = SCENE_TYPE_STAGE;
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( Config ) ){
			( (Config*) m_pCurScene.get() )->SetEventMediator( m_pEventMediator );
			( (Config*) m_pCurScene.get() )->AttachConfigData( m_ConfigData );
			m_CurSceneType = SCENE_TYPE_CONFIG;
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( Score ) ){
			( (Score*) m_pCurScene.get() )->AttachDisplayedSaveData( m_SaveData );
			m_CurSceneType = SCENE_TYPE_SCORE;
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( Replay ) ){
			( (Replay*) m_pCurScene.get() )->AttachDisplayedReplayInfo( m_DisplayedReplayInfo );
			m_CurSceneType = SCENE_TYPE_REPLAY;
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( ScoreEntry ) ){
			( (ScoreEntry*) m_pCurScene.get() )->AttachDisplayedSaveData( m_SaveData );
			( (ScoreEntry*) m_pCurScene.get() )->SetDifficulty( m_GameDifficulty );
			( (ScoreEntry*) m_pCurScene.get() )->AttachRecord( m_SaveDataRecord );
			( (ScoreEntry*) m_pCurScene.get() )->SetRecordRank( m_RecordRank );
			m_CurSceneType = SCENE_TYPE_SCORE_ENTRY;
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( ReplayEntry ) ){
			( (ReplayEntry*) m_pCurScene.get() )->AttachDisplayedReplayInfo( m_DisplayedReplayInfo );
			( (ReplayEntry*) m_pCurScene.get() )->AttachReplayDataRecord( m_ReplayDataRecord );
			m_CurSceneType = SCENE_TYPE_REPLAY_ENTRY;
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( DifficultySelection ) ){
			( (DifficultySelection*) m_pCurScene.get() )->AttachNormalPlayStat( m_DispNormalPlayStat );
			m_CurSceneType = SCENE_TYPE_DIFFICULTY_SELECTION;
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( StageSelection ) ){
			( (StageSelection*) m_pCurScene.get() )->AttachStageSelectionStat( m_DispStagePlayStat );
			m_CurSceneType = SCENE_TYPE_STAGE_SELECTION;
		}
		m_pCurScene->AttachResourceMap( m_ResourceMap );
		m_pCurScene->Init();
	}

	bool SceneManager::Impl::NeedToSwitch() const
	{
		return m_CurSceneType == SCENE_TYPE_LOADING;
	}

	bool SceneManager::Impl::Paused() const
	{
		return m_Paused;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------


	SceneManager::SceneManager( std::shared_ptr < EventMediator > pEventMediator ) : m_pImpl( new SceneManager::Impl( pEventMediator ) )
	{
	}

	SceneManager::~SceneManager()
	{
	}

	void SceneManager::Draw()
	{
		m_pImpl->Draw();
	}

	SceneType SceneManager::Update()
	{
		return m_pImpl->Update();
	}

	void SceneManager::AttachSceneResourceMap( const ResourceMap& map )
	{
		m_pImpl->AttachSceneResourceMap( map );
	}

	void SceneManager::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_pImpl->AttachButtonState( pHolder );
	}

	void SceneManager::ChangeScene( SceneType scene )
	{
		m_pImpl->ChangeScene( scene );
	}

	void SceneManager::AttachScriptData( const ScriptData& data )
	{
		m_pImpl->AttachScriptData( data );
	}

	void SceneManager::AttachDisplayedSaveData( const DisplayedSaveData& data )
	{
		m_pImpl->AttachDisplayedSaveData( data );
	}

	void SceneManager::AttachDisplayedReplayInfo( const DisplayedReplayInfo& info )
	{
		m_pImpl->AttachDisplayedReplayInfo( info );
	}

	void SceneManager::AttachGameStat( const GameStat& stat )
	{
		m_pImpl->AttachGameStat( stat );
	}

	void SceneManager::AttachConfigData( const GameConfigData& data )
	{
		m_pImpl->AttachConfigData( data );
	}

	void SceneManager::AttachInitialGameData( const InitialGameData& data )
	{
		m_pImpl->AttachInitialGameData( data );
	}

	void SceneManager::AttachDisplayedNormalPlayStat( const DisplayedNormalPlayStat& stat )
	{
		m_pImpl->AttachDisplayedNormalPlayStat( stat );
	}

	void SceneManager::AttachDisplayedStageSelectionPlayStat( const DisplayedStageSelectionPlayStat& stat )
	{
		m_pImpl->AttachDisplayedStageSelectionPlayStat( stat );
	}

	const DisplayedSaveData& SceneManager::GetDisplayedSaveData() const
	{
		return m_pImpl->GetDisplayedSaveData();
	}

	SceneType SceneManager::GetCurSceneType() const
	{
		return m_pImpl->GetCurSceneType();
	}

	void SceneManager::SetGameDifficulty( int difficulty )
	{
		m_pImpl->SetGameDifficulty( difficulty );
	}

	void SceneManager::SetRecordRank( int rank )
	{
		m_pImpl->SetRecordRank( rank );
	}

	void SceneManager::SetHIScore( int score )
	{
		m_pImpl->SetHIScore( score );
	}

	const SaveDataRecord& SceneManager::GetSaveDataRecord() const
	{
		return m_pImpl->GetSaveDataRecord();
	}

	const ReplayDataRecord& SceneManager::GetReplayDataRecord() const
	{
		return m_pImpl->GetReplayDataRecord();
	}

	const GameConfigData& SceneManager::GetConfigData() const
	{
		return m_pImpl->GetConfigData();
	}

	const GameStat& SceneManager::GetGameStat() const
	{
		return m_pImpl->GetGameStat();
	}

	int SceneManager::GetGameDifficulty() const
	{
		return m_pImpl->GetGameDifficulty();
	}

	void SceneManager::ClearGameData()
	{
		m_pImpl->ClearGameData();
	}

	const DisplayedReplayInfo::Entry& SceneManager::GetReplayInfo() const
	{
		return m_pImpl->GetReplayInfo();
	}

	int SceneManager::GetGameMode() const
	{
		return m_pImpl->GetGameMode();
	}

	int SceneManager::GetReplayNo() const
	{
		return m_pImpl->GetReplayNo();
	}

	void SceneManager::SwitchToNextScene()
	{
		m_pImpl->SwitchToNextScene();
	}

	bool SceneManager::NeedToSwitch() const
	{
		return m_pImpl->NeedToSwitch();
	}

	bool SceneManager::Paused() const
	{
		return m_pImpl->Paused();
	}
}
#include "EventMediator.h"
#include "EventTypes.h"

#include "SceneManager.h"
#include "SceneTypes.h"

#include "GeneralButtonManager.h"

#include "GameStateManager.h"

#include "ResourceManager.h"
#include "ResourceID.h"

#include "Loading.h"

#include "ScriptManager.h"

#include "GlobalDefinitions.h"

#include "Util.h"

namespace GameEngine
{
	// EventMediator実装クラス
	class EventMediator::Impl
	{
	private:
		std::shared_ptr < SceneManager >			m_pSceneManager;
		std::shared_ptr < GeneralButtonManager >	m_pButtonManager;
		std::shared_ptr < GameStateManager >		m_pGameStateManager;
		std::shared_ptr < ResourceManager >			m_pResourceManager;
		std::shared_ptr < ScriptManager >			m_pScriptManager;

		Loading										m_Loading;			// バックグラウンドでロードするクラス

		float										m_CurFPS;			// 現在のFPS値

		bool										m_HasTermSig;

		void FrameUpdate();

		void LoadGlobalResources();
	public:
		Impl();
		~Impl();
		void SendEvent( int type );
		void SendEvent( int type, void* pArg );
		void SetSceneManager( std::shared_ptr < SceneManager > pSceneManager );
		void SetButtonManager( std::shared_ptr < GeneralButtonManager > pButtonManager );
		void SetGameStateManger( std::shared_ptr < GameStateManager > pGameStateManager );
		void SetResourceManager( std::shared_ptr < ResourceManager > pResourceManager );
		void SetScriptManager( std::shared_ptr < ScriptManager > pScriptManager );
		bool HasTermSig() const;
	};

	EventMediator::Impl::Impl() :	m_HasTermSig( false ),
									m_CurFPS( 60.0f )
	{
		m_Loading.Init();
		m_Loading.CleanupSession();
		m_Loading.Create();
	}

	EventMediator::Impl::~Impl()
	{
		m_Loading.Terminate();
		Sleep( 100 );
	}

	void EventMediator::Impl::FrameUpdate()
	{
		// 3倍速モード
		static bool isHighSpeedMode = false;
		// リプレイモード時のみ、高速モード
		if( MAPIL::IsKeyboardKeyPushed( MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_SPACE ) ) && m_pSceneManager->GetGameMode() == GAME_MODE_REPLAY ){
			isHighSpeedMode = true;
		}
		else{
			isHighSpeedMode = false;
		}

		// 更新間隔回数
		int iter = 1;
		if( isHighSpeedMode ){
			iter = 3;
		}

		for( int i = 0; i < iter; ++i ){

			// ローディングが完了した時の処理
			if( m_pSceneManager->NeedToSwitch() && m_Loading.SessionEnded() ){
				// 前段階のリソースの解放
				m_pResourceManager->ReleaseStageResources();
				m_pSceneManager->AttachSceneResourceMap( m_pResourceManager->GetStageResourceMap() );
				m_pSceneManager->AttachScriptData( m_pScriptManager->GetScriptData() );
				m_pResourceManager->SetBGMVolume( m_pGameStateManager->GetConfigData().m_BGMVolume );
				m_pResourceManager->SetSEVolume( m_pGameStateManager->GetConfigData().m_SEVolume );
				m_pSceneManager->SwitchToNextScene();
			}

			// ロード時は、リプレイデータを読み込まないようにするため、入力は不可能とする。
			if( m_pSceneManager->GetCurSceneType() != SCENE_TYPE_LOADING &&
				!m_pSceneManager->Paused() ){
				// ボタンの取得
				ButtonStatusHolder holder;
				m_pButtonManager->Update();
				m_pButtonManager->GetButtonStatus( &holder );
				m_pSceneManager->AttachButtonState( &holder );
			}
			else{
				// ボタン無効化
				ButtonStatusHolder holder;
				MAPIL::ZeroObject( &holder, sizeof( holder ) );
				m_pSceneManager->AttachButtonState( &holder );
			}

			// 画面の更新
			m_pSceneManager->Update();

		}

		m_pSceneManager->Draw();
		

		// 時間の表示（暫定版）
		m_pGameStateManager->UpdatePlayTime();
		int time = m_pGameStateManager->GetPlayTime();
		MAPIL::BeginRendering2DGraphics();
		MAPIL::DrawString( 10.0f, 10.0f, "%d : %d : %d", time / 3600, ( time / 60 ) % 60, time % 60 );
		MAPIL::EndRendering2DGraphics();

		// FPS表示
		MAPIL::BeginRendering2DGraphics();
		if( m_pSceneManager->GetCurSceneType() != SCENE_TYPE_INITIALIZE && m_pSceneManager->GetCurSceneType() != SCENE_TYPE_LOADING ){
			DrawFontString( m_pResourceManager->GetStageResourceMap(), 582.0f, 444.0f, 0.4f, "%.1f", m_CurFPS );
		}
		MAPIL::EndRendering2DGraphics();
	}

	void EventMediator::Impl::LoadGlobalResources()
	{
		// ロードの予約
		m_Loading.CleanupSession();
		m_Loading.SetupSession( m_pResourceManager, m_pScriptManager );
		m_Loading.AddGlobalResourceItem( RESOURCE_TYPE_SE, GLOBAL_RESOURCE_ID_ITEM_1_SE, "archive/resource/se/item01.wav", true );
		m_Loading.AddGlobalResourceItem( RESOURCE_TYPE_SE, GLOBAL_RESOURCE_ID_ITEM_2_SE, "archive/resource/se/item02.wav", true );
		m_Loading.AddGlobalResourceItem( RESOURCE_TYPE_SE, GLOBAL_RESOURCE_ID_SHOT_SE, "archive/resource/se/shot.wav", true );
		m_Loading.AddGlobalResourceItem( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_HP_BAR_TEXTURE, "archive/resource/texture/hpbar.png", true );
		m_Loading.AddGlobalResourceItem( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_CONS_BAR_TEXTURE, "archive/resource/texture/consbar.png", true );
		m_Loading.AddGlobalResourceItem( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_BACKGROUND_TEXTURE, "archive/resource/texture/background.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_MULTI_TEXTURE,
											GLOBAL_RESOURCE_ID_FONT_TEXTURE_FIRST,
											GLOBAL_RESOURCE_ID_FONT_TEXTURE_LAST,
											5, 8, 32, 32, "archive/resource/texture/game_font.png", true );
		m_Loading.AddGlobalResourceItem( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_POWER_UP_ITEM_1_TEXTURE, "archive/resource/texture/powerup_item_1.png", true );
		m_Loading.AddGlobalResourceItem( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_POWER_UP_ITEM_2_TEXTURE, "archive/resource/texture/powerup_item_2.png", true );
		m_Loading.AddGlobalResourceItem( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE, "archive/resource/texture/crystal.png", true );
		m_Loading.AddGlobalResourceItem( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_PLAYER_SHOT_1_TEXTURE, "archive/resource/texture/player_shot_1.png", true );
		m_Loading.AddGlobalResourceItem( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_PLAYER_SHOT_COLLIDED_TEXTURE, "archive/resource/texture/player_shot_collided_effect.png", true );
		m_Loading.AddGlobalResourceItem( RESOURCE_TYPE_TEXTURE, GLOBAL_RESOURCE_ID_ENEMY_DESTROYED_TEXTURE, "archive/resource/texture/enemy_destroyed.png", true );
		m_Loading.AddGlobalResourceItem( RESOURCE_TYPE_SE, GLOBAL_RESOURCE_ID_ENEMY_DESTROYED_SE, "archive/resource/se/rtg_se2.wav", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_PLAYER_SHOT_GREEN_MAIN,
											"archive/resource/texture/player_shot_2.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_SE,
											GLOBAL_RESOURCE_SE_ID_PLAYER_CHANGE_MODE,
											"archive/resource/se/change_mode.wav", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_BAR,
											"archive/resource/texture/bar.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_CONS_RED_SYMBOL,
											"archive/resource/texture/red_cons_2_1.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_CONS_GREEN_SYMBOL,
											"archive/resource/texture/green_cons.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_CONS_BLUE_SYMBOL,
											"archive/resource/texture/blue_cons.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_PLAYER_SHOT_BLUE_MAIN,
											"archive/resource/texture/player_shot_3.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_PLAYER_SHOT_RED_MAIN,
											"archive/resource/texture/player_shot_4.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_SE,
											GLOBAL_RESOURCE_SE_ID_PLAYER_DAMAGED,
											"archive/resource/se/player_damaged.wav", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_SE,
											GLOBAL_RESOURCE_SE_ID_PLAYER_DESTROYED,
											"archive/resource/se/player_destroyed.wav", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_MENU_GAME_START,
											"archive/resource/texture/game_start.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_MENU_GAME_START_SELECTED,
											"archive/resource/texture/game_start_selected.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_MENU_EXIT,
											"archive/resource/texture/exit.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_MENU_EXIT_SELECTED,
											"archive/resource/texture/exit_selected.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_MENU_SCORE,
											"archive/resource/texture/score.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_MENU_SCORE_SELECTED,
											"archive/resource/texture/score_selected.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_MENU_REPLAY,
											"archive/resource/texture/replay.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_MENU_REPLAY_SELECTED,
											"archive/resource/texture/replay_selected.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_MENU_CONFIG,
											"archive/resource/texture/config.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_MENU_CONFIG_SELECTED,
											"archive/resource/texture/config_selected.png", true );

		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_HI_SCORE,
											"archive/resource/texture/hi_score.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_GAME_SCORE,
											"archive/resource/texture/game_score.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_GAME_KILLED,
											"archive/resource/texture/game_killed.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_GAME_CRYSTAL,
											"archive/resource/texture/game_crystal.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_GAME_POWER,
											"archive/resource/texture/game_power.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_GAME_HP,
											"archive/resource/texture/game_hp.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_SE,
											GLOBAL_RESOURCE_SE_ID_MENU_SELECTED,
											"archive/resource/se/select.wav", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_BGM,
											GLOBAL_RESOURCE_BGM_ID_MENU,
											"archive/resource/bgm/eriKs_title.wav", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_ITEM_CONS_LEVEL_RECOVER_1,
											"archive/resource/texture/cons_level_recover_part1.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_ITEM_CONS_LEVEL_RECOVER_2,
											"archive/resource/texture/cons_level_recover_part2.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_ITEM_CONS_LEVEL_RECOVER_3,
											"archive/resource/texture/cons_level_recover_part3.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_ITEM_CONS_LEVEL_RECOVER_4,
											"archive/resource/texture/cons_level_recover_part4.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_ITEM_CONS_LEVEL_RECOVER_5,
											"archive/resource/texture/cons_level_recover_part5.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_ITEM_RECOVER_1,
											"archive/resource/texture/recover_1.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_ITEM_RECOVER_2,
											"archive/resource/texture/recover_2.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_1,
											"archive/resource/texture/cons_skill_effect_6.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_2,
											"archive/resource/texture/cons_skill_effect_2.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_3,
											"archive/resource/texture/cons_skill_effect_5.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_SE,
											GLOBAL_RESOURCE_SE_ID_EFFECT_CONS_SKILL_1,
											"archive/resource/se/cons_skill_effect_1.wav", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_SE,
											GLOBAL_RESOURCE_SE_ID_EFFECT_CONS_SKILL_2,
											"archive/resource/se/cons_skill_effect_4.wav", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_SE,
											GLOBAL_RESOURCE_SE_ID_EFFECT_CONS_SKILL_3,
											"archive/resource/se/cons_skill_effect_3.wav", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_SE,
											GLOBAL_RESOURCE_SE_ID_EFFECT_CONS_SKILL_4,
											"archive/resource/se/cons_skill_effect_5.wav", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_MODEL,
											GLOBAL_RESOURCE_MODEL_ID_PLAYER,
											"archive/resource/model/player.x", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_STAGE_BACKGROUND,
											"archive/resource/texture/eriKs_stage_background.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_STAGE_BACKGROUND_2,
											"archive/resource/texture/eriKs_stage_background2.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_TITLE,
											"archive/resource/texture/eriKs_title.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND,
											"archive/resource/texture/eriKs_background_small.png", true );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_CALM,
											"archive/resource/texture/difficulty_selection/calm_rank_selection.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_EASY,
											"archive/resource/texture/difficulty_selection/easy_rank_selection.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_NORMAL,
											"archive/resource/texture/difficulty_selection/normal_rank_selection.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_HARD,
											"archive/resource/texture/difficulty_selection/hard_rank_selection.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_HAZARD,
											"archive/resource/texture/difficulty_selection/hazard_rank_selection.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_DIFFICULTY_SELECTION_TITLE,
											"archive/resource/texture/difficulty_selection/title.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_STAGE_1,
											"archive/resource/texture/stage_selection/stage_1_selection.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_STAGE_2,
											"archive/resource/texture/stage_selection/stage_2_selection.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_STAGE_3,
											"archive/resource/texture/stage_selection/stage_3_selection.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_STAGE_4,
											"archive/resource/texture/stage_selection/stage_4_selection.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_STAGE_5,
											"archive/resource/texture/stage_selection/stage_5_selection.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_STAGE_SELECTION_TITLE,
											"archive/resource/texture/stage_selection/title.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_SCORE_CALM,
											"archive/resource/texture/score/calm.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_SCORE_EASY,
											"archive/resource/texture/score/easy.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_SCORE_NORMAL,
											"archive/resource/texture/score/normal.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_SCORE_HARD,
											"archive/resource/texture/score/hard.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_SCORE_HAZARD,
											"archive/resource/texture/score/hazard.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_SCORE_TITLE,
											"archive/resource/texture/score/title.png", false );
		m_Loading.AddGlobalResourceItem(	RESOURCE_TYPE_TEXTURE,
											GLOBAL_RESOURCE_TEXTURE_ID_REPLAY_TITLE,
											"archive/resource/texture/replay/title.png", false );
		m_Loading.Start();
	}

	void EventMediator::Impl::SendEvent( int type )
	{
		switch( type ){
			// 初期化要求
			case EVENT_TYPE_INITIALIZE:{
				m_pGameStateManager->LoadConfigData();
				m_pGameStateManager->LoadGameData();
				m_pSceneManager->ChangeScene( SCENE_TYPE_INITIALIZE );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				m_pScriptManager->BuildFileStructure( m_pResourceManager->GetArchiveHandle(), "archive/script/build.isc", SCRIPT_FILE_TYPE_BINARY );
				DisplayedSaveData data;
				for( int i = 0; i < GAME_DIFFICULTY_TOTAL; ++i ){
					for( int j = 0; j < MAX_SCORE_ENTRY; ++j ){
						data.m_Difficulty[ i ].m_Record[ j ] = m_pGameStateManager->GetRecord( i, j );
					}
					data.m_Difficulty[ i ].m_PlayTime = m_pGameStateManager->GetPlayTime( i );
					data.m_Difficulty[ i ].m_AllClear = m_pGameStateManager->GetAllClearCount( i );
				}
				data.m_PlayTime = m_pGameStateManager->GetPlayTime();
				m_pSceneManager->AttachDisplayedSaveData( data );
				// 統計情報の設定
				const GameStat& stat = m_pGameStateManager->GetGameStat();
				m_pSceneManager->AttachGameStat( stat );
				break;
			}
			// 初期化状態からメニュー画面移行要求
			case EVENT_TYPE_MOVE_TO_MENU_FROM_INITIALIZE:{
				m_pResourceManager->ReleaseStageResources();
				m_pSceneManager->ChangeScene( SCENE_TYPE_MENU );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				LoadGlobalResources();
				break;
			}
			// メニュー画面移行要求
			case EVENT_TYPE_MOVE_TO_MENU:{
				m_pResourceManager->ReleaseStageResources();
				m_pSceneManager->ChangeScene( SCENE_TYPE_MENU );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}
			// 難易度選択画面移行要求
			case EVENT_TYPE_MOVE_TO_DIFFICULTY_SELECTION:{
				m_pResourceManager->ReleaseStageResources();
				m_pSceneManager->AttachDisplayedNormalPlayStat( m_pGameStateManager->GetDisplayedNormalPlayStat() );
				m_pSceneManager->ChangeScene( SCENE_TYPE_DIFFICULTY_SELECTION );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}
			// 難易度選択画面（ステージ選択あり）移行要求
			case EVENT_TYPE_MOVE_TO_STAGE_SELECTION:{
				m_pResourceManager->ReleaseStageResources();
				m_pSceneManager->AttachDisplayedStageSelectionPlayStat( m_pGameStateManager->GetDisplayedStageSelectionPlayStat() );
				m_pSceneManager->ChangeScene( SCENE_TYPE_STAGE_SELECTION );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}
			// ステージ選択画面移行要求
			/*case EVENT_TYPE_MOVE_TO_STAGE_SELECTION:{
				m_pResourceManager->ReleaseStageResources();
				m_pSceneManager->ChangeScene( SCENE_TYPE_STAGE_SELECTION );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}*/
			// コンフィグ画面移行要求
			case EVENT_TYPE_MOVE_TO_CONFIG:{
				m_pResourceManager->ReleaseStageResources();
				m_pSceneManager->AttachConfigData( m_pGameStateManager->GetConfigData() );
				m_pSceneManager->ChangeScene( SCENE_TYPE_CONFIG );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}
			// コンフィグ画面からメニューへ移行要求
			case EVENT_TYPE_MOVE_TO_MENU_FROM_CONFIG:{
				m_pResourceManager->ReleaseStageResources();
				m_pGameStateManager->SaveConfigData( m_pSceneManager->GetConfigData() );
				m_pSceneManager->ChangeScene( SCENE_TYPE_MENU );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}
			// スコア画面移行要求
			case EVENT_TYPE_MOVE_TO_SCORE:{
				m_pResourceManager->ReleaseStageResources();
				// 履歴表示
				DisplayedSaveData data;
				for( int i = 0; i < GAME_DIFFICULTY_TOTAL; ++i ){
					for( int j = 0; j < MAX_SCORE_ENTRY; ++j ){
						data.m_Difficulty[ i ].m_Record[ j ] = m_pGameStateManager->GetRecord( i, j );
					}
					data.m_Difficulty[ i ].m_PlayTime = m_pGameStateManager->GetPlayTime( i );
					data.m_Difficulty[ i ].m_AllClear = m_pGameStateManager->GetAllClearCount( i );
				}
				data.m_PlayTime = m_pGameStateManager->GetPlayTime();
				m_pSceneManager->AttachDisplayedSaveData( data );
				// シーン変更
				m_pSceneManager->ChangeScene( SCENE_TYPE_SCORE );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}
			// リプレイ画面移行要求
			case EVENT_TYPE_MOVE_TO_REPLAY:{
				m_pResourceManager->ReleaseStageResources();
				// リプレイ取得
				m_pSceneManager->AttachDisplayedReplayInfo( m_pGameStateManager->GetDisplayedReplayInfo() );
				// シーン変更
				m_pSceneManager->ChangeScene( SCENE_TYPE_REPLAY );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}
			case EVENT_TYPE_MOVE_TO_SCORE_ENTRY:{
				m_pResourceManager->ReleaseStageResources();
				// ランキングを取得
				int difficulty = m_pGameStateManager->GetRank( m_pSceneManager->GetGameDifficulty(), m_pSceneManager->GetSaveDataRecord() );
				m_pSceneManager->SetRecordRank( difficulty );
				// 履歴表示
				DisplayedSaveData data;
				for( int i = 0; i < GAME_DIFFICULTY_TOTAL; ++i ){
					for( int j = 0; j < MAX_SCORE_ENTRY; ++j ){
						data.m_Difficulty[ i ].m_Record[ j ] = m_pGameStateManager->GetRecord( i, j );
					}
					data.m_Difficulty[ i ].m_PlayTime = m_pGameStateManager->GetPlayTime( i );
					data.m_Difficulty[ i ].m_AllClear = m_pGameStateManager->GetAllClearCount( i );
				}
				data.m_PlayTime = m_pGameStateManager->GetPlayTime();
				m_pSceneManager->AttachDisplayedSaveData( data );
				// シーン変更
				m_pSceneManager->ChangeScene( SCENE_TYPE_SCORE_ENTRY );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}
			case EVENT_TYPE_MOVE_TO_REPLAY_ENTRY_FROM_SCORE_ENTRY:{
				m_pResourceManager->ReleaseStageResources();
				// スコアの保存
				m_pGameStateManager->SetRecord( m_pSceneManager->GetGameDifficulty(), m_pSceneManager->GetSaveDataRecord() );
				m_pGameStateManager->FlushGameData();
				// リプレイ一覧を取得
				DisplayedReplayInfo info = m_pGameStateManager->GetDisplayedReplayInfo();
				m_pSceneManager->AttachDisplayedReplayInfo( info );
				// シーンの変更
				m_pSceneManager->ChangeScene( SCENE_TYPE_REPLAY_ENTRY );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}
			case EVENT_TYPE_MOVE_TO_MENU_FROM_REPLAY_ENTRY:{
				m_pResourceManager->ReleaseStageResources();
				m_pGameStateManager->SetGameStat( m_pSceneManager->GetGameStat() );
				m_pSceneManager->ChangeScene( SCENE_TYPE_MENU );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}
			case EVENT_TYPE_MOVE_TO_MENU_FROM_SCORE_ENTRY:{
				m_pResourceManager->ReleaseStageResources();
				m_pGameStateManager->SetGameStat( m_pSceneManager->GetGameStat() );
				m_pSceneManager->ChangeScene( SCENE_TYPE_MENU );
				m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				break;
			}
			case EVENT_TYPE_MOVE_TO_REPLAY_ENTRY_FROM_SELF:{
				m_pResourceManager->ReleaseStageResources();
				// リプレイの保存
				m_pGameStateManager->SaveReplay( m_pSceneManager->GetReplayDataRecord() );
				// リプレイ一覧を取得
				DisplayedReplayInfo info = m_pGameStateManager->GetDisplayedReplayInfo();
				m_pSceneManager->AttachDisplayedReplayInfo( info );
				break;
			}
			// フレーム更新要求
			case EVENT_TYPE_FRAME_UPDATE:
				FrameUpdate();
				break;
			// ゲーム終了要求
			case EVENT_TYPE_GAME_TERM:
				m_pGameStateManager->FlushGameData();
				m_HasTermSig = true;
				break;
			default:
				break;
		}
	}


	void EventMediator::Impl::SendEvent( int type, void* pArg )
	{
		switch( type ){
			// ステージ変更要求
			case EVENT_TYPE_MOVE_TO_STAGE:{
				// スクリプトデータの読み込み
				int stage = *( static_cast < int* > ( pArg ) );
				m_Loading.CleanupSession();
				m_Loading.SetupSession( m_pResourceManager, m_pScriptManager );
#if defined ( USE_FLOATING_POINT )
				m_Loading.AddStageResourceItem( stage, false );
				//m_Loading.AddStageResourceItem( 2, false );
#elif defined ( USE_GAME_UNIT )
#if defined ( MAKE_MODE_RELEASE )
				//m_Loading.AddStageResourceItem( 2, false );
				m_Loading.AddStageResourceItem( stage, false );
#elif defined ( MAKE_MODE_DEBUG )
				//m_Loading.AddStageResourceItem( stage, false );
				m_Loading.AddStageResourceItem( 4, false );
#endif
#endif
				m_Loading.Start();
				// ゲームデータの初期化（※2面以降が問題？）
				m_pSceneManager->ClearGameData();
				// 入力デバイス変更
				if( m_pSceneManager->GetGameMode() == GAME_MODE_NORMAL ){
					m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
					// ハイスコアの設定
					m_pSceneManager->SetHIScore( m_pGameStateManager->GetHIScore( m_pSceneManager->GetGameDifficulty() ) );
				}
				else if( m_pSceneManager->GetGameMode() == GAME_MODE_REPLAY ){
					m_pButtonManager->ChangeDevice( INPUT_DEVICE_FILE );
					m_pButtonManager->SetReplayNo( m_pSceneManager->GetReplayNo() );
					// 難易度の設定（※複数の難易度で問題化？）
					m_pSceneManager->SetGameDifficulty( m_pGameStateManager->GetReplayGameDifficulty( m_pSceneManager->GetReplayNo() ) );
					// ハイスコアの設定
					m_pSceneManager->SetHIScore( m_pGameStateManager->GetHIScore( m_pSceneManager->GetGameDifficulty() ) );
					// リプレイ用初期データ取得
					InitialGameData iniGameData = m_pGameStateManager->GetReplayIniData( m_pSceneManager->GetReplayNo(), stage );
					iniGameData.m_HIScore = m_pGameStateManager->GetHIScore( m_pSceneManager->GetGameDifficulty() );
					// 初期データ設定
					m_pSceneManager->AttachInitialGameData( iniGameData );
					m_pButtonManager->SetStageNo( stage );
				}
				// シーン変更
				m_pSceneManager->ChangeScene( SCENE_TYPE_STAGE );
				break;
			}
			// 次のステージへ移行
			case EVENT_TYPE_MOVE_TO_NEXT_STAGE:{
				// スクリプトデータの読み込み
				int stage = *( static_cast < int* > ( pArg ) );
				m_Loading.CleanupSession();
				m_Loading.SetupSession( m_pResourceManager, m_pScriptManager );
#if defined ( USE_FLOATING_POINT )
				m_Loading.AddStageResourceItem( stage, false );
				//m_Loading.AddStageResourceItem( 2, false );
#elif defined ( USE_GAME_UNIT )
				m_Loading.AddStageResourceItem( stage, false );
				//m_Loading.AddStageResourceItem( 2, false );
#endif
				m_Loading.Start();

				// 入力デバイス変更
				if( m_pSceneManager->GetGameMode() == GAME_MODE_NORMAL ){
					m_pButtonManager->ChangeDevice( INPUT_DEVICE_KEYBOARD );
				}
				else if( m_pSceneManager->GetGameMode() == GAME_MODE_REPLAY ){
					m_pButtonManager->ChangeDevice( INPUT_DEVICE_FILE );
					m_pButtonManager->SetReplayNo( m_pSceneManager->GetReplayNo() );
					// リプレイ用初期データ取得
					InitialGameData iniGameData = m_pGameStateManager->GetReplayIniData( m_pSceneManager->GetReplayNo(), stage );
					iniGameData.m_HIScore = m_pGameStateManager->GetHIScore( m_pSceneManager->GetGameDifficulty() );
					// 初期データ設定
					m_pSceneManager->AttachInitialGameData( iniGameData );
					m_pButtonManager->SetStageNo( stage );
				}
				// シーン変更
				m_pSceneManager->ChangeScene( SCENE_TYPE_STAGE );
				break;
			}
			// BGMの音量が変更された
			case EVENT_TYPE_BGM_VOLUME_CHANGED:{
				GameConfigData* pConfig = static_cast < GameConfigData* > ( pArg );
				m_pResourceManager->SetBGMVolume( pConfig->m_BGMVolume );
				break;
			}
			// SEの音量が変更された
			case EVENT_TYPE_SE_VOLUME_CHANGED:{
				GameConfigData* pConfig = static_cast < GameConfigData* > ( pArg );
				m_pResourceManager->SetSEVolume( pConfig->m_SEVolume );
				break;
			}
			// FPSの値が更新された
			case EVENT_TYPE_FPS_CHANGED:{
				float fps = *( static_cast < float* > ( pArg ) );
				m_CurFPS = fps;
				break;
			}
			default:
				break;
		}
	}

	void EventMediator::Impl::SetSceneManager( std::shared_ptr < SceneManager > pSceneManager )
	{
		m_pSceneManager = pSceneManager;
	}

	void EventMediator::Impl::SetButtonManager( std::shared_ptr < GeneralButtonManager > pButtonManager )
	{
		m_pButtonManager = pButtonManager;
	}

	void EventMediator::Impl::SetGameStateManger( std::shared_ptr < GameStateManager > pGameStateManager )
	{
		m_pGameStateManager = pGameStateManager;
	}

	void EventMediator::Impl::SetResourceManager( std::shared_ptr < ResourceManager > pResourceManager )
	{
		m_pResourceManager = pResourceManager;
	}

	void EventMediator::Impl::SetScriptManager( std::shared_ptr < ScriptManager > pScriptManager )
	{
		m_pScriptManager = pScriptManager;
	}

	bool EventMediator::Impl::HasTermSig() const
	{
		return m_HasTermSig;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	EventMediator::EventMediator() : m_pImpl( new EventMediator::Impl )
	{
	}

	EventMediator::~EventMediator()
	{
	}

	void EventMediator::SendEvent( int type )
	{
		m_pImpl->SendEvent( type );
	}

	void EventMediator::SendEvent( int type, void* pArg )
	{
		m_pImpl->SendEvent( type, pArg );
	}


	void EventMediator::SetSceneManager( std::shared_ptr < SceneManager > pSceneManager )
	{
		m_pImpl->SetSceneManager( pSceneManager );
	}

	void EventMediator::SetButtonManager( std::shared_ptr < GeneralButtonManager > pButtonManager )
	{
		m_pImpl->SetButtonManager( pButtonManager );
	}

	void EventMediator::SetGameStateManager( std::shared_ptr < GameStateManager > pGameStateManager )
	{
		m_pImpl->SetGameStateManger( pGameStateManager );
	}

	void EventMediator::SetResourceManager( std::shared_ptr < ResourceManager > pResourceManager )
	{
		m_pImpl->SetResourceManager( pResourceManager );
	}

	void EventMediator::SetScriptManager( std::shared_ptr < ScriptManager > pScriptManager )
	{
		m_pImpl->SetScriptManager( pScriptManager );
	}

	bool EventMediator::HasTermSig() const
	{
		return m_pImpl->HasTermSig();
	}
}
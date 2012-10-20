#include "SceneManager.h"

#include <time.h>

#include "SceneBuilder.h"
#include "Scene.h"
#include "Stage.h"
#include "Score.h"
#include "Menu.h"
#include "Initialize.h"
#include "ScoreEntry.h"
#include "ReplayEntry.h"

#include "SceneTypes.h"

#include "EventMediator.h"
#include "EventTypes.h"


namespace GameEngine
{
	// SceneManager実装クラス
	class SceneManager::Impl
	{
		std::tr1::shared_ptr < SceneBuilder >	m_pSceneBuilder;		// シーン構築クラス
		std::auto_ptr < Scene >					m_pCurScene;			// 現在のシーン
		std::weak_ptr < EventMediator >			m_pEventMediator;		// イベント仲介役クラス
		ResourceMap								m_ResourceMap;			// リソース一覧
		ScriptData								m_ScriptData;			// スクリプトデータ
		SceneType								m_CurSceneType;			// 現在のシーン
		DisplayedSaveData						m_SaveData;				// セーブデータ
		SaveDataRecord							m_SaveDataRecord;		// 現在プレイ中の記録
		DisplayedReplayInfo						m_DisplayedReplayInfo;	// 表示用リプレイ情報
		int										m_GameDifficulty;		// 難易度
		int										m_CurStage;				// 現在のステージ
		int										m_RecordRank;			// スコアのランク
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
		const DisplayedSaveData& GetDisplayedSaveData() const;
		//GameDataMsg GetFrameScoreData() const;
		void ChangeScene( SceneType scene );
		SceneType GetCurSceneType() const;
		void SetGameDifficulty( int difficulty );
		void SetRecordRank( int rank );
		const SaveDataRecord& GetRecord() const;
		int GetGameDifficulty() const;
		void ClearGameData();
	};

	SceneManager::Impl::Impl( std::shared_ptr < EventMediator > pEventMediator ) :	m_pSceneBuilder( new SceneBuilder ),
																					m_pEventMediator( pEventMediator )
	{
		m_pCurScene.reset( m_pSceneBuilder->CreateNextScene( SCENE_TYPE_UNKNOWN ) );
		m_CurSceneType = SCENE_TYPE_UNKNOWN;
		m_GameDifficulty = GAME_DIFFICULTY_EASY;
		m_CurStage = 0;
		MAPIL::ZeroObject( &m_SaveDataRecord, sizeof( m_SaveDataRecord ) );
		MAPIL::ZeroObject( &m_DisplayedReplayInfo, sizeof( m_DisplayedReplayInfo ) );
	}

	void SceneManager::Impl::Draw()
	{
		m_pCurScene->Draw();
	}

	SceneType SceneManager::Impl::Update()
	{
		// シーン更新
		SceneType next = m_pCurScene->Update();

		// シーン遷移
		if( next != SCENE_TYPE_NOT_CHANGE ){
			if( std::shared_ptr < EventMediator > p = m_pEventMediator.lock() ){
				if( next == SCENE_TYPE_MENU ){
					if( m_CurSceneType == SCENE_TYPE_SCORE_ENTRY ){
						//ScoreEntry* pp = dynamic_cast < ScoreEntry* > ( m_pCurScene.get() );
						//if( pp ){
						//	m_SaveData = pp->GetDisplayedSaveData();
						//	//m_SaveDataRecord = pp->GetRecord();
						//}
						//else{
						//	exit( 1 );
						//}
						p->SendEvent( EVENT_TYPE_MOVE_TO_MENU_FROM_SCORE_ENTRY );
					}
					else{
						p->SendEvent( EVENT_TYPE_MOVE_TO_MENU );
					}
				}
				else if( next == SCENE_TYPE_STAGE ){
					if( m_CurSceneType == SCENE_TYPE_MENU ){
						m_CurStage = STAGE_ID_STAGE_1;
					}
					else if( m_CurSceneType == SCENE_TYPE_STAGE ){
						++m_CurStage;
					}
					p->SendEvent( EVENT_TYPE_MOVE_TO_STAGE, &m_CurStage );
				}
				else if( next == SCENE_TYPE_SCORE ){
					p->SendEvent( EVENT_TYPE_MOVE_TO_SCORE );
				}
				else if( next == SCENE_TYPE_SCORE_ENTRY ){
					if( m_CurSceneType == SCENE_TYPE_STAGE ){
						// ゲーム到達度の保存
						m_SaveDataRecord.m_Progress = m_CurStage;
						// 最後のステージの記録
						Stage* pp = dynamic_cast < Stage* > ( m_pCurScene.get() );
						if( pp ){
							m_SaveDataRecord.m_StageData[ m_CurStage ].m_Crystal = pp->GetCrystal();
							m_SaveDataRecord.m_StageData[ m_CurStage ].m_Killed = pp->GetKilled();
							m_SaveDataRecord.m_StageData[ m_CurStage ].m_Score = pp->GetScore();
							m_SaveDataRecord.m_StageData[ m_CurStage ].m_Progress = pp->GetProgress();
						}
						else{
							exit( 1 );
						}
						// 最終的なスコアを算出
						for( int i = 0; i < m_CurStage + 1; ++i ){
							m_SaveDataRecord.m_Crystal += m_SaveDataRecord.m_StageData[ i ].m_Crystal;
							m_SaveDataRecord.m_Killed += m_SaveDataRecord.m_StageData[ i ].m_Killed;
							m_SaveDataRecord.m_Score += m_SaveDataRecord.m_StageData[ i ].m_Score;
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
					p->SendEvent( EVENT_TYPE_MOVE_TO_SCORE_ENTRY );
				}
				else if( next == SCENE_TYPE_REPLAY_ENTRY ){
					ScoreEntry* pp = dynamic_cast < ScoreEntry* > ( m_pCurScene.get() );
					if( pp ){
						//m_SaveData = pp->GetDisplayedSaveData();
						m_SaveDataRecord = pp->GetRecord();
					}
					else{
						exit( 1 );
					}
					// ※リプレイ情報を取得する。
					p->SendEvent( EVENT_TYPE_MOVE_TO_REPLAY_ENTRY_FROM_SCORE_ENTRY );
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


	//GameDataMsg SceneManager::Impl::GetFrameScoreData() const
	//{
	//	return m_pCurScene->GetFrameData();
	//}

	void SceneManager::Impl::ChangeScene( SceneType scene )
	{
		m_pCurScene.reset( m_pSceneBuilder->CreateNextScene( scene ) );
		//Stage* p = dynamic_cast < Stage* > ( m_pCurScene.get() );
		if( typeid( *m_pCurScene.get() ) == typeid( Initialize ) ){
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( Menu ) ){
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( Stage ) ){
			//p->AttachResourceMap( m_ResourceMap );
			( (Stage*) m_pCurScene.get() )->AttachScriptData( m_ScriptData );
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( Score ) ){
			( (Score*) m_pCurScene.get() )->AttachDisplayedSaveData( m_SaveData );
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( ScoreEntry ) ){
			( (ScoreEntry*) m_pCurScene.get() )->AttachDisplayedSaveData( m_SaveData );
			( (ScoreEntry*) m_pCurScene.get() )->SetDifficulty( m_GameDifficulty );
			( (ScoreEntry*) m_pCurScene.get() )->AttachRecord( m_SaveDataRecord );
			( (ScoreEntry*) m_pCurScene.get() )->SetRecordRank( m_RecordRank );
		}
		else if( typeid( *m_pCurScene.get() ) == typeid( ReplayEntry ) ){
			( (ReplayEntry*) m_pCurScene.get() )->AttachDisplayedReplayInfo( m_DisplayedReplayInfo );
		}
		m_pCurScene->AttachResourceMap( m_ResourceMap );
		m_pCurScene->Init();
		m_CurSceneType = scene;
	}

	void SceneManager::Impl::AttachScriptData( const ScriptData& data )
	{
		m_ScriptData = data;
	}

	void SceneManager::Impl::AttachDisplayedSaveData( const DisplayedSaveData& data )
	{
		m_SaveData = data;
		//Score* p = dynamic_cast < Score* > ( m_pCurScene.get() );
		//if( p ){
		//	p->AttachDisplayedSaveData( data );
		//}
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

	const SaveDataRecord& SceneManager::Impl::GetRecord() const
	{
		return m_SaveDataRecord;
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

	//GameDataMsg SceneManager::GetFrameScoreData() const
	//{
	//	return m_pImpl->GetFrameScoreData();
	//}

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

	const SaveDataRecord& SceneManager::GetRecord() const
	{
		return m_pImpl->GetRecord();
	}

	int SceneManager::GetGameDifficulty() const
	{
		return m_pImpl->GetGameDifficulty();
	}

	void SceneManager::ClearGameData()
	{
		m_pImpl->ClearGameData();
	}
}
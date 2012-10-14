#include "SceneManager.h"
#include "SceneBuilder.h"
#include "Scene.h"
#include "Stage.h"
#include "Score.h"
#include "Menu.h"
#include "Initialize.h"
#include "ScoreEntry.h"
#include "SceneTypes.h"

#include "EventMediator.h"
#include "EventTypes.h"


namespace GameEngine
{
	// SceneManager�����N���X
	class SceneManager::Impl
	{
		std::tr1::shared_ptr < SceneBuilder >	m_pSceneBuilder;		// �V�[���\�z�N���X
		std::auto_ptr < Scene >					m_pCurScene;			// ���݂̃V�[��
		std::weak_ptr < EventMediator >			m_pEventMediator;		// �C�x���g������N���X
		ResourceMap								m_ResourceMap;			// ���\�[�X�ꗗ
		ScriptData								m_ScriptData;			// �X�N���v�g�f�[�^
		SceneType								m_CurSceneType;			// ���݂̃V�[��
		DisplayedSaveData						m_SaveData;				// �Z�[�u�f�[�^
		SaveDataRecord							m_SaveDataRecord;		// ���݃v���C���̋L�^
		int										m_GameDifficulty;		// ��Փx
		int										m_CurStage;				// ���݂̃X�e�[�W
	public:
		Impl( std::shared_ptr < EventMediator > pEventMediator );
		~Impl(){}
		void Draw();
		SceneType Update();
		void AttachSceneResourceMap( const ResourceMap& map );
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachScriptData( const ScriptData& data );
		void AttachGameData( const GameDataMsg& msg );
		void AttachDisplayedSaveData( const DisplayedSaveData& data );
		GameDataMsg GetFrameScoreData() const;
		void ChangeScene( SceneType scene );
		SceneType GetCurSceneType() const;
		void SetGameDifficulty( int difficulty );
	};

	SceneManager::Impl::Impl( std::shared_ptr < EventMediator > pEventMediator ) :	m_pSceneBuilder( new SceneBuilder ),
																					m_pEventMediator( pEventMediator )
	{
		m_pCurScene.reset( m_pSceneBuilder->CreateNextScene( SCENE_TYPE_UNKNOWN ) );
		m_CurSceneType = SCENE_TYPE_UNKNOWN;
		m_GameDifficulty = GAME_DIFFICULTY_EASY;
		m_CurStage = 0;
	}

	void SceneManager::Impl::Draw()
	{
		m_pCurScene->Draw();
	}

	SceneType SceneManager::Impl::Update()
	{
		// �V�[���X�V
		SceneType next = m_pCurScene->Update();

		// �V�[���J��
		if( next != SCENE_TYPE_NOT_CHANGE ){
			if( std::shared_ptr < EventMediator > p = m_pEventMediator.lock() ){
				if( next == SCENE_TYPE_MENU ){
					p->SendEvent( EVENT_TYPE_MOVE_TO_MENU );
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
						m_SaveDataRecord.m_Progress = m_CurStage;
					}
					p->SendEvent( EVENT_TYPE_MOVE_TO_SCORE_ENTRY );
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

	void SceneManager::Impl::AttachGameData( const GameDataMsg& msg )
	{
		m_pCurScene->AttachGameData( msg );
	}

	GameDataMsg SceneManager::Impl::GetFrameScoreData() const
	{
		return m_pCurScene->GetFrameData();
	}

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
			( (ScoreEntry*) m_pCurScene.get() )->SetDifficulty( m_GameDifficulty );
			( (ScoreEntry*) m_pCurScene.get() )->AttachRecord( m_SaveDataRecord );
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


	// ----------------------------------
	// �����N���X�̌Ăяo��
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

	void SceneManager::AttachGameData( const GameDataMsg& msg )
	{
		m_pImpl->AttachGameData( msg );
	}

	GameDataMsg SceneManager::GetFrameScoreData() const
	{
		return m_pImpl->GetFrameScoreData();
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

	SceneType SceneManager::GetCurSceneType() const
	{
		return m_pImpl->GetCurSceneType();
	}

	void SceneManager::SetGameDifficulty( int difficulty )
	{
		m_pImpl->SetGameDifficulty( difficulty );
	}
}
#include "SceneManager.h"
#include "SceneBuilder.h"
#include "Scene.h"
#include "Stage.h"
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
	public:
		Impl( std::shared_ptr < EventMediator > pEventMediator );
		~Impl(){}
		void Draw();
		SceneType Update();
		void AttachSceneResourceMap( const ResourceMap& map );
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachScriptData( const ScriptData& data );
		void AttachGameData( const GameDataMsg& msg );
		GameDataMsg GetFrameScoreData() const;
		void ChangeScene( SceneType scene );
		SceneType GetCurSceneType() const;
	};

	SceneManager::Impl::Impl( std::shared_ptr < EventMediator > pEventMediator ) :	m_pSceneBuilder( new SceneBuilder ),
																					m_pEventMediator( pEventMediator )
	{
		m_pCurScene.reset( m_pSceneBuilder->CreateNextScene( SCENE_TYPE_UNKNOWN ) );
		m_CurSceneType = SCENE_TYPE_UNKNOWN;
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
					p->SendEvent( EVENT_TYPE_MOVE_TO_MENU );
				}
				else if( next == SCENE_TYPE_STAGE ){
					int stage = 1;
					p->SendEvent( EVENT_TYPE_MOVE_TO_STAGE, &stage );
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
		Stage* p = dynamic_cast < Stage* > ( m_pCurScene.get() );
		if( p ){
			//p->AttachResourceMap( m_ResourceMap );
			p->AttachScriptData( m_ScriptData );
		}
		m_pCurScene->AttachResourceMap( m_ResourceMap );
		m_pCurScene->Init();
		m_CurSceneType = scene;
	}

	void SceneManager::Impl::AttachScriptData( const ScriptData& data )
	{
		m_ScriptData = data;
	}

	SceneType SceneManager::Impl::GetCurSceneType() const
	{
		return m_CurSceneType;
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

	SceneType SceneManager::GetCurSceneType() const
	{
		return m_pImpl->GetCurSceneType();
	}
}
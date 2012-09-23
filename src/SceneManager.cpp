#include "SceneManager.h"
#include "SceneBuilder.h"
#include "Scene.h"
#include "Stage.h"
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
	public:
		Impl( std::shared_ptr < EventMediator > pEventMediator );
		~Impl(){}
		void Draw();
		SceneType Update();
		void AttachSceneResourceMap( const ResourceMap& map );
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachScriptData( const ScriptData& data );
		void GetScoreData();
		void ChangeScene( int scene );
	};

	SceneManager::Impl::Impl( std::shared_ptr < EventMediator > pEventMediator ) :	m_pSceneBuilder( new SceneBuilder ),
																					m_pEventMediator( pEventMediator )
	{
		m_pCurScene.reset( m_pSceneBuilder->CreateNextScene( SCENE_TYPE_UNKNOWN ) );
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
					int stage = 1;
					p->SendEvent( EVENT_TYPE_MOVE_TO_STAGE, &stage );
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

	void SceneManager::Impl::GetScoreData()
	{
	}

	void SceneManager::Impl::ChangeScene( int scene )
	{
		m_pCurScene.reset( m_pSceneBuilder->CreateNextScene( scene ) );
		Stage* p = dynamic_cast < Stage* > ( m_pCurScene.get() );
		if( p ){
			p->AttachResourceMap( m_ResourceMap );
			p->AttachScriptData( m_ScriptData );
		}
		m_pCurScene->Init();
	}

	void SceneManager::Impl::AttachScriptData( const ScriptData& data )
	{
		m_ScriptData = data;
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

	void SceneManager::GetScoreData()
	{
		m_pImpl->GetScoreData();
	}

	void SceneManager::ChangeScene( int scene )
	{
		m_pImpl->ChangeScene( scene );
	}

	void SceneManager::AttachScriptData( const ScriptData& data )
	{
		m_pImpl->AttachScriptData( data );
	}
}
#include <memory>

#include "SceneBuilder.h"
#include "SceneTypes.h"
#include "Menu.h"
#include "Config.h"
#include "Stage.h"
#include "Score.h"
#include "Replay.h"
#include "ScoreEntry.h"
#include "ReplayEntry.h"
#include "Initialize.h"
#include "DifficultySelection.h"
#include "StageSelection.h"

namespace GameEngine
{
	// SceneBuilder実装クラス
	class SceneBuilder::Impl
	{
		std::map < int, std::unique_ptr < Scene > >			m_SceneStroge;		// シーンの一時保存場所
	public:
		Impl();
		~Impl();
		Scene* CreateNextScene( int scene );
		void SaveScene( int type, Scene* scene );		// シーンの保存
	};

	SceneBuilder::Impl::Impl() : m_SceneStroge()
	{
		m_SceneStroge.clear();
	}

	SceneBuilder::Impl::~Impl()
	{
		m_SceneStroge.clear();
	}

	Scene* SceneBuilder::Impl::CreateNextScene( int scene )
	{
		switch( scene ){
			case SCENE_TYPE_INITIALIZE:
				return new Initialize;
			case SCENE_TYPE_MENU:
				if( m_SceneStroge[ SCENE_TYPE_MENU ] != NULL ){
					m_SceneStroge[ SCENE_TYPE_MENU ]->Reflesh();
					return m_SceneStroge[ SCENE_TYPE_MENU ].release();
				}
				else{
					return new Menu;
				}
				break;
			case SCENE_TYPE_CONFIG:
				return new Config;
			case SCENE_TYPE_SCORE:
				return new Score;
			case SCENE_TYPE_REPLAY:
				return new Replay;
			case SCENE_TYPE_SCORE_ENTRY:
				return new ScoreEntry;
			case SCENE_TYPE_STAGE:
				return new Stage( 0, false );
			case SCENE_TYPE_REPLAY_ENTRY:
				return new ReplayEntry;
			case SCENE_TYPE_DIFFICULTY_SELECTION:
				if( m_SceneStroge[ SCENE_TYPE_DIFFICULTY_SELECTION ] != NULL ){
					m_SceneStroge[ SCENE_TYPE_DIFFICULTY_SELECTION ]->Reflesh();
					return m_SceneStroge[ SCENE_TYPE_DIFFICULTY_SELECTION ].release();
				}
				else{
					return new DifficultySelection;
				}
				break;
			case SCENE_TYPE_STAGE_SELECTION:
				if( m_SceneStroge[ SCENE_TYPE_STAGE_SELECTION ] != NULL ){
					m_SceneStroge[ SCENE_TYPE_STAGE_SELECTION ]->Reflesh();
					return m_SceneStroge[ SCENE_TYPE_STAGE_SELECTION ].release();
				}
				else{
					return new StageSelection;
				}
				break;
			default:
				return NULL;
		}
	}

	void SceneBuilder::Impl::SaveScene( int type, Scene* scene )
	{
		m_SceneStroge[ type ].reset( scene );
	}


	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	SceneBuilder::SceneBuilder() : m_pImpl( new SceneBuilder::Impl )
	{
	}

	SceneBuilder::~SceneBuilder()
	{
	}

	Scene* SceneBuilder::CreateNextScene( int scene )
	{
		return m_pImpl->CreateNextScene( scene );
	}

	void SceneBuilder::SaveScene( int type, Scene* scene )
	{
		m_pImpl->SaveScene( type, scene );
	}
}
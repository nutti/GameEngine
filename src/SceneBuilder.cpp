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
	public:
		Impl();
		~Impl(){}
		Scene* CreateNextScene( int scene );
	};


	SceneBuilder::Impl::Impl()
	{
	}

	Scene* SceneBuilder::Impl::CreateNextScene( int scene )
	{
		switch( scene ){
			case SCENE_TYPE_INITIALIZE:
				return new Initialize;
			case SCENE_TYPE_MENU:
				return new Menu;
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
				return new DifficultySelection;
			case SCENE_TYPE_STAGE_SELECTION:
				return new StageSelection;
			default:
				return NULL;
		}
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
}
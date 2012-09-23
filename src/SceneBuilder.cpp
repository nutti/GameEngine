#include "SceneBuilder.h"
#include "SceneTypes.h"
#include "Menu.h"
#include "Stage.h"
#include "Initialize.h"

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
			case SCENE_TYPE_STAGE:
				return new Stage( 0, false );
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
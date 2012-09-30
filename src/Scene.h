#ifndef INCLUDED_GAMEENGINE_SCENE_H
#define INCLUDED_GAMEENGINE_SCENE_H

#include <memory>

#include "SceneTypes.h"
#include "InputTypes.h"
#include "ResourceTypes.h"
#include "GameStateTypes.h"

namespace GameEngine
{
	class SceneBuilder;
	class Scene
	{
	private:
	public:
		Scene();
		virtual ~Scene();
		virtual void Init() = 0;
		virtual SceneType Update() = 0;
		virtual void Draw() = 0;
		void IsLoading();
		void SetLoadingState();
		void SetButtonState();
		virtual void AttachButtonState( ButtonStatusHolder* pHolder ) = 0;
		virtual void AttachResourceMap( const ResourceMap& map ) = 0;
		virtual void AttachGameData( const GameDataMsg& msg ){}
		virtual GameDataMsg GetFrameData() const{ GameDataMsg msg; return msg;}
	};
}

#endif
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
	protected:
		bool			m_Initialized;		// 初期化されたらtrue
		virtual void InitImpl() = 0;		// 具体的な初期化処理
	public:
		Scene();
		virtual ~Scene();
		//virtual void Init() = 0;
		void Init();
		virtual SceneType Update() = 0;
		virtual void Draw() = 0;
		virtual void Reflesh(){}		// 内部状態を保持した初期化
		void IsLoading();
		void SetLoadingState();
		void SetButtonState();
		virtual void AttachButtonState( ButtonStatusHolder* pHolder ) = 0;
		virtual void AttachResourceMap( const ResourceMap& map ) = 0;
	};
}

#endif
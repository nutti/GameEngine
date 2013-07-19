#ifndef INCLUDED_GAMEENGINE_STAGE_SELECTION_H
#define INCLUDED_GAMEENGINE_STAGE_SELECTION_H

#include <memory>
#include "Scene.h"

namespace GameEngine
{
	class StageSelection : public Scene
	{
	private:
		class Impl;
		std::auto_ptr < StageSelection::Impl >		m_pImpl;
	public:
		StageSelection();
		~StageSelection();
		void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachStageSelectionStat( const DisplayedStageSelectionPlayStat& stat );
		int GetStageNo() const;
		int GetDifficulty() const;
	};
}

#endif
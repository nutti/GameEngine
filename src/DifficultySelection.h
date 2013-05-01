#ifndef INCLUDED_GAMEENGINE_DIFFICULTY_SELECTION_H
#define INCLUDED_GAMEENGINE_DIFFICULTY_SELECTION_H

#include <memory>
#include "Scene.h"

namespace GameEngine
{
	class DifficultySelection : public Scene
	{
	private:
		class Impl;
		std::auto_ptr < DifficultySelection::Impl >		m_pImpl;
	public:
		DifficultySelection();
		~DifficultySelection();
		void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		int GetDifficulty() const;
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_SCORE_H
#define INCLUDED_GAMEENGINE_SCORE_H

#include <memory>
#include "Scene.h"

namespace GameEngine
{
	class Score : public Scene
	{
	private:
		class Impl;
		std::auto_ptr < Score::Impl >		m_pImpl;
	public:
		Score();
		~Score();
		void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachDisplayedSaveData( const DisplayedSaveData& data );
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_REPLAY_H
#define INCLUDED_GAMEENGINE_REPLAY_H

#include <memory>
#include "Scene.h"

namespace GameEngine
{
	class Replay : public Scene
	{
	private:
		class Impl;
		std::auto_ptr < Replay::Impl >		m_pImpl;

		void InitImpl();
	public:
		Replay();
		~Replay();
		//void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachDisplayedReplayInfo( const DisplayedReplayInfo& info );
		int GetReplayStage() const;
		int GetReplayNo() const;
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_REPLAYENTRY_H
#define INCLUDED_GAMEENGINE_REPLAYENTRY_H

#include <memory>
#include "Scene.h"

namespace GameEngine
{
	class ReplayEntry : public Scene
	{
	private:
		class Impl;
		std::auto_ptr < ReplayEntry::Impl >		m_pImpl;
	public:
		ReplayEntry();
		~ReplayEntry();
		void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachDisplayedReplayInfo( const DisplayedReplayInfo& info );
		//void AttachReplayInfo( const DisplayedReplayInfo::Entry& info );
		//const DisplayedReplayInfo::Entry& GetReplayInfo() const;
		void AttachReplayDataRecord( const ReplayDataRecord& record );
		const ReplayDataRecord& GetReplayDataRecord() const;
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_REPLAYDATALOADER_H
#define INCLUDED_GAMEENGINE_REPLAYDATALOADER_H

#include <memory>
#include <string>

#include "InputTypes.h"
#include "GameStateTypes.h"

namespace GameEngine
{
	extern const char* REPLAY_FILE_DIR;
	extern const char* REPLAY_FILE_NAME_PREFIX;
	extern const char* REPLAY_FILE_NAME_SUFFIX;

	class ReplayDataLoader
	{
	private:
		class Impl;
		std::auto_ptr < ReplayDataLoader::Impl >	m_pImpl;
	public:
		ReplayDataLoader();
		~ReplayDataLoader();
		ButtonPushedStatus GetButtonState( int stage, int frame ) const;
		void Load( const std::string& fileName );
		void Cleanup();
		DisplayedReplayInfo::Entry GetDisplayedInfo( const std::string& fileName ) const;
		const ReplayDataRecord& GetReplayDataRecord() const;
	};
}

#endif
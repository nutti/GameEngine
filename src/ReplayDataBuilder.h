#ifndef INCLUDED_GAMEENGINE_REPLAYDATABUILDER_H
#define INCLUDED_GAMEENGINE_REPLAYDATABUILDER_H

#include <memory>
#include <string>

#include "InputTypes.h"
#include "Date.h"

#include "GameStateTypes.h"

namespace GameEngine
{
	class ReplayDataBuilder
	{
	private:
		class Impl;
		std::auto_ptr < ReplayDataBuilder::Impl >	m_pImpl;
	public:
		ReplayDataBuilder();
		~ReplayDataBuilder();
		void AddButtonState( ButtonPushedStatus status );
		void Save( const std::string& fileName );
		void Save( const std::string& fileName, const ReplayDataRecord& record );
		void Cleanup();
		void SetName( const char* pName );
		void SetProgress( int progress );
		void SetScore( int score );
		void SetCrystal( int crystal );
		void SetKilled( int killed );
		void SetDifficulty( int difficulty );
		void SetDate( const Date& date );
	};
}

#endif
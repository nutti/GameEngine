#ifndef INCLUDED_GAMEENGINE_GAMEDATAHOLDER_H
#define INCLUDED_GAMEENGINE_GAMEDATAHOLDER_H

#include <memory>

#include "GameStateTypes.h"

namespace GameEngine
{
	class GameDataHolder
	{
	private:
		class Impl;
		std::auto_ptr < GameDataHolder::Impl >	m_pImpl;
	public:
		GameDataHolder();
		~GameDataHolder();
		void StartRecording();
		void EndRecording();
		void Update();
		GameDataMsg GetScoreData() const;
		void Add( const GameDataMsg& data );
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_SCOREMANAGER_H
#define INCLUDED_GAMEENGINE_SCOREMANAGER_H

#include <memory>

namespace GameEngine
{
	class ScoreManager
	{
	private:
		class Impl;
		std::auto_ptr < ScoreManager::Impl >	m_pImpl;
	public:
		ScoreManager();
		~ScoreManager();
		void Add( int score );
		void Update();
		int GetScore() const;
		void Flush();
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_TIMER_H
#define INCLUDED_GAMEENGINE_TIMER_H

#include <time.h>

namespace GameEngine
{
	class Timer
	{
	private:
		::time_t			m_Begin;
		::time_t			m_End;

		::time_t			m_PauseBegin;
		::time_t			m_PauseEnd;

		int					m_PausedTime;
	public:
		Timer();
		~Timer();
		void Init();
		void Start();
		void Stop();
		void Pause();
		void Resume();
		int GetInterval() const;
	};
}

#endif
#include "Timer.h"

namespace GameEngine
{
	Timer::Timer() : m_Begin( 0 ), m_End( 0 ), m_PauseBegin( 0 ), m_PauseEnd( 0 ), m_PausedTime( 0 )
	{
	}

	Timer::~Timer()
	{
		m_Begin = 0;
		m_End = 0;
		m_PauseBegin = 0;
		m_PauseEnd = 0;
		m_PausedTime = 0;
	}

	void Timer::Init()
	{
		m_Begin = 0;
		m_End = 0;
		m_PauseBegin = 0;
		m_PauseEnd = 0;
		m_PausedTime = 0;
	}

	void Timer::Start()
	{
		::time( &m_Begin );
	}

	void Timer::Stop()
	{
		::time( &m_End );
	}

	void Timer::Pause()
	{
		::time( &m_PauseBegin );
	}

	void Timer::Resume()
	{
		::time( &m_PauseEnd );
		m_PausedTime = m_PauseEnd - m_PauseBegin;
	}

	int Timer::GetInterval() const
	{
		return m_End - m_Begin - m_PausedTime;
	}
}
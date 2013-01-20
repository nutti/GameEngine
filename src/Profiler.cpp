#include "Profiler.h"

#include <Windows.h>

namespace GameEngine
{
	Profiler::Profiler()
	{
		m_ProfileDataList.clear();
	}

	Profiler::~Profiler()
	{
		m_ProfileDataList.clear();
	}

	void Profiler::Begin( const std::string& name )
	{
		m_ProfileDataList[ name ].m_Begin = ::timeGetTime();
	}

	void Profiler::End( const std::string& name )
	{
		m_ProfileDataList[ name ].m_Time += ::timeGetTime() - m_ProfileDataList[ name ].m_Begin;
	}

	void Profiler::Clear()
	{
		m_ProfileDataList.clear();
	}

	int Profiler::GetProfile( const std::string& name )
	{
		return m_ProfileDataList[ name ].m_Time;
	}
}
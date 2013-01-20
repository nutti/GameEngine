#ifndef INCLUDED_GAMEENGINE_PROFILER_H
#define INCLUDED_GAMEENGINE_PROFILER_H

#include <string>
#include <map>

namespace GameEngine
{
	class Profiler
	{
	private:
		struct ProfileData
		{
			int			m_Begin;
			int			m_Time;
		};
		std::map < std::string, ProfileData >		m_ProfileDataList;
	public:
		Profiler();
		~Profiler();
		void Begin( const std::string& name );
		void End( const std::string& name );
		void Clear();
		int GetProfile( const std::string& name );
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_SCRIPTCOMPILER_H
#define INCLUDED_GAMEENGINE_SCRIPTCOMPILER_H

#include <memory>
#include <string>

#include "ScriptTypes.h"

namespace GameEngine
{
	class EventMediator;
	class ScriptCompiler
	{
		class Impl;
		std::auto_ptr < ScriptCompiler::Impl >		m_pImpl;
	public:
		ScriptCompiler();
		~ScriptCompiler();
		void Compile( const std::string& fileName );
		ScriptData GetScriptData();
	};
}

#endif
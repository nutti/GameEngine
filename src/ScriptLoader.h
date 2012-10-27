#ifndef INCLUDED_GAMEENGINE_SCRIPTLOADER_H
#define INCLUDED_GAMEENGINE_SCRIPTLOADER_H

#include <memory>
#include <string>

#include "ScriptTypes.h"

namespace GameEngine
{
	class ScriptLoader
	{
		class Impl;
		std::auto_ptr < ScriptLoader::Impl >		m_pImpl;
	public:
		ScriptLoader();
		~ScriptLoader();
		void Load( const std::string& fileName );
		void Load( int archiveHandle, const std::string& filePath );
		ScriptData GetScriptData();
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_SCRIPTMANAGER_H
#define INCLUDED_GAMEENGINE_SCRIPTMANAGER_H

#include <memory>
#include <string>

#include "ScriptTypes.h"

namespace GameEngine
{
	class EventMediator;
	class ScriptManager
	{
		class Impl;
		std::auto_ptr < ScriptManager::Impl >		m_pImpl;
	public:
		ScriptManager( std::shared_ptr < EventMediator > pEventMediator );
		~ScriptManager();
		void BuildFileStructure( const std::string& fileName, ScriptFileType type );	// スクリプトファイル構成の構築
		void BuildScriptData( int stage );												// スクリプトデータの構築
		ScriptData GetScriptData();														// スクリプトデータの取得
		// アーカイバ
		void BuildFileStructure( int archiveHandle, const std::string& fileName, ScriptFileType type );
		void BuildScriptData( int archiveHandle, int stage );
	};
}

#endif
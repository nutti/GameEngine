#ifndef INCLUDED_GAMEENGINE_RESOURCE_MANAGER_H
#define INCLUDED_GAMEENGINE_RESOURCE_MANAGER_H

#include <memory>

#include "ScriptTypes.h"
#include "ResourceTypes.h"

namespace GameEngine
{
	class ResourceManager
	{
		class Impl;
		std::auto_ptr < ResourceManager::Impl >		m_pImpl;
	public:
		ResourceManager();
		~ResourceManager();
		void LoadStageResources( const ScriptData& data );		// ステージリソースの読み込み
		void ReleaseStageResources();							// ステージリソースの解放
		ResourceMap GetStageResourceMap();	// ステージリソースMAPの取得
	};
}

#endif
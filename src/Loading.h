#ifndef INCLUDED_GAMEENGINE_LOADING_H
#define INCLUDED_GAMEENGINE_LOADING_H

#include <MAPIL/Thread/WinAPIThread.h>
#include <memory>
#include <string>

namespace GameEngine
{
	class ResourceManager;
	class ScriptManager;
	class Loading : public MAPIL::WinAPIThread
	{
	private:
		class Impl;
		std::auto_ptr < Loading::Impl >		m_pImpl;
	public:
		Loading();
		~Loading();
		void Init();
		int ThreadFunc();
		void SetupSession(	std::shared_ptr < ResourceManager > pResourceManager,
							std::shared_ptr < ScriptManager > pScriptManager );
		void AddGlobalResourceItem(	int resourceType,
									int index,
									const std::string& filePath,
									bool archived = false );
		void AddGlobalResourceItem(	int resourceType,
									int indexFirst,
									int indexLast,
									int column,
									int row,
									int width,
									int height,
									const std::string& filePath,
									bool archived = false );
		void AddStageResourceItem( int stage, bool archived = false );
		void CleanupSession();
		bool SessionEnded() const;
		void Start();
		void Terminate();
	};
}

#endif
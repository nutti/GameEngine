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
		void BuildFileStructure( const std::string& fileName, ScriptFileType type );	// �X�N���v�g�t�@�C���\���̍\�z
		void BuildScriptData( int stage );												// �X�N���v�g�f�[�^�̍\�z
		ScriptData GetScriptData();														// �X�N���v�g�f�[�^�̎擾
		// �A�[�J�C�o
		void BuildFileStructure( int archiveHandle, const std::string& fileName, ScriptFileType type );
		void BuildScriptData( int archiveHandle, int stage );
	};
}

#endif
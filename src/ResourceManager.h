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
		void LoadStageResources( const ScriptData& data );		// �X�e�[�W���\�[�X�̓ǂݍ���
		void ReleaseStageResources();							// �X�e�[�W���\�[�X�̉��
		ResourceMap GetStageResourceMap();						// �X�e�[�W���\�[�XMAP�̎擾
		void LoadGlobalResource( int resourceType, int index, const std::string& fileName );	// �O���[�o���p���\�[�X�̓ǂݍ���
		void LoadGlobalResource(	int resourceType,
									int indexFirst,
									int indexLast,
									int column,
									int row,
									int width,
									int height,
									const std::string& fileName );
		void AddGlobalTextureAtlasItem( int index, int texID, int x, int y, int width, int height );
		// �A�[�J�C�o
		void OpenArchive( const std::string& fileName );
		void LoadStageResourcesFromArchive( const ScriptData& data );
		void LoadGlobalResourceFromArchive( int resourceType, int index, const std::string& fileName );
		void LoadGlobalResourceFromArchive(	int resourceType,
											int indexFirst,
											int indexLast,
											int column,
											int row,
											int width,
											int height,
											const std::string& fileName );
		int GetArchiveHandle() const;
		void SetBGMVolume( int volume );
		void SetSEVolume( int volume );
	};
}

#endif
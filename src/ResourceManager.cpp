#include <MAPIL/MAPIL.h>

#include "ResourceManager.h"

namespace GameEngine
{
	// GeneralButtonManager�����N���X
	class ResourceManager::Impl
	{
	private:
		ResourceMap			m_ResourceMap;		// ���\�[�X�Ή��֌W
	public:
		Impl();
		~Impl(){}
		void LoadStageResources( const ScriptData& data );			// �X�e�[�W���\�[�X�̓ǂݍ���
		void ReleaseStageResources();								// �X�e�[�W���\�[�X�̉��
		ResourceMap GetStageResourceMap();		// �X�e�[�W���\�[�XMAP�̎擾
	};

	ResourceManager::Impl::Impl()
	{
	}

	void ResourceManager::Impl::LoadStageResources( const ScriptData& data )
	{
		m_ResourceMap.m_pElm.reset( new ResourceMap::ResourceMapElm );

		// �e�탊�\�[�X�̓ǂݍ���
		const int INITIAL_TEXTURE_MAP_RESERVE_CAP = 50;			// �����̃e�N�X�`��MAP���e��
		const int INITIAL_SE_MAP_RESERVE_CAP = 50;				// ������SEMAP���e��
		const int INITIAL_BGM_MAP_RESERVE_CAP = 50;				// ������BGMMAP���e��
		m_ResourceMap.m_pElm->m_TextureMap.resize( INITIAL_TEXTURE_MAP_RESERVE_CAP, -1 );
		m_ResourceMap.m_pElm->m_SEMap.resize( INITIAL_SE_MAP_RESERVE_CAP, -1 );
		m_ResourceMap.m_pElm->m_BGMMap.resize( INITIAL_BGM_MAP_RESERVE_CAP, -1 );
		std::shared_ptr < ResourceScriptData > pScriptData = data.m_pResourceScriptData;
		// �e�N�X�`���̓ǂݍ���
		typedef std::map < int, std::string > ::iterator	TextureIter;
		for(	TextureIter it = pScriptData->m_TextureList.begin();
				it != pScriptData->m_TextureList.end();
				++it ){
			// ���e�l�𒴂����C���f�b�N�X���K�v�ȏꍇ�́A�w�肳�ꂽ�C���f�b�N�X��2�{�̃T�C�Y��resize����B
			if( it->first > m_ResourceMap.m_pElm->m_TextureMap.size() ){
				m_ResourceMap.m_pElm->m_TextureMap.resize( it->first * 2 );
			}
			//m_pResourceMap->m_TextureMap[ it->first ] = MAPIL::CreateTexture( m_Archiver, it->second.c_str() );
		}
		// SE�̓ǂݍ���
		typedef std::map < int, std::string > ::iterator	SEIter;
		for(	SEIter it = pScriptData->m_SEList.begin();
				it != pScriptData->m_SEList.end();
				++it ){
			if( it->first > m_ResourceMap.m_pElm->m_SEMap.size() ){
				m_ResourceMap.m_pElm->m_SEMap.resize( it->first * 2 );
			}
			//int id = MAPIL::CreateStaticBuffer( m_Archiver, it->second.c_str() );
			//MAPIL::SetStaticBufferVolume( id, ResourceHandler::GetInst()->m_pGameManager->GetSEVolume() );
			//m_ResourceMap.m_SEMap[ it->first ] = id;
		}
		// BGM�̓ǂݍ���
		typedef std::map < int, std::string > ::iterator	BGMIter;
		for(	BGMIter it = pScriptData->m_BGMList.begin();
				it != pScriptData->m_BGMList.end();
				++it ){
			if( it->first > m_ResourceMap.m_pElm->m_BGMMap.size() ){
				m_ResourceMap.m_pElm->m_BGMMap.resize( it->first * 2 );
			}
			//int id = MAPIL::CreateStreamingBuffer( m_Archiver, it->second.c_str() );
			//MAPIL::SetStreamingBufferVolume( id, ResourceHandler::GetInst()->m_pGameManager->GetBGMVolume() );
			//m_ResourceMap.m_BGMMap[ it->first ] = id;
		}
	}

	void ResourceManager::Impl::ReleaseStageResources()
	{
	}

	ResourceMap ResourceManager::Impl::GetStageResourceMap()
	{
		return m_ResourceMap;
	}


	// ----------------------------------
	// �����N���X�̌Ăяo��
	// ----------------------------------


	ResourceManager::ResourceManager() : m_pImpl( new ResourceManager::Impl )
	{
	}

	ResourceManager::~ResourceManager()
	{
	}

	void ResourceManager::LoadStageResources( const ScriptData& data )
	{
		m_pImpl->LoadStageResources( data );
	}

	void ResourceManager::ReleaseStageResources()
	{
		m_pImpl->ReleaseStageResources();
	}

	ResourceMap ResourceManager::GetStageResourceMap()
	{
		return m_pImpl->GetStageResourceMap();
	}

}
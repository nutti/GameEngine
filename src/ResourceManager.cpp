#include <MAPIL/MAPIL.h>

#include "ResourceManager.h"

namespace GameEngine
{
	// GeneralButtonManager�����N���X
	class ResourceManager::Impl
	{
	private:
		ResourceMap			m_ResourceMap;		// ���\�[�X�Ή��֌W
		int					m_ArchiveHandle;	// �A�[�J�C�o�̃n���h��
	public:
		Impl();
		~Impl(){}
		void LoadStageResources( const ScriptData& data );			// �X�e�[�W���\�[�X�̓ǂݍ���
		void ReleaseStageResources();								// �X�e�[�W���\�[�X�̉��
		ResourceMap GetStageResourceMap();		// �X�e�[�W���\�[�XMAP�̎擾
		void LoadGlobalResource( int resourceType, int index, const std::string& fileName );	// �O���[�o���p���\�[�X�̓ǂݍ���
		void LoadGlobalResource(	int resourceType,
									int indexFirst,
									int indexLast,
									int column,
									int row,
									int width,
									int height,
									const std::string& fileName );
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
	};

	ResourceManager::Impl::Impl()
	{
		m_ArchiveHandle = -1;

		m_ResourceMap.m_pGlobalResourceMap.reset( new ResourceMap::GlobalResourceMapElm );

		const int INITIAL_TEXTURE_MAP_RESERVE_CAP = 100;			// �����̃e�N�X�`��MAP���e��
		const int INITIAL_SE_MAP_RESERVE_CAP = 50;				// ������SEMAP���e��
		const int INITIAL_BGM_MAP_RESERVE_CAP = 50;				// ������BGMMAP���e��
		const int INITIAL_MODEL_MAP_RESERVE_CAP = 50;			// �����̃��f��MAP���e��
		m_ResourceMap.m_pGlobalResourceMap->m_TextureMap.resize( INITIAL_TEXTURE_MAP_RESERVE_CAP, -1 );
		m_ResourceMap.m_pGlobalResourceMap->m_SEMap.resize( INITIAL_SE_MAP_RESERVE_CAP, -1 );
		m_ResourceMap.m_pGlobalResourceMap->m_BGMMap.resize( INITIAL_BGM_MAP_RESERVE_CAP, -1 );
		m_ResourceMap.m_pGlobalResourceMap->m_ModelMap.resize( INITIAL_MODEL_MAP_RESERVE_CAP, -1 );


		m_ResourceMap.m_pStageResourceMap.reset( new ResourceMap::StageResourceMapElm );

		const int INITIAL_STAGE_TEXTURE_MAP_RESERVE_CAP = 100;				// �����̃e�N�X�`��MAP���e��
		const int INITIAL_STAGE_SE_MAP_RESERVE_CAP = 50;					// ������SEMAP���e��
		const int INITIAL_STAGE_BGM_MAP_RESERVE_CAP = 50;					// ������BGMMAP���e��
		const int INITIAL_STAGE_MODEL_MAP_RESERVE_CAP = 50;					// �����̃��f��MAP���e��
		m_ResourceMap.m_pStageResourceMap->m_TextureMap.resize( INITIAL_STAGE_TEXTURE_MAP_RESERVE_CAP, -1 );
		m_ResourceMap.m_pStageResourceMap->m_SEMap.resize( INITIAL_STAGE_SE_MAP_RESERVE_CAP, -1 );
		m_ResourceMap.m_pStageResourceMap->m_BGMMap.resize( INITIAL_STAGE_BGM_MAP_RESERVE_CAP, -1 );
		m_ResourceMap.m_pStageResourceMap->m_ModelMap.resize( INITIAL_STAGE_MODEL_MAP_RESERVE_CAP, -1 );
	}

	void ResourceManager::Impl::LoadStageResources( const ScriptData& data )
	{
		// �e�탊�\�[�X�̓ǂݍ���
		std::shared_ptr < ResourceScriptData > pScriptData = data.m_pResourceScriptData;
		// �e�N�X�`���̓ǂݍ���
		typedef std::map < int, std::string > ::iterator	TextureIter;
		for(	TextureIter it = pScriptData->m_TextureList.begin();
				it != pScriptData->m_TextureList.end();
				++it ){
			// ���e�l�𒴂����C���f�b�N�X���K�v�ȏꍇ�́A�w�肳�ꂽ�C���f�b�N�X��2�{�̃T�C�Y��resize����B
			if( it->first > m_ResourceMap.m_pStageResourceMap->m_TextureMap.size() ){
				m_ResourceMap.m_pStageResourceMap->m_TextureMap.resize( it->first * 2 );
			}
			m_ResourceMap.m_pStageResourceMap->m_TextureMap[ it->first ] = MAPIL::CreateTexture( it->second.c_str() );
		}
		// SE�̓ǂݍ���
		typedef std::map < int, std::string > ::iterator	SEIter;
		for(	SEIter it = pScriptData->m_SEList.begin();
				it != pScriptData->m_SEList.end();
				++it ){
			if( it->first > m_ResourceMap.m_pStageResourceMap->m_SEMap.size() ){
				m_ResourceMap.m_pStageResourceMap->m_SEMap.resize( it->first * 2 );
			}
			int id = MAPIL::CreateStaticBuffer( it->second.c_str() );
			m_ResourceMap.m_pStageResourceMap->m_SEMap[ it->first ] = id;
		}
		// BGM�̓ǂݍ���
		typedef std::map < int, std::string > ::iterator	BGMIter;
		for(	BGMIter it = pScriptData->m_BGMList.begin();
				it != pScriptData->m_BGMList.end();
				++it ){
			if( it->first > m_ResourceMap.m_pStageResourceMap->m_BGMMap.size() ){
				m_ResourceMap.m_pStageResourceMap->m_BGMMap.resize( it->first * 2 );
			}
			int id = MAPIL::CreateStreamingBuffer( it->second.c_str() );
			m_ResourceMap.m_pStageResourceMap->m_BGMMap[ it->first ] = id;
		}
		// 3D���f���̓ǂݍ���
		typedef std::map < int, std::string > ::iterator	ModelIter;
		for(	ModelIter it = pScriptData->m_ModelList.begin();
				it != pScriptData->m_ModelList.end();
				++it ){
			if( it->first > m_ResourceMap.m_pStageResourceMap->m_ModelMap.size() ){
				m_ResourceMap.m_pStageResourceMap->m_ModelMap.resize( it->first * 2 );
			}
			int id = MAPIL::CreateModel( it->second.c_str() );
			m_ResourceMap.m_pStageResourceMap->m_ModelMap[ it->first ] = id;
		}
	}

	void ResourceManager::Impl::ReleaseStageResources()
	{
		// �e�N�X�`���̍폜
		for( int i = 0; i < m_ResourceMap.m_pStageResourceMap->m_TextureMap.size(); ++i ){
			if( m_ResourceMap.m_pStageResourceMap->m_TextureMap[ i ] != -1 ){
				MAPIL::DeleteTexture( m_ResourceMap.m_pStageResourceMap->m_TextureMap[ i ] );
				m_ResourceMap.m_pStageResourceMap->m_TextureMap[ i ] = -1;
			}
		}
		// SE�̍폜
		for( int i = 0; i < m_ResourceMap.m_pStageResourceMap->m_SEMap.size(); ++i ){
			if( m_ResourceMap.m_pStageResourceMap->m_SEMap[ i ] != -1 ){
				MAPIL::DeleteStaticBuffer( m_ResourceMap.m_pStageResourceMap->m_SEMap[ i ] );
				m_ResourceMap.m_pStageResourceMap->m_SEMap[ i ] = -1;
			}
		}
		// BGM�̍폜
		for( int i = 0; i < m_ResourceMap.m_pStageResourceMap->m_BGMMap.size(); ++i ){
			if( m_ResourceMap.m_pStageResourceMap->m_BGMMap[ i ] != -1 ){
				MAPIL::DeleteStreamingBuffer( m_ResourceMap.m_pStageResourceMap->m_BGMMap[ i ] );
				m_ResourceMap.m_pStageResourceMap->m_BGMMap[ i ] = -1;
			}
		}
		// ���f���̍폜
		for( int i = 0; i < m_ResourceMap.m_pStageResourceMap->m_ModelMap.size(); ++i ){
			if( m_ResourceMap.m_pStageResourceMap->m_ModelMap[ i ] != -1 ){
				MAPIL::DeleteModel( m_ResourceMap.m_pStageResourceMap->m_ModelMap[ i ] );
				m_ResourceMap.m_pStageResourceMap->m_ModelMap[ i ] = -1;
			}
		}
	}

	ResourceMap ResourceManager::Impl::GetStageResourceMap()
	{
		return m_ResourceMap;
	}

	void ResourceManager::Impl::LoadGlobalResource( int resourceType, int index, const std::string& fileName )
	{
		switch( resourceType ){
			case RESOURCE_TYPE_BGM:{
				int id = MAPIL::CreateStreamingBuffer( fileName.c_str() );
				m_ResourceMap.m_pGlobalResourceMap->m_BGMMap[ index ] = id;
				break;
			}
			case RESOURCE_TYPE_SE:{
				int id = MAPIL::CreateStaticBuffer( fileName.c_str() );
				m_ResourceMap.m_pGlobalResourceMap->m_SEMap[ index ] = id;
				break;
			}
			case RESOURCE_TYPE_TEXTURE:{
				int id = MAPIL::CreateTexture( fileName.c_str() );
				m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ index ] = id;
				break;
			}
			case RESOURCE_TYPE_MODEL:{
				int id = MAPIL::CreateModel( fileName.c_str() );
				m_ResourceMap.m_pGlobalResourceMap->m_ModelMap[ index ] = id;
				break;
			}
			default:
				break;
		}
	}

	void ResourceManager::Impl::LoadGlobalResource(	int resourceType,
													int indexFirst,
													int indexLast,
													int column,
													int row,
													int width,
													int height,
													const std::string& fileName )
	{
		switch( resourceType ){
			case RESOURCE_TYPE_MULTI_TEXTURE:{
				int* pID = new int [ column * row ];
				MAPIL::CreateSplitedTexture( pID, fileName.c_str(), column, row, width, height );
				for( int i = 0; i < indexLast - indexFirst; ++i ){
					m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ indexFirst + i ] = pID[ i ];
				}
				MAPIL::SafeDeleteArray( pID );
				break;
			}
			default:
				break;
		}
	}

	void ResourceManager::Impl::OpenArchive( const std::string& fileName )
	{
		m_ArchiveHandle = MAPIL::OpenArchiveFile( fileName.c_str(), MAPIL::FILE_OPEN_READ_MODE );
	}

	void ResourceManager::Impl::LoadStageResourcesFromArchive( const ScriptData& data )
	{
		if( m_ArchiveHandle == -1 ){
			exit( -1 );
		}

		// �e�탊�\�[�X�̓ǂݍ���
		std::shared_ptr < ResourceScriptData > pScriptData = data.m_pResourceScriptData;
		// �e�N�X�`���̓ǂݍ���
		typedef std::map < int, std::string > ::iterator	TextureIter;
		for(	TextureIter it = pScriptData->m_TextureList.begin();
				it != pScriptData->m_TextureList.end();
				++it ){
			// ���e�l�𒴂����C���f�b�N�X���K�v�ȏꍇ�́A�w�肳�ꂽ�C���f�b�N�X��2�{�̃T�C�Y��resize����B
			if( it->first > m_ResourceMap.m_pStageResourceMap->m_TextureMap.size() ){
				m_ResourceMap.m_pStageResourceMap->m_TextureMap.resize( it->first * 2 );
			}
			m_ResourceMap.m_pStageResourceMap->m_TextureMap[ it->first ] = MAPIL::CreateTexture( m_ArchiveHandle, it->second.c_str() );
		}
		// SE�̓ǂݍ���
		typedef std::map < int, std::string > ::iterator	SEIter;
		for(	SEIter it = pScriptData->m_SEList.begin();
				it != pScriptData->m_SEList.end();
				++it ){
			if( it->first > m_ResourceMap.m_pStageResourceMap->m_SEMap.size() ){
				m_ResourceMap.m_pStageResourceMap->m_SEMap.resize( it->first * 2 );
			}
			int id = MAPIL::CreateStaticBuffer( m_ArchiveHandle, it->second.c_str() );
			m_ResourceMap.m_pStageResourceMap->m_SEMap[ it->first ] = id;
		}
		// BGM�̓ǂݍ���
		typedef std::map < int, std::string > ::iterator	BGMIter;
		for(	BGMIter it = pScriptData->m_BGMList.begin();
				it != pScriptData->m_BGMList.end();
				++it ){
			if( it->first > m_ResourceMap.m_pStageResourceMap->m_BGMMap.size() ){
				m_ResourceMap.m_pStageResourceMap->m_BGMMap.resize( it->first * 2 );
			}
			int id = MAPIL::CreateStreamingBuffer( m_ArchiveHandle, it->second.c_str() );
			m_ResourceMap.m_pStageResourceMap->m_BGMMap[ it->first ] = id;
		}
	}

	void ResourceManager::Impl::LoadGlobalResourceFromArchive( int resourceType, int index, const std::string& fileName )
	{
		if( m_ArchiveHandle == -1 ){
			exit( -1 );
		}

		switch( resourceType ){
			case RESOURCE_TYPE_BGM:{
				int id = MAPIL::CreateStreamingBuffer( m_ArchiveHandle, fileName.c_str() );
				m_ResourceMap.m_pGlobalResourceMap->m_BGMMap[ index ] = id;
				break;
			}
			case RESOURCE_TYPE_SE:{
				int id = MAPIL::CreateStaticBuffer( m_ArchiveHandle, fileName.c_str() );
				m_ResourceMap.m_pGlobalResourceMap->m_SEMap[ index ] = id;
				break;
			}
			case RESOURCE_TYPE_TEXTURE:{
				int id = MAPIL::CreateTexture( m_ArchiveHandle, fileName.c_str() );
				m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ index ] = id;
				break;
			}
			default:
				break;
		}
	}

	void ResourceManager::Impl::LoadGlobalResourceFromArchive(	int resourceType,
																int indexFirst,
																int indexLast,
																int column,
																int row,
																int width,
																int height,
																const std::string& fileName )
	{
		if( m_ArchiveHandle == -1 ){
			exit( -1 );
		}

		switch( resourceType ){
			case RESOURCE_TYPE_MULTI_TEXTURE:{
				int* pID = new int [ column * row ];
				MAPIL::CreateSplitedTexture( pID, m_ArchiveHandle, fileName.c_str(), column, row, width, height );
				for( int i = 0; i < indexLast - indexFirst; ++i ){
					m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ indexFirst + i ] = pID[ i ];
				}
				MAPIL::SafeDeleteArray( pID );
				break;
			}
			default:
				break;
		}
	}

	int ResourceManager::Impl::GetArchiveHandle() const
	{
		return m_ArchiveHandle;
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

	void ResourceManager::LoadGlobalResource( int resourceType, int index, const std::string& fileName )
	{
		m_pImpl->LoadGlobalResource( resourceType, index, fileName );
	}

	void ResourceManager::LoadGlobalResource(	int resourceType,
												int indexFirst,
												int indexLast,
												int column,
												int row,
												int width,
												int height,
												const std::string& fileName )
	{
		m_pImpl->LoadGlobalResource( resourceType, indexFirst, indexLast, column, row, width, height, fileName );
	}

	void ResourceManager::OpenArchive( const std::string& fileName )
	{
		m_pImpl->OpenArchive( fileName );
	}

	void ResourceManager::LoadStageResourcesFromArchive( const ScriptData& data )
	{
		m_pImpl->LoadStageResourcesFromArchive( data );
	}

	void ResourceManager::LoadGlobalResourceFromArchive( int resourceType, int index, const std::string& fileName )
	{
		m_pImpl->LoadGlobalResourceFromArchive( resourceType, index, fileName );
	}

	void ResourceManager::LoadGlobalResourceFromArchive(	int resourceType,
															int indexFirst,
															int indexLast,
															int column,
															int row,
															int width,
															int height,
															const std::string& fileName )
	{
		m_pImpl->LoadGlobalResourceFromArchive( resourceType, indexFirst, indexLast, column, row, width, height, fileName );
	}

	int ResourceManager::GetArchiveHandle() const
	{
		return m_pImpl->GetArchiveHandle();
	}

}
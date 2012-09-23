#include <MAPIL/MAPIL.h>

#include "ResourceManager.h"

namespace GameEngine
{
	// GeneralButtonManager実装クラス
	class ResourceManager::Impl
	{
	private:
		ResourceMap			m_ResourceMap;		// リソース対応関係
	public:
		Impl();
		~Impl(){}
		void LoadStageResources( const ScriptData& data );			// ステージリソースの読み込み
		void ReleaseStageResources();								// ステージリソースの解放
		ResourceMap GetStageResourceMap();		// ステージリソースMAPの取得
	};

	ResourceManager::Impl::Impl()
	{
	}

	void ResourceManager::Impl::LoadStageResources( const ScriptData& data )
	{
		m_ResourceMap.m_pElm.reset( new ResourceMap::ResourceMapElm );

		// 各種リソースの読み込み
		const int INITIAL_TEXTURE_MAP_RESERVE_CAP = 50;			// 初期のテクスチャMAP許容量
		const int INITIAL_SE_MAP_RESERVE_CAP = 50;				// 初期のSEMAP許容量
		const int INITIAL_BGM_MAP_RESERVE_CAP = 50;				// 初期のBGMMAP許容量
		m_ResourceMap.m_pElm->m_TextureMap.resize( INITIAL_TEXTURE_MAP_RESERVE_CAP, -1 );
		m_ResourceMap.m_pElm->m_SEMap.resize( INITIAL_SE_MAP_RESERVE_CAP, -1 );
		m_ResourceMap.m_pElm->m_BGMMap.resize( INITIAL_BGM_MAP_RESERVE_CAP, -1 );
		std::shared_ptr < ResourceScriptData > pScriptData = data.m_pResourceScriptData;
		// テクスチャの読み込み
		typedef std::map < int, std::string > ::iterator	TextureIter;
		for(	TextureIter it = pScriptData->m_TextureList.begin();
				it != pScriptData->m_TextureList.end();
				++it ){
			// 許容値を超えたインデックスが必要な場合は、指定されたインデックスの2倍のサイズのresizeする。
			if( it->first > m_ResourceMap.m_pElm->m_TextureMap.size() ){
				m_ResourceMap.m_pElm->m_TextureMap.resize( it->first * 2 );
			}
			//m_pResourceMap->m_TextureMap[ it->first ] = MAPIL::CreateTexture( m_Archiver, it->second.c_str() );
		}
		// SEの読み込み
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
		// BGMの読み込み
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
	// 実装クラスの呼び出し
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
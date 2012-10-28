#include "Loading.h"

#include <MAPIL/MAPIL.h>

#include "ResourceManager.h"
#include "ScriptManager.h"

namespace GameEngine
{
	struct GlobalResourceItem
	{
		int				m_ResourceType;		// リソースタイプ
		int				m_Index;			// リソースインデックス
		std::string		m_FilePath;			// ファイルパス
		bool			m_Archived;			// アーカイブ化されている場合true
	};

	struct GlobalMultiTextureItem
	{
		int				m_ResourceType;		// リソースタイプ
		int				m_FirstIndex;		// 最初のインデックス
		int				m_LastIndex;		// 最後のインデックス
		int				m_Column;			// 横
		int				m_Row;				// 縦
		int				m_Width;			// 幅
		int				m_Height;			// 高さ
		std::string		m_FilePath;			// ファイルパス
		bool			m_Archived;			// アーカイブ化されている場合true
	};

	struct StageResourceItem
	{
		int				m_Stage;			// ステージ番号
		bool			m_Archived;			// アーカイブ化されている場合true
	};

	enum LoadingState
	{
		LOADING_STATE_LOADING		= 0,
		LOADING_STATE_STOPPED		= 1,
		LOADING_STATE_SESSION_END	= 2,
		LOADING_STATE_UNINITIALIZED	= 3,
	};

	class Loading::Impl
	{
	private:
		bool									m_HasTermSig;		// 終了通知が来た場合
		LoadingState							m_State;			// 状態

		std::shared_ptr < ResourceManager >		m_pResourceManager;
		std::shared_ptr < ScriptManager >		m_pScriptManager;
		std::vector < GlobalResourceItem >		m_GlobalResourceList;
		std::vector < GlobalMultiTextureItem >	m_GlobalMultiTextureList;
		std::vector < StageResourceItem >		m_StageResourceList;

		::HANDLE		m_Mutex;

		void Cleanup();
	public:
		Impl();
		~Impl();
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

	Loading::Impl::Impl()
	{
		m_HasTermSig = false;
		Cleanup();
		m_Mutex = ::CreateMutex( NULL, FALSE, TSTR( "Mutex" ) );
	}

	Loading::Impl::~Impl()
	{
		Cleanup();
		::CloseHandle( m_Mutex );
	}

	void Loading::Impl::Cleanup()
	{
		m_GlobalResourceList.clear();
		m_StageResourceList.clear();
		m_GlobalMultiTextureList.clear();
		m_pResourceManager.reset();
		m_pScriptManager.reset();
		::WaitForSingleObject( m_Mutex, INFINITE );
		m_State = LOADING_STATE_UNINITIALIZED;
		::ReleaseMutex( m_Mutex );
	}

	void Loading::Impl::Init()
	{
		Cleanup();
	}

	int Loading::Impl::ThreadFunc()
	{
		if( m_HasTermSig ){
			return 1;
		}

		::WaitForSingleObject( m_Mutex, INFINITE );
		bool result = ( m_State == LOADING_STATE_LOADING );
		::ReleaseMutex( m_Mutex );

		if( result ){
			for( int i = 0; i < m_GlobalResourceList.size(); ++i ){
				GlobalResourceItem item = m_GlobalResourceList[ i ];
				if( item.m_Archived ){
					m_pResourceManager->LoadGlobalResourceFromArchive( item.m_ResourceType, item.m_Index, item.m_FilePath );
				}
				else{
					m_pResourceManager->LoadGlobalResource( item.m_ResourceType, item.m_Index, item.m_FilePath );
				}
			}
			for( int i = 0; i < m_GlobalMultiTextureList.size(); ++i ){
				GlobalMultiTextureItem item = m_GlobalMultiTextureList[ i ];
				if( item.m_Archived ){
					m_pResourceManager->LoadGlobalResourceFromArchive(	item.m_ResourceType,
																		item.m_FirstIndex, item.m_LastIndex,
																		item.m_Column, item.m_Row,
																		item.m_Width, item.m_Height,
																		item.m_FilePath );
				}
				else{
					m_pResourceManager->LoadGlobalResource(	item.m_ResourceType,
															item.m_FirstIndex, item.m_LastIndex,
															item.m_Column, item.m_Row,
															item.m_Width, item.m_Height,
															item.m_FilePath );
				}
			}
			for( int i = 0; i < m_StageResourceList.size(); ++i ){
				StageResourceItem item = m_StageResourceList[ i ];
				if( item.m_Archived ){
					m_pScriptManager->BuildScriptData( m_pResourceManager->GetArchiveHandle(), m_StageResourceList[ i ].m_Stage );
					m_pResourceManager->LoadStageResourcesFromArchive( m_pScriptManager->GetScriptData() );
				}
				else{
					m_pScriptManager->BuildScriptData( m_StageResourceList[ i ].m_Stage );
					m_pResourceManager->LoadStageResources( m_pScriptManager->GetScriptData() );
				}
			}
			::WaitForSingleObject( m_Mutex, INFINITE );
			m_State = LOADING_STATE_SESSION_END;
			::ReleaseMutex( m_Mutex );
		}
		else{
			Sleep( 1 );
		}

		return 0;
	}

	void Loading::Impl::SetupSession(	std::shared_ptr < ResourceManager > pResourceManager,
										std::shared_ptr < ScriptManager > pScriptManager )
	{
		Cleanup();
		m_pResourceManager = pResourceManager;
		m_pScriptManager = pScriptManager;
		::WaitForSingleObject( m_Mutex, INFINITE );
		m_State = LOADING_STATE_STOPPED;
		::ReleaseMutex( m_Mutex );
	}

	void Loading::Impl::AddGlobalResourceItem(	int resourceType,
												int index,
												const std::string& filePath,
												bool archived )
	{
		GlobalResourceItem item;
		item.m_ResourceType = resourceType;
		item.m_Index = index;
		item.m_FilePath = filePath;
		item.m_Archived = archived;
		m_GlobalResourceList.push_back( item );
	}

	void Loading::Impl::AddGlobalResourceItem(	int resourceType,
												int indexFirst,
												int indexLast,
												int column,
												int row,
												int width,
												int height,
												const std::string& filePath,
												bool archived )
	{
		GlobalMultiTextureItem item;
		item.m_ResourceType = resourceType;
		item.m_FirstIndex = indexFirst;
		item.m_LastIndex = indexLast;
		item.m_Column = column;
		item.m_Row = row;
		item.m_Width = width;
		item.m_Height = height;
		item.m_FilePath = filePath;
		item.m_Archived = archived;
		m_GlobalMultiTextureList.push_back( item );
	}

	void Loading::Impl::AddStageResourceItem( int stage, bool archived )
	{
		StageResourceItem item;
		item.m_Stage = stage;
		item.m_Archived = archived;
		m_StageResourceList.push_back( item );
	}

	void Loading::Impl::CleanupSession()
	{
		Cleanup();
	}

	bool Loading::Impl::SessionEnded() const
	{
		::WaitForSingleObject( m_Mutex, INFINITE );
		bool result = ( m_State == LOADING_STATE_SESSION_END || m_State == LOADING_STATE_UNINITIALIZED );
		::ReleaseMutex( m_Mutex );
		return result;
	}

	void Loading::Impl::Start()
	{
		::WaitForSingleObject( m_Mutex, INFINITE );
		if( m_State == LOADING_STATE_STOPPED ){
			m_State = LOADING_STATE_LOADING;
		}
		::ReleaseMutex( m_Mutex );
	}

	void Loading::Impl::Terminate()
	{
		m_HasTermSig = true;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	Loading::Loading() : m_pImpl( new Loading::Impl )
	{
	}

	Loading::~Loading()
	{
	}

	void Loading::Init()
	{
		m_pImpl->Init();
	}

	int Loading::ThreadFunc()
	{
		while( !m_pImpl->ThreadFunc() ){
		}

		return 0;
	}

	void Loading::SetupSession(	std::shared_ptr < ResourceManager > pResourceManager,
								std::shared_ptr < ScriptManager > pScriptManager )
	{
		m_pImpl->SetupSession( pResourceManager, pScriptManager );
	}

	void Loading::AddGlobalResourceItem(	int resourceType,
											int index,
											const std::string& filePath,
											bool archived )
	{
		m_pImpl->AddGlobalResourceItem( resourceType, index, filePath, archived );
	}

	void Loading::AddGlobalResourceItem(	int resourceType,
											int indexFirst,
											int indexLast,
											int column,
											int row,
											int width,
											int height,
											const std::string& filePath,
											bool archived )
	{
		m_pImpl->AddGlobalResourceItem(	resourceType, indexFirst, indexLast,
										column, row, width, height, filePath, archived );
	}

	void Loading::AddStageResourceItem( int stage, bool archived )
	{
		m_pImpl->AddStageResourceItem( stage, archived );
	}

	void Loading::CleanupSession()
	{
		m_pImpl->CleanupSession();
	}

	bool Loading::SessionEnded() const
	{
		return m_pImpl->SessionEnded();
	}

	void Loading::Start()
	{
		m_pImpl->Start();
	}

	void Loading::Terminate()
	{
		m_pImpl->Terminate();
	}
}
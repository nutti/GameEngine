#include <MAPIL/MAPIL.h>

#include "InputTypes.h"

#include "FileInputStateHolder.h"
#include "ReplayDataLoader.h"

namespace GameEngine
{
	class FileInputStateHolder::Impl
	{
	private:
		ButtonPushedStatus	m_ButtonStatus;
		ReplayDataLoader	m_Loader;
		int					m_Frame;
		int					m_StageNo;
	public:
		Impl();
		~Impl(){}
		void Init();
		void Update();
		void Cleanup();
		ButtonPushedStatus GetButtonStatus();
		void LoadFile( int fileNo );
		void SetStageNo( int stageNo );
	};


	FileInputStateHolder::Impl::Impl()
	{
		Cleanup();
	}

	void FileInputStateHolder::Impl::Init()
	{
		Cleanup();
	}

	void FileInputStateHolder::Impl::Update()
	{
		m_ButtonStatus = m_Loader.GetButtonState( m_StageNo, m_Frame );
		++m_Frame;
	}

	void FileInputStateHolder::Impl::Cleanup()
	{
		m_Loader.Cleanup();
		m_Frame = 0;
		m_ButtonStatus = 0;
		m_StageNo = 0;
	}

	ButtonPushedStatus FileInputStateHolder::Impl::GetButtonStatus()
	{
		return m_ButtonStatus;
	}

	void FileInputStateHolder::Impl::LoadFile( int entryNo )
	{
		Cleanup();

		std::string fileName = REPLAY_FILE_DIR;
		fileName += '/';
		fileName += REPLAY_FILE_NAME_PREFIX;
		fileName += ( entryNo / 10 ) + '0';
		fileName += ( entryNo % 10 ) + '0';
		fileName += REPLAY_FILE_NAME_SUFFIX;

		m_Loader.Load( fileName );
	}

	void FileInputStateHolder::Impl::SetStageNo( int stageNo )
	{
		m_Frame = 0;
		m_ButtonStatus = 0;
		m_StageNo = stageNo;
	}


	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	FileInputStateHolder::FileInputStateHolder() : m_pImpl( new FileInputStateHolder::Impl )
	{
	}

	FileInputStateHolder::~FileInputStateHolder()
	{
	}

	void FileInputStateHolder::Init()
	{
		m_pImpl->Init();
	}

	void FileInputStateHolder::Update()
	{
		m_pImpl->Update();
	}

	ButtonPushedStatus FileInputStateHolder::GetButtonState()
	{
		return m_pImpl->GetButtonStatus();
	}

	void FileInputStateHolder::LoadFile( int entryNo )
	{
		m_pImpl->LoadFile( entryNo );
	}

	void FileInputStateHolder::SetStageNo( int stageNo )
	{
		m_pImpl->SetStageNo( stageNo );
	}
}
#include "StageBackground.h"

#include "Stage.h"
#include "ScriptTypes.h"
#include "StageBackgroundVCPU.h"

namespace GameEngine
{
	class StageBackground::Impl
	{
	private:
		StageBackgroundData		m_Data;
		ScriptData				m_ScriptData;
		StageBackgroundVCPU		m_VM;			// 仮想マシン
		bool					m_HasTermSig;
	public:
		Impl( int stageNo );
		~Impl();
		void Init();
		void Draw();
		void Update();
		void AttachStageData( StageData* pData );
		void AttachScriptData( const ScriptData& data );
		void Terminate();
	};

	StageBackground::Impl::Impl( int stageNo )
	{
		m_Data.m_Counter = 0;
		m_Data.m_Stage = stageNo;
		m_HasTermSig = false;
	}

	StageBackground::Impl::~Impl()
	{
	}

	void StageBackground::Impl::Init()
	{
		m_VM.Init( &m_ScriptData.m_pStageBackgroundScriptData->m_Data, &m_Data );
	}

	void StageBackground::Impl::Draw()
	{
		if( m_HasTermSig ){
			return ;
		}

		// スクリプトの実行
		m_VM.Run();
		if( m_VM.Terminated() ){
			exit( 1 );
		}
	}

	void StageBackground::Impl::Update()
	{
		++m_Data.m_Counter;
	}

	void StageBackground::Impl::AttachStageData( StageData* pData )
	{
		m_Data.m_pStageData = pData;
	}

	void StageBackground::Impl::AttachScriptData( const ScriptData& data )
	{
		m_ScriptData = data;
	}

	void StageBackground::Impl::Terminate()
	{
		m_HasTermSig = true;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	StageBackground::StageBackground( int stageNo ) : m_pImpl( new StageBackground::Impl( stageNo ) )
	{
	}

	StageBackground::~StageBackground()
	{
	}

	void StageBackground::Init()
	{
		m_pImpl->Init();
	}

	void StageBackground::Draw()
	{
		m_pImpl->Draw();
	}

	void StageBackground::Update()
	{
		m_pImpl->Update();
	}

	void StageBackground::AttachStageData( StageData* pData )
	{
		m_pImpl->AttachStageData( pData );
	}

	void StageBackground::AttachScriptData( const ScriptData& data )
	{
		m_pImpl->AttachScriptData( data );
	}

	void StageBackground::Terminate()
	{
		m_pImpl->Terminate();
	}
}
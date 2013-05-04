#include <MAPIL/MAPIL.h>

#include "StagePause.h"
#include "GeneralButtonManager.h"

#include "ResourceID.h"

#include "Util.h"

namespace GameEngine
{
	enum PauseMenuID
	{
		PAUSE_MENU_ID_RESTART			= 0,		// ゲーム続行
		PAUSE_MENU_ID_RETURN_TO_MENU	= 1,		// メニュー画面へ移行
		PAUSE_MENU_ID_TOTAL,
	};

	class StagePause::Impl
	{
	private:
		int						m_Counter;
		ButtonStatusHolder		m_ButtonStatus;		// ボタンの状態
		StageData*				m_pStageData;
		GeneralButtonManager	m_ButtonManager;
		int						m_MenuPointed;		// 現在指しているメニュー位置

		void PauseAllSounds();
		void ResumeAllSounds();
	public:
		Impl();
		~Impl();
		void Init();
		void Draw();
		SceneType Update();
		void AttachStageData( StageData* pData );
		void Reset();
	};

	StagePause::Impl::Impl()
	{
		m_pStageData = NULL;
		m_ButtonManager.ChangeDevice( INPUT_DEVICE_KEYBOARD );
		m_MenuPointed = PAUSE_MENU_ID_RESTART;
	}

	StagePause::Impl::~Impl()
	{
	}

	void StagePause::Impl::Init()
	{
	}

	void StagePause::Impl::Draw()
	{
		//MAPIL::BeginRendering2DGraphics();

		const char* pStr[] = { "Restart", "Return to Menu" };

		MAPIL::DrawTexture(	m_pStageData->m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
							0.0f, 0.0f, 50.0f, 30.0f, 0.0f, false, 0xFF888888 );

		for( int i = 0; i < PAUSE_MENU_ID_TOTAL; ++i ){
			if( i == m_MenuPointed ){
				DrawFontString( m_pStageData->m_ResourceMap, 200.0f, 200.0f + 30.0f * i, 0.5f, 0xFFFFFF00, pStr[ i ] );
			}
			else{
				DrawFontString( m_pStageData->m_ResourceMap, 200.0f, 200.0f + 30.0f * i, 0.5f, 0xFFFFFFFF, pStr[ i ] );
			}
		}

		

		//MAPIL::EndRendering2DGraphics();
	}

	SceneType StagePause::Impl::Update()
	{
		m_ButtonManager.Update();
		m_ButtonManager.GetButtonStatus( &m_ButtonStatus );

		++m_Counter;

		if( m_Counter <= 5 ){
			PauseAllSounds();
			return SCENE_TYPE_PAUSED;
		}

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_DOWN ) ){
			++m_MenuPointed;
			if( m_MenuPointed >= PAUSE_MENU_ID_TOTAL ){
				m_MenuPointed = PAUSE_MENU_ID_RESTART;
			}
		}
		else if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_MOVE_UP ) ){
			--m_MenuPointed;
			if( m_MenuPointed < PAUSE_MENU_ID_RESTART ){
				m_MenuPointed = PAUSE_MENU_ID_TOTAL - 1;
			}
		}

		if( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) ){
			if( m_MenuPointed == PAUSE_MENU_ID_RESTART ){
				ResumeAllSounds();
				return SCENE_TYPE_STAGE;
			}
			else if( m_MenuPointed == PAUSE_MENU_ID_RETURN_TO_MENU ){
				return SCENE_TYPE_MENU;
			}
		}

		return SCENE_TYPE_PAUSED;
	}

	void StagePause::Impl::AttachStageData( StageData* pData )
	{
		m_pStageData = pData;
	}

	void StagePause::Impl::Reset()
	{
		m_Counter = 0;
	}

	void StagePause::Impl::PauseAllSounds()
	{
		for( int i = 0; i < m_pStageData->m_ResourceMap.m_pStageResourceMap->m_BGMMap.size(); ++i ){
			if( m_pStageData->m_ResourceMap.m_pStageResourceMap->m_BGMMap[ i ] == -1 ){
				continue;
			}
			if( MAPIL::IsStreamingBufferStopping( m_pStageData->m_ResourceMap.m_pStageResourceMap->m_BGMMap[ i ] ) ){
				continue;
			}
			MAPIL::PauseStreamingBuffer( m_pStageData->m_ResourceMap.m_pStageResourceMap->m_BGMMap[ i ] );
		}
	}

	void StagePause::Impl::ResumeAllSounds()
	{
		for( int i = 0; i < m_pStageData->m_ResourceMap.m_pStageResourceMap->m_BGMMap.size(); ++i ){
			if( m_pStageData->m_ResourceMap.m_pStageResourceMap->m_BGMMap[ i ] == -1 ){
				continue;
			}
			if( MAPIL::IsStreamingBufferPausing( m_pStageData->m_ResourceMap.m_pStageResourceMap->m_BGMMap[ i ] ) ){
				MAPIL::PlayStreamingBuffer( m_pStageData->m_ResourceMap.m_pStageResourceMap->m_BGMMap[ i ] );
			}
		}
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	StagePause::StagePause() : m_pImpl( new StagePause::Impl )
	{
	}

	StagePause::~StagePause()
	{
	}

	void StagePause::Init()
	{
		m_pImpl->Init();
	}

	void StagePause::Draw()
	{
		m_pImpl->Draw();
	}

	SceneType StagePause::Update()
	{
		return m_pImpl->Update();
	}

	void StagePause::AttachStageData( StageData* pData )
	{
		m_pImpl->AttachStageData( pData );
	}

	void StagePause::Reset()
	{
		m_pImpl->Reset();
	}


}
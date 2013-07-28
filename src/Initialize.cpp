#include <MAPIL/MAPIL.h>

#include "Initialize.h"
#include "ResourceID.h"

namespace GameEngine
{
	class Initialize::Impl
	{
	private:
		int				m_Counter;
		ResourceMap		m_ResourceMap;
	public:
		Impl();
		~Impl(){}
		SceneType Update();
		void Draw();
		void AttachResourceMap( const ResourceMap& map );
	};

	Initialize::Impl::Impl()
	{
		m_Counter = 0;
	}

	SceneType Initialize::Impl::Update()
	{
		++m_Counter;

		return SCENE_TYPE_MENU;
	}

	void Initialize::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();
		
		const float CHAR_INTERVAL = 96.0f;
		const float INI_CHAR_POS = 8.0f;
		const float DISP_Y = 234.0f;
		const float DISP_X_OFFSET = 123.0f;

		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_INITIALIZE ],
							0.0f, 0.0f, false );

		// タイトル
		for( int i = 0; i < 5; ++i ){
			MAPIL::DrawClipedTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_TITLE_2 ],
										DISP_X_OFFSET + CHAR_INTERVAL * i, DISP_Y, 1.0f, 1.0f,
										0.0f, INI_CHAR_POS + CHAR_INTERVAL * i, 0.0f, INI_CHAR_POS + CHAR_INTERVAL * ( i + 1 ),
										104.0f );
		}

		MAPIL::EndRendering2DGraphics();
	}

	void Initialize::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	Initialize::Initialize() : m_pImpl( new Initialize::Impl )
	{
	}

	Initialize::~Initialize()
	{
	}

	void Initialize::InitImpl()
	{
	}

	SceneType Initialize::Update()
	{
		return m_pImpl->Update();
	}

	void Initialize::Draw()
	{
		m_pImpl->Draw();
	}

	void Initialize::AttachButtonState( ButtonStatusHolder* pHolder )
	{
	}

	void Initialize::AttachResourceMap( const ResourceMap& map )
	{
		m_pImpl->AttachResourceMap( map );
	}
}
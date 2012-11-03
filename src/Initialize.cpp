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
		//MAPIL::DrawString( 300.0f, 240.0f, "Initializing... %d", m_Counter );
		MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_INITIALIZE ],
							0.0f, 0.0f, false );
		MAPIL::EndRendering2DGraphics();
	}

	void Initialize::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	// ----------------------------------
	// ŽÀ‘•ƒNƒ‰ƒX‚ÌŒÄ‚Ño‚µ
	// ----------------------------------

	Initialize::Initialize() : m_pImpl( new Initialize::Impl )
	{
	}

	Initialize::~Initialize()
	{
	}

	void Initialize::Init()
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
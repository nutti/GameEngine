#include <MAPIL/MAPIL.h>

#include "Initialize.h"

namespace GameEngine
{
	class Initialize::Impl
	{
	public:
		Impl();
		~Impl(){}
		SceneType Update();
		void Draw();
	};

	Initialize::Impl::Impl()
	{
	}

	SceneType Initialize::Impl::Update()
	{
		return SCENE_TYPE_MENU;
	}

	void Initialize::Impl::Draw()
	{
		MAPIL::BeginRendering2DGraphics();
		MAPIL::DrawString( 300.0f, 240.0f, "Initializing..." );

		MAPIL::EndRendering2DGraphics();
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
	}
}
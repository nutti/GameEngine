#include "Scene.h"

namespace GameEngine
{

	Scene::Scene() : m_Initialized( false )
	{
	}

	Scene::~Scene()
	{
		m_Initialized = false;
	}

	void Scene::Init()
	{
		if( !m_Initialized ){
			InitImpl();
			m_Initialized = true;
		}
	}

	void Scene::IsLoading()
	{
	}

	void Scene::SetLoadingState()
	{
	}

	void Scene::SetButtonState()
	{
	}
}
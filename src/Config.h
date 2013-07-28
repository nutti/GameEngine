#ifndef INCLUDED_GAMEENGINE_CONFIG_H
#define INCLUDED_GAMEENGINE_CONFIG_H

#include <memory>
#include "Scene.h"

namespace GameEngine
{
	class EventMediator;
	class Config : public Scene
	{
	private:
		class Impl;
		std::auto_ptr < Config::Impl >		m_pImpl;
		void InitImpl();
	public:
		Config();
		~Config();
		//void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachConfigData( const GameConfigData& data );
		void SetEventMediator( std::weak_ptr < EventMediator > pMediator );
		const GameConfigData& GetConfigData() const;
	};
}

#endif
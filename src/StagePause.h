#ifndef INCLUDED_GAMEENGINE_STAGEPAUSE_H
#define INCLUDED_GAMEENGINE_STAGEPAUSE_H

#include <memory>

#include "SceneTypes.h"
#include "Stage.h"

namespace GameEngine
{
	
	class StagePause
	{
	private:
		class Impl;
		std::auto_ptr < StagePause::Impl >		m_pImpl;
	public:
		StagePause();
		~StagePause();
		void Init();
		void Draw();
		SceneType Update();
		void AttachStageData( StageData* pData );
		void Reset();
	};
}

#endif
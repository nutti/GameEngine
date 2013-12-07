#ifndef INCLUDED_GAMEENGINE_STAGEVIEW_H
#define INCLUDED_GAMEENGINE_STAGEVIEW_H

#include <list>

#include "../../ResourceTypes.h"
#include "../../GameStateTypes.h"

#include "../../Enemy.h"

namespace GameEngine
{
	class StageView
	{
	protected:
		ResourceMap				m_ResourceMap;
	public:
		StageView();
		virtual ~StageView();
		void AttachResourceMap( const ResourceMap& map );
		virtual void Draw() const{}
		virtual void Update(){}
		virtual void Init(){}
		virtual void OnEnemyDestroyed( const EnemyData& data ){}
	};

	class EnemyDestroyPointView : public StageView
	{
	private:
		struct EnemyScore
		{
			int		m_Magnification;
			int		m_BaseScore;
			int		m_Difficulty;
			int		m_MaxHP;
			int		m_UpdateCounter;
		};

		int			m_Counter;
		int			m_RefleshCounter;

		EnemyDestroyPointView::EnemyScore		m_FirstDispEntry;
		EnemyDestroyPointView::EnemyScore		m_LastDispEntry;

		std::list < EnemyDestroyPointView::EnemyScore >		m_ScoreList;
	public:
		EnemyDestroyPointView();
		~EnemyDestroyPointView();
		void Draw() const;
		void Update();
		void Init();
		void OnEnemyDestroyed( const EnemyData& data );
	};

	
}

#endif
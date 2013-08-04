#ifndef INCLUDED_GAMEENGINE_REPLAYVIEW_H
#define INCLUDED_GAMEENGINE_REPLAYVIEW_H

#include "../../ResourceTypes.h"
#include "../../GameStateTypes.h"

namespace GameEngine
{
	class ReplayView
	{
	protected:
		ResourceMap				m_ResourceMap;
	public:
		ReplayView();
		virtual ~ReplayView();
		void AttachResourceMap( const ResourceMap& map );
		virtual void Draw() const{}
		virtual void Update(){}
		virtual void BackToMenu(){}
		virtual void Init(){}
		virtual void Finalize(){}
	};

	class ReplayBackgroundView : public ReplayView
	{
	private:
		enum Mode
		{
			MODE_INVISIBLE			= 0,
			MODE_INITIALIZE			= 1,
			MODE_BACK_TO_MENU		= 2,
			MODE_FINALIZE			= 3,
			MODE_NORMAL				= 4,
			MODE_TOTAL,
		};

		Mode		m_CurMode;
		int			m_TransitionCounter;
		int			m_Counter;
	public:
		ReplayBackgroundView();
		~ReplayBackgroundView();
		void Draw() const;
		void Update();
		void BackToMenu();
		void Init();
		void Finalize();
	};

	class ReplayTitleView : public ReplayView
	{
	private:
		enum Mode
		{
			MODE_INVISIBLE			= 0,
			MODE_INITIALIZE			= 1,
			MODE_BACK_TO_MENU		= 2,
			MODE_FINALIZE			= 3,
			MODE_NORMAL				= 4,
			MODE_TOTAL,
		};

		Mode		m_CurMode;
		int			m_TransitionCounter;
		int			m_Counter;
	public:
		ReplayTitleView();
		~ReplayTitleView();
		void Draw() const;
		void Update();
		void BackToMenu();
		void Init();
		void Finalize();
	};

}

#endif
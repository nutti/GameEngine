#ifndef INCLUDED_GAMEENGINE_SCOREENTRYVIEW_H
#define INCLUDED_GAMEENGINE_SCOREENTRYVIEW_H

#include "../../ResourceTypes.h"
#include "../../GameStateTypes.h"

namespace GameEngine
{
	class ScoreEntryView
	{
	protected:
		ResourceMap				m_ResourceMap;
	public:
		ScoreEntryView();
		virtual ~ScoreEntryView();
		void AttachResourceMap( const ResourceMap& map );
		virtual void Draw() const{}
		virtual void Update(){}
		virtual void BackToMenu(){}
		virtual void Init(){}
		virtual void Finalize(){}
		virtual void NameInput(){}
		virtual void NameDeleted(){}
	};

	class ScoreEntryBackgroundView : public ScoreEntryView
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
		ScoreEntryBackgroundView();
		~ScoreEntryBackgroundView();
		void Draw() const;
		void Update();
		void BackToMenu();
		void Init();
		void Finalize();
	};

	class ScoreEntryTilteView : public ScoreEntryView
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
		ScoreEntryTilteView();
		~ScoreEntryTilteView();
		void Draw() const;
		void Update();
		void BackToMenu();
		void Init();
		void Finalize();
	};

}

#endif
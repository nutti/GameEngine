#ifndef INCLUDED_GAMEENGINE_SCOREVIEW_H
#define INCLUDED_GAMEENGINE_SCOREVIEW_H

#include "../../ResourceTypes.h"
#include "../../GameStateTypes.h"

namespace GameEngine
{
	class ScoreView
	{
	protected:
		ResourceMap				m_ResourceMap;
	public:
		ScoreView();
		virtual ~ScoreView();
		void AttachResourceMap( const ResourceMap& map );
		virtual void Draw() const{}
		virtual void Update(){}
		virtual void BackToMenu(){}
		virtual void Init(){}
		virtual void ChangeDifficulty( int difficulty ){}
	};

	class ScoreBackgroundView : public ScoreView
	{
	private:
		enum Mode
		{
			MODE_INVISIBLE			= 0,
			MODE_INITIALIZE			= 1,
			MODE_BACK_TO_MENU		= 2,
			MODE_NORMAL				= 3,
			MODE_TOTAL,
		};

		Mode		m_CurMode;
		int			m_TransitionCounter;
		int			m_Counter;
	public:
		ScoreBackgroundView();
		~ScoreBackgroundView();
		void Draw() const;
		void Update();
		void BackToMenu();
		void Init();
	};

	class ScoreDifficultyView : public ScoreView
	{
	private:
		enum Mode
		{
			MODE_INVISIBLE			= 0,
			MODE_INITIALIZE			= 1,
			MODE_BACK_TO_MENU		= 2,
			MODE_DIFFICULTY_CHANGED	= 3,
			MODE_NORMAL				= 4,
			MODE_TOTAL,
		};

		Mode		m_CurMode;
		int			m_TransitionCounter;
		int			m_Counter;
		int			m_SelectedDifficulty;
		int			m_PrevSelectedDifficulty;
	public:
		ScoreDifficultyView();
		~ScoreDifficultyView();
		void Draw() const;
		void Update();
		void BackToMenu();
		void Init();
		void ChangeDifficulty( int difficulty );
	};

	class ScoreTitleView : public ScoreView
	{
	private:
		enum Mode
		{
			MODE_INVISIBLE			= 0,
			MODE_INITIALIZE			= 1,
			MODE_BACK_TO_MENU		= 2,
			MODE_NORMAL				= 3,
			MODE_TOTAL,
		};

		Mode		m_CurMode;
		int			m_TransitionCounter;
		int			m_Counter;
	public:
		ScoreTitleView();
		~ScoreTitleView();
		void Draw() const;
		void Update();
		void BackToMenu();
		void Init();
	};
}

#endif
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
		virtual void SelectReplay( int replayNo, int stage ){}
		virtual void CancelStage(){}
		virtual void ChangeStage( int stage ){}
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

	class ReplayStageView : public ReplayView
	{
	private:
		enum Mode
		{
			MODE_INVISIBLE			= 0,
			MODE_REPLAY_SELECTED	= 1,
			MODE_STAGE_CANCELED		= 2,
			MODE_STAGE_CHANGED		= 3,
			MODE_FINALIZE			= 4,
			MODE_NORMAL				= 5,
			MODE_TOTAL,
		};

		Mode		m_CurMode;
		int			m_TransitionCounter;
		int			m_Counter;
		int			m_SelectedStage;
		int			m_SelectedReplayNo;
		DisplayedReplayInfo*		m_pDispReplayInfo;

	public:
		ReplayStageView();
		~ReplayStageView();
		void AttachDisplayedReplayInfo( DisplayedReplayInfo* pInfo );
		void Draw() const;
		void Update();
		void Finalize();
		void SelectReplay( int replayNo, int stage );
		void CancelStage();
		void ChangeStage( int stage );
	};
}

#endif
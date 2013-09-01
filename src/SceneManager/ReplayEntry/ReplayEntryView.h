#ifndef INCLUDED_GAMEENGINE_REPLAYENTRYVIEW_H
#define INCLUDED_GAMEENGINE_REPLAYENTRYVIEW_H

#include "../../ResourceTypes.h"
#include "../../GameStateTypes.h"

namespace GameEngine
{
	class ReplayEntryView
	{
	protected:
		ResourceMap				m_ResourceMap;
	public:
		ReplayEntryView();
		virtual ~ReplayEntryView();
		void AttachResourceMap( const ResourceMap& map );
		virtual void Draw() const{}
		virtual void Update(){}
		virtual void Init(){}
		virtual void Finalize(){}
		virtual void NameInput( std::string name, char c ){}
		virtual void NameDelete( std::string name, char c ){}
		virtual void ChangeNameKey( int x, int y, char c ){}
		virtual void SelectReplay( int replayNo ){}
		virtual void CancelReplay(){}
		virtual void ChangeReplay( int replayNo ){}
		virtual void SaveReplay(){}
	};

	class ReplayEntryBackgroundView : public ReplayEntryView
	{
	private:
		enum Mode
		{
			MODE_INVISIBLE			= 0,
			MODE_INITIALIZE			= 1,
			MODE_FINALIZE			= 2,
			MODE_NORMAL				= 3,
			MODE_TOTAL,
		};

		Mode		m_CurMode;
		int			m_TransitionCounter;
		int			m_Counter;
	public:
		ReplayEntryBackgroundView();
		~ReplayEntryBackgroundView();
		void Draw() const;
		void Update();
		void Init();
		void Finalize();
	};

	class ReplayEntryTitleView : public ReplayEntryView
	{
	private:
		enum Mode
		{
			MODE_INVISIBLE			= 0,
			MODE_INITIALIZE			= 1,
			MODE_FINALIZE			= 2,
			MODE_NORMAL				= 3,
			MODE_TOTAL,
		};

		Mode		m_CurMode;
		int			m_TransitionCounter;
		int			m_Counter;
	public:
		ReplayEntryTitleView();
		~ReplayEntryTitleView();
		void Draw() const;
		void Update();
		void Init();
		void Finalize();
	};

	class ReplayEntryInfoView : public ReplayEntryView
	{
	private:
		enum Mode
		{
			MODE_INVISIBLE			= 0,
			MODE_INITIALIZE			= 1,
			MODE_FINALIZE			= 2,
			MODE_NORMAL				= 3,
			MODE_REPLAY_SELECTED	= 4,
			MODE_REPLAY_CANCELED	= 5,
			MODE_REPLAY_CHANGED		= 6,
			MODE_REPLAY_SAVED		= 7,
			MODE_NAME_INPUT			= 8,
			MODE_NAME_DELETED		= 9,
			MODE_TOTAL,
		};

		Mode					m_CurMode;
		int						m_TransitionCounter;
		int						m_Counter;
		DisplayedReplayInfo*	m_pDispReplayInfo;
		int						m_ReplayNo;
	public:
		ReplayEntryInfoView();
		~ReplayEntryInfoView();
		void Draw() const;
		void Update();
		void Init();
		void Finalize();
		void AttachDisplayReplayInfo( DisplayedReplayInfo* pInfo );
		void SelectReplay( int replayNo );
		void CancelReplay();
		void ChangeReplay( int replayNo );
		void SaveReplay();
	};

	class ReplayEntryKeySelectionView : public ReplayEntryView
	{
		private:
		enum Mode
		{
			MODE_INVISIBLE			= 0,
			MODE_NORMAL				= 1,
			MODE_REPLAY_SELECTED	= 2,
			MODE_REPLAY_SAVED		= 3,
			MODE_REPLAY_CANCELED	= 4,
			MODE_NAME_INPUT			= 5,
			MODE_NAME_KEY_CHANGED		= 6,
			MODE_TOTAL,
		};

		Mode					m_CurMode;
		int						m_TransitionCounter;
		int						m_Counter;
		char					m_SelectedCharX;
		char					m_SelectedCharY;
	public:
		ReplayEntryKeySelectionView();
		~ReplayEntryKeySelectionView();
		void Draw() const;
		void Update();
		void SelectReplay( int replayNo );
		void CancelReplay();
		void SaveReplay();
		void NameInput( std::string name, char c );
		void ChangeNameKey( int x, int y, char c );
	};

	class ReplayEntryNameView : public ReplayEntryView
	{
		private:
		enum Mode
		{
			MODE_INVISIBLE			= 0,
			MODE_NORMAL				= 1,
			MODE_REPLAY_SELECTED	= 2,
			MODE_REPLAY_SAVED		= 3,
			MODE_REPLAY_CANCELED	= 4,
			MODE_NAME_INPUT			= 5,
			MODE_NAME_DELETED		= 6,
			MODE_NAME_KEY_CHANGED	= 7,
			MODE_TOTAL,
		};

		Mode					m_CurMode;
		int						m_TransitionCounter;
		int						m_Counter;
		std::string				m_Name;
		char					m_SelectedCharX;
		char					m_SelectedCharY;
	public:
		ReplayEntryNameView();
		~ReplayEntryNameView();
		void Draw() const;
		void Update();
		void SelectReplay( int replayNo );
		void CancelReplay();
		void SaveReplay();
		void NameInput( std::string name, char c );
		void NameDelete( std::string name, char c );
		void ChangeNameKey( int x, int y, char c );
	};
}

#endif
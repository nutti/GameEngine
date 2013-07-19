#ifndef INCLUDED_GAMEENGINE_STAGESELECTIONVIEW_H
#define INCLUDED_GAMEENGINE_STAGESELECTIONVIEW_H

#include "../../ResourceTypes.h"
#include "../../GameStateTypes.h"

namespace GameEngine
{
	class StageSelectionView
	{
	protected:
		ResourceMap				m_ResourceMap;
	public:
		StageSelectionView();
		virtual ~StageSelectionView();
		void AttachResourceMap( const ResourceMap& map );
		virtual void Draw() const{}
		virtual void Update(){}
		virtual void CancelStage(){}
		virtual void SelectStage(){}
		virtual void ChangeStage( int stage ){}
		virtual void CancelDifficulty(){}
		virtual void SelectDifficulty(){}
		virtual void ChangeDifficulty( int difficulty ){}
		virtual void Init(){}
		virtual void Finalize(){}
	};

	class StageView : public StageSelectionView
	{
	private:
		enum Mode
		{
			MODE_INVISIBLE				= 0,
			MODE_DIFFICULTY_SELECTED	= 1,
			MODE_CANCELED				= 2,
			MODE_STAGE_SELECTED			= 3,
			MODE_CHANGED				= 4,
			MODE_FINALIZE				= 5,
			MODE_NORMAL					= 6,
			MODE_TOTAL,
		};

		Mode		m_CurMode;
		int			m_SelectedStage;
		int			m_PrevSelectedStage;
		int			m_PermittedStage;

		int			m_TransitionCounter;
	public:
		StageView();
		~StageView();
		void SetPermittedStage( int stage );
		void Draw() const;
		void Update();
		void CancelStage();
		void SelectStage();
		void ChangeStage( int stage );
		void SelectDifficulty();
		void Finalize();
	};

	class StageSelectionTitleView : public StageSelectionView
	{
	private:
		enum Mode
		{
			MODE_INVISIBLE				= 0,
			MODE_INITIALIZE				= 1,
			MODE_STAGE_CANCELED			= 2,
			MODE_DIFFICULTY_SELECTED	= 3,
			MODE_FINALIZE				= 4,
			MODE_NORMAL					= 5,
			MODE_TOTAL,
		};

		Mode		m_CurMode;
		int			m_TransitionCounter;
		bool		m_IsStageSelectionMode;
	public:
		StageSelectionTitleView();
		~StageSelectionTitleView();
		void Draw() const;
		void Update();
		void CancelStage();
		void SelectDifficulty();
		void Init();
		void Finalize();
	};

	class StageSelectionBackgroundView : public StageSelectionView
	{
	private:
		enum Mode
		{
			MODE_INVISIBLE				= 0,
			MODE_INITIALIZE				= 1,
			MODE_FINALIZE				= 2,
			MODE_NORMAL					= 3,
			MODE_TOTAL,
		};

		Mode		m_CurMode;
		int			m_TransitionCounter;
		int			m_Counter;
	public:
		StageSelectionBackgroundView();
		~StageSelectionBackgroundView();
		void Draw() const;
		void Update();
		void Init();
		void Finalize();
	};

	class StageSelectionStatView : public StageSelectionView
	{
	private:
		enum Mode
		{
			MODE_INVISIBLE				= 0,
			MODE_INITIALIZE				= 1,
			MODE_STAGE_CANCELED			= 2,
			MODE_DIFFICULTY_SELECTED	= 3,
			MODE_STAGE_CHANGED			= 4,
			MODE_FINALIZE				= 5,
			MODE_NORMAL					= 6,
			MODE_TOTAL,
		};

		Mode								m_CurMode;
		int									m_TransitionCounter;
		int									m_SelectedStage;
		int									m_SelectedDifficulty;
		DisplayedStageSelectionPlayStat		m_DispStagePlayStat;
	public:
		StageSelectionStatView();
		~StageSelectionStatView();
		void AttachDisplayedStagePlayStat( const DisplayedStageSelectionPlayStat& stat );
		void Draw() const;
		void Update();
		void CancelStage();
		void ChangeStage( int stage );
		void ChangeDifficulty( int difficulty );
		void SelectDifficulty();
		void Init();
		void Finalize();
	};
}

#endif
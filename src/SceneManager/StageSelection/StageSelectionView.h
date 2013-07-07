#ifndef INCLUDED_GAMEENGINE_STAGESELECTIONVIEW_H
#define INCLUDED_GAMEENGINE_STAGESELECTIONVIEW_H

#include "../../ResourceTypes.h"

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

		int			m_DifficultySelectCounter;
		int			m_CancelCounter;
		int			m_SelectCounter;
		int			m_ChangeCounter;
		int			m_FinalizeCounter;

		Mode		m_CurMode;
		int			m_SelectedStage;
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
}

#endif
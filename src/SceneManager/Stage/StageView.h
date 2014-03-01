#ifndef INCLUDED_GAMEENGINE_STAGEVIEW_H
#define INCLUDED_GAMEENGINE_STAGEVIEW_H

#include <list>

#include "../../ResourceTypes.h"
#include "../../GameStateTypes.h"

#include "../../Enemy.h"

namespace GameEngine
{
	struct FixedGameData;
	struct StageData;
	struct PlayerData;
	class Item;
	class Player;

	class StageView
	{
	protected:
		ResourceMap				m_ResourceMap;
	public:
		StageView();
		virtual ~StageView();
		void AttachResourceMap( const ResourceMap& map );
		virtual void PreDraw() const{}
		virtual void Draw() const{}
		virtual void PostDraw() const{}
		virtual void Update(){}
		virtual void Init(){}
		virtual void OnPlayerObtainedItem( const Item& item, const Player& player ){}
		virtual void OnPlayerDamaged( const Player& player ){}
		virtual void OnPlayerDestroyed( const Player& player ){}
		virtual void OnPlayerUsedCons( const Player& player ){}
		virtual void OnEnemyDamaged( const EnemyData& data ){}
		virtual void OnEnemyDestroyed( const EnemyData& data ){}
		virtual void OnCrystalObtained( int crystal ){}
	};

	class DifficultyView : public StageView
	{
	private:
		int			m_Counter;
		int			m_Difficulty;
	public:
		DifficultyView( bool firstStage, int difficulty );
		~DifficultyView();
		void PostDraw() const;
		void Update();
		void Init();
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

	class CrystalView : public StageView
	{
	private:
		int				m_Counter;
		StageData*		m_pStageData;
		FixedGameData*	m_pFixedData;
	public:
		CrystalView( StageData* pStageData, FixedGameData* pFixedData );
		~CrystalView();
		void Draw() const;
		void PostDraw() const;
		void Update();
		void Init();
		void OnCrystalObtained( int crystal );

	};

	class KilledView : public StageView
	{
	private:
		int				m_Counter;
		StageData*		m_pStageData;
		FixedGameData*	m_pFixedData;
	public:
		KilledView( StageData* pStageData, FixedGameData* pFixedData );
		~KilledView();
		void Draw() const;
		void PostDraw() const;
		void Update();
		void Init();
		void OnEnemyDestroyed( const EnemyData& data );
	};
	
	class ProgressView : public StageView
	{
	private:
		int				m_Counter;
		StageData*		m_pStageData;
	public:
		ProgressView( StageData* pStageData );
		~ProgressView();
		void PostDraw() const;
		void Update();
	};

	class EnemyInfoView : public StageView
	{
	private:
		int				m_StaticCounter;

		int				m_Counter;
		StageData*		m_pStageData;
		std::string		m_EnemyName;
		int				m_EnemyHP;
		int				m_EnemyMaxHP;
		int				m_EnemyCons;
		int				m_EnemyMaxCons;
	public:
		EnemyInfoView( StageData* pStageData );
		~EnemyInfoView();
		void PostDraw() const;
		void Update();
		void OnEnemyDamaged( const EnemyData& data );
		void OnEnemyDestroyed( const EnemyData& data );
	};

	class PlayerHPView : public StageView
	{
	private:

		enum Mode
		{
			NORMAL,
			DAMAGED,
			RECOVERED,
			TOTAL,
		};

		int					m_StaticCounter;
		int					m_Counter;
		Player*				m_pPlayer;
		int					m_PrevHP;
		PlayerHPView::Mode	m_Mode;

	public:
		PlayerHPView( Player* pPlayer );
		~PlayerHPView();
		void PostDraw() const;
		void Update();
		void OnPlayerDamaged( const Player& player );
		void OnPlayerObtainedItem( const Item& item, const Player& player );
	};

	class PlayerConsView : public StageView
	{
	private:

		int					m_StaticCounter;
		Player*				m_pPlayer;
	public:
		PlayerConsView( Player* pPlayer );
		~PlayerConsView();
		void PostDraw() const;
		void Update();
		void OnPlayerObtainedItem( const Item& item, const Player& player );
		void OnPlayerDamaged( const Player& player );
		void OnPlayerUsedCons( const Player& player );
	};
}

#endif
#include <MAPIL/MAPIL.h>

#include "GameStateTypes.h"

namespace GameEngine
{
	EnemyStat::EnemyStat() : m_Destroy( 0 ), m_Damaged( 0 ), m_KO( 0 )
	{
	}
	
	EnemyStat::~EnemyStat()
	{
		m_Destroy = 0;
		m_Damaged = 0;
		m_KO = 0;
	}

	StageStat::StageStat() : m_EnemyStat()
	{
		m_EnemyStat.clear();
		MAPIL::ZeroObject( m_ConsTime, sizeof( m_ConsTime ) );
	}

	StageStat::~StageStat()
	{
		m_EnemyStat.clear();
		MAPIL::ZeroObject( m_ConsTime, sizeof( m_ConsTime ) );
	}

	NormalPlayStat::NormalPlayStat() :	m_Play( 0 ),
										m_AllClear( 0 ),
										m_PlayTime( 0 ),
										m_Progress( 0 )
	{
	}

	NormalPlayStat::~NormalPlayStat()
	{
	}

	StageSelectionPlayStat::StageSelectionPlayStat() :	m_Play( 0 ),
														m_Clear( 0 ),
														m_PlayTime( 0 )
	{
	}

	StageSelectionPlayStat::~StageSelectionPlayStat()
	{
	}

	GameStat::GameStat()
	{
	}

	GameStat::~GameStat()
	{
	}
}
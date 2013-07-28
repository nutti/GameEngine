#include <MAPIL/MAPIL.h>

#include "GameStateTypes.h"

#include "Util.h"

namespace GameEngine
{
	SkillStat::SkillStat() : m_Used( 0 )
	{
	}

	SkillStat::~SkillStat()
	{
		m_Used = 0;
	}

	SkillStat& SkillStat::operator=( const SkillStat& stat )
	{
		m_Used = stat.m_Used;

		return *this;
	}

	SkillStat& SkillStat::operator+=( const SkillStat& stat )
	{
		m_Used += stat.m_Used;

		return *this;
	}

	SkillStat::SkillStat( const SkillStat& stat )
	{
		m_Used = stat.m_Used;
	}

	EnemyStat::EnemyStat() : m_Destroy( 0 ), m_Damaged( 0 ), m_KO( 0 ), m_SkillStat()
	{
		m_SkillStat.clear();
	}
	
	EnemyStat::~EnemyStat()
	{
		m_Destroy = 0;
		m_Damaged = 0;
		m_KO = 0;
		m_SkillStat.clear();
	}

	EnemyStat& EnemyStat::operator=( const EnemyStat& stat )
	{
		m_Destroy = stat.m_Destroy;
		m_Damaged = stat.m_Damaged;
		m_KO = stat.m_KO;
		m_SkillStat = stat.m_SkillStat;

		return *this;
	}

	EnemyStat& EnemyStat::operator+=( const EnemyStat& stat )
	{
		m_Destroy += stat.m_Destroy;
		m_Damaged += stat.m_Damaged;
		m_KO += stat.m_KO;

		for( auto it = stat.m_SkillStat.begin(); it != stat.m_SkillStat.end(); ++it ){
			if( m_SkillStat.find( it->first ) == m_SkillStat.end() ){
				SkillStat s;
				m_SkillStat[ it->first ] = s;
			}
			m_SkillStat[ it->first ] += it->second;
		}

		return *this;
	}

	EnemyStat::EnemyStat( const EnemyStat& stat )
	{
		m_Destroy = stat.m_Destroy;
		m_Damaged = stat.m_Damaged;
		m_KO = stat.m_KO;
		m_SkillStat = stat.m_SkillStat;
	}

	StageStat::StageStat() :	m_EnemyStat(),
								m_Play( 0 ),
								m_Clear( 0 ),
								m_PlayTime( 0 ),
								m_HIScore( 0 )
	{
		m_EnemyStat.clear();
		for( int i = 0; i < NUM_OF( m_ConsTime ); ++i ){
			m_ConsTime[ i ] = 0;
		}
	}

	StageStat::~StageStat()
	{
		m_EnemyStat.clear();
		for( int i = 0; i < NUM_OF( m_ConsTime ); ++i ){
			m_ConsTime[ i ] = 0;
		}
		m_Play = 0;
		m_Clear = 0;
		m_PlayTime = 0;
		m_HIScore = 0;
	}

	StageStat& StageStat::operator=( const StageStat& stat )
	{
		m_EnemyStat = stat.m_EnemyStat;
		m_Clear = stat.m_Clear;
		for( int i = 0; i < NUM_OF( m_ConsTime ); ++i ){
			m_ConsTime[ i ] = stat.m_ConsTime[ i ];
		}
		m_HIScore = stat.m_HIScore;
		m_Play = stat.m_Play;
		m_PlayTime = stat.m_PlayTime;

		return *this;
	}

	StageStat::StageStat( const StageStat& stat )
	{
		m_EnemyStat = stat.m_EnemyStat;
		m_Clear = stat.m_Clear;
		for( int i = 0; i < NUM_OF( m_ConsTime ); ++i ){
			m_ConsTime[ i ] = stat.m_ConsTime[ i ];
		}
		m_HIScore = stat.m_HIScore;
		m_Play = stat.m_Play;
		m_PlayTime = stat.m_PlayTime;
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
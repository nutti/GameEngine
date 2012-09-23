#include "RandomGenerator.h"

#include "Stage.h"
#include "Player.h"

namespace GameEngine
{
	RandomGenerator::RandomGenerator() :	m_RandSeed( 47 ),
											RAND_MAXIMUM( 65535 ),
											m_RandCount( 0 )
	{
	}

	RandomGenerator::~RandomGenerator()
	{
	}

	int RandomGenerator::GetRand( int posX, int posY, int score, int frame )
	{
		++m_RandCount;
		return ( ( posX + score + frame + m_RandCount * m_RandSeed ) * ( frame + m_RandCount + score + posY ) ) % RAND_MAXIMUM;
	}

	void RandomGenerator::Reset()
	{
		m_RandCount = 0;
	}

	void RandomGenerator::SetRandSeed( int seed )
	{
		m_RandSeed = seed;
	}

	void RandomGenerator::Update( const StageData& data )
	{
		float x;
		float y;
		data.m_pPlayer->GetPos( &x, &y );
		m_RandData.m_PosX = static_cast < int > ( x );
		m_RandData.m_PosY = static_cast < int > ( y );
		m_RandData.m_Score = data.m_Score;
		m_RandData.m_Frame = data.m_Frame;
	}

	int RandomGenerator::GetRand()
	{
		++m_RandCount;
		return GetRand( m_RandData.m_PosX, m_RandData.m_PosY, m_RandData.m_Score, m_RandData.m_Frame );
	}

	int RandomGenerator::GetRandMax()
	{
		return RAND_MAXIMUM;
	}
}
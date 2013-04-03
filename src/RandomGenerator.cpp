#include "RandomGenerator.h"

#include "Stage.h"
#include "Player.h"

namespace GameEngine
{
	RandomGenerator::RandomGenerator() :	m_RandSeed( 47 ),
											RAND_MAXIMUM( 65535 ),
											m_RandCount( 0 ),
											m_Logger( TSTR( "ReplayLogger.log" ) )
	{
		m_Logger.Create();
	}

	RandomGenerator::~RandomGenerator()
	{
		m_Logger.Terminate();
	}

	int RandomGenerator::GetRand( int posX, int posY, int score, int frame )
	{
		++m_RandCount;
		//std::basic_ostringstream < TCHAR > oss;
		//std::basic_ostringstream < TCHAR > oss2;
		//oss << m_RandCount;
		//oss2 << TSTR( "RandCount" ) << m_RandData.m_Frame;
		//m_Logger.Write( oss2.str(), oss.str() );
		int rand = ( posX + score + frame + m_RandCount * m_RandSeed ) * ( frame + m_RandCount + score + posY );
		// Overflow!!
		if( rand < 0 ){
			rand &= 0x7FFFFFFF;
		}
		return rand % RAND_MAXIMUM;
	}

	void RandomGenerator::Reset()
	{
		m_RandCount = 0;
	}

	void RandomGenerator::SetRandSeed( int seed )
	{
		m_RandSeed = seed;
	}

#if defined ( USE_FLOATING_POINT )
	void RandomGenerator::Update( const StageData& data )
	{
		float x;
		float y;
		data.m_pPlayer->GetPos( &x, &y );
		m_RandData.m_PosX = static_cast < int > ( x );
		m_RandData.m_PosY = static_cast < int > ( y );
		m_RandData.m_Score = data.m_GameData.m_Score;
		m_RandData.m_Frame = data.m_Frame;

		std::basic_ostringstream < TCHAR > oss;
		std::basic_ostringstream < TCHAR > oss2;
		oss << TSTR( "Player: " ) << m_RandData.m_PosX << TSTR( "," ) << m_RandData.m_PosY
			<< TSTR( " Score: " ) << m_RandData.m_Score;
		oss2 << TSTR( "Frame: " ) << m_RandData.m_Frame;
		m_Logger.Write( oss2.str(), oss.str() );
	}
#elif defined ( USE_GAME_UNIT )
	void RandomGenerator::Update( const StageData& data )
	{
		GameUnit x;
		GameUnit y;
		data.m_pPlayer->GetPos( &x, &y );
		m_RandData.m_PosX = x.GetInt();
		m_RandData.m_PosY = y.GetInt();
		m_RandData.m_Score = data.m_GameData.m_Score;
		m_RandData.m_Frame = data.m_Frame;

		std::basic_ostringstream < TCHAR > oss;
		std::basic_ostringstream < TCHAR > oss2;
		oss << TSTR( "Player: " ) << m_RandData.m_PosX << TSTR( "," ) << m_RandData.m_PosY
			<< TSTR( " Score: " ) << m_RandData.m_Score;
		oss2 << TSTR( "Frame: " ) << m_RandData.m_Frame;
		m_Logger.Write( oss2.str(), oss.str() );
	}
#endif

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
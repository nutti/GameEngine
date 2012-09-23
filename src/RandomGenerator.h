#ifndef INCLUDED_GAMEENGINE_RANDOMGENERATOR_H
#define INCLUDED_GAMEENGINE_RANDOMGENERATOR_H


namespace GameEngine
{
	struct RandData
	{
		int		m_PosX;
		int		m_PosY;
		int		m_Score;
		int		m_Frame;
	};

	struct StageData;
	class RandomGenerator
	{
	private:
		int			m_RandSeed;
		const int	RAND_MAXIMUM;
		int			m_RandCount;
		RandData	m_RandData;
		int GetRand( int posX, int posY, int score, int frame );
	public:
		RandomGenerator();
		~RandomGenerator();
		void Reset();
		void SetRandSeed( int seed );
		void Update( const StageData& data );
		int GetRand();
		int GetRandMax();
	};

}

#endif
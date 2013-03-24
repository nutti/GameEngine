#ifndef INCLUDED_GAMEENGINE_ENEMY_PATTERN_LOADER_H
#define INCLUDED_GAMEENGINE_ENEMY_PATTERN_LOADER_H

#include <vector>
#include <string>

namespace GameEngine
{
	const int		MAX_ENEMY_REGS	= 10;
	const float		MAX_ENEMY_FREGS	= 10;

	struct EnemyPatternInfo
	{
		float					m_PosX;						// �G�̈ʒu�iX���W�j
		float					m_PosY;						// �G�̈ʒu�iY���W�j
		int						m_EnemyID;					// �GID
		int						m_Regs[ MAX_ENEMY_REGS ];	// ���W�X�^
	};

	struct EnemyPattern
	{
		std::vector < EnemyPatternInfo >		m_InfoList;
	};

	int CreateStageEnemyPattern( const std::string& fileName );

	void DeleteStageEnemyPattern( int id );

	bool ExistEnemyPattern( int id, int frame );

	void GetEnemyPattern( int id, int frame, EnemyPattern* pPattern );

	int CreateStageEnemyPattern( int archiveHandle, const std::string& filePath );
}

#endif
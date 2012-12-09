#ifndef INCLUDED_GAMEENGINE_ENEMY_PATTERN_LOADER_H
#define INCLUDED_GAMEENGINE_ENEMY_PATTERN_LOADER_H

#include <vector>
#include <string>

namespace GameEngine
{
	struct EnemyPatternInfo
	{
		float	m_PosX;
		float	m_PosY;
		int		m_EnemyID;
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
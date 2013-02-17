#ifndef INCLUDED_GAMEENGINE_ENEMY_PATTERN_LOADER_H
#define INCLUDED_GAMEENGINE_ENEMY_PATTERN_LOADER_H

#include <vector>
#include <string>

namespace GameEngine
{
	struct EnemyPatternInfo
	{
		float					m_PosX;			// �G�̈ʒu�iX���W�j
		float					m_PosY;			// �G�̈ʒu�iY���W�j
		int						m_EnemyID;		// �GID
		std::vector < int >		m_Regs;			// ���W�X�^
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
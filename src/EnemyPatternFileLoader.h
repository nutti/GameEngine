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
		float					m_PosX;						// 敵の位置（X座標）
		float					m_PosY;						// 敵の位置（Y座標）
		int						m_EnemyID;					// 敵ID
		int						m_Regs[ MAX_ENEMY_REGS ];	// レジスタ
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
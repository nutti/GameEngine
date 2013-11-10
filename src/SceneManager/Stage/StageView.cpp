#include "StageView.h"

#include <MAPIL/MAPIL.h>

#include "../../GameStateTypes.h"

#include "../../Util.h"
#include "../../ResourceID.h"

#include "../../Stage.h"

namespace GameEngine
{
	StageView::StageView()
	{
	}

	StageView::~StageView()
	{
	}

	void StageView::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
	}

	//-------------------------------------------
	// ScoreBackgroundView
	//-------------------------------------------

	EnemyDestroyPointView::EnemyDestroyPointView() :	m_Counter( 0 ),
														m_RefleshCounter( 0 ),
														m_ScoreList()
	{
		m_ScoreList.clear();
	}

	EnemyDestroyPointView::~EnemyDestroyPointView()
	{
		m_Counter = 0;
		m_ScoreList.clear();
	}

	void EnemyDestroyPointView::Draw() const
	{
		int i = 0;
		int size = m_ScoreList.size();
		std::for_each(	m_ScoreList.begin(), m_ScoreList.end(),
						[this,&i,size]( const EnemyDestroyPointView::EnemyScore& data ){
			// スコア倍率を10倍したもの（難易度倍率*蓄積ポイント/最大HP）
			int scoreFact = 10 + ( ( data.m_Difficulty + 1 ) * 10 * data.m_Magnification ) / data.m_MaxHP;
			if( scoreFact > ( data.m_Difficulty + 1 ) * 10 ){
				scoreFact = ( data.m_Difficulty + 1 ) * 10;
			}
			if( scoreFact < 5 ){
				scoreFact = 5;
			}
			// 獲得スコア
			int addScore = ( data.m_BaseScore * scoreFact ) / 10;

			if( i == 0 ){
				DrawFontString(	m_ResourceMap, 350.0f, 30.0f + i * 15.0f, 1.0f,
								( m_RefleshCounter > 10 ? 255 : m_RefleshCounter * 25 ) << 24 | 0xFFFFFF,
								"%02d %8d", scoreFact, addScore );
			}
			else if( ( i < 10 && i == size - 1 ) || i == 10 ){
				DrawFontString(	m_ResourceMap, 350.0f, 30.0f + i * 15.0f, 1.0f,
								( m_RefleshCounter > 10 ? 0 : ( 250 - m_RefleshCounter * 25 ) ) << 24 | 0xFFFFFF,
								"%02d %8d", scoreFact, addScore );
			}
			else if( i < 10 ){
				DrawFontString( m_ResourceMap, 350.0f, 30.0f + i * 15.0f, 1.0f, "%02d %8d", scoreFact, addScore );
			}
			++i;
		} );
	}

	void EnemyDestroyPointView::Update()
	{
		int size = 0;
		if( m_RefleshCounter == 0 ){
			size = m_ScoreList.size();
			if( !m_ScoreList.empty() ){
				m_ScoreList.pop_front();
			}
			if( size >= 10 ){
				m_RefleshCounter = 10;
			}
			else if( size >= 5 ){
				m_RefleshCounter = 20;
			}
			else{
				m_RefleshCounter = 40;
			}
		}

		--m_RefleshCounter;
		++m_Counter;
	}

	void EnemyDestroyPointView::Init()
	{
		m_ScoreList.clear();
	}

	void EnemyDestroyPointView::OnEnemyDestroyed( const EnemyData& data )
	{
		EnemyDestroyPointView::EnemyScore score;
		
		score.m_BaseScore = data.m_Score;
		score.m_Magnification = data.m_ScoreBonus;
		score.m_MaxHP = data.m_MaxHP;
		score.m_Difficulty = data.m_pStageData->m_Difficulty;

		if( m_ScoreList.empty() ){
			m_RefleshCounter = 40;
		}

		m_ScoreList.push_back( score );
	}


}
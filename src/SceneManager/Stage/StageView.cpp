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

			float dispX;
			float dispY;

			if( data.m_UpdateCounter <= 0 ){
				dispX = 350.0f;
			}
			else{
				dispX = 350.0f - data.m_UpdateCounter * 5.0f;
			}

			if( m_RefleshCounter >= 10 ){
				dispY = 30.0f + i * 15.0f;
			}
			else{
				dispY = 30.0f - ( 10 - m_RefleshCounter ) * 1.5f + i * 15.0f;
			}

			int color;
			color =	( ( scoreFact + 160 ) * 0xFF / 210 ) << 16 |
					( ( scoreFact + 160 ) * 0xFF / 210 ) << 8 |
					( ( scoreFact + 160 ) * 0xFF / 210 );

			if( i == 0 ){
				for( int iter = 0; iter < 1; ++iter ){
					DrawFontString(	m_ResourceMap, dispX, dispY, 1.0f,
									( m_RefleshCounter > 10 ? 255 : m_RefleshCounter * 25 ) << 24 | color,
									"x%d.%d %6d", scoreFact / 10, scoreFact % 10, addScore );
				}
			}
			else if( i < 10 ){
				for( int iter = 0; iter < 1; ++iter ){
					DrawFontString( m_ResourceMap, dispX, dispY, 1.0f,
									( data.m_UpdateCounter <= 0 ? 255 : ( 10 - data.m_UpdateCounter ) * 25 ) << 24 | color, 
									"x%d.%d %6d", scoreFact / 10, scoreFact % 10, addScore );
				}
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
				m_RefleshCounter = 30;
			}
			else{
				m_RefleshCounter = 60;
			}
		}

		std::for_each(	m_ScoreList.begin(), m_ScoreList.end(),
						[this]( EnemyDestroyPointView::EnemyScore& data ){
			--data.m_UpdateCounter;
		} );

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
		score.m_UpdateCounter = 10;

		if( m_ScoreList.empty() ){
			m_RefleshCounter = 60;
		}

		m_ScoreList.push_back( score );
	}


}
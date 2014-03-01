#include "StageView.h"

#include <MAPIL/MAPIL.h>

#include "../../GameStateTypes.h"

#include "../../Util.h"
#include "../../ResourceID.h"

#include "../../Stage.h"
#include "../../Player.h"
#include "../../Item.h"

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
	// DifficultyView
	//-------------------------------------------

	DifficultyView::DifficultyView( bool firstStage, int difficulty ) : m_Difficulty( difficulty )
	{
		if( firstStage ){
			m_Counter = 80;
		}
	}

	DifficultyView::~DifficultyView()
	{
	}

	void DifficultyView::PostDraw() const
	{
		const char* DIFFICULTY_DISP[] = { "calm", "easy", "normal", "hard", "hazard" };
		const unsigned int DIFFICULTY_COLOR[] = { 0xFF5555FF, 0xFF77AAFF, 0xFF55FF55, 0xFFFFFF55, 0xFFFF5555 };
		
		const float DISP_X = 50.0f;
		const float DISP_Y	= 440.0f;

		DrawFontString( m_ResourceMap, DISP_X, DISP_Y, 1.0f, DIFFICULTY_COLOR[ m_Difficulty ], DIFFICULTY_DISP[ m_Difficulty ] );

		if( m_Counter > 0 ){
			MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
			DrawFontString(	m_ResourceMap, DISP_X, DISP_Y,
							1.1f + 0.1f * std::sin( MAPIL::DegToRad( m_Counter * 50 ) ),
							false, DIFFICULTY_COLOR[ m_Difficulty ], DIFFICULTY_DISP[ m_Difficulty ] );
			MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
		}
	}

	void DifficultyView::Update()
	{
		--m_Counter;
		if( m_Counter < 0 ){
			m_Counter = 0;
		}
	}

	void DifficultyView::Init()
	{
	}

	//-------------------------------------------
	// EnemyDestroyPointView
	//-------------------------------------------

	EnemyDestroyPointView::EnemyDestroyPointView() :	StageView(),
														m_Counter( 0 ),
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

	//-------------------------------------------
	// CrystalView
	//-------------------------------------------

	CrystalView::CrystalView( StageData* pStageData, FixedGameData* pFixedData ) :	StageView(),
																					m_Counter( 0 ),
																					m_pStageData( pStageData ),
																					m_pFixedData( pFixedData )
	{
	}

	CrystalView::~CrystalView()
	{
		m_Counter = 0;
	}

	void CrystalView::Draw() const
	{
	}

	void CrystalView::PostDraw() const
	{
		const float RIGHT_DISP_OFFSET_X = 525.0f;
		const float RIGHT_DISP_OFFSET_Y	= 218.0f;

		DrawFontString( m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, RIGHT_DISP_OFFSET_Y + 35.0f, 1.0f, "%d/%d",
						( m_pFixedData->m_PrevCrystal + m_pStageData->m_GameData.m_CrystalTotal ) - ( m_pStageData->m_GameData.m_CrystalUsed + m_pFixedData->m_PrevCrystalUsed ),
						m_pFixedData->m_PrevCrystal + m_pStageData->m_GameData.m_CrystalTotal );
		if( m_Counter > 0 ){
			MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
			for( int i = 0; i < 1; ++i ){
				DrawFontString(	m_ResourceMap,
								RIGHT_DISP_OFFSET_X + 5.0f,
								RIGHT_DISP_OFFSET_Y + 35.0f,
								1.0f + m_Counter * 0.07f, false, 0xAAFFFF33,
								"%d/%d",
								( m_pFixedData->m_PrevCrystal + m_pStageData->m_GameData.m_CrystalTotal ) - ( m_pStageData->m_GameData.m_CrystalUsed + m_pFixedData->m_PrevCrystalUsed ),
								m_pFixedData->m_PrevCrystal + m_pStageData->m_GameData.m_CrystalTotal );
			}
			MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
		}
		DrawFontString( m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, RIGHT_DISP_OFFSET_Y + 55.0f, 1.0f, 0xFF888888, "%d", m_pStageData->m_GameData.m_CrystalTotal );
	}

	void CrystalView::Update()
	{
		--m_Counter;
		if( m_Counter < 0 ){
			m_Counter = 0;
		}
	}

	void CrystalView::Init()
	{
	}

	void CrystalView::OnCrystalObtained( int crystal )
	{
		m_Counter = 10;
	}

	//-------------------------------------------
	// KilledView
	//-------------------------------------------

	KilledView::KilledView( StageData* pStageData, FixedGameData* pFixedData ) :	StageView(),
																					m_Counter( 0 ),
																					m_pStageData( pStageData ),
																					m_pFixedData( pFixedData )
	{
	}

	KilledView::~KilledView()
	{
		m_Counter = 0;
	}

	void KilledView::Draw() const
	{
	}

	void KilledView::PostDraw() const
	{
		const float RIGHT_DISP_OFFSET_X = 525.0f;
		const float RIGHT_DISP_OFFSET_Y	= 152.0f;

		DrawFontString( m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, RIGHT_DISP_OFFSET_Y + 35.0f, 1.0f, "%d", m_pStageData->m_GameData.m_Killed + m_pFixedData->m_PrevKilled );
		DrawFontString( m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, RIGHT_DISP_OFFSET_Y + 55.0f, 1.0f, 0xFF888888, "%d", m_pStageData->m_GameData.m_Killed );

		if( m_Counter > 0 ){
			MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
			for( int i = 0; i < 1; ++i ){
				DrawFontString(	m_ResourceMap,
								RIGHT_DISP_OFFSET_X + 5.0f,
								RIGHT_DISP_OFFSET_Y + 35.0f,
								1.0f + m_Counter * 0.07f, false, 0xFFFF3333,
								"%d",
								m_pStageData->m_GameData.m_Killed + m_pFixedData->m_PrevKilled );
			}
			MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
		}
	}

	void KilledView::Update()
	{
		--m_Counter;
		if( m_Counter < 0 ){
			m_Counter = 0;
		}
	}

	void KilledView::Init()
	{
	}

	void KilledView::OnEnemyDestroyed( const EnemyData& data )
	{
		m_Counter = 10;
	}

	//-------------------------------------------
	// ProgressView
	//-------------------------------------------

	ProgressView::ProgressView( StageData* pStageData ) :	StageView(),
															m_Counter( 0 ),
															m_pStageData( pStageData )
	{
	}

	ProgressView::~ProgressView()
	{
	}

	void ProgressView::PostDraw() const
	{
		const float RIGHT_DISP_OFFSET_X = 525.0f;
		const float RIGHT_DISP_OFFSET_Y	= 308.0f;

		if( m_pStageData->m_Frame < m_pStageData->m_FrameTotal ){
			MAPIL::DrawClipedTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_STAGE_BACKGROUND_2 ],
										RIGHT_DISP_OFFSET_X - 12.0f, RIGHT_DISP_OFFSET_Y + 16.0f, 0.13f + m_pStageData->m_Frame * 0.75f / m_pStageData->m_FrameTotal, 1.0f, false, ( 255 - ( m_Counter % 30 ) * 2 ) << 24 | 0x00AAFF );
		}
		else{
			MAPIL::DrawClipedTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_STAGE_BACKGROUND_2 ],
										RIGHT_DISP_OFFSET_X - 12.0f, RIGHT_DISP_OFFSET_Y + 16.0f, 1.0f, 1.0f, false, ( 255 - ( m_Counter % 30 ) * 2 ) << 24 | 0x00AAFF );
		}
	}

	void ProgressView::Update()
	{
		++m_Counter;
	}

	//-------------------------------------------
	// EnemyInfoView
	//-------------------------------------------

	EnemyInfoView::EnemyInfoView( StageData* pStageData ) :	StageView(),
															m_StaticCounter( 0 ),
															m_Counter( 0 ),
															m_pStageData( pStageData ),
															m_EnemyName(),
															m_EnemyHP( 0 ),
															m_EnemyCons( 0 )
	{
	}

	EnemyInfoView::~EnemyInfoView()
	{
	}

	void EnemyInfoView::PostDraw() const
	{
		const float RIGHT_DISP_OFFSET_X = 525.0f;
		const float RIGHT_DISP_OFFSET_Y	= 365.0f;

		if( m_Counter > 0 ){
			DrawFontString( m_ResourceMap, RIGHT_DISP_OFFSET_X + 29.0f, RIGHT_DISP_OFFSET_Y, 0.7f, 0xFFAAFFAA, "%s", m_EnemyName.c_str() );
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								RIGHT_DISP_OFFSET_X + 50.0f, RIGHT_DISP_OFFSET_Y + 16.0f, m_EnemyHP * 3.55f / m_EnemyMaxHP, 0.55f, false, ( 255 - ( m_StaticCounter % 5 ) * 8 ) << 24 | 0xFF2222 );
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								RIGHT_DISP_OFFSET_X + 50.0f, RIGHT_DISP_OFFSET_Y + 34.0f, m_EnemyCons * 3.55f / m_EnemyMaxCons, 0.55f, false, ( 255 - ( m_StaticCounter % 10 ) * 8 ) << 24 | 0x22FF22 );
		}
	}

	void EnemyInfoView::Update()
	{
		--m_Counter;
		if( m_Counter < 0 ){
			m_Counter = 0;
		}

		++m_StaticCounter;
	}

	void EnemyInfoView::OnEnemyDamaged( const EnemyData& data )
	{
		m_EnemyName = data.m_Name;
		m_EnemyHP = data.m_HP;
		m_EnemyMaxHP = data.m_MaxHP;
		m_EnemyCons = data.m_ConsGauge;
		m_EnemyMaxCons = data.m_MaxConsGauge;
		m_Counter = 60;
	}

	void EnemyInfoView::OnEnemyDestroyed( const EnemyData& data )
	{
		m_EnemyName = "";
		m_EnemyHP = 0;
		m_EnemyMaxHP = data.m_MaxHP;
		m_EnemyCons = 0;
		m_EnemyMaxCons = data.m_MaxConsGauge;
		m_Counter = 0;
	}

	//-------------------------------------------
	// PlayerHPView
	//-------------------------------------------

	PlayerHPView::PlayerHPView( Player* pPlayer ) :	StageView(),
													m_StaticCounter( 0 ),
													m_Counter( 0 ),
													m_pPlayer( pPlayer ),
													m_Mode( PlayerHPView::Mode::NORMAL )
	{
	}

	PlayerHPView::~PlayerHPView()
	{
	}

	void PlayerHPView::PostDraw() const
	{
		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );

		int base;
		float scale;
		int multiplicity = 2;
		int hp = m_pPlayer->GetHP();

		if( m_Mode == PlayerHPView::Mode::DAMAGED ){
			base = hp;
			scale = 1.0f + ( 20 - m_Counter ) * 0.05f;
			for( int m = 0; m < multiplicity; ++m ){
				for( int i = 0; i < m_PrevHP - hp; ++i ){
					MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
										16.0f + 11.0f * ( hp + i ), 88.0f, 0.31f * scale, 0.6f * scale, true,
										( m_Counter * 8 ) << 24 | ( 120 + ( hp + i ) * 15 ) << 8 | ( ( hp + i ) * 10 ) | ( 255 - ( hp + i ) * 10 ) << 16 );
				}
			}
		}
		else if( m_Mode == PlayerHPView::Mode::RECOVERED ){
			base = hp;
			scale = 1.0f + m_Counter * 0.05f;
			for( int m = 0; m < multiplicity; ++m ){
				for( int i = 0; i < hp - m_PrevHP; ++i ){
					MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
										16.0f + 11.0f * ( m_PrevHP + i + i ), 88.0f, 0.31f * scale, 0.6f * scale, true,
										( ( 20 - m_Counter ) * 12 ) << 24 | ( 120 + ( m_PrevHP + i + i ) * 15 ) << 8 | ( ( m_PrevHP + i + i ) * 10 ) | ( 255 - ( m_PrevHP + i + i ) * 10 ) << 16 );
				}
			}
		}
		else{
			base = hp;
		}

		for( int i = 0; i < base && i < 10; ++i ){
			int surplus = ( 30 + m_StaticCounter - i * 3 ) % 30;
			if( surplus < 20 ){
				MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									16.0f + 11.0f * i, 88.0f, 0.51f - surplus * 0.01f, 0.9f - surplus * 0.015f, true,
									( 200 - surplus * 10 ) << 24 | ( 120 + i * 15 ) << 8 | ( i * 10 ) | ( 255 - i * 10 ) << 16 );
			}
			MAPIL::DrawTexture(	m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								16.0f + 11.0f * i, 88.0f, 0.31f, 0.6f, true, 0xFF000000 | ( 120 + i * 15 ) << 8 | ( i * 10 ) | ( 255 - i * 10 ) << 16 );
		}
		DrawFontString( m_ResourceMap, 80.0f, 110.0f, 1.0f, 0xFFFFFFFF, "%d",
						hp - 10 > 0 ? hp - 10 : 0 );

		
	}

	void PlayerHPView::Update()
	{
		--m_Counter;
		if( m_Counter < 0 ){
			m_Counter = 0;
			m_Mode = PlayerHPView::Mode::NORMAL;
		}

		if( m_Mode == PlayerHPView::Mode::NORMAL ){
			m_PrevHP = m_pPlayer->GetHP();
		}

		++m_StaticCounter;
	}

	void PlayerHPView::OnPlayerDamaged( const Player& player )
	{
		m_Counter = 20;
		m_Mode = PlayerHPView::Mode::DAMAGED;
	}

	void PlayerHPView::OnPlayerObtainedItem( const Item& item, const Player& player )
	{
		if( item.GetItemID() == ITEM_ID_RECOVER ){
			m_Counter = 20;
			m_Mode = PlayerHPView::Mode::RECOVERED;
		}
	}
}
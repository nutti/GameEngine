#include "Stage.h"

#include <MAPIL/MAPIL.h>

#include "Player.h"
#include "Enemy.h"
#include "PlayerShot.h"
#include "EnemyShot.h"
#include "Item.h"
#include "Effect.h"

#include "GameObjectBuilder.h"

#include "ScriptTypes.h"

#include "StageVCPU.h"

namespace GameEngine
{
	class Stage::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;		// ボタンの状態
		ResourceMap					m_ResourceMap;		// リソース割り当て
		ScriptData					m_ScriptData;		// スクリプトデータ
		StageData					m_Data;				// ステージ用データ

		StageVCPU					m_VM;				// 仮想マシン
	public:
		Impl( int stageNo, bool isReplay );
		~Impl();
		void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachScriptData( const ScriptData& data );
	};

	Stage::Impl::Impl( int stageNo, bool isReplay )	:	m_ButtonStatus(),
														m_Data(),
														m_VM()
	{
		m_Data.m_EnemyList.clear();
		m_Data.m_EnemyShotList.clear();
		m_Data.m_PlayerShotList.clear();
		m_Data.m_ItemList.clear();
		m_Data.m_EffectList.clear();
		m_Data.m_StageNo = stageNo;
		m_Data.m_IsReplay = isReplay;
		m_Data.m_Frame = 0;
		m_Data.m_Score = 0;
		m_Data.m_RandGen.SetRandSeed( 47 );
		m_Data.m_RandGen.Reset();
	}

	Stage::Impl::~Impl()
	{
		for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ++it ){
			delete ( *it );
		}
		for( PlayerShotList::iterator it = m_Data.m_PlayerShotList.begin(); it != m_Data.m_PlayerShotList.end(); ++it ){
			delete ( *it );
		}
		for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ++it ){
			delete ( *it );
		}
		for( ItemList::iterator it = m_Data.m_ItemList.begin(); it != m_Data.m_ItemList.end(); ++it ){
			delete ( *it );
		}
		for( EffectList::iterator it = m_Data.m_EffectList.begin(); it != m_Data.m_EffectList.end(); ++it ){
			delete ( *it );
		}


		MAPIL::SafeDelete( m_Data.m_pPlayer );
		m_Data.m_EnemyList.clear();
		m_Data.m_PlayerShotList.clear();
		m_Data.m_EnemyShotList.clear();
		m_Data.m_ItemList.clear();
		m_Data.m_EffectList.clear();
	}

	void Stage::Impl::Init()
	{	
		m_VM.Init( &m_ScriptData.m_pStageScriptData->m_Data, &m_Data );
		m_Data.m_ObjBuilder.AttachStageData( &m_Data );
		m_Data.m_pPlayer = reinterpret_cast < Player* > ( m_Data.m_ObjBuilder.CreateCollisionObject( GAME_OBJECT_ID_PLAYER ) );
	}

	SceneType Stage::Impl::Update()
	{
		// ランダムジェネレータの更新
		m_Data.m_RandGen.Update( m_Data );

		// スクリプトコマンドの実行
		m_VM.Run();
		if( m_VM.Terminated() ){
			return SCENE_TYPE_MENU;
		}

		// 衝突判定
		// 敵-プレイヤーショット
		for( PlayerShotList::iterator itShot = m_Data.m_PlayerShotList.begin(); itShot != m_Data.m_PlayerShotList.end(); ++itShot ){
			float psX;
			float psY;
			float psRad;
			( *itShot )->GetPos( &psX, &psY );
			psRad = ( *itShot )->GetCollisionRadius();
			for( EnemyList::iterator itEnemy = m_Data.m_EnemyList.begin(); itEnemy != m_Data.m_EnemyList.end(); ++itEnemy ){
				float eX;
				float eY;
				float eRad;
				( *itEnemy )->GetPos( &eX, &eY );
				eRad = ( *itEnemy )->GetCollisionRadius();
				float distance = ::sqrt( ( eX - psX ) * ( eX - psX ) + ( eY - psY ) * ( eY - psY ) );
				if( distance < psRad + eRad ){
					( *itShot )->Colided( *itEnemy );
					( *itEnemy )->Colided( *itShot );
				}
			}
		}
		// プレイヤー-敵ショット
		float pX;
		float pY;
		float pRad;
		m_Data.m_pPlayer->GetPos( &pX, &pY );
		pRad = m_Data.m_pPlayer->GetCollisionRadius();
		for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ++it ){
			float eX;
			float eY;
			float eRad;
			( *it )->GetPos( &eX, &eY );
			eRad = ( *it )->GetCollisionRadius();
			float distance = ::sqrt( ( eX - pX ) * ( eX - pX ) + ( eY - pY ) * ( eY - pY ) );
			if( distance < pRad + eRad ){
				( *it )->Colided( m_Data.m_pPlayer );
				m_Data.m_pPlayer->Colided( *it );
			}
		}
		// プレイヤー-アイテム
		for( ItemList::iterator it = m_Data.m_ItemList.begin(); it != m_Data.m_ItemList.end(); ++it ){
			float iX;
			float iY;
			float iRad;
			( *it )->GetPos( &iX, &iY );
			iRad = ( *it )->GetCollisionRadius();
			float distance = ::sqrt( ( iX - pX ) * ( iX - pX ) + ( iY - pY ) * ( iY - pY ) );
			if( distance < pRad + iRad ){
				( *it )->Colided( m_Data.m_pPlayer );
				m_Data.m_pPlayer->Colided( *it );
			}
		}

		// プレイヤーの更新
		m_Data.m_pPlayer->AttachButtonState( m_ButtonStatus );
		m_Data.m_pPlayer->Update();
		// 敵の更新
		for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ){
			if( !( *it )->Update() ){
				delete ( *it );
				it = m_Data.m_EnemyList.erase( it );
				continue;
			}
			++it;
		}
		// プレイヤーショットの更新
		for( PlayerShotList::iterator it = m_Data.m_PlayerShotList.begin(); it != m_Data.m_PlayerShotList.end(); ){
			if( !( *it )->Update() ){
				delete ( *it );
				it = m_Data.m_PlayerShotList.erase( it );
				continue;
			}
			++it;
		}
		// 敵ショットの更新
		for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ){
			if( !( *it )->Update() ){
				delete ( *it );
				it = m_Data.m_EnemyShotList.erase( it );
				continue;
			}
			++it;
		}
		// アイテムの更新
		for( ItemList::iterator it = m_Data.m_ItemList.begin(); it != m_Data.m_ItemList.end(); ){
			if( !( *it )->Update() ){
				delete ( *it );
				it = m_Data.m_ItemList.erase( it );
				continue;
			}
			++it;
		}
		// エフェクトの更新
		for( EffectList::iterator it = m_Data.m_EffectList.begin(); it != m_Data.m_EffectList.end(); ){
			if( !( *it )->Update() ){
				delete ( *it );
				it = m_Data.m_EffectList.erase( it );
				continue;
			}
			++it;
		}

		++m_Data.m_Frame;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void Stage::Impl::Draw()
	{
		// 2D画像描画開始
		MAPIL::BeginRendering2DGraphics();

		// プレイヤーの描画
		m_Data.m_pPlayer->Draw();

		// 敵の描画
		for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ++it ){
			( *it )->Draw();
		}
		// プレイヤーショットの描画
		for( PlayerShotList::iterator it = m_Data.m_PlayerShotList.begin(); it != m_Data.m_PlayerShotList.end(); ++it ){
			( *it )->Draw();
		}
		// 敵ショットの描画
		for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ++it ){
			( *it )->Draw();
		}
		// アイテムの描画
		for( ItemList::iterator it = m_Data.m_ItemList.begin(); it != m_Data.m_ItemList.end(); ++it ){
			( *it )->Draw();
		}
		// エフェクトの描画
		for( EffectList::iterator it = m_Data.m_EffectList.begin(); it != m_Data.m_EffectList.end(); ++it ){
			( *it )->Draw();
		}

		// 状態画面の描画
		// 左画面
		MAPIL::DrawString( 10.0f, 10.0f, "HP : %d / 10", m_Data.m_pPlayer->GetHP() );
		MAPIL::DrawString( 10.0f, 100.0f, "Cons Gauge" );
		MAPIL::DrawString( 10.0f, 120.0f, "1 : %d / 200", m_Data.m_pPlayer->GetConsGauge( 0 ) );
		MAPIL::DrawString( 10.0f, 140.0f, "2 : %d / 200", m_Data.m_pPlayer->GetConsGauge( 1 ) );
		MAPIL::DrawString( 10.0f, 160.0f, "3 : %d / 200", m_Data.m_pPlayer->GetConsGauge( 2 ) );
		MAPIL::DrawString( 10.0f, 220.0f, "Cons Level" );
		MAPIL::DrawString( 10.0f, 240.0f, "1 : %d / 10", m_Data.m_pPlayer->GetConsLevel( 0 ) );
		MAPIL::DrawString( 10.0f, 260.0f, "2 : %d / 10", m_Data.m_pPlayer->GetConsLevel( 1 ) );
		MAPIL::DrawString( 10.0f, 280.0f, "3 : %d / 10", m_Data.m_pPlayer->GetConsLevel( 2 ) );
		MAPIL::DrawString( 10.0f, 400.0f, "Hazard" );
		// 右画面
		MAPIL::DrawString( 500.0f, 100.0f, "HI Score : %d", m_Data.m_Score );
		MAPIL::DrawString( 500.0f, 140.0f, "Score : %d", m_Data.m_Score );
		MAPIL::DrawString( 500.0f, 180.0f, "Killed : %d", 0 );
		MAPIL::DrawString( 500.0f, 220.0f, "Crystal : %d", 0 );
		MAPIL::DrawString( 500.0f, 260.0f, "Progress" );
		MAPIL::DrawString( 500.0f, 400.0f, "FPS" );
		
		
		// 2D描画終了
		MAPIL::EndRendering2DGraphics();
	}

	void Stage::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_ButtonStatus = *pHolder;
	}

	void Stage::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_ResourceMap = map;
		m_Data.m_ObjBuilder.AttachResourceMap( map );
	}

	void Stage::Impl::AttachScriptData( const ScriptData& data )
	{
		m_ScriptData = data;
		m_Data.m_ObjBuilder.AttachScriptData( data );
	}



	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	Stage::Stage( int stageNo, bool isReplay ) : Scene(), m_pImpl( new Stage::Impl( stageNo, isReplay ) )
	{
	}

	Stage::~Stage()
	{
	}

	void Stage::Init()
	{
		m_pImpl->Init();
	}

	SceneType Stage::Update()
	{
		return m_pImpl->Update();
	}

	void Stage::Draw()
	{
		m_pImpl->Draw();
	}

	void Stage::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_pImpl->AttachButtonState( pHolder );
	}

	void Stage::AttachResourceMap( const ResourceMap& map )
	{
		m_pImpl->AttachResourceMap( map );
	}

	void Stage::AttachScriptData( const ScriptData& data )
	{
		m_pImpl->AttachScriptData( data );
	}
}
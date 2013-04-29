#include "Stage.h"

#include <MAPIL/MAPIL.h>

#include "Player.h"
#include "Enemy.h"
#include "PlayerShot.h"
#include "EnemyShot.h"
#include "Item.h"
#include "Effect.h"

#include "EnemyShotGroup.h"

#include "GameObjectBuilder.h"

#include "ScriptTypes.h"

#include "ResourceID.h"

#include "StageVCPU.h"
#include "StageBackground.h"

#include "ScoreManager.h"

#include "SpriteBatch.h"
#include "Util.h"
#include "Profiler.h"

#include <bitset>

namespace GameEngine
{
	// クリアした時のボーナス
	const int CLEARED_CRYSTAL_BONUS			= 10;
	const int CLEARED_CRYSTAL_REST_BONUS	= 20;
	const int CLEARED_HP_REST_BONUS			= 1000;
	const int CLEARED_KILLED_BONUS			= 20;

	class Stage::Impl
	{
	private:
		

		// フラグ管理
		enum StatusFlag
		{
			CLEARED					= 0,		// クリアしたか？
			PREPARED_NEXT_STAGE		= 1,		// 次のステージへ移行する準備が出来たか？
			STATUS_FLAG_TOTAL,
		};

		struct PrivateData
		{
			// 固定化されたゲームデータ
			struct FixedGameData
			{
				int		m_HIScore;			// ハイスコア
				int		m_PrevScore;		// 前のステージまでのスコア
				int		m_PrevKilled;		// 前のステージまでに倒した敵の数
				int		m_PrevCrystal;		// 前のステージまでに取得したクリスタルの総数
				int		m_PrevCrystalUsed;	// 前のステージまでに使用したクリスタルの総数
			};

			// 意識技
			struct ConsSkillModeData
			{
				bool			m_IsConsSkillMode;	// 意識スキルモードならtrue
				int				m_Counter;			// エフェクト用カウンタ
				int				m_PostCounter;		// エフェクト終了ときのカウンタ
				std::string		m_SkillName;		// 意識技名
				int				m_SkillCost;		// 消費意識ゲージ
			};
			// ボス状態に関するデータ
			struct BossModeData
			{
				bool			m_IsBossMode;		// ボス出現状態ならtrue
				int				m_DamagedCounter;	// ダメージを受けた時のエフェクトカウンタ
			};
			// ボムに関するデータ
			struct BombModeData
			{
				bool			m_IsBombMode;		// ボムモードならtrue
				int				m_BombedCounter;	// カウンタ
				int				m_ConsAttr;			// ボムの属性
				float			m_IniPosX;			// 初期位置（X座標）
				float			m_IniPosY;			// 初期位置（Y座標）
				int				m_DurationTime;		// 持続時間
			};
			// 最後にダメージを受けた敵に関するデータ
			struct LastDamagedEnemyData
			{
				std::string		m_EnemyName;		// 敵の名前
				int				m_HP;				// 現在のHP
				int				m_MaxHP;			// 最大HP
				int				m_ConsGauge;		// 現在の意識ゲージ
				int				m_MaxConsGauge;		// 最大意識ゲージ
			};
			// ステージクリア時の処理に関するデータ
			struct StageClearModeData
			{
				int				m_Counter;			// カウンタ
				int				m_NextStageNo;		// 次のステージ番号
			};
			// アイテム取得時の処理に関するデータ
			struct ItemObtainedData
			{
				int				m_Counter[ ITEM_ID_TOTAL ];		// アイテムごとにエフェクトカウンタ
			};
			// ステージ完了時のデータ
			struct StageResultData
			{
				int		m_Score;				// スコア
				int		m_Crystal;				// 取得クリスタルの総数
				int		m_CrystalUsed;			// 使用したクリスタル総数
				int		m_Killed;				// 倒した敵の数
				int		m_PosX;					// 最後にいた場所（X座標）
				int		m_PosY;					// 最後にいた場所（Y座標）
				int		m_HP;					// 残りHP
				int		m_ShotPower;			// ショットパワー
				int		m_Cons;					// 意識状態
				int		m_ConsGauge[ 3 ];		// 意識ゲージ
				int		m_ConsLevel[ 3 ];		// 意識レベル
				int		m_Progress;				// ステージ進行度
			};

			ConsSkillModeData						m_ConsSkillModeData;
			BossModeData							m_BossModeData;
			BombModeData							m_BombModeData;
			LastDamagedEnemyData					m_LastDamagedEnemyData;
			StageClearModeData						m_ClearModeData;
			ItemObtainedData						m_ItemObtainedData;
			FixedGameData							m_FixedGameData;
			std::bitset < STATUS_FLAG_TOTAL >		m_StatusFlags;
			InitialGameData							m_IniGameData;
			StageResultData							m_StageResultData;
		};


		ButtonStatusHolder					m_ButtonStatus;		// ボタンの状態
		ScriptData							m_ScriptData;		// スクリプトデータ
		StageData							m_Data;				// ステージ用データ
		ScoreManager						m_ScoreManager;		// スコア管理クラス
		ReplayDataRecord::StageKeyStates	m_KeyStateList;		// リプレイ用キーリスト

		StageVCPU					m_VM;				// 仮想マシン
		StageBackground				m_Background;		// 背景

		Profiler					m_Profiler;			// プロファイラ
		bool						m_DispProf;			// プロファイラの結果を表示

		PrivateData					m_PrivData;

		void ProcessCollision();		// 衝突判定
		void UpdateGameObjects();		// 全GameObjectの更新
		void ProcessMessage();			// ステージ用メッセージの処理
		void SaveStageResultData();		// ステージ完了データの保存
		int GetStageClearBonus() const;	// ステージクリア時のボーナス

		void UpdateConsSkillEffect();		// 意識技エフェクト処理の更新
		void DrawConsSkillEffect() const;	// 意識技エフェクト処理の描画
		void LateDrawConsSkillEffect() const;	// 意識技エフェクト処理の描画（後で描画）
		void UpdateBossModeEffect();		// ボスモード時のエフェクト処理の更新
		void DrawBossModeEffect() const;	// ボスモード時のエフェクト処理の描画
		void UpdateBombModeEffect();		// ボムモード時のエフェクト処理の更新
		void DrawBombModeEffect() const;	// ボムモード時のエフェクト処理の描画
		void DrawResult() const;			// ステージクリア時のリザルト画面表示

		// GameObject全削除メソッド
		void DeleteAllEnemyShots();
		void DeleteAllPlayerShots();
		void DeleteAllEnemies();
		void DeleteBoss();
		void DeleteAllEffects();
		void DeleteAllItems();

	public:
		Impl( int stageNo, bool isReplay );
		~Impl();
		void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachScriptData( const ScriptData& data );
		void AttachGameData( const GameDataMsg& msg );
		int GetProgress() const;
		int GetScore() const;
		int GetKilled() const;
		int GetCrystal() const;
		int GetCrystalUsed() const;

		void GetPlayerPos( int* pPosX, int* pPosY ) const;
		int GetPlayerHP() const;
		int GetPlayerShotPower() const;
		int GetPlayerCons() const;
		void GetPlayerConsGauge( int* pGauge ) const;
		void GetPlayerConsLevel( int* pLevel ) const;
		ReplayDataRecord::StageKeyStates GetKeyStates() const;

		void SetInitialData( const InitialGameData& data );
		void SetDifficulty( int difficulty );

		int GetNextStageNo() const;
		GameDataMsg GetFrameData() const;
	};

	Stage::Impl::Impl( int stageNo, bool isReplay )	:	m_ButtonStatus(),
														m_Data(),
														m_VM(),
														m_ScoreManager(),
														m_Background( stageNo )
	{
		MAPIL::ZeroObject( &m_PrivData.m_FixedGameData, sizeof( m_PrivData.m_FixedGameData ) );
		MAPIL::ZeroObject( &m_PrivData.m_IniGameData, sizeof( m_PrivData.m_IniGameData ) );

		m_Data.m_EnemyList.clear();
		m_Data.m_EnemyShotList.clear();
		m_Data.m_PlayerShotList.clear();
		m_Data.m_ItemList.clear();
		m_Data.m_EffectList.clear();
		m_Data.m_StageNo = stageNo;
		m_Data.m_IsReplay = isReplay;
		m_Data.m_Frame = 0;
		m_Data.m_RandGen.SetRandSeed( 47 );
		m_Data.m_RandGen.Reset();
		m_Data.m_ConsLevel = 700;
		m_Data.m_HasTermSig = false;
		//m_Data.m_BossMode = 0;
		m_Data.m_pBoss = NULL;
		m_Data.m_FrameTotal = 100000;

		m_PrivData.m_ConsSkillModeData.m_IsConsSkillMode = false;
		m_PrivData.m_ConsSkillModeData.m_Counter = 0;
		m_PrivData.m_ConsSkillModeData.m_PostCounter = 100;
		m_PrivData.m_ConsSkillModeData.m_SkillName = "";
		m_PrivData.m_BossModeData.m_DamagedCounter = 0;
		m_PrivData.m_BossModeData.m_IsBossMode = false;
		m_PrivData.m_BombModeData.m_BombedCounter = 0;
		m_PrivData.m_BombModeData.m_IsBombMode = false;
		m_PrivData.m_LastDamagedEnemyData.m_EnemyName = "";
		m_PrivData.m_LastDamagedEnemyData.m_HP = 0;
		m_PrivData.m_LastDamagedEnemyData.m_MaxHP = 10000;
		m_PrivData.m_LastDamagedEnemyData.m_ConsGauge = 0;
		m_PrivData.m_LastDamagedEnemyData.m_MaxConsGauge = 10000;
		m_PrivData.m_ClearModeData.m_Counter = 0;
		m_PrivData.m_ClearModeData.m_NextStageNo = 0;
		MAPIL::ZeroObject( m_PrivData.m_ItemObtainedData.m_Counter, sizeof( m_PrivData.m_ItemObtainedData.m_Counter ) );
		MAPIL::ZeroObject( &m_PrivData.m_StageResultData, sizeof( m_PrivData.m_StageResultData ) );

		m_Profiler.Clear();
		m_DispProf = false;
		m_KeyStateList.m_StatusList.clear();
	}

	Stage::Impl::~Impl()
	{
		// GameObjectの破棄
		DeleteAllPlayerShots();
		DeleteAllEnemies();
		DeleteBoss();
		DeleteAllEnemyShots();
		DeleteAllItems();
		DeleteAllEffects();
		for( EnemyShotGroupList::iterator it = m_Data.m_EnemyShotGroupList.begin(); it != m_Data.m_EnemyShotGroupList.end(); ++it ){
			delete ( *it );
		}
		m_Data.m_EnemyShotGroupList.clear();
		MAPIL::SafeDelete( m_Data.m_pPlayer );
	}

#if defined ( USE_FLOATING_POINT )
	void Stage::Impl::ProcessCollision()
	{
		// 衝突判定
		// 敵-プレイヤーショット
		// 敵-プレイヤー
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
				
				// 敵 - プレイヤー
				float distance = ( eX - psX ) * ( eX - psX ) + ( eY - psY ) * ( eY - psY );
				float radius = ( psRad + eRad ) * ( psRad + eRad );
				if( distance < radius ){
					( *itShot )->Colided( *itEnemy );
					( *itEnemy )->Colided( *itShot );
				}
			}
		}

		// ボス-プレイヤーショット
		if( m_Data.m_pBoss ){
			float eX;
			float eY;
			float eRad;
			m_Data.m_pBoss->GetPos( &eX, &eY );
			eRad = m_Data.m_pBoss->GetCollisionRadius();
			for( PlayerShotList::iterator itShot = m_Data.m_PlayerShotList.begin(); itShot != m_Data.m_PlayerShotList.end(); ++itShot ){
				float psX;
				float psY;
				float psRad;
				( *itShot )->GetPos( &psX, &psY );
				psRad = ( *itShot )->GetCollisionRadius();
				float distance = ( eX - psX ) * ( eX - psX ) + ( eY - psY ) * ( eY - psY );
				float radius = ( psRad + eRad ) * ( psRad + eRad );
				if( distance < radius ){
					( *itShot )->Colided( m_Data.m_pBoss );
					m_Data.m_pBoss->Colided( *itShot );
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
			if( ( *it )->DoesColideWithPlayer( pX, pY, pRad ) ){
				( *it )->Colided( m_Data.m_pPlayer );
				m_Data.m_pPlayer->Colided( *it );
			}
		}
		// プレイヤー-アイテム
		float extraRadius = 50.0f;
		if(	m_Data.m_pPlayer->GetCurCons() != PLAYER_CONS_MODE_NORMAL &&
			m_Data.m_pPlayer->GetConsGauge( m_Data.m_pPlayer->GetCurCons() - 1 ) > 0 ){
			extraRadius = 600.0f;
		}
		for( ItemList::iterator it = m_Data.m_ItemList.begin(); it != m_Data.m_ItemList.end(); ++it ){
			float iX;
			float iY;
			float iRad;
			
			( *it )->GetPos( &iX, &iY );
			iRad = ( *it )->GetCollisionRadius();
			float distance = ( iX - pX ) * ( iX - pX ) + ( iY - pY ) * ( iY - pY );
			float nearRadius = ( pRad + iRad + extraRadius ) * ( pRad + iRad + extraRadius );
			float colRadius = ( pRad + iRad ) * ( pRad + iRad );
			if( distance < nearRadius ){
				// 衝突時
				if( distance < colRadius ){
					( *it )->Colided( m_Data.m_pPlayer );
					m_Data.m_pPlayer->Colided( ( *it ).get() );
					//m_Data.m_pPlayer->Colided( ( *it ) );
				}
				// 近くにいる時
				else{
					( *it )->PlayerIsNear( m_Data.m_pPlayer );
				}
			}
		}

		// プレイヤー-敵
		// 敵-アイテム
		for( EnemyList::iterator itEnemy = m_Data.m_EnemyList.begin(); itEnemy != m_Data.m_EnemyList.end(); ++itEnemy ){
			float eX;
			float eY;
			float eRad;
			( *itEnemy )->GetPos( &eX, &eY );
			eRad = ( *itEnemy )->GetCollisionRadius();
				
			// 敵-プレイヤー
			float distance = ( eX - pX ) * ( eX - pX ) + ( eY - pY ) * ( eY - pY );
			float radius = ( pRad + eRad ) * ( pRad + eRad );
			if( distance < radius ){
				m_Data.m_pPlayer->Colided( *itEnemy );
				( *itEnemy )->Colided( m_Data.m_pPlayer );
			}
			// 敵-アイテム
			for( ItemList::iterator itItem = m_Data.m_ItemList.begin(); itItem != m_Data.m_ItemList.end(); ++itItem ){
				float iX;
				float iY;
				float iRad;
				( *itItem )->GetPos( &iX, &iY );
				iRad = ( *itItem )->GetCollisionRadius();
				float distance = ( iX - eX ) * ( iX - eX ) + ( iY - eY ) * ( iY - eY );
				float radius = ( iRad + eRad ) * ( iRad + eRad );
				if( distance < radius ){
					( *itEnemy )->Colided( ( *itItem ).get() );
				//	( *itEnemy )->Colided( ( *itItem ) );
					( *itItem )->Colided( *itEnemy );
				}
			}
		}			
	}
#elif defined ( USE_GAME_UNIT )
	void Stage::Impl::ProcessCollision()
	{
		// 衝突判定
		// 敵-プレイヤーショット
		// 敵-プレイヤー
		for( PlayerShotList::iterator itShot = m_Data.m_PlayerShotList.begin(); itShot != m_Data.m_PlayerShotList.end(); ++itShot ){
			GameUnit psX;
			GameUnit psY;
			GameUnit psRad;
			( *itShot )->GetPos( &psX, &psY );
			psRad = ( *itShot )->GetCollisionRadius();
			for( EnemyList::iterator itEnemy = m_Data.m_EnemyList.begin(); itEnemy != m_Data.m_EnemyList.end(); ++itEnemy ){
				GameUnit eX;
				GameUnit eY;
				GameUnit eRad;
				( *itEnemy )->GetPos( &eX, &eY );
				eRad = ( *itEnemy )->GetCollisionRadius();
				
				// 敵 - プレイヤー
				GameUnit distance = ( eX - psX ) * ( eX - psX ) + ( eY - psY ) * ( eY - psY );
				GameUnit radius = ( psRad + eRad ) * ( psRad + eRad );
				if( distance < radius ){
					( *itShot )->Colided( *itEnemy );
					( *itEnemy )->Colided( *itShot );
				}
			}
		}

		// ボス-プレイヤーショット
		if( m_Data.m_pBoss ){
			GameUnit eX;
			GameUnit eY;
			GameUnit eRad;
			m_Data.m_pBoss->GetPos( &eX, &eY );
			eRad = m_Data.m_pBoss->GetCollisionRadius();
			for( PlayerShotList::iterator itShot = m_Data.m_PlayerShotList.begin(); itShot != m_Data.m_PlayerShotList.end(); ++itShot ){
				GameUnit psX;
				GameUnit psY;
				GameUnit psRad;
				( *itShot )->GetPos( &psX, &psY );
				psRad = ( *itShot )->GetCollisionRadius();
				GameUnit distance = ( eX - psX ) * ( eX - psX ) + ( eY - psY ) * ( eY - psY );
				GameUnit radius = ( psRad + eRad ) * ( psRad + eRad );
				if( distance < radius ){
					( *itShot )->Colided( m_Data.m_pBoss );
					m_Data.m_pBoss->Colided( *itShot );
				}
			}
		}
		// プレイヤー-敵ショット
		GameUnit pX;
		GameUnit pY;
		GameUnit pRad;
		m_Data.m_pPlayer->GetPos( &pX, &pY );
		pRad = m_Data.m_pPlayer->GetCollisionRadius();
		for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ++it ){
			if( ( *it )->DoesColideWithPlayer( pX, pY, pRad ) ){
				( *it )->Colided( m_Data.m_pPlayer );
				m_Data.m_pPlayer->Colided( *it );
			}
		}
		// プレイヤー-アイテム
		GameUnit extraRadius = GameUnit( 50 );
		if(	m_Data.m_pPlayer->GetCurCons() != PLAYER_CONS_MODE_NORMAL &&
			m_Data.m_pPlayer->GetConsGauge( m_Data.m_pPlayer->GetCurCons() - 1 ) > 0 ){
			extraRadius = GameUnit( 600 );
		}
		for( ItemList::iterator it = m_Data.m_ItemList.begin(); it != m_Data.m_ItemList.end(); ++it ){
			GameUnit iX;
			GameUnit iY;
			GameUnit iRad;
			
			( *it )->GetPos( &iX, &iY );
			iRad = ( *it )->GetCollisionRadius();
			GameUnit distance = ( iX - pX ) * ( iX - pX ) + ( iY - pY ) * ( iY - pY );
			GameUnit nearRadius = ( pRad + iRad + extraRadius ) * ( pRad + iRad + extraRadius );
			GameUnit colRadius = ( pRad + iRad ) * ( pRad + iRad );
			if( distance < nearRadius ){
				// 衝突時
				if( distance < colRadius ){
					( *it )->Colided( m_Data.m_pPlayer );
					m_Data.m_pPlayer->Colided( ( *it ).get() );
					//m_Data.m_pPlayer->Colided( ( *it ) );
				}
				// 近くにいる時
				else{
					( *it )->PlayerIsNear( m_Data.m_pPlayer );
				}
			}
		}

		// プレイヤー-敵
		// 敵-アイテム
		for( EnemyList::iterator itEnemy = m_Data.m_EnemyList.begin(); itEnemy != m_Data.m_EnemyList.end(); ++itEnemy ){
			GameUnit eX;
			GameUnit eY;
			GameUnit eRad;
			( *itEnemy )->GetPos( &eX, &eY );
			eRad = ( *itEnemy )->GetCollisionRadius();
				
			// 敵-プレイヤー
			GameUnit distance = ( eX - pX ) * ( eX - pX ) + ( eY - pY ) * ( eY - pY );
			GameUnit radius = ( pRad + eRad ) * ( pRad + eRad );
			if( distance < radius ){
				m_Data.m_pPlayer->Colided( *itEnemy );
				( *itEnemy )->Colided( m_Data.m_pPlayer );
			}
			// 敵-アイテム
			for( ItemList::iterator itItem = m_Data.m_ItemList.begin(); itItem != m_Data.m_ItemList.end(); ++itItem ){
				GameUnit iX;
				GameUnit iY;
				GameUnit iRad;
				( *itItem )->GetPos( &iX, &iY );
				iRad = ( *itItem )->GetCollisionRadius();
				GameUnit distance = ( iX - eX ) * ( iX - eX ) + ( iY - eY ) * ( iY - eY );
				GameUnit radius = ( iRad + eRad ) * ( iRad + eRad );
				if( distance < radius ){
					( *itEnemy )->Colided( ( *itItem ).get() );
				//	( *itEnemy )->Colided( ( *itItem ) );
					( *itItem )->Colided( *itEnemy );
				}
			}
		}			
	}
#endif


	void Stage::Impl::UpdateGameObjects()
	{
		// プレイヤーの更新
		m_Data.m_pPlayer->AttachButtonState( m_ButtonStatus );
		m_Data.m_pPlayer->Update();
		// 敵ショットグループの更新（Updateが2回呼ばれないようにするため、敵の更新の上に持ってくる。）
		for( EnemyShotGroupList::iterator it = m_Data.m_EnemyShotGroupList.begin(); it != m_Data.m_EnemyShotGroupList.end(); ){
			if( !( *it )->Update() ){
				delete ( *it );
				it = m_Data.m_EnemyShotGroupList.erase( it );
				continue;
			}
			++it;
		}
		// 敵の更新
		for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ){
			if( !( *it )->Update() ){
				delete ( *it );
				it = m_Data.m_EnemyList.erase( it );
				continue;
			}
			++it;
		}
		// ボスの更新
		if( m_Data.m_pBoss ){
			if( !m_Data.m_pBoss->Update() ){
				MAPIL::SafeDelete( m_Data.m_pBoss );
				StageMessage msg;
				msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_BOSS_MODE_ENDED;
				m_Data.m_MsgQueue.push( msg );
			}
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
				//delete ( *it );
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
	}

	void Stage::Impl::ProcessMessage()
	{
		while( !m_Data.m_MsgQueue.empty() ){
			int msg = m_Data.m_MsgQueue.front().m_MsgID;
			switch( msg ){
				case StageMessage::STAGE_MESSAGE_ID_PLAYER_DAMAGED:
					for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ++it ){
						( *it )->PostMessage( EnemyShotMessage::ENEMY_SHOT_MESSAGE_ID_PLAYER_DAMAGED );
					}
					break;
				case StageMessage::STAGE_MESSAGE_ID_PLAYER_DESTORYED:
					DeleteAllEnemyShots();
					DeleteAllEnemies();
					m_Data.m_HasTermSig = true;
					break;
				case StageMessage::STAGE_MESSAGE_ID_BOSS_DAMAGED:
					if( m_PrivData.m_BossModeData.m_DamagedCounter <= 5 ){
						m_PrivData.m_BossModeData.m_DamagedCounter = 10;
					}
					break;
				case StageMessage::STAGE_MESSAGE_ID_BOSS_INVOKE_CONS_SKILL:
					m_PrivData.m_ConsSkillModeData.m_IsConsSkillMode = true;
					m_PrivData.m_ConsSkillModeData.m_Counter = 0;
					m_PrivData.m_ConsSkillModeData.m_PostCounter = 0;
					m_PrivData.m_ConsSkillModeData.m_SkillName = *m_Data.m_MsgQueue.front().m_MsgDataList[ 1 ].m_pString;
					m_PrivData.m_ConsSkillModeData.m_SkillCost = m_Data.m_MsgQueue.front().m_MsgDataList[ 0 ].m_Integer;
					MAPIL::SafeDelete( m_Data.m_MsgQueue.front().m_MsgDataList[ 1 ].m_pString );
					break;
				case StageMessage::STAGE_MESSAGE_ID_BOSS_STOP_CONS_SKILL:
					m_PrivData.m_ConsSkillModeData.m_IsConsSkillMode = false;
					break;
				case StageMessage::STAGE_MESSAGE_ID_BOSS_MODE_STARTED:
					m_PrivData.m_BossModeData.m_IsBossMode = true;
					break;
				case StageMessage::STAGE_MESSAGE_ID_BOSS_MODE_ENDED:
					m_PrivData.m_BossModeData.m_IsBossMode = false;
					break;
				case StageMessage::STAGE_MESSAGE_ID_BOSS_SHIFT_NEXT_MODE:
					for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ++it ){
						( *it )->PostMessage( EnemyShotMessage::ENEMY_SHOT_MESSAGE_ID_PLAYER_DAMAGED );
					}
					break;
				case StageMessage::STAGE_MESSAGE_ID_ENEMY_INVOKE_CONS_SKILL:
					break;
				case StageMessage::STAGE_MESSAGE_ID_PLAYER_BOMBED:
					m_PrivData.m_BombModeData.m_IsBombMode = true;
					m_PrivData.m_BombModeData.m_ConsAttr = m_Data.m_MsgQueue.front().m_MsgDataList[ 0 ].m_Integer;
					m_PrivData.m_BombModeData.m_IniPosX = m_Data.m_MsgQueue.front().m_MsgDataList[ 1 ].m_Float;
					m_PrivData.m_BombModeData.m_IniPosY = m_Data.m_MsgQueue.front().m_MsgDataList[ 2 ].m_Float;
					m_PrivData.m_BombModeData.m_DurationTime = m_Data.m_MsgQueue.front().m_MsgDataList[ 3 ].m_Integer;
					m_PrivData.m_BombModeData.m_BombedCounter = 0;
					break;
				case StageMessage::STAGE_MESSAGE_ID_ENEMY_DAMAGED:
					m_PrivData.m_LastDamagedEnemyData.m_EnemyName = *m_Data.m_MsgQueue.front().m_MsgDataList[ 0 ].m_pString;
					m_PrivData.m_LastDamagedEnemyData.m_HP = m_Data.m_MsgQueue.front().m_MsgDataList[ 1 ].m_Integer;
					m_PrivData.m_LastDamagedEnemyData.m_MaxHP = m_Data.m_MsgQueue.front().m_MsgDataList[ 2 ].m_Integer;
					m_PrivData.m_LastDamagedEnemyData.m_ConsGauge = m_Data.m_MsgQueue.front().m_MsgDataList[ 3 ].m_Integer;
					m_PrivData.m_LastDamagedEnemyData.m_MaxConsGauge = m_Data.m_MsgQueue.front().m_MsgDataList[ 4 ].m_Integer;
					MAPIL::SafeDelete( m_Data.m_MsgQueue.front().m_MsgDataList[ 0 ].m_pString );
					break;
				case StageMessage::STAGE_MESSAGE_ID_STAGE_CLEARED:
					m_PrivData.m_StatusFlags.set( CLEARED );
					m_PrivData.m_ClearModeData.m_Counter = 0;
					m_PrivData.m_ClearModeData.m_NextStageNo = m_Data.m_MsgQueue.front().m_MsgDataList[ 0 ].m_Integer;
					break;
				case StageMessage::STAGE_MESSAGE_ID_ITEM_OBTAINED:{
					int itemID = m_Data.m_MsgQueue.front().m_MsgDataList[ 0 ].m_Integer;
					const int counts[ ITEM_ID_TOTAL ] = { 60, 60, 60, 60, 60, 60 };
					m_PrivData.m_ItemObtainedData.m_Counter[ itemID ] = counts[ itemID ];
					break;
				}
				default:
					break;
			}
			m_Data.m_MsgQueue.pop();
		}
	}

	void Stage::Impl::DeleteAllEnemyShots()
	{
		for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ++it ){
			delete ( *it );
		}
		m_Data.m_EnemyShotList.clear();
	}

	void Stage::Impl::DeleteAllPlayerShots()
	{
		for( PlayerShotList::iterator it = m_Data.m_PlayerShotList.begin(); it != m_Data.m_PlayerShotList.end(); ++it ){
			delete ( *it );
		}
		m_Data.m_PlayerShotList.clear();
	}

	void Stage::Impl::DeleteAllEnemies()
	{
		for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ++it ){
			delete ( *it );
		}
		m_Data.m_EnemyList.clear();
	}

	void Stage::Impl::DeleteBoss()
	{
		MAPIL::SafeDelete( m_Data.m_pBoss );
	}

	void Stage::Impl::DeleteAllEffects()
	{
		for( EffectList::iterator it = m_Data.m_EffectList.begin(); it != m_Data.m_EffectList.end(); ++it ){
			delete ( *it );
		}
		m_Data.m_EffectList.clear();
	}

	void Stage::Impl::DeleteAllItems()
	{
		for( ItemList::iterator it = m_Data.m_ItemList.begin(); it != m_Data.m_ItemList.end(); ++it ){
			//delete ( *it );
		}
		m_Data.m_ItemList.clear();
	}

	void Stage::Impl::UpdateConsSkillEffect()
	{
		if( m_PrivData.m_ConsSkillModeData.m_IsConsSkillMode ){
			++m_PrivData.m_ConsSkillModeData.m_Counter;
		}
		else{
			++m_PrivData.m_ConsSkillModeData.m_PostCounter;
			//if( m_Data.m_pBoss ){
			//	m_PrivData.m_ConsSkillModeData.m_PrevConsGauge = m_Data.m_pBoss->GetConsGauge();
			//}
		}
	}

	void Stage::Impl::LateDrawConsSkillEffect() const
	{
		if( !m_Data.m_pBoss ){
			return;
		}

		if( m_PrivData.m_ConsSkillModeData.m_IsConsSkillMode ){
			if( m_PrivData.m_ConsSkillModeData.m_Counter < 60 ){
				DrawFontString( m_Data.m_ResourceMap, 400.0f - m_PrivData.m_ConsSkillModeData.m_Counter, 30.0f, 0.5f,
								( ( m_PrivData.m_ConsSkillModeData.m_Counter * 4 ) << 24 ) | 0xFFFFFF,
								m_PrivData.m_ConsSkillModeData.m_SkillName.c_str() );
			}
			else{
				DrawFontString( m_Data.m_ResourceMap, 340.0f, 30.0f, 0.5f, m_PrivData.m_ConsSkillModeData.m_SkillName.c_str() );
			}

			if( m_PrivData.m_ConsSkillModeData.m_Counter <= 60 ){
				if( m_PrivData.m_ConsSkillModeData.m_Counter <= 30 ){
					MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
										150.0f + m_Data.m_pBoss->GetConsGauge() * 16.0f / 100.0f, 20.0f,
										( m_PrivData.m_ConsSkillModeData.m_SkillCost ) * 10.0f / 1000.0f, 0.5f, false,
										0xCC00FF00 );
					if( ( m_PrivData.m_ConsSkillModeData.m_Counter % 4 ) == 0 ){
						MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
						MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
											150.0f + m_Data.m_pBoss->GetConsGauge() * 16.0f / 100.0f, 20.0f,
											( m_PrivData.m_ConsSkillModeData.m_SkillCost ) * 10.0f / 1000.0f, 0.5f, false,
											0x99FFFFFF );
						MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
					}
				}
				else{
					float scale = - 0.5f + m_PrivData.m_ConsSkillModeData.m_Counter / 30.0f;
					MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
					int length = m_PrivData.m_ConsSkillModeData.m_SkillCost;
					MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
										150.0f + ( m_Data.m_pBoss->GetConsGauge() + length / 2 ) * 16.0f / 100.0f, 24.0f,
										( m_PrivData.m_ConsSkillModeData.m_SkillCost ) * 10.0f / 1000.0f + scale, 0.5f + scale, true,
										( ( 60 - m_PrivData.m_ConsSkillModeData.m_Counter ) * 5 ) << 24 | ( ( 60 - m_PrivData.m_ConsSkillModeData.m_Counter )* 10 ) << 8 );
					MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
				}
			}
		}
		else{
			if( m_PrivData.m_ConsSkillModeData.m_PostCounter < 30 ){
				DrawFontString(	m_Data.m_ResourceMap, 340.0f - m_PrivData.m_ConsSkillModeData.m_PostCounter, 30.0f, 0.5f,
								( ( ( 30 - m_PrivData.m_ConsSkillModeData.m_PostCounter ) * 8 ) << 24 ) | 0xFFFFFF, m_PrivData.m_ConsSkillModeData.m_SkillName.c_str() );
			}
		}
	}

	void Stage::Impl::DrawConsSkillEffect() const
	{
		if( !m_Data.m_pBoss ){
			return;
		}

		if( m_PrivData.m_ConsSkillModeData.m_IsConsSkillMode ){
			if( m_PrivData.m_ConsSkillModeData.m_Counter < 60 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									0.0f, 0.0f,
									40.0f, 30.0f, false,
									( ( m_PrivData.m_ConsSkillModeData.m_Counter * 3 ) << 24 ) | 0x333333 );
			}
			else{
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									0.0f, 0.0f,
									40.0f, 30.0f, false, 180 << 24 | 0x333333 );
			}
		}
		else{
			if( m_PrivData.m_ConsSkillModeData.m_PostCounter < 30 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									0.0f, 0.0f,
									40.0f, 30.0f, false,
									( ( ( 30 - m_PrivData.m_ConsSkillModeData.m_PostCounter ) * 6 ) << 24 ) | 0x333333 );
			}
		}
	}

	void Stage::Impl::UpdateBossModeEffect()
	{
		--m_PrivData.m_BossModeData.m_DamagedCounter;
	}

	void Stage::Impl::DrawBossModeEffect() const
	{
		if( m_PrivData.m_BossModeData.m_IsBossMode ){
			if( m_Data.m_pBoss ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									150.0f, 10.0f,
									m_Data.m_pBoss->GetHP() * 20.0f / m_Data.m_pBoss->GetMaxHP(), 0.5f, false, 0xCCFF0000 );
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									150.0f, 20.0f,
									m_Data.m_pBoss->GetConsGauge() * 10.0f / 1000.0f, 0.5f, false, 0xCC00FF00 );
				if( m_PrivData.m_BossModeData.m_DamagedCounter >= 5 ){
					MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
					MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
										150.0f, 10.0f,
										m_Data.m_pBoss->GetHP() * 20.0f / m_Data.m_pBoss->GetMaxHP(), 0.5f, false,
										( m_PrivData.m_BossModeData.m_DamagedCounter * 10 ) << 24 | 0xFFFFFF );
					MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
				}

				for( int i = 0; i < 6; ++i ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									150.0f + i * 64.0f, 10.0f,
									0.1f, 0.5f, false, 0xFFFFFFFF );
				}
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									246.0f, 20.0f,
									0.1f, 0.5f, false, 0xFFFFFFFF );
			}
		}
	}

#if defined ( USE_FLOATING_POINT )
	void Stage::Impl::UpdateBombModeEffect()
	{
		if( m_PrivData.m_BombModeData.m_BombedCounter < m_PrivData.m_BombModeData.m_DurationTime ){
			if( m_PrivData.m_BombModeData.m_ConsAttr == PLAYER_CONS_MODE_RED ){
				float iX = m_PrivData.m_BombModeData.m_IniPosX;
				float iY = m_PrivData.m_BombModeData.m_IniPosY;
				
				float colFirst = m_PrivData.m_BombModeData.m_BombedCounter * 7.0f - 130.0f;
				if( colFirst >= 0.0f ){
					colFirst *= colFirst;
				}
				else{
					colFirst = 0.0f;
				}

				float colLast = ( m_PrivData.m_BombModeData.m_BombedCounter * 7.0f ) * ( m_PrivData.m_BombModeData.m_BombedCounter * 7.0f );
			
				// 敵とボムの衝突判定
				for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ){
					float eX;
					float eY;
					( *it )->GetPos( &eX, &eY );
					float radius = ( eX - iX ) * ( eX - iX ) + ( eY - iY ) * ( eY - iY );
					if( radius > colFirst && radius < colLast ){
						( *it )->Damage( 15 );
					}
					++it;
				}
				// ボスとボムの衝突判定
				if( m_Data.m_pBoss ){
					float eX;
					float eY;
					m_Data.m_pBoss->GetPos( &eX, &eY );
					float radius = ( eX - iX ) * ( eX - iX ) + ( eY - iY ) * ( eY - iY );
					if( radius > colFirst && radius < colLast ){
						m_Data.m_pBoss->Damage( 15 );
					}
				}
				// 敵ショットの更新
				for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ){
					float eX;
					float eY;
					( *it )->GetPos( &eX, &eY );
					float radius = ( eX - iX ) * ( eX - iX ) + ( eY - iY ) * ( eY - iY );
					if( radius > colFirst && radius < colLast ){
						( *it )->PostMessage( EnemyShotMessage::ENEMY_SHOT_MESSAGE_ID_PLAYER_BOMBED );
					}
					++it;
				}
			}
			else if( m_PrivData.m_BombModeData.m_ConsAttr == PLAYER_CONS_MODE_BLUE ){
				if( m_PrivData.m_BombModeData.m_BombedCounter % 30 == 0 ){
					// 敵の行動停止
					for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ){
						( *it )->Pause();
						++it;
					}
					// ボスの行動停止
					if( m_Data.m_pBoss ){
						m_Data.m_pBoss->Pause();
					}
					// 敵ショットの行動停止
					for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ){
						( *it )->Pause();
						++it;
					}
				}
				else if( m_PrivData.m_BombModeData.m_BombedCounter == m_PrivData.m_BombModeData.m_DurationTime - 1 ){
					// 敵の行動再開
					for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ){
						( *it )->Resume();
						++it;
					}
					// ボスの行動再開
					if( m_Data.m_pBoss ){
						m_Data.m_pBoss->Resume();
					}
					// 敵ショットの削除
					for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ){
						( *it )->Resume();
						( *it )->PostMessage( EnemyShotMessage::ENEMY_SHOT_MESSAGE_ID_PLAYER_BOMBED );
						++it;
					}
				}
			}
			else if( m_PrivData.m_BombModeData.m_ConsAttr == PLAYER_CONS_MODE_GREEN ){
				if( m_PrivData.m_BombModeData.m_BombedCounter % 2 == 0 ){
					PlayerShot* pNewShot = m_Data.m_ObjBuilder.CreatePlayerShot( 4 );
					float pX;
					float pY;
					m_Data.m_pPlayer->GetPos( &pX, &pY );
					float random1 = -15.0f + m_Data.m_RandGen.GetRand() * 30.0f / m_Data.m_RandGen.GetRandMax();
					float random2 = -15.0f + m_Data.m_RandGen.GetRand() * 30.0f / m_Data.m_RandGen.GetRandMax();
					float random3 = 45.0f - m_Data.m_RandGen.GetRand() * 90.0f / m_Data.m_RandGen.GetRandMax();
					pNewShot->SetPos( pX + random1, pY + random2 );
					pNewShot->SetSpeed( 2.0f );
					pNewShot->SetShotPower( 20 );
					pNewShot->SetAngle( random3 );
					pNewShot->SetStageData( &m_Data );
					m_Data.m_PlayerShotList.push_back( pNewShot );
				}
				// 敵ショットの削除
				if( m_PrivData.m_BombModeData.m_BombedCounter % 20 == 0 ){
					for( PlayerShotList::iterator itP = m_Data.m_PlayerShotList.begin(); itP != m_Data.m_PlayerShotList.end(); ++itP ){
						if( ( *itP )->GetID() != 4 ){
							continue;
						}
						float iX;
						float iY;
						float iR = ( *itP )->GetCollisionRadius();
						( *itP )->GetPos( &iX, &iY );
						for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ){
							float eX;
							float eY;
							float eR = ( *it )->GetCollisionRadius();
							( *it )->GetPos( &eX, &eY );
							float range = ( eX - iX ) * ( eX - iX ) + ( eY - iY ) * ( eY - iY ) + 20.0f * 20.0f;
							if( range < ( iR + eR ) * ( iR + eR ) ){
								( *it )->PostMessage( EnemyShotMessage::ENEMY_SHOT_MESSAGE_ID_PLAYER_BOMBED );
							}
							++it;
						}
					}
				}
			}
			++m_PrivData.m_BombModeData.m_BombedCounter;
		}
		else{
			m_PrivData.m_BombModeData.m_IsBombMode = false;
		}
	}
#elif defined ( USE_GAME_UNIT )
	void Stage::Impl::UpdateBombModeEffect()
	{
		if( m_PrivData.m_BombModeData.m_BombedCounter < m_PrivData.m_BombModeData.m_DurationTime ){
			if( m_PrivData.m_BombModeData.m_ConsAttr == PLAYER_CONS_MODE_RED ){
				GameUnit iX = m_PrivData.m_BombModeData.m_IniPosX;
				GameUnit iY = m_PrivData.m_BombModeData.m_IniPosY;
				
				GameUnit colFirst = GameUnit( m_PrivData.m_BombModeData.m_BombedCounter ) * GameUnit( 7 ) - GameUnit( 130 );
				if( colFirst >= GameUnit( 0 ) ){
					colFirst *= colFirst;
				}
				else{
					colFirst = GameUnit( 0 );
				}

				GameUnit colLast = ( GameUnit( m_PrivData.m_BombModeData.m_BombedCounter ) * GameUnit( 7 ) ) * ( GameUnit( m_PrivData.m_BombModeData.m_BombedCounter ) * GameUnit( 7 ) );
			
				// 敵とボムの衝突判定
				for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ){
					GameUnit eX;
					GameUnit eY;
					( *it )->GetPos( &eX, &eY );
					GameUnit radius = ( eX - iX ) * ( eX - iX ) + ( eY - iY ) * ( eY - iY );
					if( radius > colFirst && radius < colLast ){
						( *it )->Damage( 15 );
					}
					++it;
				}
				// ボスとボムの衝突判定
				if( m_Data.m_pBoss ){
					GameUnit eX;
					GameUnit eY;
					m_Data.m_pBoss->GetPos( &eX, &eY );
					GameUnit radius = ( eX - iX ) * ( eX - iX ) + ( eY - iY ) * ( eY - iY );
					if( radius > colFirst && radius < colLast ){
						m_Data.m_pBoss->Damage( 15 );
					}
				}
				// 敵ショットの更新
				for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ){
					GameUnit eX;
					GameUnit eY;
					( *it )->GetPos( &eX, &eY );
					GameUnit radius = ( eX - iX ) * ( eX - iX ) + ( eY - iY ) * ( eY - iY );
					if( radius > colFirst && radius < colLast ){
						( *it )->PostMessage( EnemyShotMessage::ENEMY_SHOT_MESSAGE_ID_PLAYER_BOMBED );
					}
					++it;
				}
			}
			else if( m_PrivData.m_BombModeData.m_ConsAttr == PLAYER_CONS_MODE_BLUE ){
				if( m_PrivData.m_BombModeData.m_BombedCounter % 30 == 0 ){
					// 敵の行動停止
					for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ){
						( *it )->Pause();
						++it;
					}
					// ボスの行動停止
					if( m_Data.m_pBoss ){
						m_Data.m_pBoss->Pause();
					}
					// 敵ショットの行動停止
					for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ){
						( *it )->Pause();
						++it;
					}
				}
				else if( m_PrivData.m_BombModeData.m_BombedCounter == m_PrivData.m_BombModeData.m_DurationTime - 1 ){
					// 敵の行動再開
					for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ){
						( *it )->Resume();
						++it;
					}
					// ボスの行動再開
					if( m_Data.m_pBoss ){
						m_Data.m_pBoss->Resume();
					}
					// 敵ショットの削除
					for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ){
						( *it )->Resume();
						( *it )->PostMessage( EnemyShotMessage::ENEMY_SHOT_MESSAGE_ID_PLAYER_BOMBED );
						++it;
					}
				}
			}
			else if( m_PrivData.m_BombModeData.m_ConsAttr == PLAYER_CONS_MODE_GREEN ){
				if( m_PrivData.m_BombModeData.m_BombedCounter % 2 == 0 ){
					PlayerShot* pNewShot = m_Data.m_ObjBuilder.CreatePlayerShot( 4 );
					GameUnit pX;
					GameUnit pY;
					m_Data.m_pPlayer->GetPos( &pX, &pY );
					GameUnit random1 = GameUnit( -15 ) + ( ( GameUnit( m_Data.m_RandGen.GetRand() ) * GameUnit( 30 ) ) / m_Data.m_RandGen.GetRandMax() );
					GameUnit random2 = GameUnit( -15 ) + ( ( GameUnit( m_Data.m_RandGen.GetRand() ) * GameUnit( 30 ) ) / m_Data.m_RandGen.GetRandMax() );
					GameUnit random3 = GameUnit( 45 ) - ( ( GameUnit( m_Data.m_RandGen.GetRand() ) * GameUnit( 90 ) ) / m_Data.m_RandGen.GetRandMax() );
					pNewShot->SetPos( pX + random1, pY + random2 );
					pNewShot->SetSpeed( GameUnit( 2 ) );
					pNewShot->SetShotPower( 20 );
					pNewShot->SetAngle( random3 );
					pNewShot->SetStageData( &m_Data );
					m_Data.m_PlayerShotList.push_back( pNewShot );
				}
				// 敵ショットの削除
				if( m_PrivData.m_BombModeData.m_BombedCounter % 20 == 0 ){
					for( PlayerShotList::iterator itP = m_Data.m_PlayerShotList.begin(); itP != m_Data.m_PlayerShotList.end(); ++itP ){
						if( ( *itP )->GetID() != 4 ){
							continue;
						}
						GameUnit iX;
						GameUnit iY;
						GameUnit iR = ( *itP )->GetCollisionRadius();
						( *itP )->GetPos( &iX, &iY );
						for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ){
							GameUnit eX;
							GameUnit eY;
							GameUnit eR = ( *it )->GetCollisionRadius();
							( *it )->GetPos( &eX, &eY );
							GameUnit range = ( eX - iX ) * ( eX - iX ) + ( eY - iY ) * ( eY - iY ) + GameUnit( 20 * 20 );
							if( range < ( iR + eR ) * ( iR + eR ) ){
								( *it )->PostMessage( EnemyShotMessage::ENEMY_SHOT_MESSAGE_ID_PLAYER_BOMBED );
							}
							++it;
						}
					}
				}
			}
			++m_PrivData.m_BombModeData.m_BombedCounter;
		}
		else{
			m_PrivData.m_BombModeData.m_IsBombMode = false;
		}
	}
#endif

	void Stage::Impl::DrawBombModeEffect() const
	{
		if( m_PrivData.m_BombModeData.m_BombedCounter < m_PrivData.m_BombModeData.m_DurationTime ){
			if( m_PrivData.m_BombModeData.m_ConsAttr == PLAYER_CONS_MODE_RED ){
				MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
				for( int i = 0; i < 2; ++i ){
					MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_3 ],
										m_PrivData.m_BombModeData.m_IniPosX, m_PrivData.m_BombModeData.m_IniPosY,
										m_PrivData.m_BombModeData.m_BombedCounter / 4.0f, m_PrivData.m_BombModeData.m_BombedCounter / 4.0f,
										true, 0xFFFF0000 );
				}
				MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
			}
			else if( m_PrivData.m_BombModeData.m_ConsAttr == PLAYER_CONS_MODE_BLUE ){
				if( m_PrivData.m_BombModeData.m_BombedCounter < m_PrivData.m_BombModeData.m_DurationTime - 2 ){
					MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SUBTRACT_2 );
				}
				else if( m_PrivData.m_BombModeData.m_BombedCounter == m_PrivData.m_BombModeData.m_DurationTime - 2 ){
					MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
				}
				for( int i = 0; i < 2; ++i ){
					float scale = ( m_PrivData.m_BombModeData.m_DurationTime - m_PrivData.m_BombModeData.m_BombedCounter ) / 4.0f;
					MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_EFFECT_CONS_SKILL_3 ],
										m_PrivData.m_BombModeData.m_IniPosX, m_PrivData.m_BombModeData.m_IniPosY,
										scale, scale,
										true, 0xFF3333FF );
				}
			}
		}
	}

	void Stage::Impl::Init()
	{	
		m_VM.Init( &m_ScriptData.m_pStageScriptData->m_Data, &m_Data );
		m_Data.m_ObjBuilder.AttachStageData( &m_Data );
		m_Data.m_pPlayer = reinterpret_cast < Player* > ( m_Data.m_ObjBuilder.CreateCollisionObject( GAME_OBJECT_ID_PLAYER ) );
		MAPIL::ZeroObject( &m_Data.m_FrameGameData, sizeof( m_Data.m_FrameGameData ) );
		MAPIL::ZeroObject( &m_Data.m_GameData, sizeof( m_Data.m_GameData ) );
		m_Background.AttachStageData( &m_Data );
		m_Background.AttachScriptData( m_ScriptData );
		m_Background.Init();

		// 初期データから各データを設定
		m_PrivData.m_FixedGameData.m_HIScore = m_PrivData.m_IniGameData.m_HIScore;
		m_PrivData.m_FixedGameData.m_PrevScore = m_PrivData.m_IniGameData.m_Score;
		m_PrivData.m_FixedGameData.m_PrevKilled = m_PrivData.m_IniGameData.m_Killed;
		m_PrivData.m_FixedGameData.m_PrevCrystal = m_PrivData.m_IniGameData.m_Crystal;
		m_PrivData.m_FixedGameData.m_PrevCrystalUsed = m_PrivData.m_IniGameData.m_CrystalUsed;
		m_Data.m_pPlayer->SetPos( m_PrivData.m_IniGameData.m_PosX, m_PrivData.m_IniGameData.m_PosY );
		m_Data.m_pPlayer->SetHP( m_PrivData.m_IniGameData.m_HP );
		m_Data.m_pPlayer->SetShotPower( m_PrivData.m_IniGameData.m_ShotPower );
		for( int i = 0; i < 3; ++i ){
			m_Data.m_pPlayer->SetConsGauge( i, m_PrivData.m_IniGameData.m_ConsGauge[ i ] );
			m_Data.m_pPlayer->SetConsLevel( i, m_PrivData.m_IniGameData.m_ConsLevel[ i ] );
		}
		m_Data.m_pPlayer->SetCons( m_PrivData.m_IniGameData.m_Cons );
	}

	void Stage::Impl::DrawResult() const
	{
		if( m_PrivData.m_StatusFlags[ CLEARED ] && m_PrivData.m_ClearModeData.m_Counter > 100 ){

			DrawFontString(	m_Data.m_ResourceMap, 170.0f, 200.0f, 0.45f, 0xFFFFFFFF, "Score %d",
							m_PrivData.m_StageResultData.m_Score );
			DrawFontString( m_Data.m_ResourceMap, 150.0f, 100.0f, 0.7f, 0xFFAA4444, "Stage %d Cleared",
							m_Data.m_StageNo );
			DrawFontString( m_Data.m_ResourceMap, 170.0f, 180.0f, 0.45f, 0xFFFFFFFF, "HP Rest %d x 10000 %d",
							m_PrivData.m_StageResultData.m_HP, m_PrivData.m_StageResultData.m_HP * CLEARED_HP_REST_BONUS );
			DrawFontString( m_Data.m_ResourceMap, 170.0f, 220.0f, 0.45f, 0xFFFFFFFF, "Killed %d x 200 %d",
							m_PrivData.m_StageResultData.m_Killed, m_PrivData.m_StageResultData.m_Killed * CLEARED_KILLED_BONUS );
			DrawFontString( m_Data.m_ResourceMap, 170.0f, 240.0f, 0.45f, 0xFFFFFFFF, "Crystal %d x 100 %d",
							m_PrivData.m_StageResultData.m_Crystal, m_PrivData.m_StageResultData.m_Crystal * CLEARED_CRYSTAL_BONUS );
			DrawFontString( m_Data.m_ResourceMap, 170.0f, 260.0f, 0.45f, 0xFFFFFFFF, "Crystal Rest %d x 500 %d",
							m_PrivData.m_StageResultData.m_Crystal - m_PrivData.m_StageResultData.m_CrystalUsed,
							( m_PrivData.m_StageResultData.m_Crystal - m_PrivData.m_StageResultData.m_CrystalUsed ) * CLEARED_CRYSTAL_REST_BONUS );
			DrawFontString( m_Data.m_ResourceMap, 170.0f, 300.0f, 0.55f, 0xFFFFFFFF, "Bonus Total %d", GetStageClearBonus() );
			DrawFontString( m_Data.m_ResourceMap, 170.0f, 330.0f, 0.55f, 0xFFFFFFFF, "Total Score %d", m_Data.m_GameData.m_Score );
		}

	}

	void Stage::Impl::SaveStageResultData()
	{
		m_PrivData.m_StageResultData.m_Cons = m_Data.m_pPlayer->GetCurCons();
		for( int i = 0; i < 3; ++i ){
			m_PrivData.m_StageResultData.m_ConsGauge[ i ] = m_Data.m_pPlayer->GetConsGauge( i );
			m_PrivData.m_StageResultData.m_ConsLevel[ i ] = m_Data.m_pPlayer->GetConsLevel( i );
		}
		m_PrivData.m_StageResultData.m_Crystal = m_Data.m_GameData.m_CrystalTotal;
		m_PrivData.m_StageResultData.m_CrystalUsed = m_Data.m_GameData.m_CrystalUsed;
		m_PrivData.m_StageResultData.m_HP = m_Data.m_pPlayer->GetHP();
		m_PrivData.m_StageResultData.m_Killed = m_Data.m_GameData.m_Killed;
		GameUnit x;
		GameUnit y;
		m_Data.m_pPlayer->GetPos( &x, &y );
		m_PrivData.m_StageResultData.m_PosX = x.GetIntegerPart();
		m_PrivData.m_StageResultData.m_PosY = y.GetIntegerPart();
		m_PrivData.m_StageResultData.m_Progress = m_Data.m_Frame;
		m_PrivData.m_StageResultData.m_Score = m_Data.m_GameData.m_Score;
		m_PrivData.m_StageResultData.m_ShotPower = m_Data.m_pPlayer->GetShotPower();
	}

	int Stage::Impl::GetStageClearBonus() const
	{
		return	m_PrivData.m_StageResultData.m_Crystal * CLEARED_CRYSTAL_BONUS +
				( m_PrivData.m_StageResultData.m_Crystal - m_PrivData.m_StageResultData.m_CrystalUsed ) * CLEARED_CRYSTAL_REST_BONUS +
				m_PrivData.m_StageResultData.m_HP * CLEARED_HP_REST_BONUS +
				m_PrivData.m_StageResultData.m_Killed * CLEARED_KILLED_BONUS;
	}

	SceneType Stage::Impl::Update()
	{
		// リプレイバグ対策（浮動小数点演算器の初期化を行う。）
		__asm{
			finit
		}

		static bool prevPushed = false;
		if( MAPIL::IsKeyboardKeyPushed( MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_Q ) ) && !prevPushed ){
			m_DispProf = !m_DispProf;
			prevPushed = true;
		}
		else if( !MAPIL::IsKeyboardKeyPushed( MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_Q ) ) ){
			prevPushed = false;
		}

		m_Profiler.Begin( "Update" );
		
		// 現フレームでのスコアをリセット
		MAPIL::ZeroObject( &m_Data.m_FrameGameData, sizeof( m_Data.m_FrameGameData ) );

		// ランダムジェネレータの更新
		m_Data.m_RandGen.Update( m_Data );

		// ステージ向けのメッセージを処理
		ProcessMessage();
		// ゲームオーバー時の処理
		if( m_Data.m_HasTermSig ){
			m_Background.Terminate();
			SaveStageResultData();
			return SCENE_TYPE_SCORE_ENTRY;
		}
		// ステージクリア時の処理
		if( m_PrivData.m_StatusFlags[ CLEARED ] ){
			if( m_PrivData.m_ClearModeData.m_Counter == 0 ){
				SaveStageResultData();
			}
			// ボーナス加点
			if( m_PrivData.m_ClearModeData.m_Counter == 200 ){
				m_Data.m_FrameGameData.m_Score += GetStageClearBonus();
			}
			if( m_PrivData.m_ClearModeData.m_Counter == 400 ){
				SaveStageResultData();
			}
			if( m_PrivData.m_ClearModeData.m_Counter > 400 ){
				m_PrivData.m_StatusFlags.set( PREPARED_NEXT_STAGE );
			}
			if(	m_PrivData.m_StatusFlags[ PREPARED_NEXT_STAGE ] &&
				( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) || m_PrivData.m_ClearModeData.m_Counter >= 1200 ) ){
				SaveStageResultData();
				return SCENE_TYPE_STAGE;
			}
			++m_PrivData.m_ClearModeData.m_Counter;
		}

		// 背景の更新
		m_Background.Update();

		// スクリプトコマンドの実行
		m_VM.Run();
		if( m_VM.Terminated() ){
			m_Background.Terminate();
			SaveStageResultData();
			return SCENE_TYPE_SCORE_ENTRY;
		}

		// 衝突判定
		ProcessCollision();
		
		// 全GameObjectの更新
		UpdateGameObjects();

		// スキル使用時のエフェクト処理を更新
		UpdateConsSkillEffect();

		// ボス戦時のエフェクト処理を更新
		UpdateBossModeEffect();

		// ボム時のエフェクト処理を更新
		if( m_PrivData.m_BombModeData.m_IsBombMode ){
			UpdateBombModeEffect();
		}

		// スコアの更新
		m_ScoreManager.Add( m_Data.m_FrameGameData.m_Score );
		m_ScoreManager.Update();
		m_Data.m_GameData.m_Score = m_ScoreManager.GetScore();
		m_Data.m_GameData.m_CrystalTotal += m_Data.m_FrameGameData.m_CrystalTotal;
		m_Data.m_GameData.m_CrystalUsed += m_Data.m_FrameGameData.m_CrystalUsed;
		m_Data.m_GameData.m_Killed += m_Data.m_FrameGameData.m_Killed;

		// リプレイデータ更新
		m_KeyStateList.m_StatusList.push_back( m_ButtonStatus.m_RawButtonStatus );

		// 敵が最後にダメージを受けた時のメッセージを破棄
		Enemy::ClearLastDamagedMsg();

		++m_Data.m_Frame;

		for( int i = 0; i < sizeof( m_PrivData.m_ItemObtainedData.m_Counter ) / sizeof( m_PrivData.m_ItemObtainedData.m_Counter[ 0 ] ); ++i ){
			--m_PrivData.m_ItemObtainedData.m_Counter[ i ];
		}

		m_Profiler.End( "Update" );

		return SCENE_TYPE_NOT_CHANGE;
	}

	void Stage::Impl::Draw()
	{
		m_Profiler.Begin( "Draw" );

		static bool isFirst = true;
		static int light;

		if( m_Data.m_HasTermSig ){
			return;
		}

		if( isFirst ){
			//light = MAPIL::createdirec
			isFirst = false;
			light = MAPIL::CreateDirectionalLight();
			light = MAPIL::CreatePointLight();
		}

		MAPIL::SetCullingMode( MAPIL::CULL_MODE_COUNTERCLOCKWISE );
		//MAPIL::SetCullingMode( MAPIL::CULL_MODE_DISABLED );
		MAPIL::EnableLighting();
		MAPIL::DisableBlending();

		// 背景の描画
		m_Background.Draw();

		MAPIL::EnableBlending();

		// 2D画像描画開始
		MAPIL::BeginRendering2DGraphics();
		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
		
		// プレイヤーの描画
		m_Data.m_pPlayer->Draw();

		/*MAPIL::EndRendering2DGraphics();
		MAPIL::DisableLighting();
		MAPIL::DoAllModelOn2DBatchWorks();
		MAPIL::EnableLighting();
		MAPIL::BeginRendering2DGraphics();*/

		// 敵の描画
		for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ++it ){
			( *it )->Draw();
		}
		// ボスの描画
		if( m_Data.m_pBoss ){
			m_Data.m_pBoss->Draw();
		}

		MAPIL::EndRendering2DGraphics();
		MAPIL::DisableLighting();
		MAPIL::DoAllModelOn2DBatchWorks();
		MAPIL::EnableLighting();
		MAPIL::BeginRendering2DGraphics();

		ProcAllBatchWorks();

		// スキル使用時のエフェクトを描画
		DrawConsSkillEffect();

		// ボム使用時のエフェクトを描画
		if( m_PrivData.m_BombModeData.m_IsBombMode ){
			DrawBombModeEffect();
		}

		// プレイヤーショットの描画
		for( PlayerShotList::iterator it = m_Data.m_PlayerShotList.begin(); it != m_Data.m_PlayerShotList.end(); ++it ){
			( *it )->Draw();
		}

		// 敵ショットの描画
		for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ++it ){
			( *it )->Draw();
		}

		ProcAllBatchWorks();
		
		// アイテムの描画
		for( ItemList::iterator it = m_Data.m_ItemList.begin(); it != m_Data.m_ItemList.end(); ++it ){
			( *it )->Draw();
		}
		// エフェクトの描画
		for( EffectList::iterator it = m_Data.m_EffectList.begin(); it != m_Data.m_EffectList.end(); ++it ){
			( *it )->Draw();
		}

		

		// ボス戦闘時のエフェクトの描画
		DrawBossModeEffect();

		LateDrawConsSkillEffect();

		// リザルト画面表示
		DrawResult();


		// 状態画面の描画
		m_Profiler.Begin( "Game Stat" );
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_BACKGROUND_TEXTURE ],
							0.0f, 0.0f, false );
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_BACKGROUND_TEXTURE ],
							512.0f, 0.0f, false );
		// 左画面
		
		// HP表示
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GAME_HP ],
							43.0f, 40.0f, false );
		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
		int hp = m_Data.m_pPlayer->GetHP();
		for( int i = 0; i < hp; ++i ){
			int surplus = ( 30 + m_Data.m_Frame - i * 3 ) % 30;
			if( surplus < 20 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									23.0f + 10.0f * i, 80.0f, 0.7f - surplus * 0.01f, 1.1f - surplus * 0.015f, true,
									( 200 - surplus * 10 ) << 24 | ( 120 + i * 15 ) << 8 | ( i * 10 ) | ( 255 - i * 10 ) << 16 );
			}
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								23.0f + 10.0f * i, 80.0f, 0.5f, 0.8f, true, 0xFF000000 | ( 120 + i * 15 ) << 8 | ( i * 10 ) | ( 255 - i * 10 ) << 16 );
		}
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GAME_POWER ],
							27.0f, 110.0f, 0.8f, 0.8f, false );
		int lv =  m_Data.m_pPlayer->GetShotPower() / 10;
		for( int i = 0; i < lv; ++i ){
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								15.0f + 20.0f * i, 140.0f, 1.0f, 0.5f, false, 0xEE33FF33 );
			if( ( m_Data.m_Frame % 3 ) == 0 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								15.0f + 20.0f * i, 140.0f, 1.0f, 0.5f, false, 0xFFFFFFFF );
			}
		}
		MAPIL::DrawTexture( m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
							15.0f + 20.0f * lv, 140.0f, 0.1f * ( m_Data.m_pPlayer->GetShotPower() % 10 ), 0.5f, false,
							0xEE000033 | ( ( m_Data.m_pPlayer->GetShotPower() % 10 ) * 25 ) << 8 | ( ( 10 - ( m_Data.m_pPlayer->GetShotPower() % 10 ) ) * 25 ) << 16 );

		static int moveCount = 0;
		static int prevCons = PLAYER_CONS_MODE_NORMAL;

		const float CONS_GAUGE_OFFSET = 32.0f;
		const float CONS_GAUGE_BASE = 200.0f;

		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
							0.0f, 166.0f, 8.0f, 7.5f, false, 0xFF000000 );

		if( m_Data.m_pPlayer->GetCurCons() == PLAYER_CONS_MODE_NORMAL ){
			prevCons = PLAYER_CONS_MODE_NORMAL;
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE, m_Data.m_pPlayer->GetConsGauge( 0 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x22FF22 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET, m_Data.m_pPlayer->GetConsGauge( 1 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x5555FF );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET * 2, m_Data.m_pPlayer->GetConsGauge( 2 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0xFF7722 );
		}
		else if( m_Data.m_pPlayer->GetCurCons() == PLAYER_CONS_MODE_GREEN ){
			if( prevCons == PLAYER_CONS_MODE_NORMAL ){
				prevCons = PLAYER_CONS_MODE_GREEN;
			}
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								0.0f, 190.0f, 8.0f, 2.0f, false, 0x4400CCEE );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE, m_Data.m_pPlayer->GetConsGauge( 0 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0x55FF55 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE, m_Data.m_pPlayer->GetConsGauge( 0 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0x55FF55 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET, m_Data.m_pPlayer->GetConsGauge( 1 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x5555FF );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET * 2, m_Data.m_pPlayer->GetConsGauge( 2 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0xFF7722 );
		}
		else if( m_Data.m_pPlayer->GetCurCons() == PLAYER_CONS_MODE_BLUE ){
			if( prevCons == PLAYER_CONS_MODE_GREEN ){
				moveCount = 0;
				prevCons = PLAYER_CONS_MODE_BLUE;
			}
			else if( moveCount < 32 ){
				moveCount += 2;
			}
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								0.0f, 190.0f + moveCount, 8.0f, 2.0f, false, 0x4400CCEE );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET, m_Data.m_pPlayer->GetConsGauge( 1 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0x5555FF );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET, m_Data.m_pPlayer->GetConsGauge( 1 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0x5555FF );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE, m_Data.m_pPlayer->GetConsGauge( 0 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x22FF22 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET * 2, m_Data.m_pPlayer->GetConsGauge( 2 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0xFF7722 );
		}
		else if( m_Data.m_pPlayer->GetCurCons() == PLAYER_CONS_MODE_RED ){
			if( prevCons == PLAYER_CONS_MODE_BLUE ){
				moveCount = 0;
				prevCons = PLAYER_CONS_MODE_RED;
			}
			else if( moveCount < 32 ){
				moveCount += 2;
			}
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								0.0f, 222.0f + moveCount, 8.0f, 2.0f, false, 0x4400CCEE );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET * 2, m_Data.m_pPlayer->GetConsGauge( 2 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0xFF7722 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET * 2, m_Data.m_pPlayer->GetConsGauge( 2 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0xFF7722 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE, m_Data.m_pPlayer->GetConsGauge( 0 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x22FF22 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET, m_Data.m_pPlayer->GetConsGauge( 1 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x5555FF );
		}

		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
							0.0f, 292.0f, 8.0f, 7.5f, false, 0xFF000000 );

		// 意識レベルの描画
		for( int i = 1; i <= 10; ++i ){
			if( m_Data.m_pPlayer->GetConsLevel( 0 ) / ( i * 100 ) >= 1 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									25.0f + i * 9.0f, 340.0f - 16 * ( 0.2f + i * 0.08f ), 0.3f, 0.2f + i * 0.08f, false, 0xAA55FF55 );
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									25.0f + i * 9.0f, 340.0f - 16 * ( 0.2f + i * 0.08f ), 0.3f, 0.2f + i * 0.08f, false, ( ( m_Data.m_Frame % 10 ) * 15 + 100 ) << 24 | 0x55FF55 );
			}
			else{
				MAPIL::DrawTexture( m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									25.0f + i * 9.0f, 340.0f - 16 * ( 0.2f + i * 0.08f ), 0.3f, 0.2f + i * 0.08f, false, ( ( m_Data.m_pPlayer->GetConsLevel( 0 ) % 100 ) * 255 / 100 ) << 24 | 0x22FF22 );
				break;
			}
		}
		for( int i = 1; i <= 10; ++i ){
			if( m_Data.m_pPlayer->GetConsLevel( 1 ) / ( i * 100 ) >= 1 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									25.0f + i * 9.0f, 350.0f - 16 * ( 0.2f + i * 0.08f ) + 20.0f, 0.3f, 0.2f + i * 0.08f, false, 0xAA5555FF );
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									25.0f + i * 9.0f, 350.0f - 16 * ( 0.2f + i * 0.08f ) + 20.0f, 0.3f, 0.2f + i * 0.08f, false, ( ( m_Data.m_Frame % 10 ) * 15 + 100 ) << 24 | 0x5555FF );
			}
			else{
				MAPIL::DrawTexture( m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									25.0f + i * 9.0f, 350.0f - 16 * ( 0.2f + i * 0.08f ) + 20.0f, 0.3f, 0.2f + i * 0.08f, false, ( ( m_Data.m_pPlayer->GetConsLevel( 1 ) % 100 ) * 255 / 100 ) << 24 | 0x5555FF );
				break;
			}
		}
		for( int i = 1; i <= 10; ++i ){
			if( m_Data.m_pPlayer->GetConsLevel( 2 ) / ( i * 100 ) >= 1 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									25.0f + i * 9.0f, 360.0f - 16 * ( 0.2f + i * 0.08f ) + 40.0f, 0.3f, 0.2f + i * 0.08f, false, 0xAAFF7722 );
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									25.0f + i * 9.0f, 360.0f - 16 * ( 0.2f + i * 0.08f ) + 40.0f, 0.3f, 0.2f + i * 0.08f, false, ( ( m_Data.m_Frame % 10 ) * 15 + 100 ) << 24 | 0xFF7722 );
			}
			else{
				MAPIL::DrawTexture( m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									25.0f + i * 9.0f, 360.0f - 16 * ( 0.2f + i * 0.08f ) + 40.0f, 0.3f, 0.2f + i * 0.08f, false, ( ( m_Data.m_pPlayer->GetConsLevel( 2 ) % 100 ) * 255 / 100 ) << 24 | 0xFF7722 );
				break;
			}
		}

		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );

		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_CONS_GREEN_SYMBOL ],
							18.0f, 207.0f, 0.3f, 0.3f );
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_CONS_BLUE_SYMBOL ],
							18.0f, 239.0f, 0.3f, 0.3f );
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_CONS_RED_SYMBOL ],
							18.0f, 270.0f, 0.3f, 0.3f );

		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_CONS_GREEN_SYMBOL ],
							18.0f, 330.0f, 0.3f, 0.3f );
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_CONS_BLUE_SYMBOL ],
							18.0f, 362.0f, 0.3f, 0.3f );
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_CONS_RED_SYMBOL ],
							18.0f, 393.0f, 0.3f, 0.3f );


		const char* DIFFICULTY_DISP[] = { "calm", "easy", "normal", "hard", "hazard" };

		DrawFontString( m_Data.m_ResourceMap, 30.0f, 440.0f, 0.4f, 0xFFFF0000, DIFFICULTY_DISP[ m_Data.m_Difficulty ] );
		
		
		// 右画面
		const float RIGHT_DISP_OFFSET_X = 525.0f;
		const float RIGHT_DISP_OFFSET_Y	= 55.0f;
		float dispOffsetY = RIGHT_DISP_OFFSET_Y;
		// ハイスコア
		
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_HI_SCORE ],
							RIGHT_DISP_OFFSET_X, dispOffsetY, 0.8f, 0.8f, false );
		if( m_PrivData.m_IniGameData.m_HIScore <= m_Data.m_GameData.m_Score + m_PrivData.m_IniGameData.m_Score ){
			DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 35.0f, 0.4f, "%08d", m_Data.m_GameData.m_Score + m_PrivData.m_IniGameData.m_Score );
		}
		else{
			DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 35.0f, 0.4f, "%08d", m_PrivData.m_IniGameData.m_HIScore );
		}

		// スコア
		dispOffsetY += 50.0f;
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GAME_SCORE ],
							RIGHT_DISP_OFFSET_X, dispOffsetY, 0.8f, 0.8f, false );
		DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 35.0f, 0.4f, "%08d", m_Data.m_GameData.m_Score + m_PrivData.m_FixedGameData.m_PrevScore );
		DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 55.0f, 0.4f, 0xFF888888, "%08d", m_Data.m_GameData.m_Score );
		// 撃破数
		dispOffsetY += 70.0f;
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GAME_KILLED ],
							RIGHT_DISP_OFFSET_X, dispOffsetY, 0.8f, 0.8f, false );
		DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 35.0f, 0.4f, "%d", m_Data.m_GameData.m_Killed + m_PrivData.m_FixedGameData.m_PrevKilled );
		DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 55.0f, 0.4f, 0xFF888888, "%d", m_Data.m_GameData.m_Killed );
		// 取得クリスタル数
		dispOffsetY += 70.0f;
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GAME_CRYSTAL ],
							RIGHT_DISP_OFFSET_X, dispOffsetY, 0.8f, 0.8f, false );
		
		DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 35.0f, 0.4f, "%d/%d",
						( m_PrivData.m_FixedGameData.m_PrevCrystal + m_Data.m_GameData.m_CrystalTotal ) - ( m_Data.m_GameData.m_CrystalUsed + m_PrivData.m_FixedGameData.m_PrevCrystalUsed ),
						m_PrivData.m_FixedGameData.m_PrevCrystal + m_Data.m_GameData.m_CrystalTotal );
		if( m_PrivData.m_ItemObtainedData.m_Counter[ ITEM_ID_CRYSTAL ] > 0 ){
			MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
			DrawFontString(	m_Data.m_ResourceMap,
							RIGHT_DISP_OFFSET_X + 5.0f,
							dispOffsetY + 35.0f,
							0.4f + m_PrivData.m_ItemObtainedData.m_Counter[ ITEM_ID_CRYSTAL ] * 0.002f, 0xAAFFFF33,
							"%d/%d",
							m_Data.m_GameData.m_CrystalTotal - m_Data.m_GameData.m_CrystalUsed,
							m_Data.m_GameData.m_CrystalTotal );
			MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
		}
		DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 55.0f, 0.4f, 0xFF888888, "%d", m_Data.m_GameData.m_CrystalTotal );
		// 進行度
		dispOffsetY += 90.0f;
		DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY, 0.4f, 0xFFAAFFAA, "progress" );
		if( m_Data.m_Frame < m_Data.m_FrameTotal ){
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 20.0f, 6.0f, 0.4f, false, 0xFF222222 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 20.0f, m_Data.m_Frame * 6.0f / m_Data.m_FrameTotal, 0.4f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x2222FF );
			
		}
		else{
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 20.0f, 6.0f, 0.4f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x2222FF );
		}
		// 最後に攻撃した敵データ
		dispOffsetY += 50.0f;
		if( m_PrivData.m_LastDamagedEnemyData.m_MaxConsGauge != 10000 ){
			DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY, 0.4f, 0xFFAAFFAA, "%s", m_PrivData.m_LastDamagedEnemyData.m_EnemyName.c_str() );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 10.0f, m_PrivData.m_LastDamagedEnemyData.m_HP * 6.0f / m_PrivData.m_LastDamagedEnemyData.m_MaxHP, 0.5f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0xFF2222 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 20.0f, m_PrivData.m_LastDamagedEnemyData.m_ConsGauge * 6.0f / m_PrivData.m_LastDamagedEnemyData.m_MaxConsGauge, 0.5f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x22FF22 );
		}

		// FPS
		DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, 450.0f, 0.4f, 0xFFAAFFAA, "fps" );
		
		m_Profiler.End( "Game Stat" );

		

		m_Profiler.End( "Draw" );

		m_Profiler.Begin( "Debug Stat" );

		// デバッグ表示
		if( m_DispProf ){
			MAPIL::DrawTexture( m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								230.0f, 16.0f, 17.0f, 10.0f, false, 0x66000000 );
			MAPIL::DrawString( 250.0f, 20.0f, "--Stat--", m_Data.m_Frame );
			MAPIL::DrawString( 250.0f, 50.0f, "Frame : %d", m_Data.m_Frame );
			MAPIL::DrawString( 250.0f, 70.0f, "EnemyShot : %d", m_Data.m_EnemyShotList.size() );
			MAPIL::DrawString( 250.0f, 90.0f, "Item : %d", m_Data.m_ItemList.size() );
			MAPIL::DrawString( 250.0f, 110.0f, "Effect : %d", m_Data.m_EffectList.size() );
			MAPIL::DrawString( 250.0f, 130.0f, "PlayerShot : %d", m_Data.m_PlayerShotList.size() );
			MAPIL::DrawString( 250.0f, 150.0f, "EnemyShotGroup : %d", m_Data.m_EnemyShotGroupList.size() );
		
		
		

			static int count = 0;
			static int time[ 4 ] = { 0, 0, 0, 0 };
			static float interval = 0;
			++count;
			MAPIL::DrawString( 380.0f, 20.0f, "--Profile--", time[ 0 ] / 10.0f );
			MAPIL::DrawString( 380.0f, 50.0f, "Update : %.1f %%", time[ 0 ] / 10.0f );
			MAPIL::DrawString( 380.0f, 70.0f, "Draw : %.1f %%", time[ 1 ] / 10.0f );
			MAPIL::DrawString( 380.0f, 90.0f, "Debug Stat : %.1f %%", time[ 2 ] / 10.0f );
			MAPIL::DrawString( 380.0f, 110.0f, "Game Stat : %.1f %%", time[ 3 ] / 10.0f );
			int total = 0;
			for( int i = 0; i < 3; ++i ){
				total += time[ i ];
			}
			MAPIL::DrawString( 380.0f, 140.0f, "Total : %.2f %%", total / 10.0f );

			m_Profiler.End( "Debug Stat" );

			if( count % 60 == 0 ){
				time[ 0 ] = m_Profiler.GetProfile( "Update" );
				time[ 1 ] = m_Profiler.GetProfile( "Draw" );
				time[ 2 ] = m_Profiler.GetProfile( "Debug Stat" );
				time[ 3 ] = m_Profiler.GetProfile( "Game Stat" );
				m_Profiler.Clear();
			}

		}

		// 2D描画終了
		MAPIL::EndRendering2DGraphics();
		
	}

	void Stage::Impl::AttachButtonState( ButtonStatusHolder* pHolder )
	{
		m_ButtonStatus = *pHolder;
	}

	void Stage::Impl::AttachResourceMap( const ResourceMap& map )
	{
		m_Data.m_ResourceMap = map;
		m_Data.m_ObjBuilder.AttachResourceMap( map );
	}

	void Stage::Impl::AttachScriptData( const ScriptData& data )
	{
		m_ScriptData = data;
		m_Data.m_ObjBuilder.AttachScriptData( data );
	}

	void Stage::Impl::AttachGameData( const GameDataMsg& msg )
	{
		m_Data.m_GameData = msg;
	}

	int Stage::Impl::GetProgress() const
	{
		return m_PrivData.m_StageResultData.m_Progress;
		//return m_Data.m_Frame;
	}
	
	int Stage::Impl::GetScore() const
	{
		return m_PrivData.m_StageResultData.m_Score;
		//return m_Data.m_GameData.m_Score;
	}

	int Stage::Impl::GetKilled() const
	{
		return m_PrivData.m_StageResultData.m_Killed;
		//return m_Data.m_GameData.m_Killed;
	}

	int Stage::Impl::GetCrystal() const
	{
		return m_PrivData.m_StageResultData.m_Crystal;
		//return m_Data.m_GameData.m_CrystalTotal;
	}

	int Stage::Impl::GetCrystalUsed() const
	{
		return m_PrivData.m_StageResultData.m_CrystalUsed;
		//return m_Data.m_GameData.m_CrystalUsed;
	}

	void Stage::Impl::GetPlayerPos( int* pPosX, int* pPosY ) const
	{
		*pPosX = m_PrivData.m_StageResultData.m_PosX;
		*pPosY = m_PrivData.m_StageResultData.m_PosY;
	}

	int Stage::Impl::GetPlayerHP() const
	{
		return m_PrivData.m_StageResultData.m_HP;
		//return m_Data.m_pPlayer->GetHP();
	}

	int Stage::Impl::GetPlayerShotPower() const
	{
		return m_PrivData.m_StageResultData.m_ShotPower;
		//return m_Data.m_pPlayer->GetShotPower();
	}

	int Stage::Impl::GetPlayerCons() const
	{
		return m_PrivData.m_StageResultData.m_Cons;
		//return m_Data.m_pPlayer->GetCurCons();
	}

	void Stage::Impl::GetPlayerConsGauge( int* pGauge ) const
	{
		for( int i = 0; i < 3; ++i ){
			pGauge[ i ] = m_PrivData.m_StageResultData.m_ConsGauge[ i ];
			//pGauge[ i ] = m_Data.m_pPlayer->GetConsGauge( i );
		}
	}

	void Stage::Impl::GetPlayerConsLevel( int* pLevel ) const
	{
		for( int i = 0; i < 3; ++i ){
			pLevel[ i ] = m_PrivData.m_StageResultData.m_ConsLevel[ i ];
			//pLevel[ i ] = m_Data.m_pPlayer->GetConsLevel( i );
		}
	}

	ReplayDataRecord::StageKeyStates Stage::Impl::GetKeyStates() const
	{
		return m_KeyStateList;
	}


	void Stage::Impl::SetInitialData( const InitialGameData& data )
	{
		m_PrivData.m_IniGameData = data;
	}

	int Stage::Impl::GetNextStageNo() const
	{
		return m_PrivData.m_ClearModeData.m_NextStageNo;
	}

	GameDataMsg Stage::Impl::GetFrameData() const
	{
		return m_Data.m_FrameGameData;
	}

	void Stage::Impl::SetDifficulty( int difficulty )
	{
		m_Data.m_Difficulty = difficulty;
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

	void Stage::AttachGameData( const GameDataMsg& msg )
	{
		m_pImpl->AttachGameData( msg );
	}

	int Stage::GetProgress() const
	{
		return m_pImpl->GetProgress();
	}

	int Stage::GetScore() const
	{
		return m_pImpl->GetScore();
	}

	int Stage::GetKilled() const
	{
		return m_pImpl->GetKilled();
	}

	int Stage::GetCrystal() const
	{
		return m_pImpl->GetCrystal();
	}

	int Stage::GetCrystalUsed() const
	{
		return m_pImpl->GetCrystalUsed();
	}

	void Stage::GetPlayerPos( int* pPosX, int* pPosY ) const
	{
		m_pImpl->GetPlayerPos( pPosX, pPosY );
	}

	int Stage::GetPlayerHP() const
	{
		return m_pImpl->GetPlayerHP();
	}

	int Stage::GetPlayerShotPower() const
	{
		return m_pImpl->GetPlayerShotPower();
	}

	int Stage::GetPlayerCons() const
	{
		return m_pImpl->GetPlayerCons();
	}

	void Stage::GetPlayerConsGauge( int* pGauge ) const
	{
		m_pImpl->GetPlayerConsGauge( pGauge );
	}

	void Stage::GetPlayerConsLevel( int* pLevel ) const
	{
		m_pImpl->GetPlayerConsLevel( pLevel );
	}

	ReplayDataRecord::StageKeyStates Stage::GetKeyStates() const
	{
		return m_pImpl->GetKeyStates();
	}


	void Stage::SetInitialData( const InitialGameData& data )
	{
		m_pImpl->SetInitialData( data );
	}

	void Stage::SetDifficulty( int difficulty )
	{
		m_pImpl->SetDifficulty( difficulty );
	}

	int Stage::GetNextStageNo() const
	{
		return m_pImpl->GetNextStageNo();
	}

	GameDataMsg Stage::GetFrameData() const
	{
		return m_pImpl->GetFrameData();
	}
}
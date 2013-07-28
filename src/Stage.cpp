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
#include "StagePause.h"

#include "ScoreManager.h"

#include "SpriteBatch.h"
#include "Util.h"
#include "Profiler.h"

#include <bitset>

namespace GameEngine
{
	// �N���A�������̃{�[�i�X
	const int CLEARED_CRYSTAL_BONUS			= 10;
	const int CLEARED_CRYSTAL_REST_BONUS	= 30;
	const int CLEARED_HP_REST_BONUS			= 5000;
	const int CLEARED_KILLED_BONUS			= 40;

	class Stage::Impl
	{
	private:
		

		// �t���O�Ǘ�
		enum StatusFlag
		{
			CLEARED					= 0,		// �N���A����
			PREPARING				= 1,		// ���̃X�e�[�W�ֈڍs��
			PREPARED_NEXT_STAGE		= 2,		// ���̃X�e�[�W�ֈڍs���鏀�����o����
			PAUSED					= 3,		// �|�[�Y���
			HALT					= 4,		// �����I�����ꂽ
			STATUS_FLAG_TOTAL,
		};

		struct PrivateData
		{
			// �Œ艻���ꂽ�Q�[���f�[�^
			struct FixedGameData
			{
				int		m_HIScore;			// �n�C�X�R�A
				int		m_PrevScore;		// �O�̃X�e�[�W�܂ł̃X�R�A
				int		m_PrevKilled;		// �O�̃X�e�[�W�܂łɓ|�����G�̐�
				int		m_PrevCrystal;		// �O�̃X�e�[�W�܂łɎ擾�����N���X�^���̑���
				int		m_PrevCrystalUsed;	// �O�̃X�e�[�W�܂łɎg�p�����N���X�^���̑���
			};

			// �ӎ��Z
			struct ConsSkillModeData
			{
				bool			m_IsConsSkillMode;	// �ӎ��X�L�����[�h�Ȃ�true
				int				m_Counter;			// �G�t�F�N�g�p�J�E���^
				int				m_PostCounter;		// �G�t�F�N�g�I���Ƃ��̃J�E���^
				std::string		m_SkillName;		// �ӎ��Z��
				int				m_SkillCost;		// ����ӎ��Q�[�W
			};
			// �{�X��ԂɊւ���f�[�^
			struct BossModeData
			{
				bool			m_IsBossMode;			// �{�X�o����ԂȂ�true
				int				m_DamagedCounter;		// �_���[�W���󂯂����̃G�t�F�N�g�J�E���^
				int				m_ShiftFrame[ 10 ];		// ���̒i�K�ֈڍs�����Ƃ��̃t���[��
				int				m_PhaseInterval;		// ���̒i�K�ւ̃C���^�[�o��
				int				m_CurPhase;				// �i�K��
			};
			// �{���Ɋւ���f�[�^
			struct BombModeData
			{
				bool			m_IsBombMode;		// �{�����[�h�Ȃ�true
				int				m_BombedCounter;	// �J�E���^
				int				m_ConsAttr;			// �{���̑���
				float			m_IniPosX;			// �����ʒu�iX���W�j
				float			m_IniPosY;			// �����ʒu�iY���W�j
				int				m_DurationTime;		// ��������
			};
			// �Ō�Ƀ_���[�W���󂯂��G�Ɋւ���f�[�^
			struct LastDamagedEnemyData
			{
				std::string		m_EnemyName;		// �G�̖��O
				int				m_HP;				// ���݂�HP
				int				m_MaxHP;			// �ő�HP
				int				m_ConsGauge;		// ���݂̈ӎ��Q�[�W
				int				m_MaxConsGauge;		// �ő�ӎ��Q�[�W
			};
			// �X�e�[�W�N���A���̏����Ɋւ���f�[�^
			struct StageClearModeData
			{
				int				m_Counter;			// �J�E���^
				int				m_NextStageNo;		// ���̃X�e�[�W�ԍ�
				int				m_PrepareCounter;	// ���̃X�e�[�W�ֈڍs���邽�߂̃J�E���^
			};
			// �A�C�e���擾���̏����Ɋւ���f�[�^
			struct ItemObtainedData
			{
				int				m_Counter[ ITEM_ID_TOTAL ];		// �A�C�e�����ƂɃG�t�F�N�g�J�E���^
			};
			// �X�e�[�W�������̃f�[�^
			struct StageResultData
			{
				int		m_Score;				// �X�R�A
				int		m_Crystal;				// �擾�N���X�^���̑���
				int		m_CrystalUsed;			// �g�p�����N���X�^������
				int		m_Killed;				// �|�����G�̐�
				int		m_PosX;					// �Ō�ɂ����ꏊ�iX���W�j
				int		m_PosY;					// �Ō�ɂ����ꏊ�iY���W�j
				int		m_HP;					// �c��HP
				int		m_ShotPower;			// �V���b�g�p���[
				int		m_Cons;					// �ӎ����
				int		m_ConsGauge[ 3 ];		// �ӎ��Q�[�W
				int		m_ConsLevel[ 3 ];		// �ӎ����x��
				int		m_Progress;				// �X�e�[�W�i�s�x
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
			bool									m_IsFirstTime;
		};


		ButtonStatusHolder					m_ButtonStatus;		// �{�^���̏��
		ScriptData							m_ScriptData;		// �X�N���v�g�f�[�^
		StageData							m_Data;				// �X�e�[�W�p�f�[�^
		ScoreManager						m_ScoreManager;		// �X�R�A�Ǘ��N���X
		ReplayDataRecord::StageKeyStates	m_KeyStateList;		// ���v���C�p�L�[���X�g

		StageVCPU					m_VM;				// ���z�}�V��
		StageBackground				m_Background;		// �w�i
		StagePause					m_Pause;			// �|�[�Y���

		Profiler					m_Profiler;			// �v���t�@�C��
		bool						m_DispProf;			// �v���t�@�C���̌��ʂ�\��

		PrivateData					m_PrivData;

		void ProcessCollision();		// �Փ˔���
		void UpdateGameObjects();		// �SGameObject�̍X�V
		void ProcessMessage();			// �X�e�[�W�p���b�Z�[�W�̏���
		void SaveStageResultData();		// �X�e�[�W�����f�[�^�̕ۑ�
		int GetStageClearBonus() const;	// �X�e�[�W�N���A���̃{�[�i�X

		void UpdateConsSkillEffect();		// �ӎ��Z�G�t�F�N�g�����̍X�V
		void DrawConsSkillEffect() const;	// �ӎ��Z�G�t�F�N�g�����̕`��
		void LateDrawConsSkillEffect() const;	// �ӎ��Z�G�t�F�N�g�����̕`��i��ŕ`��j
		void UpdateBossModeEffect();		// �{�X���[�h���̃G�t�F�N�g�����̍X�V
		void DrawBossModeEffect() const;	// �{�X���[�h���̃G�t�F�N�g�����̕`��
		void UpdateBombModeEffect();		// �{�����[�h���̃G�t�F�N�g�����̍X�V
		void DrawBombModeEffect() const;	// �{�����[�h���̃G�t�F�N�g�����̕`��
		void DrawResult() const;			// �X�e�[�W�N���A���̃��U���g��ʕ\��
		void UpdateBossPhaseShiftEffect();		// �{�X�����̒i�K�֐i�ނƂ��̍X�V����
		void DrawBossPhaseShiftEffect() const;	// �{�X�����̒i�K�֐i�ނƂ��̕`�揈��

		// GameObject�S�폜���\�b�h
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
		StageStat& GetStageStat();

		void SetInitialData( const InitialGameData& data );
		void SetDifficulty( int difficulty );

		int GetNextStageNo() const;
		GameDataMsg GetFrameData() const;

		void MarkFirstTime();
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
		m_Data.m_BossPhaseTotal = 4;

		// �X�e�[�W�̓��v���̃N���A
		//m_Data.m_StageStat.m_EnemyStat.clear();
		//m_Data.m_StageStat.m_Clear = 0;
		//MAPIL::ZeroObject( m_Data.m_StageStat.m_ConsTime, sizeof( m_Data.m_StageStat.m_ConsTime ) );
		//m_Data.m_StageStat.m_EnemyStat.clear();
		//m_Data.m_StageStat.m_HIScore = 0;
		//m_Data.m_StageStat.m_Play = 0;
		//m_Data.m_StageStat.m_PlayTime = 0;

		// �����f�[�^�̃N���A
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
		m_PrivData.m_ClearModeData.m_PrepareCounter = 0;
		MAPIL::ZeroObject( m_PrivData.m_ItemObtainedData.m_Counter, sizeof( m_PrivData.m_ItemObtainedData.m_Counter ) );
		MAPIL::ZeroObject( &m_PrivData.m_StageResultData, sizeof( m_PrivData.m_StageResultData ) );
		MAPIL::ZeroObject( &m_PrivData.m_BossModeData, sizeof( m_PrivData.m_BossModeData ) );
		m_PrivData.m_IsFirstTime = false;

		m_Pause.Init();
		m_Pause.AttachStageData( &m_Data );

		m_Profiler.Clear();
		m_DispProf = false;
		m_KeyStateList.m_StatusList.clear();
	}

	Stage::Impl::~Impl()
	{
		// GameObject�̔j��
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
		// �Փ˔���
		// �G-�v���C���[�V���b�g
		// �G-�v���C���[
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
				
				// �G - �v���C���[
				float distance = ( eX - psX ) * ( eX - psX ) + ( eY - psY ) * ( eY - psY );
				float radius = ( psRad + eRad ) * ( psRad + eRad );
				if( distance < radius ){
					( *itShot )->Colided( *itEnemy );
					( *itEnemy )->Colided( *itShot );
				}
			}
		}

		// �{�X-�v���C���[�V���b�g
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
		// �v���C���[-�G�V���b�g
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
		// �v���C���[-�A�C�e��
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
				// �Փˎ�
				if( distance < colRadius ){
					( *it )->Colided( m_Data.m_pPlayer );
					m_Data.m_pPlayer->Colided( ( *it ).get() );
					//m_Data.m_pPlayer->Colided( ( *it ) );
				}
				// �߂��ɂ��鎞
				else{
					( *it )->PlayerIsNear( m_Data.m_pPlayer );
				}
			}
		}

		// �v���C���[-�G
		// �G-�A�C�e��
		for( EnemyList::iterator itEnemy = m_Data.m_EnemyList.begin(); itEnemy != m_Data.m_EnemyList.end(); ++itEnemy ){
			float eX;
			float eY;
			float eRad;
			( *itEnemy )->GetPos( &eX, &eY );
			eRad = ( *itEnemy )->GetCollisionRadius();
				
			// �G-�v���C���[
			float distance = ( eX - pX ) * ( eX - pX ) + ( eY - pY ) * ( eY - pY );
			float radius = ( pRad + eRad ) * ( pRad + eRad );
			if( distance < radius ){
				m_Data.m_pPlayer->Colided( *itEnemy );
				( *itEnemy )->Colided( m_Data.m_pPlayer );
			}
			// �G-�A�C�e��
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
		// �Փ˔���
		// �G-�v���C���[�V���b�g
		// �G-�v���C���[
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
				
				// �G - �v���C���[
				GameUnit distance = ( eX - psX ) * ( eX - psX ) + ( eY - psY ) * ( eY - psY );
				GameUnit radius = ( psRad + eRad ) * ( psRad + eRad );
				if( distance < radius ){
					( *itShot )->Colided( *itEnemy );
					( *itEnemy )->Colided( *itShot );
				}
			}
		}

		// �{�X-�v���C���[�V���b�g
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
		// �v���C���[-�G�V���b�g
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
		// �v���C���[-�A�C�e��
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
				// �Փˎ�
				if( distance < colRadius ){
					( *it )->Colided( m_Data.m_pPlayer );
					m_Data.m_pPlayer->Colided( ( *it ).get() );
				}
				// �߂��ɂ��鎞
				else{
					( *it )->PlayerIsNear( m_Data.m_pPlayer );
				}
			}
		}

		// �v���C���[-�G
		// �G-�A�C�e��
		for( EnemyList::iterator itEnemy = m_Data.m_EnemyList.begin(); itEnemy != m_Data.m_EnemyList.end(); ++itEnemy ){
			GameUnit eX;
			GameUnit eY;
			GameUnit eRad;
			( *itEnemy )->GetPos( &eX, &eY );
			eRad = ( *itEnemy )->GetCollisionRadius();
				
			// �G-�v���C���[
			GameUnit distance = ( eX - pX ) * ( eX - pX ) + ( eY - pY ) * ( eY - pY );
			GameUnit radius = ( pRad + eRad ) * ( pRad + eRad );
			if( distance < radius ){
				m_Data.m_pPlayer->Colided( *itEnemy );
				( *itEnemy )->Colided( m_Data.m_pPlayer );
			}
			// �G-�A�C�e��
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
					( *itItem )->Colided( *itEnemy );
				}
			}
		}			
	}
#endif


	void Stage::Impl::UpdateGameObjects()
	{
		// �v���C���[�̍X�V
		m_Data.m_pPlayer->AttachButtonState( m_ButtonStatus );
		m_Data.m_pPlayer->Update();
		// �G�V���b�g�O���[�v�̍X�V�iUpdate��2��Ă΂�Ȃ��悤�ɂ��邽�߁A�G�̍X�V�̏�Ɏ����Ă���B�j
		for( EnemyShotGroupList::iterator it = m_Data.m_EnemyShotGroupList.begin(); it != m_Data.m_EnemyShotGroupList.end(); ){
			if( !( *it )->Update() ){
				delete ( *it );
				it = m_Data.m_EnemyShotGroupList.erase( it );
				continue;
			}
			++it;
		}
		// �G�̍X�V
		for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ){
			if( !( *it )->Update() ){
				delete ( *it );
				it = m_Data.m_EnemyList.erase( it );
				continue;
			}
			++it;
		}
		// �{�X�̍X�V
		if( m_Data.m_pBoss ){
			if( !m_Data.m_pBoss->Update() ){
				MAPIL::SafeDelete( m_Data.m_pBoss );
				StageMessage msg;
				msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_BOSS_MODE_ENDED;
				m_Data.m_MsgQueue.push( msg );
			}
		}
		// �v���C���[�V���b�g�̍X�V
		for( PlayerShotList::iterator it = m_Data.m_PlayerShotList.begin(); it != m_Data.m_PlayerShotList.end(); ){
			if( !( *it )->Update() ){
				delete ( *it );
				it = m_Data.m_PlayerShotList.erase( it );
				continue;
			}
			++it;
		}
		// �G�V���b�g�̍X�V
		for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ){
			if( !( *it )->Update() ){
				delete ( *it );
				it = m_Data.m_EnemyShotList.erase( it );
				continue;
			}
			++it;
		}
		// �A�C�e���̍X�V
		for( ItemList::iterator it = m_Data.m_ItemList.begin(); it != m_Data.m_ItemList.end(); ){
			if( !( *it )->Update() ){
				//delete ( *it );
				it = m_Data.m_ItemList.erase( it );
				continue;
			}
			++it;
		}
		// �G�t�F�N�g�̍X�V
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
					m_PrivData.m_BossModeData.m_ShiftFrame[ 0 ] = m_Data.m_Frame;
					break;
				case StageMessage::STAGE_MESSAGE_ID_BOSS_MODE_ENDED:
					break;
				case StageMessage::STAGE_MESSAGE_ID_BOSS_SHIFT_NEXT_MODE:
					m_PrivData.m_BossModeData.m_PhaseInterval = 0;
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
		m_Data.m_ItemList.clear();
	}

	void Stage::Impl::UpdateConsSkillEffect()
	{
		if( m_PrivData.m_ConsSkillModeData.m_IsConsSkillMode ){
			++m_PrivData.m_ConsSkillModeData.m_Counter;
		}
		else{
			++m_PrivData.m_ConsSkillModeData.m_PostCounter;
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
	
		UpdateBossPhaseShiftEffect();
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

				for( int i = 0; i < m_Data.m_BossPhaseTotal; ++i ){
					MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
										150.0f + 320.0f * m_Data.m_BossPhaseStartHP[ i ] / m_Data.m_pBoss->GetMaxHP(), 10.0f,
										0.1f, 0.5f, false, 0xFFFFFFFF );
				}
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									150.0f + 320.0f, 10.0f,
									0.1f, 0.5f, false, 0xFFFFFFFF );
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									246.0f, 20.0f,
									0.1f, 0.5f, false, 0xFFFFFFFF );

				for( int i = 0; i < m_PrivData.m_BossModeData.m_CurPhase; ++i ){
					float interval = ( m_PrivData.m_BossModeData.m_ShiftFrame[ i + 1 ] - m_PrivData.m_BossModeData.m_ShiftFrame[ i ] ) / 60.0f;
					DrawFontString( m_Data.m_ResourceMap, 420.0f, 50.0f + i * 14.0f, 0.4f, 0xFF11FF55, "%d", i + 1 );
					DrawFontString( m_Data.m_ResourceMap, 440.0f, 50.0f + i * 14.0f, 0.4f, "%.2f", interval );
				}
				DrawFontString(	m_Data.m_ResourceMap,
								140.0f,
								50.0f,
								0.5f,
								0xFFFFAAAA,
								"%.2f",
								( m_Data.m_Frame - m_PrivData.m_BossModeData.m_ShiftFrame[ m_PrivData.m_BossModeData.m_CurPhase ] ) / 60.0f );
				DrawFontString(	m_Data.m_ResourceMap,
								140.0f,
								70.0f,
								0.4f,
								0xFFFFAAFF,
								"%.2f",
								( m_Data.m_Frame - m_PrivData.m_BossModeData.m_ShiftFrame[ 0 ] ) / 60.0f );

			}

		}

		DrawBossPhaseShiftEffect();
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
			
				// �G�ƃ{���̏Փ˔���
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
				// �{�X�ƃ{���̏Փ˔���
				if( m_Data.m_pBoss ){
					float eX;
					float eY;
					m_Data.m_pBoss->GetPos( &eX, &eY );
					float radius = ( eX - iX ) * ( eX - iX ) + ( eY - iY ) * ( eY - iY );
					if( radius > colFirst && radius < colLast ){
						m_Data.m_pBoss->Damage( 15 );
					}
				}
				// �G�V���b�g�̍X�V
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
					// �G�̍s����~
					for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ){
						( *it )->Pause();
						++it;
					}
					// �{�X�̍s����~
					if( m_Data.m_pBoss ){
						m_Data.m_pBoss->Pause();
					}
					// �G�V���b�g�̍s����~
					for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ){
						( *it )->Pause();
						++it;
					}
				}
				else if( m_PrivData.m_BombModeData.m_BombedCounter == m_PrivData.m_BombModeData.m_DurationTime - 1 ){
					// �G�̍s���ĊJ
					for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ){
						( *it )->Resume();
						++it;
					}
					// �{�X�̍s���ĊJ
					if( m_Data.m_pBoss ){
						m_Data.m_pBoss->Resume();
					}
					// �G�V���b�g�̍폜
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
				// �G�V���b�g�̍폜
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
			
				// �G�ƃ{���̏Փ˔���
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
				// �{�X�ƃ{���̏Փ˔���
				if( m_Data.m_pBoss ){
					GameUnit eX;
					GameUnit eY;
					m_Data.m_pBoss->GetPos( &eX, &eY );
					GameUnit radius = ( eX - iX ) * ( eX - iX ) + ( eY - iY ) * ( eY - iY );
					if( radius > colFirst && radius < colLast ){
						m_Data.m_pBoss->Damage( 15 );
					}
				}
				// �G�V���b�g�̍X�V
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
					// �G�̍s����~
					for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ){
						( *it )->Pause();
						++it;
					}
					// �{�X�̍s����~
					if( m_Data.m_pBoss ){
						m_Data.m_pBoss->Pause();
					}
					// �G�V���b�g�̍s����~
					for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ){
						( *it )->Pause();
						++it;
					}
				}
				else if( m_PrivData.m_BombModeData.m_BombedCounter == m_PrivData.m_BombModeData.m_DurationTime - 1 ){
					// �G�̍s���ĊJ
					for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ){
						( *it )->Resume();
						++it;
					}
					// �{�X�̍s���ĊJ
					if( m_Data.m_pBoss ){
						m_Data.m_pBoss->Resume();
					}
					// �G�V���b�g�̍폜
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
				// �G�V���b�g�̍폜
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
		MAPIL::ZeroObject( &m_Data.m_TotalGameData, sizeof( m_Data.m_TotalGameData ) );
		m_Background.AttachStageData( &m_Data );
		m_Background.AttachScriptData( m_ScriptData );
		m_Background.Init();

		// �����f�[�^����e�f�[�^��ݒ�
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
		if( m_PrivData.m_StatusFlags[ CLEARED ] ){

			if( m_PrivData.m_ClearModeData.m_Counter <= 100 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									0.0f, 0.0f,
									40.0f, 32.0f,
									false,
									( m_PrivData.m_ClearModeData.m_Counter * 255 / 100 ) << 24 );
			}
			else{
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									0.0f, 0.0f,
									40.0f, 32.0f,
									false,
									0xFF000000 );
			}
		
			if( m_PrivData.m_ClearModeData.m_Counter > 100 ){

				DrawFontString(	m_Data.m_ResourceMap, 170.0f, 200.0f, 0.45f, 0xFFFFFFFF, "Score %d",
								m_PrivData.m_StageResultData.m_Score );
				DrawFontString( m_Data.m_ResourceMap, 150.0f, 100.0f, 0.7f, 0xFFAA4444, "Stage %d Cleared",
								m_Data.m_StageNo );
				DrawFontString( m_Data.m_ResourceMap, 170.0f, 180.0f, 0.45f, 0xFFFFFFFF, "HP Rest %d x %d %d",
								m_PrivData.m_StageResultData.m_HP,
								CLEARED_HP_REST_BONUS,
								m_PrivData.m_StageResultData.m_HP * CLEARED_HP_REST_BONUS );
				DrawFontString( m_Data.m_ResourceMap, 170.0f, 220.0f, 0.45f, 0xFFFFFFFF, "Killed %d x %d %d",
								m_PrivData.m_StageResultData.m_Killed,
								CLEARED_KILLED_BONUS,
								m_PrivData.m_StageResultData.m_Killed * CLEARED_KILLED_BONUS );
				DrawFontString( m_Data.m_ResourceMap, 170.0f, 240.0f, 0.45f, 0xFFFFFFFF, "Crystal %d x %d %d",
								m_PrivData.m_StageResultData.m_Crystal,
								CLEARED_CRYSTAL_BONUS,
								m_PrivData.m_StageResultData.m_Crystal * CLEARED_CRYSTAL_BONUS );
				DrawFontString( m_Data.m_ResourceMap, 170.0f, 260.0f, 0.45f, 0xFFFFFFFF, "Crystal Rest %d x %d %d",
								m_PrivData.m_StageResultData.m_Crystal - m_PrivData.m_StageResultData.m_CrystalUsed,
								CLEARED_CRYSTAL_REST_BONUS,
								( m_PrivData.m_StageResultData.m_Crystal - m_PrivData.m_StageResultData.m_CrystalUsed ) * CLEARED_CRYSTAL_REST_BONUS );
				DrawFontString( m_Data.m_ResourceMap, 170.0f, 300.0f, 0.55f, 0xFFFFFFFF, "Bonus Total %d", GetStageClearBonus() );
				DrawFontString( m_Data.m_ResourceMap, 170.0f, 330.0f, 0.55f, 0xFFFFFFFF, "Total Score %d", m_Data.m_GameData.m_Score );
			}

			
		}

		if( m_PrivData.m_ClearModeData.m_PrepareCounter > 0 && m_PrivData.m_ClearModeData.m_PrepareCounter <= 20 ){
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
								0.0f, -( 20 - m_PrivData.m_ClearModeData.m_PrepareCounter ) * 24.0f, false );
		}
		else if( m_PrivData.m_ClearModeData.m_PrepareCounter >= 20 ){
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
								0.0f, 0.0f, false );
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

	void Stage::Impl::UpdateBossPhaseShiftEffect()
	{
		if( m_PrivData.m_BossModeData.m_IsBossMode ){
			if( m_PrivData.m_BossModeData.m_PhaseInterval == 0 ){
				for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ++it ){
					( *it )->PostMessage( EnemyShotMessage::ENEMY_SHOT_MESSAGE_ID_PLAYER_DAMAGED );
				}
				++m_PrivData.m_BossModeData.m_CurPhase;
				m_PrivData.m_BossModeData.m_ShiftFrame[ m_PrivData.m_BossModeData.m_CurPhase ] = m_Data.m_Frame;
				int elapsedTime =	m_PrivData.m_BossModeData.m_ShiftFrame[ m_PrivData.m_BossModeData.m_CurPhase ] -
									m_PrivData.m_BossModeData.m_ShiftFrame[ m_PrivData.m_BossModeData.m_CurPhase - 1 ];
				int bonus = ( 3600 - elapsedTime ) >= 0 ? ( 3600 - elapsedTime ) * 10 : 0;
				m_Data.m_FrameGameData.m_Score += bonus;
			}
			++m_PrivData.m_BossModeData.m_PhaseInterval;
			if( m_PrivData.m_BossModeData.m_PhaseInterval == 15 && m_Data.m_pBoss == NULL ){
				int elapsedTimeTotal =	m_PrivData.m_BossModeData.m_ShiftFrame[ m_PrivData.m_BossModeData.m_CurPhase ] -
										m_PrivData.m_BossModeData.m_ShiftFrame[ 0 ];
				int bonus = ( 18000 - elapsedTimeTotal ) >= 0 ? ( 18000 - elapsedTimeTotal ) * 20 : 0;
				m_Data.m_FrameGameData.m_Score += bonus;
			}
			if( m_PrivData.m_BossModeData.m_PhaseInterval == 240 && m_Data.m_pBoss == NULL ){
				m_PrivData.m_BossModeData.m_IsBossMode = false;
			}
		}
		else{
			m_PrivData.m_BossModeData.m_PhaseInterval = 350;
		}
	}

	void Stage::Impl::DrawBossPhaseShiftEffect() const
	{
		if( m_PrivData.m_BossModeData.m_IsBossMode ){
			int time = m_PrivData.m_BossModeData.m_PhaseInterval;
			int elapsedTime =	m_PrivData.m_BossModeData.m_ShiftFrame[ m_PrivData.m_BossModeData.m_CurPhase ] -
								m_PrivData.m_BossModeData.m_ShiftFrame[ m_PrivData.m_BossModeData.m_CurPhase - 1 ];
			int bonus = ( 3600 - elapsedTime ) >= 0 ? ( 3600 - elapsedTime ) * 10 : 0;
			if( time <= 30 ){
				DrawFontString( m_Data.m_ResourceMap, 600.0f - time * 14.0f, 120.0f, 0.5f, 0xFFFFFF00, "Time Bonus" );
				DrawFontString( m_Data.m_ResourceMap, 600.0f - time * 14.0f, 140.0f, 0.5f, "%.2f  %d", elapsedTime / 60.0f, bonus );
			}
			else if( time <= 180 ){
				DrawFontString( m_Data.m_ResourceMap, 180.0f, 120.0f, 0.5f, 0xFFFFFF00, "Time Bonus" );
				DrawFontString( m_Data.m_ResourceMap, 180.0f, 140.0f, 0.5f, "%.2f  %d", elapsedTime / 60.0f, bonus );
			}
			else if( time <= 220 ){
				DrawFontString( m_Data.m_ResourceMap, 2700.0f - time * 14.0f, 120.0f, 0.5f, 0xFFFFFF00, "Time Bonus" );
				DrawFontString( m_Data.m_ResourceMap, 2700.0f - time * 14.0f, 140.0f, 0.5f, "%.2f  %d", elapsedTime / 60.0f, bonus );
			}
			if( m_Data.m_pBoss == NULL ){
				int elapsedTimeTotal =	m_PrivData.m_BossModeData.m_ShiftFrame[ m_PrivData.m_BossModeData.m_CurPhase ] -
										m_PrivData.m_BossModeData.m_ShiftFrame[ 0 ];
				int bonus2 = ( 18000 - elapsedTimeTotal ) >= 0 ? ( 18000 - elapsedTimeTotal ) * 20 : 0;
				if( time <= 45 ){
					DrawFontString( m_Data.m_ResourceMap, 810.0f - time * 14.0f, 180.0f, 0.5f, 0xFFFF55AA, "Destroy Bonus" );
					DrawFontString( m_Data.m_ResourceMap, 810.0f - time * 14.0f, 200.0f, 0.5f, "%.2f  %d", elapsedTimeTotal / 60.0f, bonus2 );
				}
				else if( time <= 195 ){
					DrawFontString( m_Data.m_ResourceMap, 180.0f, 180.0f, 0.5f, 0xFFFF55AA, "Destroy Bonus" );
					DrawFontString( m_Data.m_ResourceMap, 180.0f, 200.0f, 0.5f, "%.2f  %d", elapsedTimeTotal / 60.0f, bonus2 );
				}
				else if( time <= 235 ){
					DrawFontString( m_Data.m_ResourceMap, 2910.0f - time * 14.0f, 180.0f, 0.5f, 0xFFFF55AA, "Destroy Bonus" );
					DrawFontString( m_Data.m_ResourceMap, 2910.0f - time * 14.0f, 200.0f, 0.5f, "%.2f  %d", elapsedTimeTotal / 60.0f, bonus2 );
				}
			}
		}
	}

	SceneType Stage::Impl::Update()
	{
		// ���v���C�o�O�΍�i���������_���Z��̏��������s���B�j
		__asm{
			finit
		}

		if( m_PrivData.m_StatusFlags[ HALT ] ){
			return SCENE_TYPE_NOT_CHANGE;
		}

		// �|�[�Y��Ԃ̎�
		if( m_PrivData.m_StatusFlags[ PAUSED ] ){
			SceneType type = m_Pause.Update();
			if( type == SCENE_TYPE_PAUSED ){
				return SCENE_TYPE_PAUSED;
			}
			else if( type == SCENE_TYPE_STAGE ){
				m_PrivData.m_StatusFlags.reset( PAUSED );
				return SCENE_TYPE_NOT_CHANGE;
			}
			else if( type == SCENE_TYPE_MENU ){
				// ��3D�̓G������ƃo�O���o��B
				m_PrivData.m_StatusFlags.reset( PAUSED );
				m_PrivData.m_StatusFlags.set( HALT );
				m_Background.Terminate();
				SaveStageResultData();
				return SCENE_TYPE_MENU;
			}
			else{
				throw MAPIL::MapilException( CURRENT_POSITION, TSTR( "Invalid scene type." ), -1 );
			}
		}

		// �������ɒu���Ȃ��ƁA���v���C��1�t���[�������������B
		if( MAPIL::IsKeyboardKeyPushed( MAPIL::GetKeyboardKeyCode( MAPIL::KEYBOARD_KEY_ESCAPE ) ) ){
			m_PrivData.m_StatusFlags.set( PAUSED );
			m_Pause.Reset();
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
		
		// ���t���[���ł̃X�R�A�����Z�b�g
		MAPIL::ZeroObject( &m_Data.m_FrameGameData, sizeof( m_Data.m_FrameGameData ) );

		// �����_���W�F�l���[�^�̍X�V
		m_Data.m_RandGen.Update( m_Data );

		// �X�e�[�W�����̃��b�Z�[�W������
		ProcessMessage();
		// �Q�[���I�[�o�[���̏���
		if( m_Data.m_HasTermSig ){
			m_Background.Terminate();
			SaveStageResultData();
			return SCENE_TYPE_SCORE_ENTRY;
		}
		// �X�e�[�W�N���A���̏���
		if( m_PrivData.m_StatusFlags[ CLEARED ] ){
			if( m_PrivData.m_ClearModeData.m_Counter == 0 ){
				SaveStageResultData();
			}
			// �{�[�i�X���_
			if( m_PrivData.m_ClearModeData.m_Counter == 200 ){
				m_Data.m_FrameGameData.m_Score += GetStageClearBonus();
			}
			if( m_PrivData.m_ClearModeData.m_Counter == 400 ){
				SaveStageResultData();
			}
			if( m_PrivData.m_ClearModeData.m_Counter > 400 ){
				m_PrivData.m_StatusFlags.set( PREPARING );
			}
			if( m_PrivData.m_StatusFlags[ PREPARING ] &&
				( IsPushed( m_ButtonStatus, GENERAL_BUTTON_SHOT ) || m_PrivData.m_ClearModeData.m_Counter >= 1200 ) ){
				m_Background.Terminate();
				//SaveStageResultData();		// ���̃X�e�[�W�ɍs���Ƃ��Ƀo�O�ɂȂ�H
				m_PrivData.m_StatusFlags.set( PREPARED_NEXT_STAGE );
			}
			if(	m_PrivData.m_StatusFlags[ PREPARED_NEXT_STAGE ] ){
				if( m_PrivData.m_ClearModeData.m_PrepareCounter >= 20 ){
					return SCENE_TYPE_STAGE;
				}
				else{
					++m_PrivData.m_ClearModeData.m_PrepareCounter;
				}
			}
			++m_PrivData.m_ClearModeData.m_Counter;
		}

		// �w�i�̍X�V
		m_Background.Update();

		// �X�N���v�g�R�}���h�̎��s
		m_VM.Run();
		if( m_VM.Terminated() ){
			m_Background.Terminate();
			SaveStageResultData();
			return SCENE_TYPE_SCORE_ENTRY;
		}

		// �Փ˔���
		ProcessCollision();
		
		// �SGameObject�̍X�V
		UpdateGameObjects();

		// �X�L���g�p���̃G�t�F�N�g�������X�V
		UpdateConsSkillEffect();

		// �{�X�펞�̃G�t�F�N�g�������X�V
		UpdateBossModeEffect();

		// �{�����̃G�t�F�N�g�������X�V
		if( m_PrivData.m_BombModeData.m_IsBombMode ){
			UpdateBombModeEffect();
		}

		// �X�R�A�̍X�V
		m_ScoreManager.Add( m_Data.m_FrameGameData.m_Score );
		m_ScoreManager.Update();
		m_Data.m_GameData.m_Score = m_ScoreManager.GetScore();
		m_Data.m_GameData.m_CrystalTotal += m_Data.m_FrameGameData.m_CrystalTotal;
		m_Data.m_GameData.m_CrystalUsed += m_Data.m_FrameGameData.m_CrystalUsed;
		m_Data.m_GameData.m_Killed += m_Data.m_FrameGameData.m_Killed;

		// �S�̃f�[�^�̍X�V
		m_Data.m_TotalGameData.m_Score = m_PrivData.m_FixedGameData.m_PrevScore + m_Data.m_GameData.m_Score;
		m_Data.m_TotalGameData.m_CrystalTotal = m_PrivData.m_FixedGameData.m_PrevCrystal + m_Data.m_GameData.m_CrystalTotal;
		m_Data.m_TotalGameData.m_CrystalUsed = m_PrivData.m_FixedGameData.m_PrevCrystalUsed + m_Data.m_GameData.m_CrystalUsed;
		m_Data.m_TotalGameData.m_Killed = m_PrivData.m_FixedGameData.m_PrevKilled + m_Data.m_GameData.m_Killed;

		// ���v���C�f�[�^�X�V
		m_KeyStateList.m_StatusList.push_back( m_ButtonStatus.m_RawButtonStatus );

		// �G���Ō�Ƀ_���[�W���󂯂����̃��b�Z�[�W��j��
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

		if( m_Data.m_HasTermSig || m_PrivData.m_StatusFlags[ HALT ] ){
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

		// �w�i�̕`��
		m_Background.Draw();

		MAPIL::EnableBlending();

		// 2D�摜�`��J�n
		MAPIL::BeginRendering2DGraphics();
		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
		
		// �v���C���[�̕`��
		m_Data.m_pPlayer->Draw();

		// �G�̕`��
		for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ++it ){
			( *it )->Draw();
		}
		// �{�X�̕`��
		if( m_Data.m_pBoss ){
			m_Data.m_pBoss->Draw();
		}

		MAPIL::EndRendering2DGraphics();
		MAPIL::DisableLighting();
		MAPIL::DoAllModelOn2DBatchWorks();
		MAPIL::EnableLighting();
		MAPIL::BeginRendering2DGraphics();

		ProcAllBatchWorks();

		// �X�L���g�p���̃G�t�F�N�g��`��
		DrawConsSkillEffect();

		// �{���g�p���̃G�t�F�N�g��`��
		if( m_PrivData.m_BombModeData.m_IsBombMode ){
			DrawBombModeEffect();
		}

		// �A�C�e���̕`��
		for( ItemList::iterator it = m_Data.m_ItemList.begin(); it != m_Data.m_ItemList.end(); ++it ){
			( *it )->Draw();
		}

		// �v���C���[�V���b�g�̕`��
		for( PlayerShotList::iterator it = m_Data.m_PlayerShotList.begin(); it != m_Data.m_PlayerShotList.end(); ++it ){
			( *it )->Draw();
		}

		// �G�V���b�g�̕`��
		for( EnemyShotList::iterator it = m_Data.m_EnemyShotList.begin(); it != m_Data.m_EnemyShotList.end(); ++it ){
			( *it )->Draw();
		}

		ProcAllBatchWorks();
		
		
		// �G�t�F�N�g�̕`��
		for( EffectList::iterator it = m_Data.m_EffectList.begin(); it != m_Data.m_EffectList.end(); ++it ){
			( *it )->Draw();
		}

		

		// �{�X�퓬���̃G�t�F�N�g�̕`��
		DrawBossModeEffect();

		LateDrawConsSkillEffect();

		// ���U���g��ʕ\��
		DrawResult();

		// �|�[�Y��Ԃ̎�
		if( m_PrivData.m_StatusFlags[ PAUSED ] ){
			m_Pause.Draw();
		}

		if( !m_PrivData.m_IsFirstTime ){
			if( m_Data.m_Frame <= 10 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
									0.0f, 0.0f, false );
			}
			else if( m_Data.m_Frame >= 10 && m_Data.m_Frame <= 30 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
									0.0f, -( m_Data.m_Frame - 10 ) * 24.0f, false );
			}
		}

		// ��ԉ�ʂ̕`��
		m_Profiler.Begin( "Game Stat" );

		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_STAGE_BACKGROUND ],
							0.0f, 0.0f, false );
		// �����
		
		// HP�\��
		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT );
		int hp = m_Data.m_pPlayer->GetHP();
		for( int i = 0; i < hp && i < 10; ++i ){
			int surplus = ( 30 + m_Data.m_Frame - i * 3 ) % 30;
			if( surplus < 20 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									16.0f + 11.0f * i, 88.0f, 0.51f - surplus * 0.01f, 0.9f - surplus * 0.015f, true,
									( 200 - surplus * 10 ) << 24 | ( 120 + i * 15 ) << 8 | ( i * 10 ) | ( 255 - i * 10 ) << 16 );
			}
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								16.0f + 11.0f * i, 88.0f, 0.31f, 0.6f, true, 0xFF000000 | ( 120 + i * 15 ) << 8 | ( i * 10 ) | ( 255 - i * 10 ) << 16 );
		}
		DrawFontString( m_Data.m_ResourceMap, 80.0f, 110.0f, 0.5f, 0xFFFFFFFF, "%d",
						hp - 10 > 0 ? hp - 10 : 0 );
		int lv =  m_Data.m_pPlayer->GetShotPower() / 10;
		for( int i = 0; i < lv; ++i ){
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								12.0f + 22.0f * i, 168.0f, 1.07f, 0.5f, false, 0xEE33FF33 );
			if( ( m_Data.m_Frame % 3 ) == 0 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								12.0f + 22.0f * i, 168.0f, 1.07f, 0.5f, false, 0xFFFFFFFF );
			}
		}
		MAPIL::DrawTexture( m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
							12.0f + 22.0f * lv, 168.0f, 0.107f * ( m_Data.m_pPlayer->GetShotPower() % 10 ), 0.5f, false,
							0xEE000033 | ( ( m_Data.m_pPlayer->GetShotPower() % 10 ) * 25 ) << 8 | ( ( 10 - ( m_Data.m_pPlayer->GetShotPower() % 10 ) ) * 25 ) << 16 );

		static int moveCount = 0;
		static int prevCons = PLAYER_CONS_MODE_NORMAL;

		const float CONS_GAUGE_OFFSET = 26.0f;
		const float CONS_GAUGE_BASE = 229.0f;

		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
							0.0f, 166.0f, 8.0f, 7.5f, false, 0xFF000000 );

		if( m_Data.m_pPlayer->GetCurCons() == PLAYER_CONS_MODE_NORMAL ){
			prevCons = PLAYER_CONS_MODE_NORMAL;
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE, m_Data.m_pPlayer->GetConsGauge( 0 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x22FF22 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET, m_Data.m_pPlayer->GetConsGauge( 1 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x5555FF );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET * 2, m_Data.m_pPlayer->GetConsGauge( 2 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0xFF7722 );
		}
		else if( m_Data.m_pPlayer->GetCurCons() == PLAYER_CONS_MODE_GREEN ){
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE, m_Data.m_pPlayer->GetConsGauge( 0 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0x55FF55 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE, m_Data.m_pPlayer->GetConsGauge( 0 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0x55FF55 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET, m_Data.m_pPlayer->GetConsGauge( 1 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x5555FF );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET * 2, m_Data.m_pPlayer->GetConsGauge( 2 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0xFF7722 );
		}
		else if( m_Data.m_pPlayer->GetCurCons() == PLAYER_CONS_MODE_BLUE ){
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET, m_Data.m_pPlayer->GetConsGauge( 1 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0x5555FF );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET, m_Data.m_pPlayer->GetConsGauge( 1 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0x5555FF );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE, m_Data.m_pPlayer->GetConsGauge( 0 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x22FF22 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET * 2, m_Data.m_pPlayer->GetConsGauge( 2 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0xFF7722 );
		}
		else if( m_Data.m_pPlayer->GetCurCons() == PLAYER_CONS_MODE_RED ){
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET * 2, m_Data.m_pPlayer->GetConsGauge( 2 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0xFF7722 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET * 2, m_Data.m_pPlayer->GetConsGauge( 2 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0xFF7722 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE, m_Data.m_pPlayer->GetConsGauge( 0 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x22FF22 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								37.0f, CONS_GAUGE_BASE + CONS_GAUGE_OFFSET, m_Data.m_pPlayer->GetConsGauge( 1 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x5555FF );
		}

		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
							0.0f, 292.0f, 8.0f, 7.5f, false, 0xFF000000 );

		// �ӎ����x���̕`��
		for( int i = 1; i <= 10; ++i ){
			if( m_Data.m_pPlayer->GetConsLevel( 0 ) / ( i * 100 ) >= 1 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									31.0f + i * 8.0f, 328.0f - 16 * ( 0.22f + i * 0.068f ), 0.22f, 0.2f + i * 0.0675f, false, 0xAA55FF55 );
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									31.0f + i * 8.0f, 328.0f - 16 * ( 0.22f + i * 0.068f ), 0.22f, 0.2f + i * 0.0675f, false, ( ( m_Data.m_Frame % 10 ) * 15 + 100 ) << 24 | 0x55FF55 );
			}
			else{
				MAPIL::DrawTexture( m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									31.0f + i * 8.0f, 328.0f - 16 * ( 0.22f + i * 0.068f ), 0.22f, 0.2f + i * 0.0675f, false, ( ( m_Data.m_pPlayer->GetConsLevel( 0 ) % 100 ) * 255 / 100 ) << 24 | 0x22FF22 );
				break;
			}
		}
		for( int i = 1; i <= 10; ++i ){
			if( m_Data.m_pPlayer->GetConsLevel( 1 ) / ( i * 100 ) >= 1 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									31.0f + i * 8.0f, 328.0f - 16 * ( 0.22f + i * 0.068f ) + 27.0f, 0.22f, 0.2f + i * 0.0675f, false, 0xAA5555FF );
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									31.0f + i * 8.0f, 328.0f - 16 * ( 0.22f + i * 0.068f ) + 27.0f, 0.22f, 0.2f + i * 0.0675f, false, ( ( m_Data.m_Frame % 10 ) * 15 + 100 ) << 24 | 0x5555FF );
			}
			else{
				MAPIL::DrawTexture( m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									31.0f + i * 8.0f, 328.0f - 16 * ( 0.22f + i * 0.068f ) + 27.0f, 0.22f, 0.2f + i * 0.0675f, false, ( ( m_Data.m_pPlayer->GetConsLevel( 1 ) % 100 ) * 255 / 100 ) << 24 | 0x5555FF );
				break;
			}
		}
		for( int i = 1; i <= 10; ++i ){
			if( m_Data.m_pPlayer->GetConsLevel( 2 ) / ( i * 100 ) >= 1 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									31.0f + i * 8.0f, 328.0f - 16 * ( 0.22f + i * 0.068f ) + 53.0f, 0.22f, 0.2f + i * 0.0675f, false, 0xAAFF7722 );
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									31.0f + i * 8.0f, 328.0f - 16 * ( 0.22f + i * 0.068f ) + 53.0f, 0.22f, 0.2f + i * 0.0675f, false, ( ( m_Data.m_Frame % 10 ) * 15 + 100 ) << 24 | 0xFF7722 );
			}
			else{
				MAPIL::DrawTexture( m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									31.0f + i * 8.0f, 328.0f - 16 * ( 0.22f + i * 0.068f ) + 53.0f, 0.22f, 0.2f + i * 0.0675f, false, ( ( m_Data.m_pPlayer->GetConsLevel( 2 ) % 100 ) * 255 / 100 ) << 24 | 0xFF7722 );
				break;
			}
		}

		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );

		const char* DIFFICULTY_DISP[] = { "calm", "easy", "normal", "hard", "hazard" };

		DrawFontString( m_Data.m_ResourceMap, 30.0f, 440.0f, 0.4f, 0xFFFF0000, DIFFICULTY_DISP[ m_Data.m_Difficulty ] );
		
		
		// �E���
		const float RIGHT_DISP_OFFSET_X = 525.0f;
		const float RIGHT_DISP_OFFSET_Y	= 41.0f;
		float dispOffsetY = RIGHT_DISP_OFFSET_Y;
		// �n�C�X�R�A
	
		if( m_PrivData.m_IniGameData.m_HIScore <= m_Data.m_GameData.m_Score + m_PrivData.m_IniGameData.m_Score ){
			DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 35.0f, 0.4f, "%08d", m_Data.m_GameData.m_Score + m_PrivData.m_IniGameData.m_Score );
		}
		else{
			DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 35.0f, 0.4f, "%08d", m_PrivData.m_IniGameData.m_HIScore );
		}

		// �X�R�A
		dispOffsetY += 45.0f;
		DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 35.0f, 0.4f, "%08d", m_Data.m_GameData.m_Score + m_PrivData.m_FixedGameData.m_PrevScore );
		DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 55.0f, 0.4f, 0xFF888888, "%08d", m_Data.m_GameData.m_Score );
		// ���j��
		dispOffsetY += 66.0f;
		DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 35.0f, 0.4f, "%d", m_Data.m_GameData.m_Killed + m_PrivData.m_FixedGameData.m_PrevKilled );
		DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 5.0f, dispOffsetY + 55.0f, 0.4f, 0xFF888888, "%d", m_Data.m_GameData.m_Killed );
		// �擾�N���X�^����
		dispOffsetY += 66.0f;
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
		// �i�s�x
		dispOffsetY += 90.0f;

		

		if( m_Data.m_Frame < m_Data.m_FrameTotal ){
			//MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_STAGE_BACKGROUND_2 ],
			//					RIGHT_DISP_OFFSET_X, dispOffsetY + 21.0f, m_Data.m_Frame * 6.1f / m_Data.m_FrameTotal, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x00AAFF );
			//MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_STAGE_BACKGROUND_2 ],
			//					RIGHT_DISP_OFFSET_X - 12.0f, dispOffsetY + 16.0f, 0.0f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x00AAFF );
			MAPIL::DrawClipedTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_STAGE_BACKGROUND_2 ],
										RIGHT_DISP_OFFSET_X - 12.0f, dispOffsetY + 16.0f, 0.13f + m_Data.m_Frame * 0.75f / m_Data.m_FrameTotal, 1.0f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x00AAFF );
		}
		else{
			//MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_STAGE_BACKGROUND_2 ],
			//					RIGHT_DISP_OFFSET_X, dispOffsetY + 21.0f, 6.1f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x00AAFF );
			//MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_STAGE_BACKGROUND_2 ],
			//					RIGHT_DISP_OFFSET_X - 12.0f, dispOffsetY + 16.0f, 0.0f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x00AAFF );
			MAPIL::DrawClipedTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_STAGE_BACKGROUND_2 ],
										RIGHT_DISP_OFFSET_X - 12.0f, dispOffsetY + 16.0f, 1.0f, 1.0f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x00AAFF );
		}

		// �Ō�ɍU�������G�f�[�^
		dispOffsetY += 57.0f;
		if( m_PrivData.m_LastDamagedEnemyData.m_MaxConsGauge != 10000 ){
			DrawFontString( m_Data.m_ResourceMap, RIGHT_DISP_OFFSET_X + 27.0f, dispOffsetY, 0.25f, 0xFFAAFFAA, "%s", m_PrivData.m_LastDamagedEnemyData.m_EnemyName.c_str() );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								RIGHT_DISP_OFFSET_X + 50.0f, dispOffsetY + 16.0f, m_PrivData.m_LastDamagedEnemyData.m_HP * 3.55f / m_PrivData.m_LastDamagedEnemyData.m_MaxHP, 0.55f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0xFF2222 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								RIGHT_DISP_OFFSET_X + 50.0f, dispOffsetY + 34.0f, m_PrivData.m_LastDamagedEnemyData.m_ConsGauge * 3.55f / m_PrivData.m_LastDamagedEnemyData.m_MaxConsGauge, 0.55f, false, ( 255 - ( m_Data.m_Frame % 30 ) * 2 ) << 24 | 0x22FF22 );
		
		}

		if( m_PrivData.m_IsFirstTime ){
			if( m_Data.m_Frame <= 10 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
									0.0f, 0.0f, false );
			}
			else if( m_Data.m_Frame >= 10 && m_Data.m_Frame <= 30 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GENERAL_BACKGROUND ],
									0.0f, -( m_Data.m_Frame - 10 ) * 24.0f, false );
			}
		}
		
		m_Profiler.End( "Game Stat" );

		

		m_Profiler.End( "Draw" );

		m_Profiler.Begin( "Debug Stat" );

		// �f�o�b�O�\��
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

		// 2D�`��I��
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
	}
	
	int Stage::Impl::GetScore() const
	{
		return m_PrivData.m_StageResultData.m_Score;
	}

	int Stage::Impl::GetKilled() const
	{
		return m_PrivData.m_StageResultData.m_Killed;
	}

	int Stage::Impl::GetCrystal() const
	{
		return m_PrivData.m_StageResultData.m_Crystal;
	}

	int Stage::Impl::GetCrystalUsed() const
	{
		return m_PrivData.m_StageResultData.m_CrystalUsed;
	}

	void Stage::Impl::GetPlayerPos( int* pPosX, int* pPosY ) const
	{
		*pPosX = m_PrivData.m_StageResultData.m_PosX;
		*pPosY = m_PrivData.m_StageResultData.m_PosY;
	}

	int Stage::Impl::GetPlayerHP() const
	{
		return m_PrivData.m_StageResultData.m_HP;
	}

	int Stage::Impl::GetPlayerShotPower() const
	{
		return m_PrivData.m_StageResultData.m_ShotPower;
	}

	int Stage::Impl::GetPlayerCons() const
	{
		return m_PrivData.m_StageResultData.m_Cons;
	}

	void Stage::Impl::GetPlayerConsGauge( int* pGauge ) const
	{
		for( int i = 0; i < 3; ++i ){
			pGauge[ i ] = m_PrivData.m_StageResultData.m_ConsGauge[ i ];
		}
	}

	void Stage::Impl::GetPlayerConsLevel( int* pLevel ) const
	{
		for( int i = 0; i < 3; ++i ){
			pLevel[ i ] = m_PrivData.m_StageResultData.m_ConsLevel[ i ];
		}
	}

	ReplayDataRecord::StageKeyStates Stage::Impl::GetKeyStates() const
	{
		return m_KeyStateList;
	}

	StageStat& Stage::Impl::GetStageStat()
	{
		return m_Data.m_StageStat;
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

	void Stage::Impl::MarkFirstTime()
	{
		m_PrivData.m_IsFirstTime = true;
	}

	// ----------------------------------
	// �����N���X�̌Ăяo��
	// ----------------------------------

	Stage::Stage( int stageNo, bool isReplay ) : Scene(), m_pImpl( new Stage::Impl( stageNo, isReplay ) )
	{
	}

	Stage::~Stage()
	{
	}

	void Stage::InitImpl()
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

	StageStat& Stage::GetStageStat()
	{
		return m_pImpl->GetStageStat();
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

	void Stage::MarkFirstTime()
	{
		m_pImpl->MarkFirstTime();
	}
}
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

#include "ScoreManager.h"

#include "Util.h"

namespace GameEngine
{
	class Stage::Impl
	{
	private:
		ButtonStatusHolder			m_ButtonStatus;		// �{�^���̏��
		ScriptData					m_ScriptData;		// �X�N���v�g�f�[�^
		StageData					m_Data;				// �X�e�[�W�p�f�[�^
		ScoreManager				m_ScoreManager;		// �X�R�A�Ǘ��N���X

		StageVCPU					m_VM;				// ���z�}�V��

		void ProcessCollision();		// �Փ˔���
		void UpdateGameObjects();		// �SGameObject�̍X�V
		void ProcessMessage();			// �X�e�[�W�p���b�Z�[�W�̏���

		// GameObject�S�폜���\�b�h
		void DeleteAllEnemyShots();
		void DeleteAllPlayerShots();
		void DeleteAllEnemies();
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
		GameDataMsg GetFrameData() const;
	};

	Stage::Impl::Impl( int stageNo, bool isReplay )	:	m_ButtonStatus(),
														m_Data(),
														m_VM(),
														m_ScoreManager()
	{
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
	}

	Stage::Impl::~Impl()
	{
		// GameObject�̔j��
		DeleteAllPlayerShots();
		DeleteAllEnemies();
		DeleteAllEnemyShots();
		DeleteAllItems();
		DeleteAllEffects();
		for( EnemyShotGroupList::iterator it = m_Data.m_EnemyShotGroupList.begin(); it != m_Data.m_EnemyShotGroupList.end(); ++it ){
			delete ( *it );
		}
		m_Data.m_EnemyShotGroupList.clear();
		MAPIL::SafeDelete( m_Data.m_pPlayer );
	}

	void Stage::Impl::ProcessCollision()
	{
		// �Փ˔���
		// �G-�v���C���[�V���b�g
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
				float distance = ( eX - psX ) * ( eX - psX ) + ( eY - psY ) * ( eY - psY );
				float radius = ( psRad + eRad ) * ( psRad + eRad );
				if( distance < radius ){
					( *itShot )->Colided( *itEnemy );
					( *itEnemy )->Colided( *itShot );
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
			float eX;
			float eY;
			float eRad;
			( *it )->GetPos( &eX, &eY );
			eRad = ( *it )->GetCollisionRadius();
			float distance = ( eX - pX ) * ( eX - pX ) + ( eY - pY ) * ( eY - pY );
			float radius = ( pRad + eRad ) * ( pRad + eRad );
			if( distance < radius ){
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
					m_Data.m_pPlayer->Colided( *it );
				}
				// �߂��ɂ��鎞
				else{
					( *it )->PlayerIsNear( m_Data.m_pPlayer );
				}
			}
		}
	}

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
				delete ( *it );
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
			int msg = m_Data.m_MsgQueue.front();
			switch( msg ){
				case STAGE_MESSAGE_PLAYER_DAMAGED:
					DeleteAllEnemyShots();
					break;
				case STAGE_MESSAGE_PLAYER_DESTORYED:
					m_Data.m_HasTermSig = true;
					break;
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
			delete ( *it );
		}
		m_Data.m_ItemList.clear();
	}

	void Stage::Impl::Init()
	{	
		m_VM.Init( &m_ScriptData.m_pStageScriptData->m_Data, &m_Data );
		m_Data.m_ObjBuilder.AttachStageData( &m_Data );
		m_Data.m_pPlayer = reinterpret_cast < Player* > ( m_Data.m_ObjBuilder.CreateCollisionObject( GAME_OBJECT_ID_PLAYER ) );
		MAPIL::ZeroObject( &m_Data.m_FrameGameData, sizeof( m_Data.m_FrameGameData ) );
		MAPIL::ZeroObject( &m_Data.m_GameData, sizeof( m_Data.m_GameData ) );
	}

	SceneType Stage::Impl::Update()
	{
		// ���t���[���ł̃X�R�A�����Z�b�g
		MAPIL::ZeroObject( &m_Data.m_FrameGameData, sizeof( m_Data.m_FrameGameData ) );

		// �����_���W�F�l���[�^�̍X�V
		m_Data.m_RandGen.Update( m_Data );

		//static EnemyShotGroup group( NULL, &m_Data );
		//static int id = 5;

		//if( m_Data.m_Frame == 60 ){
		//	id = group.CreateShot();
		//	group.SetShotStatus( id, 250.0f, 100.0f, MAPIL::DegToRad( -90.0f ), 1.0f, 5.0f, 0 );
		//	id = group.CreateShot();
		//	group.SetShotStatus( id, 300.0f, 100.0f, MAPIL::DegToRad( -80.0f ), 1.0f, 5.0f, 0 );
		//}

		//if( group.IsEmpty() ){
		//	int i = 0;
		//	++i;
		//}

		// �X�e�[�W�����̃��b�Z�[�W������
		ProcessMessage();
		if( m_Data.m_HasTermSig ){
			return SCENE_TYPE_SCORE_ENTRY;
		}

		// �X�N���v�g�R�}���h�̎��s
		m_VM.Run();
		if( m_VM.Terminated() ){
			return SCENE_TYPE_SCORE_ENTRY;
		}

		// �Փ˔���
		ProcessCollision();
		
		// �SGameObject�̍X�V
		UpdateGameObjects();

		// �X�R�A�̍X�V
		m_ScoreManager.Add( m_Data.m_FrameGameData.m_Score );
		m_ScoreManager.Update();
		m_Data.m_GameData.m_Score = m_ScoreManager.GetScore();
		if( m_Data.m_GameData.m_HIScore <= m_Data.m_GameData.m_Score ){
			m_Data.m_GameData.m_HIScore = m_Data.m_GameData.m_Score;
		}
		m_Data.m_GameData.m_CrystalTotal += m_Data.m_FrameGameData.m_CrystalTotal;
		m_Data.m_GameData.m_Killed += m_Data.m_FrameGameData.m_Killed;

		++m_Data.m_Frame;

		return SCENE_TYPE_NOT_CHANGE;
	}

	void Stage::Impl::Draw()
	{
		// 2D�摜�`��J�n
		MAPIL::BeginRendering2DGraphics();

		if( m_Data.m_pPlayer->GetCurCons() == PLAYER_CONS_MODE_GREEN ){
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								0.0f, 0.0f, 40.0f, 30.0f, 0.0f, false, 0x5544FF44 );
		}
		else if( m_Data.m_pPlayer->GetCurCons() == PLAYER_CONS_MODE_BLUE ){
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								0.0f, 0.0f, 40.0f, 30.0f, 0.0f, false, 0x554444FF );
		}
		else if( m_Data.m_pPlayer->GetCurCons() == PLAYER_CONS_MODE_RED ){
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								0.0f, 0.0f, 40.0f, 30.0f, 0.0f, false, 0x55FF4444 );
		}

		

		// �v���C���[�̕`��
		m_Data.m_pPlayer->Draw();

		// �G�̕`��
		for( EnemyList::iterator it = m_Data.m_EnemyList.begin(); it != m_Data.m_EnemyList.end(); ++it ){
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
		// �A�C�e���̕`��
		for( ItemList::iterator it = m_Data.m_ItemList.begin(); it != m_Data.m_ItemList.end(); ++it ){
			( *it )->Draw();
		}
		// �G�t�F�N�g�̕`��
		for( EffectList::iterator it = m_Data.m_EffectList.begin(); it != m_Data.m_EffectList.end(); ++it ){
			( *it )->Draw();
		}
		

		// ��ԉ�ʂ̕`��
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_BACKGROUND_TEXTURE ],
							0.0f, 0.0f, false );
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_BACKGROUND_TEXTURE ],
							512.0f, 0.0f, false );
		// �����
		//DrawFontString( m_Data.m_ResourceMap, 30.0f, 80.0f, 0.5f, "HP %d", m_Data.m_pPlayer->GetHP() );
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GAME_HP ],
							43.0f, 40.0f, false );
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
		//DrawFontString( m_Data.m_ResourceMap, 30.0f, 120.0f, 0.4f, 0xFFFFFF00, "Power %d", m_Data.m_pPlayer->GetShotPower() );
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
								0.0f, 190.0f, 8.0f, 2.0f, false, 0xFF00CCEE );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE, m_Data.m_pPlayer->GetConsGauge( 0 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0x22FF22 );
			MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
								35.0f, CONS_GAUGE_BASE, m_Data.m_pPlayer->GetConsGauge( 0 ) * 5.0f / 1000.0f, 0.8f, false, ( 255 - ( m_Data.m_Frame % 6 ) * 20 ) << 24 | 0x22FF22 );
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
								0.0f, 190.0f + moveCount, 8.0f, 2.0f, false, 0xFF00CCEE );
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
								0.0f, 222.0f + moveCount, 8.0f, 2.0f, false, 0xFF00CCEE );
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

		for( int i = 1; i <= 10; ++i ){
			if( m_Data.m_pPlayer->GetConsLevel( 0 ) / ( i * 100 ) >= 1 ){
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									25.0f + i * 9.0f, 340.0f - 16 * ( 0.2f + i * 0.08f ), 0.3f, 0.2f + i * 0.08f, false, 0xAA22FF22 );
				MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_BAR ],
									25.0f + i * 9.0f, 340.0f - 16 * ( 0.2f + i * 0.08f ), 0.3f, 0.2f + i * 0.08f, false, ( ( m_Data.m_Frame % 10 ) * 15 + 100 ) << 24 | 0x22FF22 );
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
									25.0f + i * 9.0f, 350.0f - 16 * ( 0.2f + i * 0.08f ) + 20.0f, 0.3f, 0.2f + i * 0.08f, false, ( ( m_Data.m_pPlayer->GetConsLevel( 0 ) % 100 ) * 255 / 100 ) << 24 | 0x5555FF );
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
									25.0f + i * 9.0f, 360.0f - 16 * ( 0.2f + i * 0.08f ) + 40.0f, 0.3f, 0.2f + i * 0.08f, false, ( ( m_Data.m_pPlayer->GetConsLevel( 0 ) % 100 ) * 255 / 100 ) << 24 | 0xFF7722 );
				break;
			}
		}
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
		


		DrawFontString( m_Data.m_ResourceMap, 30.0f, 440.0f, 0.4f, 0xFFFF0000, "hazard" );
		// �E���
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_HI_SCORE ],
							525.0f, 55.0f, 0.8f, 0.8f, false );
		//DrawFontString( m_Data.m_ResourceMap, 530.0f, 70.0f, 0.4f, 0xFFAAFFAA, "hi score" );
		DrawFontString( m_Data.m_ResourceMap, 530.0f, 90.0f, 0.4f, "%08d", m_Data.m_GameData.m_HIScore );
		//DrawFontString( m_Data.m_ResourceMap, 530.0f, 140.0f, 0.4f, 0xFFAAFFAA, "score" );
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GAME_SCORE ],
							525.0f, 125.0f, 0.8f, 0.8f, false );
		DrawFontString( m_Data.m_ResourceMap, 530.0f, 160.0f, 0.4f, "%08d", m_Data.m_GameData.m_Score );
		//DrawFontString( m_Data.m_ResourceMap, 530.0f, 210.0f, 0.4f, 0xFFAAFFAA, "killed" );
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GAME_KILLED ],
							525.0f, 195.0f, 0.8f, 0.8f, false );
		DrawFontString( m_Data.m_ResourceMap, 530.0f, 230.0f, 0.4f, "%d", m_Data.m_GameData.m_Killed );
		//DrawFontString( m_Data.m_ResourceMap, 530.0f, 280.0f, 0.4f, 0xFFAAFFAA, "crystal" );
		MAPIL::DrawTexture(	m_Data.m_ResourceMap.m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_TEXTURE_ID_GAME_CRYSTAL ],
							525.0f, 265.0f, 0.8f, 0.8f, false );
		DrawFontString( m_Data.m_ResourceMap, 530.0f, 300.0f, 0.4f, "%d", m_Data.m_GameData.m_CrystalTotal );
		DrawFontString( m_Data.m_ResourceMap, 530.0f, 350.0f, 0.4f, 0xFFAAFFAA, "progress" );
		DrawFontString( m_Data.m_ResourceMap, 530.0f, 450.0f, 0.4f, 0xFFAAFFAA, "fps" );
		
		MAPIL::DrawString( 530.0f, 370.0f, "EnemyShot : %d", m_Data.m_EnemyShotList.size() );
		MAPIL::DrawString( 530.0f, 390.0f, "Item : %d", m_Data.m_ItemList.size() );
		MAPIL::DrawString( 530.0f, 410.0f, "Effect : %d", m_Data.m_EffectList.size() );
		MAPIL::DrawString( 530.0f, 430.0f, "PlayerShot : %d", m_Data.m_PlayerShotList.size() );
		MAPIL::DrawString( 500.0f, 450.0f, "EnemyShotGroup : %d", m_Data.m_EnemyShotGroupList.size() );
		
		
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
		return m_Data.m_Frame;
	}
	
	int Stage::Impl::GetScore() const
	{
		return m_Data.m_GameData.m_Score;
	}

	int Stage::Impl::GetKilled() const
	{
		return m_Data.m_GameData.m_Killed;
	}

	int Stage::Impl::GetCrystal() const
	{
		return m_Data.m_GameData.m_CrystalTotal;
	}

	GameDataMsg Stage::Impl::GetFrameData() const
	{
		return m_Data.m_FrameGameData;
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

	GameDataMsg Stage::GetFrameData() const
	{
		return m_pImpl->GetFrameData();
	}
}
#include <MAPIL/MAPIL.h>

#include "Player.h"

#include "Enemy.h"
#include "Item.h"

#include "ResourceTypes.h"
#include "ScriptTypes.h"
#include "Stage.h"
#include "Effect.h"
#include "PlayerShot.h"
#include "EnemyShotGroup.h"
#include "ScriptEffect.h"
#include "ResourceID.h"

#include "Util.h"

namespace GameEngine
{
	Enemy::Enemy(	std::shared_ptr < ResourceMap > pMap,
					std::shared_ptr < EnemyScriptData > pData,
					int id,
					StageData* pStageData ) :	m_VM(),
												m_ScriptID( id ),
												m_pScriptData( pData )
	{
		m_Data.m_GUData.m_PosX = 300;
		m_Data.m_GUData.m_PosY = 100;
		m_Data.m_GUData.m_PosZ = 0.5f;
		m_Data.m_GUData.m_ColRadius = 0; 

		m_Data.m_pStageData = pStageData;
		m_Data.m_Destroyed = false;
		m_Data.m_pResouceMap = pMap;
		m_Data.m_ScaleX = 1.0f;
		m_Data.m_ScaleY = 1.0f;
		m_Data.m_ScaleZ = 1.0f;
		m_Data.m_RotX = 0.0f;
		m_Data.m_RotY = 0.0f;
		m_Data.m_RotZ = 0.0f;
		m_Data.m_ImgID = -1;
		m_Data.m_Score = 0;
		m_Data.m_ScoreBonus = 0;
		m_Data.m_HP = 2000;
		m_Data.m_MaxHP = 2000;
		m_Data.m_Counter = 0;
		m_Data.m_ConsGauge = 200;
		m_Data.m_MaxConsGauge = 200;
		m_Data.m_DamagedConsGauge = 0;
		m_Data.m_IsBoss = false;
		m_Data.m_IsInvincibleMode = false;
		m_Data.m_IsNonCollisionMode = false;
		m_Data.m_IsConsSkillMode = false;
		m_Data.m_Paused = false;
		m_Data.m_DispMode = EnemyData::DISPLAY_MODE_2D;
		//m_Data.m_Is3D = false;
		m_Data.m_ConsSkillName.clear();
		m_Data.m_ConsType = 0;
		m_Data.m_ShotGroupList.clear();
		m_Data.m_Name = "No Name";
		MAPIL::ZeroObject( &m_Data.m_Regs, sizeof( m_Data.m_Regs ) );
		m_Data.m_StatusFlags.reset();

		m_PrivateData.m_PrevConsGauge = 200;
		m_PrivateData.m_ConsSkillEffectCounter = 0;
		m_PrivateData.m_ConsSkillEffectPostCounter = 500;
	}

	Enemy::~Enemy()
	{
		for( int i = 0; i < m_Data.m_ShotGroupList.size(); ++i ){
			if( m_Data.m_ShotGroupList[ i ] != NULL ){
				m_Data.m_ShotGroupList[ i ]->DetachEnemyControl();
			}
		}
		m_Data.m_ShotGroupList.clear();
	}

	void Enemy::Init( const GameUnit& posX, const GameUnit& posY )
	{
		m_Data.m_GUData.m_PosX = posX;
		m_Data.m_GUData.m_PosY = posY;
		m_VM.Init( &m_pScriptData->m_pElm[ m_ScriptID ].m_Data, &m_Data );
	}

	void Enemy::SetSelfRef( std::shared_ptr < Enemy > self )
	{
		m_Data.m_Self = self;
	}

	void Enemy::Draw()
	{
		float posX = m_Data.m_GUData.m_PosX.GetFloat();
		float posY = m_Data.m_GUData.m_PosY.GetFloat();
		float posZ = m_Data.m_GUData.m_PosZ.GetFloat();
		float colRadius = m_Data.m_GUData.m_ColRadius.GetFloat();

		// HP/�ӎ��ʂ̕\��
		if( !m_Data.m_IsNonCollisionMode && !m_Data.m_IsBoss ){
			MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_CONS_BAR_TEXTURE ],
								posX + 5.0f, posY - 5.0f,
								m_Data.m_ConsGauge / 150.0f, 0.3f, false, 0xAAFFFFFF );
			if( m_Data.m_HP > 0 ){
				MAPIL::DrawTexture(	m_Data.m_pResouceMap->m_pGlobalResourceMap->m_TextureMap[ GLOBAL_RESOURCE_ID_HP_BAR_TEXTURE ],
									posX + 5.0f, posY - 10.0f,
									m_Data.m_HP * 1.5f / m_Data.m_MaxHP, 0.3f, false, 0xAAFFFFFF );
			}
		}

		if( m_Data.m_ImgID == -1 ){
			MAPIL::DrawString( posX, posY, "��" );
		}
		else{
			if( m_Data.m_DispMode == EnemyData::DISPLAY_MODE_2D ){
				MAPIL::DrawTexture( m_Data.m_pResouceMap->m_pStageResourceMap->m_TextureMap[ m_Data.m_ImgID ],
									posX, posY );
			}
			else if( m_Data.m_DispMode == EnemyData::DISPLAY_MODE_3D ){
				MAPIL::EndRendering2DGraphics();
				MAPIL::AddModelOn2DBatchWork(	m_Data.m_pResouceMap->m_pStageResourceMap->m_ModelMap[ m_Data.m_ImgID ],
												posX, posY, posZ,
												m_Data.m_ScaleX, m_Data.m_ScaleY, m_Data.m_ScaleZ,
												m_Data.m_RotX, m_Data.m_RotY, m_Data.m_RotZ );
				MAPIL::BeginRendering2DGraphics();
			}
			else if( m_Data.m_DispMode == EnemyData::DISPLAY_MODE_3D_ANIM ){
				MAPIL::EndRendering2DGraphics();
				MAPIL::AddSkinMeshModelOn2DBatchWork(	m_Data.m_pResouceMap->m_pStageResourceMap->m_SkinModelMap[ m_Data.m_ImgID ],
														MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT,
														posX, posY, posZ,
														m_Data.m_ScaleX, m_Data.m_ScaleY, m_Data.m_ScaleZ,
														m_Data.m_RotX, m_Data.m_RotY, m_Data.m_RotZ,
														m_Data.m_Time );
				MAPIL::BeginRendering2DGraphics();
			}
		}
	}

	bool Enemy::Update()
	{
		if( m_Data.m_Paused ){
			return true;
		}

		if( m_VM.Terminated() ){
			return false;
		}
		m_VM.Run();

		// �ێ����Ă���A�G�̃V���b�g�O���[�v�̍X�V
		for( int i = 0; i < m_Data.m_ShotGroupList.size(); ++i ){
			if( m_Data.m_ShotGroupList[ i ] != NULL ){
				if( !m_Data.m_ShotGroupList[ i ]->Update() ){
					MAPIL::SafeDelete( m_Data.m_ShotGroupList[ i ] );
				}
			}
		}

		++m_Data.m_Counter;
		if( m_Data.m_IsConsSkillMode ){
			++m_PrivateData.m_ConsSkillEffectCounter;
			m_PrivateData.m_ConsSkillEffectPostCounter = 0;
		}
		else{
			m_PrivateData.m_ConsSkillEffectCounter = 0;
			++m_PrivateData.m_ConsSkillEffectPostCounter;
		}

		return true;
	}

	void Enemy::Colided( CollisionObject* pObject )
	{
		pObject->ProcessCollision( this );
	}

	void Enemy::ProcessCollision( Player* pPlayer )
	{
	}

	void Enemy::ProcessCollision( Enemy* pEnemy )
	{
	}

	void Enemy::ProcessCollision( PlayerShot* pPlayerShot )
	{
		// ��Փ˃��[�h
		if( m_Data.m_IsNonCollisionMode ){
			return;
		}

		// �����U�����󂯂����Ƃ�ʒm
		if( pPlayerShot->GetConsAttr() >= PLAYER_CONS_MODE_GREEN ){
			m_Data.m_StatusFlags.set( EnemyData::DAMAGED_BY_CONS_SHOT );
			m_Data.m_StatusFlags.set( EnemyData::DAMAGED_BY_CONS_SHOT_GREEN + pPlayerShot->GetConsAttr() - 1 );
		}
		else{
			m_Data.m_StatusFlags.reset( EnemyData::DAMAGED_BY_CONS_SHOT );
			for( int i = 0; i < PLAYER_CONS_MODE_RED; ++i ){
				m_Data.m_StatusFlags.reset( EnemyData::DAMAGED_BY_CONS_SHOT_GREEN + i );
			}
		}

		// ���j���͏I��
		if( m_Data.m_Destroyed ){
			return;
		}

		//-----------------------
		// �Փ˔���
		//-----------------------

		m_Data.m_pStageData->m_FrameGameData.m_Score += 10;
		// ���G��ԂłȂ���
		if( !m_Data.m_IsInvincibleMode ){

			int damage = pPlayerShot->GetShotPower();
			int scoreDelta = 0;

			// ��������́A�ӎ��Z�̑��� -> ��V���� �̏��ɔ��肪�s����

			// ����������ꍇ(�ӎ��Z�g�p��+���������ł͂Ȃ��ꍇ)
			if( m_Data.m_IsConsSkillMode && m_Data.m_ConsSkillAttr <= 3 ){
				// ��������v���Ă��鎞
				if( m_Data.m_ConsSkillAttr == pPlayerShot->GetConsAttr() ){
					scoreDelta = 0;		// �X�R�A�{�[�i�X�ύX�Ȃ�
				}
				// �����������Ă��鎞
				// (��->��, ��->��, ��->��)
				else if(	m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_GREEN && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_BLUE ||
							m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_BLUE && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_RED ||
							m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_RED && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_GREEN ){
					scoreDelta += 2 * pPlayerShot->GetShotPower();	// �X�R�A�{�[�i�X�����i�X�L������2�{�j
				}
				// �����������Ă��鎞
				// (��->��, ��->��, ��->��)
				else if(	m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_BLUE && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_GREEN ||
							m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_RED && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_BLUE ||
							m_Data.m_ConsSkillAttr == PLAYER_CONS_MODE_GREEN && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_RED ){
					scoreDelta -= pPlayerShot->GetShotPower();		// �X�R�A�{�[�i�X����
				}
			}
			// ��V����������ꍇ�i�ӎ��Z���g�p��+���������鎞�j
			else if( !m_Data.m_IsConsSkillMode && m_Data.m_ConsType > 0 ){
				// ��������v���Ă��鎞
				if( m_Data.m_ConsType == pPlayerShot->GetConsAttr() ){
					scoreDelta = 0;		// �X�R�A�{�[�i�X�ύX�Ȃ�
				}
				// �����������Ă��鎞
				// (��->��, ��->��, ��->��)
				else if(	m_Data.m_ConsType == PLAYER_CONS_MODE_GREEN && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_BLUE ||
							m_Data.m_ConsType == PLAYER_CONS_MODE_BLUE && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_RED ||
							m_Data.m_ConsType == PLAYER_CONS_MODE_RED && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_GREEN ){
					scoreDelta = -pPlayerShot->GetShotPower();	// �X�R�A�{�[�i�X����
				}
				// �����������Ă��鎞
				// (��->��, ��->��, ��->��)
				else if(	m_Data.m_ConsType == PLAYER_CONS_MODE_BLUE && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_GREEN ||
							m_Data.m_ConsType == PLAYER_CONS_MODE_RED && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_BLUE ||
							m_Data.m_ConsType == PLAYER_CONS_MODE_GREEN && pPlayerShot->GetConsAttr() == PLAYER_CONS_MODE_RED ){
					scoreDelta = pPlayerShot->GetShotPower();	// �X�R�A�{�[�i�X����
				}
				// ������������
				else if( pPlayerShot->GetConsAttr() == 0 ){
					scoreDelta = 0;							// �X�R�A�{�[�i�X�ύX�Ȃ�
				}
			}
			// �����������ꍇ
			else{
				scoreDelta = 0;
			}

			// �f�[�^�̍X�V
			m_Data.m_HP -= damage;
			m_Data.m_ScoreBonus += scoreDelta;

			if( m_Data.m_IsBoss ){
				StageMessage msg;
				msg.m_MsgID = StageMessage::STAGE_MESSAGE_ID_BOSS_DAMAGED;
				m_Data.m_pStageData->m_MsgQueue.push( msg );
			}
		}

		//------------------------------
		// Stage�ւ̃��b�Z�[�W���s�@�\
		//------------------------------

		// �Ō�̃_���[�W���󂯂����ɔ��s
		std::for_each(	m_Data.m_pStageData->m_ViewList.begin(), m_Data.m_pStageData->m_ViewList.end(),
						[this]( std::shared_ptr < StageView > view ){ view->OnEnemyDamaged( m_Data ); } );

		// ���j���̏���
		if( m_Data.m_HP <= 0 ){
			m_Data.m_HP = 0;
			// �X�R�A�{����10�{�������́i��Փx�{��*�~�σ|�C���g/�ő�HP�j
			int scoreFact = 10 + ( ( m_Data.m_pStageData->m_Difficulty + 1 ) * 10 * m_Data.m_ScoreBonus ) / m_Data.m_MaxHP;
			if( scoreFact > ( m_Data.m_pStageData->m_Difficulty + 1 ) * 10 ){
				scoreFact = ( m_Data.m_pStageData->m_Difficulty + 1 ) * 10;
			}
			if( scoreFact < 5 ){
				scoreFact = 5;
			}
			// �l���X�R�A
			int addScore = ( m_Data.m_Score * scoreFact ) / 10;

			m_Data.m_pStageData->m_FrameGameData.m_Score += addScore;
			
			// ���v���̍X�V
			auto it = m_Data.m_pStageData->m_StageStat.m_EnemyStat.find( m_Data.m_Name );
			if( it == m_Data.m_pStageData->m_StageStat.m_EnemyStat.end() ){
				EnemyStat stat;
				m_Data.m_pStageData->m_StageStat.m_EnemyStat[ m_Data.m_Name ] = stat;
			}
			++m_Data.m_pStageData->m_StageStat.m_EnemyStat[ m_Data.m_Name ].m_Destroy;
			// �������������̃Q�[�W�̃N���X�^�����擾
			if( m_Data.m_DamagedConsGauge >= 5 ){
				std::shared_ptr < Item > pNewItem;
				pNewItem.reset( m_Data.m_pStageData->m_ObjBuilder.CreateItem( ITEM_ID_CRYSTAL, m_Data.m_DamagedConsGauge / 5 ) );
				pNewItem->SetPos( m_Data.m_GUData.m_PosX + GameUnit( 70 ), m_Data.m_GUData.m_PosY );
				m_Data.m_pStageData->m_ItemList.push_back( pNewItem );
			}
			// ���j�����Z
			++m_Data.m_pStageData->m_FrameGameData.m_Killed;
			m_Data.m_Destroyed = true;

			// �r���[���X�g�֒ʒm
			std::for_each(	m_Data.m_pStageData->m_ViewList.begin(), m_Data.m_pStageData->m_ViewList.end(),
							[this]( std::shared_ptr < StageView > view ){ view->OnEnemyDestroyed( m_Data ); } );
		}
	}

	void Enemy::ProcessCollision( EnemyShot* pEnemyShot )
	{
	}

	void Enemy::ProcessCollision( Item* pItem )
	{
		if( m_Data.m_IsNonCollisionMode ){
			return;
		}
		
		if( !pItem->CanBeConsumed() ){
			return;
		}

		int id = pItem->GetItemID();
		int subID = pItem->GetItemSubID();

		if( id == ITEM_ID_CRYSTAL ){
			m_Data.m_ConsGauge += subID * 5;
			if( m_Data.m_ConsGauge > 200 ){
				m_Data.m_ConsGauge = 200;
			}
		}

		pItem->Consume();
	}

	void Enemy::Damage( int val )
	{
		m_Data.m_HP -= val;
	}

	void Enemy::GetPos( GameUnit* pPosX, GameUnit* pPosY )
	{
		*pPosX = m_Data.m_GUData.m_PosX;
		*pPosY = m_Data.m_GUData.m_PosY;
	}

	int Enemy::GetHP() const
	{
		return m_Data.m_HP;
	}

	int Enemy::GetMaxHP() const
	{
		return m_Data.m_MaxHP;
	}

	int Enemy::GetConsGauge() const
	{
		return m_Data.m_ConsGauge;
	}

	int Enemy::GetMaxConsGauge() const
	{
		return m_Data.m_MaxConsGauge;
	}

	GameUnit Enemy::GetCollisionRadius()
	{
		return m_Data.m_GUData.m_ColRadius;
	}

	void Enemy::Pause()
	{
		m_Data.m_Paused = true;
	}

	void Enemy::Resume()
	{
		m_Data.m_Paused = false;
	}

	void Enemy::ClearLastDamagedMsg()
	{
		m_SentLastDamagedMsg = false;
	}

	bool Enemy::IsNonCollisionMode() const
	{
		return m_Data.m_IsNonCollisionMode;
	}

	void Enemy::SetReg( int regNo, int val )
	{
		if( regNo < MAX_ENEMY_REGS && regNo >= 0 ){
			m_Data.m_Regs[ regNo ] = val;
		}
	}

	void Enemy::SendEvent( int id )
	{
		m_Data.m_EventQueue.push( id );
	}

	int Enemy::GetSubID() const
	{
		return m_Data.m_SubID;
	}

	std::string Enemy::GetName() const
	{
		return m_Data.m_Name;
	}

	bool Enemy::IsInSkillMode() const
	{
		return m_Data.m_IsConsSkillMode;
	}

	void Enemy::SetTime( double time )
	{
		m_Data.m_Time = time;
	}

	float Enemy::GetRotAngle( int axis ) const
	{
		switch( axis ){
			case 0:
				return m_Data.m_RotX;
			case 1:
				return m_Data.m_RotY;
			case 2:
				return m_Data.m_RotZ;
		}

		return 0.0f;
	}

	bool Enemy::m_SentLastDamagedMsg = false;
}
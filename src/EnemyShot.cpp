#include <MAPIL/MAPIL.h>

#include <bitset>

#include "EnemyShot.h"
#include "ResourceTypes.h"
#include "EnemyShotGroup.h"
#include "GameObjectImplBase.h"

#include "SpriteBatch.h"

namespace GameEngine
{

	struct ShotGroupData
	{
		EnemyShotGroup*						m_pShotGroup;		// �������Ă���V���b�g�O���[�v
		int									m_ID;				// �V���b�g�O���[�vID
	};

	typedef std::queue < EnemyShotMessage >			EnemyShotMessageQueue;


	class EnemyShot::Impl : public GameObjectImplBase
	{
	private:

		std::shared_ptr < ResourceMap >		m_pResourceMap;		// ���\�[�X�Ǘ��f�[�^
		ShotGroupData						m_ShotGroupData;	// �V���b�g�O���[�v�f�[�^

		// ��Ԋ֘A
		int									m_ShotID;					// �V���b�gID
		float								m_PosX;						// �ʒu�iX���W�j
		float								m_PosY;						// �ʒu�iY���W�j
		float								m_Angle;					// �p�x
		float								m_Speed;					// ���x
		float								m_ColRadius;				// �Փ˔���̔��a
		int									m_ImgID;					// �摜ID
		float								m_ImgScale;					// �摜�g�嗦
		int									m_Counter;					// �J�E���^
		int									m_DeadCounter;				// ���S�J�E���^
		int									m_Attr;						// ����
		int									m_Power;					// �U����

		// �t���O�Ǘ�
		enum StatusFlag
		{
			COLLIDED					= 0,	// �Փ˂������H
			DEAD						= 1,	// ����ł�����true
			HAS_CONS_ATTR				= 2,	// �ӎ��Z�p�̒e�̏ꍇtrue
			HAS_BLENDING_MODE_CHAGE		= 3,	// �A���t�@�u�����h�̕ω�������ꍇ�Atrue
			PAUSED						= 4,	// �ꎞ��~���̏ꍇtrue
			STATUS_FLAG_TOTAL,
		};
		std::bitset < STATUS_FLAG_TOTAL >	m_StatusFlags;		// ��ԊǗ��t���O

		// ���b�Z�[�W�֘A
		EnemyShotMessageQueue				m_MsgQueue;			// ���b�Z�[�W�L���[
		
	public:
		Impl( std::shared_ptr < ResourceMap > pMap, int id );
		~Impl();
		void Draw();										// �`��
		bool Update();										// �X�V
		void GetPos( float* pX, float* pY );
		int GetPower() const;								// �e�̍U���͂��擾
		void SetPos( float posX, float posY );
		void SetPower( int power );							// �e�̍U���͂�ݒ�
		void SetAngle( float angle );						// �p�x��ݒ�
		void SetSpeed( float speed );						// ���x��ݒ�
		void SetImage( int id );							// �摜��ݒ�
		void SetImageScale( float scale );					// �摜�̊g�嗦��ݒ�
		void SetCollisionRadius( float radius );			// �Փ˔���̔��a��ݒ�
		void SetConsAttr( int attr );						// �ӎ��Z��p�e�ɐݒ�
		void AddPos( float x, float y );					// �ʒu�����Z
		void AddAngle( float angle );						// �p�x�����Z
		void AddSpeed( float speed );						// ���x�����Z
		void JoinShotGroup( int id, EnemyShotGroup* pGroup );
		void ProcessCollision( Player* pPlayer );			// �Փˎ��̏����i�v���C���[�j
		float GetCollisionRadius() const;
		int GetCounter() const;
		void ProcessMessages();								// ���܂��Ă������b�Z�[�W�̏���
		void PostMessage( int msgID );						// ���b�Z�[�W�̒ǉ�
		void PrepDestroy();									// �폜�O����
		bool IsDead() const;
		int GetConsAttr() const;
		void Pause();										// �ꎞ��~
		void Resume();										// �ꎞ��~����ĊJ
	};

	EnemyShot::Impl::Impl( std::shared_ptr < ResourceMap > pMap, int id ) :	m_pResourceMap( pMap ),
																			m_ShotID( id )
	{
		m_Counter = 0;
		m_Attr = ENEMY_SHOT_ATTR_NORMAL;
		m_StatusFlags.reset();
		MAPIL::ZeroObject( &m_ShotGroupData, sizeof( m_ShotGroupData ) );
		m_Power = 1;
	}

	EnemyShot::Impl::~Impl()
	{
		if( m_ShotGroupData.m_pShotGroup ){
			m_ShotGroupData.m_pShotGroup->DeleteShot( m_ShotGroupData.m_ID );
		}
		MAPIL::ZeroObject( &m_ShotGroupData, sizeof( m_ShotGroupData ) );
	}

	void EnemyShot::Impl::Draw()
	{
		if( m_StatusFlags[ DEAD ] ){
			MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
								m_PosX, m_PosY,
								m_DeadCounter * 0.05f + 1.0f, m_DeadCounter * 0.05f + 1.0f,
								m_Angle + static_cast < float > ( MAPIL::DegToRad( 90.0f ) ),
								true, ( ( 20 - m_DeadCounter ) * 5 ) << 24 | 0xFFFFFF );
		}
		else{
			if( m_Counter >= 6 ){
				if( m_StatusFlags[ HAS_CONS_ATTR ] ){
					AddToSpriteBatch(	MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT,
										m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
										m_PosX, m_PosY, m_Angle + static_cast < float > ( MAPIL::DegToRad( 90.0f ) ) );
					if( ( m_Counter / 4 ) % 2 == 0  ){
						AddToSpriteBatch(	MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT,
										m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
										m_PosX, m_PosY, m_Angle + static_cast < float > ( MAPIL::DegToRad( 90.0f ) ) );
					}
				}
				else{
					MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
									m_PosX, m_PosY, m_Angle + static_cast < float > ( MAPIL::DegToRad( 90.0f ) ) );
				}
			}
			else{
				MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
									m_PosX, m_PosY,
									( 30 - m_Counter ) * 0.1f, ( 30 - m_Counter ) * 0.1f,
									m_Angle + static_cast < float > ( MAPIL::DegToRad( 90.0f ) ),
									true, ( m_Counter * 10 + 100 ) << 24 | 0xFFFFFF );
			}
		}
	}

	bool EnemyShot::Impl::Update()
	{
		if( m_StatusFlags[ PAUSED ] ){
			return true;
		}

		// ���b�Z�[�W����
		ProcessMessages();
		
		// ���S���菈��
		if( m_StatusFlags[ DEAD ] ){
			++m_DeadCounter;
			if( m_DeadCounter >= 20 ){
				return false;
			}
		}
		else{
			if( m_Counter < 20 ){
				m_Speed -= 0.1f;
			}

			m_PosX += m_Speed * ::cos( m_Angle );
			m_PosY -= m_Speed * ::sin( m_Angle );


			if( m_PosX < 0.0f || m_PosX > 640.0f || m_PosY < -30.0f || m_PosY > 500.0f ){
				return false;
			}
		}

		++m_Counter;

		return true;
	}

	inline void EnemyShot::Impl::GetPos( float* pX, float* pY )
	{
		*pX = m_PosX;
		*pY = m_PosY;
	}

	inline int EnemyShot::Impl::GetPower() const
	{
		return m_Power;
	}

	inline void EnemyShot::Impl::SetPos( float posX, float posY )
	{
		m_PosX = posX;
		m_PosY = posY;
	}

	inline void EnemyShot::Impl::SetPower( int power )
	{
		m_Power = power;
	}

	inline void EnemyShot::Impl::SetAngle( float angle )
	{
		m_Angle = angle;
	}

	inline void EnemyShot::Impl::SetSpeed( float speed )
	{
		if( m_Counter < 20 ){
			m_Speed = speed + 2.0f - m_Counter * 0.1f;
		}
		else{
			m_Speed = speed;
		}
	}

	inline void EnemyShot::Impl::SetImage( int id )
	{
		m_ImgID = id;
	}

	inline void EnemyShot::Impl::SetImageScale( float scale )
	{
		m_ImgScale = scale;
	}

	inline void EnemyShot::Impl::SetCollisionRadius( float radius )
	{
		m_ColRadius = radius;
	}

	inline void EnemyShot::Impl::SetConsAttr( int attr )
	{
		m_Attr = attr;
		m_StatusFlags.set( HAS_CONS_ATTR );
	}

	inline void EnemyShot::Impl::ProcessCollision( Player* pPlayer )
	{
		PrepDestroy();
	}

	inline float EnemyShot::Impl::GetCollisionRadius() const
	{
		return m_ColRadius;
	}

	inline void EnemyShot::Impl::JoinShotGroup( int id, EnemyShotGroup* pGroup )
	{
		m_ShotGroupData.m_ID = id;
		m_ShotGroupData.m_pShotGroup = pGroup;
	}

	inline int EnemyShot::Impl::GetCounter() const
	{
		return m_Counter;
	}

	inline void EnemyShot::Impl::AddPos( float x, float y )
	{
		m_PosX += x;
		m_PosY += y;
	}

	inline void EnemyShot::Impl::AddAngle( float angle )
	{
		m_Angle += angle;
	}

	inline void EnemyShot::Impl::AddSpeed( float speed )
	{
		m_Speed += speed;
	}
	
	void EnemyShot::Impl::ProcessMessages()
	{
		while( !m_MsgQueue.empty() ){
			int msg = m_MsgQueue.front().m_MsgID;
			switch( msg ){
				case EnemyShotMessage::ENEMY_SHOT_MESSAGE_ID_PLAYER_DAMAGED:
					PrepDestroy();
					break;
				case EnemyShotMessage::ENEMY_SHOT_MESSAGE_ID_PLAYER_BOMBED:
					PrepDestroy();
					break;
				default:
					break;
			}
			m_MsgQueue.pop();
		}
	}

	inline void EnemyShot::Impl::PostMessage( int msgID )
	{
		EnemyShotMessage msg;
		msg.m_MsgID = msgID;
		m_MsgQueue.push( msg );
	}

	void EnemyShot::Impl::PrepDestroy()
	{
		if( !m_StatusFlags[ DEAD ] ){
			m_DeadCounter = 0;
			m_StatusFlags.set( DEAD );
			//m_IsDead = true;
		}
	}

	inline bool EnemyShot::Impl::IsDead() const
	{
		return m_StatusFlags[ DEAD ];
	}

	inline int EnemyShot::Impl::GetConsAttr() const
	{
		return m_Attr;
	}

	inline void EnemyShot::Impl::Pause()
	{
		m_StatusFlags.set( PAUSED );
	}

	inline void EnemyShot::Impl::Resume()
	{
		m_StatusFlags.reset( PAUSED );
	}

	// ----------------------------------
	// �����N���X�̌Ăяo��
	// ----------------------------------

	EnemyShot::EnemyShot( std::shared_ptr < ResourceMap > pMap, int id ) :	CollisionObject(),
																			m_pImpl( new EnemyShot::Impl( pMap, id ) )
	{
	}

	EnemyShot::~EnemyShot()
	{
	}

	void EnemyShot::Init( float posX, float posY )
	{
	}

	void EnemyShot::Draw()
	{
		m_pImpl->Draw();
	}

	bool EnemyShot::Update()
	{
		return m_pImpl->Update();
	}

	void EnemyShot::SetPos( float posX, float posY )
	{
		m_pImpl->SetPos( posX, posY );
	}

	void EnemyShot::SetPower( int power )
	{
		m_pImpl->SetPower( power );
	}

	void EnemyShot::SetConsAttr( int attr )
	{
		m_pImpl->SetConsAttr( attr );
	}

	void EnemyShot::Colided( CollisionObject* pObject )
	{
		pObject->ProcessCollision( this );
	}

	void EnemyShot::ProcessCollision( Player* pPlayer )
	{
		m_pImpl->ProcessCollision( pPlayer );
	}

	void EnemyShot::ProcessCollision( Enemy* pEnemy )
	{
	}

	void EnemyShot::ProcessCollision( PlayerShot* pPlayerShot )
	{
	}

	void EnemyShot::ProcessCollision( EnemyShot* pEnemyShot )
	{
	}

	void EnemyShot::ProcessCollision( Item* pItem )
	{
	}

	void EnemyShot::GetPos( float* pPosX, float* pPosY )
	{
		m_pImpl->GetPos( pPosX, pPosY );
	}

	int EnemyShot::GetPower() const
	{
		return m_pImpl->GetPower();
	}

	float EnemyShot::GetCollisionRadius()
	{
		return m_pImpl->GetCollisionRadius();
	}

	void EnemyShot::SetAngle( float angle )
	{
		m_pImpl->SetAngle( angle );
	}

	void EnemyShot::SetSpeed( float speed )
	{
		m_pImpl->SetSpeed( speed );
	}

	void EnemyShot::SetImage( int id )
	{
		m_pImpl->SetImage( id );
	}

	void EnemyShot::SetImageScale( float scale )
	{
		m_pImpl->SetImageScale( scale );
	}

	void EnemyShot::SetCollisionRadius( float radius )
	{
		m_pImpl->SetCollisionRadius( radius );
	}

	void EnemyShot::JoinShotGroup( int id, EnemyShotGroup* pGroup )
	{
		m_pImpl->JoinShotGroup( id, pGroup );
	}

	int EnemyShot::GetCounter() const
	{
		return m_pImpl->GetCounter();
	}

	void EnemyShot::AddPos( float x, float y )
	{
		m_pImpl->AddPos( x, y );
	}

	void EnemyShot::AddAngle( float angle )
	{
		m_pImpl->AddAngle( angle );
	}

	void EnemyShot::AddSpeed( float speed )
	{
		m_pImpl->SetSpeed( speed );
	}

	void EnemyShot::PostMessage( int msgID )
	{
		m_pImpl->PostMessage( msgID );
	}

	bool EnemyShot::IsDead() const
	{
		return m_pImpl->IsDead();
	}

	int EnemyShot::GetConsAttr() const
	{
		return m_pImpl->GetConsAttr();
	}

	void EnemyShot::Pause()
	{
		m_pImpl->Pause();
	}

	void EnemyShot::Resume()
	{
		m_pImpl->Resume();
	}


}
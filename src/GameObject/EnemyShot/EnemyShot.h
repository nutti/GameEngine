#ifndef INCLUDED_GAMEENGINE_ENEMYSHOT_H
#define INCLUDED_GAMEENGINE_ENEMYSHOT_H

#include <MAPIL/MAPIL.h>

#include <bitset>
#include <memory>

#include "../../CollisionObject.h"

namespace GameEngine
{
	class EnemyShotGroup;
	struct ResourceMap;

	enum EnemyShotTextureColor
	{
		ENEMY_SHOT_TEX_COLOR_YELLOW		= 0,
		ENEMY_SHOT_TEX_COLOR_BLUE		= 1,
		ENEMY_SHOT_TEX_COLOR_RED		= 2,
		ENEMY_SHOT_TEX_COLOR_GREEN		= 3,
		ENEMY_SHOT_TEX_COLOR_PINK		= 4,
		ENEMY_SHOT_TEX_COLOR_AQUA		= 5,
		ENEMY_SHOT_TEX_COLOR_ORANGE		= 6,
		ENEMY_SHOT_TEX_COLOR_BLACK		= 7,
		ENEMY_SHOT_TEX_COLOR_TOTAL,
	};

	enum EnemyShotID
	{
		ENEMY_SHOT_ID_NONE					= 0,
		ENEMY_SHOT_ID_FOUR_RAYED_STAR_M		= 1,
		ENEMY_SHOT_ID_ELLIPSE_S				= 2,
		ENEMY_SHOT_ID_CRESCENT_M			= 3,
		ENEMY_SHOT_ID_GLORIOUS_CIRCLE_L		= 4,
		ENEMY_SHOT_ID_HEXAGRAM_M			= 5,
		ENEMY_SHOT_ID_CHEESE_M				= 6,
		ENEMY_SHOT_ID_GLORIOUS_CIRCLE_M		= 7,
		ENEMY_SHOT_ID_CIRCLE_M				= 8,
		ENEMY_SHOT_ID_NEEDLE_M				= 9,
		ENEMY_SHOT_ID_LASER_M				= 10,
		ENEMY_SHOT_ID_SMOKE_M				= 11,
		ENEMY_SHOT_ID_RING					= 12,
		ENEMY_SHOT_ID_BEAM_M				= 13,
		ENEMY_SHOT_ID_TOTAL,
	};

	enum DeleteBy
	{
		DELETE_BY_PLAYER_DAMAGE		= 0,
		DELETE_BY_PLAYER_SKILL		= 1,
		DELETE_BY_SHOT_CHANGED		= 2,
		DELETE_BY_TOTAL,
	};

	enum EnemyShotAttr
	{
		ENEMY_SHOT_ATTR_NORMAL	= 0,
		ENEMY_SHOT_ATTR_GREEN	= 1,
		ENEMY_SHOT_ATTR_BLUE	= 2,
		ENEMY_SHOT_ATTR_RED		= 3,
		ENEMY_SHOT_ATTR_TOTAL,
	};

	struct EnemyShotMessage
	{
		enum EnemyShotMessageID
		{
			ENEMY_SHOT_MESSAGE_ID_PLAYER_DAMAGED			= 0,		// �v���C���[���_���[�W���󂯂�
			ENEMY_SHOT_MESSAGE_ID_BOSS_MODE_STARTED			= 1,		// �{�X���[�h�ֈڍs����
			ENEMY_SHOT_MESSAGE_ID_BOSS_INVOKE_CONS_SKILL	= 2,		// �{�X���X�L�����g�p����
			ENEMY_SHOT_MESSAGE_ID_PLAYER_BOMBED				= 3,		// �v���C���[���{���𔭓�
		};

		int		m_MsgID;
	};

	
	struct ShotGroupData
	{
		EnemyShotGroup*						m_pShotGroup;		// �������Ă���V���b�g�O���[�v
		int									m_ID;				// �V���b�g�O���[�vID
	};

	typedef std::queue < EnemyShotMessage >			EnemyShotMessageQueue;


	class EnemyShot : public CollisionObject
	{
	protected:

		enum ImgRotMode
		{
			IMG_ROT_MODE_SYNC	= 0,			// �i�s�����Ɠ���
			IMG_ROT_MODE_AUTO	= 1,			// ���t���[���w�肵���p�x����������
			IMG_ROT_MODE_MANUAL	= 2,			// �����őS�Ďw��
			IMG_ROT_MODE_TOTAL,
		};


		// �Փ˔���
		enum ShotShape
		{
			SHOT_SHAPE_CIRCLE	= 0,		// �~
			SHOT_SHAPE_LINE		= 1,		// ����
			SHOT_SHAPE_TOTAL,
		};

		class Circle;
		class Line;
		class Shape
		{
		public:
			Shape(){}
			virtual ~Shape(){}
			virtual bool Colided( Shape* pObject ) = 0;				 // �Փ˔���̏��� �f�B�X�p�b�`��
			virtual bool JudgeCollision( Circle* pCircle ) = 0;		// �Փ˔���̏����i�~�j
			virtual bool JudgeCollision( Line* pLine ) = 0;			// �Փ˔���̏����i���j
		};

		class Circle : public Shape
		{
		private:
			float		m_CenterX;
			float		m_CenterY;
			float		m_Radius;
		public:
			Circle() : Shape(){}
			~Circle(){}
			bool Colided( Shape* pObject )
			{
				return pObject->JudgeCollision( this );
			}
			bool JudgeCollision( Circle* pCircle )
			{
				float x = pCircle->GetCenterX();
				float y = pCircle->GetCenterY();
				float radius = pCircle->GetRadius();

				float distance = ( x - m_CenterX ) * ( x - m_CenterX ) + ( y - m_CenterY ) * ( y - m_CenterY );
				float colRadius = ( radius + m_Radius ) * ( radius + m_Radius );
				if( distance < colRadius ){
					return true;
				}
				
				return false;
			}
			bool JudgeCollision( Line* pLine )
			{
				MAPIL::Vector2 < float > v1( m_CenterX - pLine->GetStartX(), m_CenterY - pLine->GetStartY() );
				MAPIL::Vector2 < float > v2( m_CenterX - pLine->GetEndX(), m_CenterY - pLine->GetEndY() );
				MAPIL::Vector2 < float > vL = v1 - v2;

				// �~�ƒ����̋���
				float d = std::abs( vL.GetOuterProduct( v1 ) ) / vL.GetNorm();
				if( d <= m_Radius + pLine->GetThickness() ){
					// �Փ�
					if( v1.GetInnerProduct( vL ) * v2.GetInnerProduct( vL ) <= 0 ){
						return true;
					}
					else{
						// ����ȏꍇ�̏Փ�
						if( m_Radius > v1.GetNorm() || m_Radius > v2.GetNorm() ){
							return true;
						}
					}
				}

				return false;
			}
			inline float GetCenterX() const
			{
				return m_CenterX;
			}
			inline float GetCenterY() const
			{
				return m_CenterY;
			}
			inline float GetRadius() const
			{
				return m_Radius;
			}
			inline void SetCenterX( float x )
			{
				m_CenterX = x;
			}
			inline void SetCenterY( float y )
			{
				m_CenterY = y;
			}
			inline void SetRadius( float radius )
			{
				m_Radius = radius;
			}
		};

		class Line : public Shape
		{
		private:
			float		m_StartX;
			float		m_StartY;
			float		m_EndX;
			float		m_EndY;
			float		m_Thickness;
		public:
			Line() : Shape(){}
			~Line(){}
			bool Colided( Shape* pObject )
			{
				return pObject->JudgeCollision( this );
			}
			bool JudgeCollision( Circle* pCircle )
			{
				MAPIL::Vector2 < float > v1( pCircle->GetCenterX() - m_StartX, pCircle->GetCenterY() - m_StartY );
				MAPIL::Vector2 < float > v2( pCircle->GetCenterX() - m_EndX, pCircle->GetCenterY() -  m_EndY );
				MAPIL::Vector2 < float > vL = v1 - v2;

				// �~�ƒ����̋���
				float d = std::abs( vL.GetOuterProduct( v1 ) ) / vL.GetNorm();
				if( d <= pCircle->GetRadius() ){
					// �Փ�
					if( v1.GetInnerProduct( vL ) * v2.GetInnerProduct( vL ) <= 0 ){
						return true;
					}
					else{
						// ����ȏꍇ�̏Փ�
						if( pCircle->GetRadius() > v1.GetNorm() || pCircle->GetRadius() > v2.GetNorm() ){
							return true;
						}
					}
				}

				return false;
			}
			bool JudgeCollision( Line* pLine )
			{
				return false;
			}
			inline float GetStartX() const
			{
				return m_StartX;
			}
			inline float GetStartY() const
			{
				return m_StartY;
			}
			inline float GetEndX() const
			{
				return m_EndX;
			}
			inline float GetEndY() const
			{
				return m_EndY;
			}
			inline float GetThickness() const
			{
				return m_Thickness;
			}
			inline void SetStartX( float x )
			{
				m_StartX = x;
			}
			inline void SetStartY( float y )
			{
				m_StartY = y;
			}
			inline void SetEndX( float x )
			{
				m_EndX = x;
			}
			inline void SetEndY( float y )
			{
				m_EndY = y;
			}
			inline void SetThickness( float thickness )
			{
				m_Thickness = thickness;
			}
		};

		std::shared_ptr < ResourceMap >		m_pResourceMap;		// ���\�[�X�Ǘ��f�[�^
		ShotGroupData						m_ShotGroupData;	// �V���b�g�O���[�v�f�[�^


		struct GameUnitData
		{
			GameUnit		m_PosX;
			GameUnit		m_PosY;
			GameUnit		m_Angle;
			GameUnit		m_Speed;
			GameUnit		m_ColRadius;

		};
		GameUnitData			m_GUData;


		// ��Ԋ֘A
		int									m_ShotID;					// �V���b�gID
		int									m_ImgID;					// �摜ID
		int									m_AtlasImgID;				// �A�g���XID(-1���W��)
		float								m_ImgScale;					// �摜�g�嗦
		int									m_Counter;					// �J�E���^
		int									m_DeadCounter;				// ���S�J�E���^
		int									m_Attr;						// ����
		int									m_Power;					// �U����
		int									m_ImgRotMode;				// �摜��]���[�h
		int									m_AlphaBlendingMode;		// ���u�����f�B���O���[�h
		float								m_ImgRotAnglePerFrame;		// ���t���[�����������]�p�x
		float								m_ImgRotAngle;				// �摜�̌���
		int									m_DrawingMultiplicity;		// �`�摽�d�x
		int									m_TexColor;					// �摜�̐F

		int				m_ShotShape;	// �V���b�g�̌`
		Circle			m_Circle;
		Line			m_Line;

		// �t���O�Ǘ�
		enum StatusFlag
		{
			COLLIDED						= 0,	// �Փ˂������H
			DEAD							= 1,	// ����ł�����true
			HAS_CONS_ATTR					= 2,	// �ӎ��Z�p�̒e�̏ꍇtrue
			HAS_BLENDING_MODE_CHAGE			= 3,	// �A���t�@�u�����h�̕ω�������ꍇ�Atrue
			PAUSED							= 4,	// �ꎞ��~���̏ꍇtrue
			INVINCIBLE						= 5,	// �{����v���C���[�_���[�W�ŏ����Ȃ��ꍇ�Atrue
			IMG_SCALE_CHANGED				= 6,	// �摜�̊g�嗦��ύX�����ꍇ�Atrue
			INVISIBLE						= 7,	// �����郂�[�h�̏ꍇ�Atrue
			NOT_DELETE_BY_PLAYER_DAMAGE		= 8,	// �v���C���[�_���[�W���ɏ����Ȃ��ꍇ�Atrue
			NOT_DELETE_BY_PLAYER_SKILL		= 9,	// �v���C���[�̃X�L���ɂ���ď����Ȃ��ꍇ�Atrue
			STATUS_FLAG_TOTAL,
		};
		std::bitset < STATUS_FLAG_TOTAL >	m_StatusFlags;		// ��ԊǗ��t���O

		// ���b�Z�[�W�֘A
		EnemyShotMessageQueue				m_MsgQueue;			// ���b�Z�[�W�L���[

		
		
	public:
		EnemyShot( std::shared_ptr < ResourceMap > pMap, int id );
		virtual ~EnemyShot();
		virtual void Draw();										// �`��
		virtual bool Update();										// �X�V
		virtual void Init( const GameUnit& posX, const GameUnit& posY );

		int GetPower() const;								// �e�̍U���͂��擾
		void SetPower( int power );							// �e�̍U���͂�ݒ�
		void SetImage( int id );							// �摜��ݒ�
		void SetAtlasImage( int id );
		void SetImageScale( float scale );					// �摜�̊g�嗦��ݒ�
		void SetConsAttr( int attr );						// �ӎ��Z��p�e�ɐݒ�
		void JoinShotGroup( int id, EnemyShotGroup* pGroup );
		void LeaveFromShotGroup();

		virtual void Colided( CollisionObject* pObject );			// �Փˎ��̏��� �f�B�X�p�b�`��
		void ProcessCollision( Player* pPlayer );			// �Փˎ��̏����i�v���C���[�j
		void ProcessCollision( Enemy* pEnemy );				// �Փˎ��̏����i�G�j
		void ProcessCollision( PlayerShot* pPlayerShot );	// �Փˎ��̏����i�v���C���[�V���b�g�j
		void ProcessCollision( EnemyShot* pEnemyShot );		// �Փˎ��̏����i�G�e�j
		void ProcessCollision( Item* pItem );				// �Փˎ��̏����i�A�C�e���j

		virtual void SetPos( const GameUnit& posX, const GameUnit& posY );
		virtual void SetLinePos(	const GameUnit& x1,
									const GameUnit& y1,
									const GameUnit& x2,
									const GameUnit& y2,
									const GameUnit& thickness );		// ���̒l��ݒ�
		virtual void GetPos( GameUnit* pX, GameUnit* pY );
		void SetAngle( const GameUnit& angle );						// �p�x��ݒ�
		void SetSpeed( const GameUnit& speed );						// ���x��ݒ�
		void SetCollisionRadius( const GameUnit& radius );			// �Փ˔���̔��a��ݒ�
		virtual void AddPos( const GameUnit& x, const GameUnit& y );					// �ʒu�����Z
		void AddAngle( const GameUnit& angle );						// �p�x�����Z
		void AddSpeed( const GameUnit& speed );						// ���x�����Z
		virtual bool DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius );
		GameUnit GetCollisionRadius();
		GameUnit GetAngle() const;
		GameUnit GetSpeed() const;
		int GetCounter() const;
		void ProcessMessages();								// ���܂��Ă������b�Z�[�W�̏���
		void PostMessage( int msgID );						// ���b�Z�[�W�̒ǉ�
		void PrepDestroy();									// �폜�O����
		bool IsDead() const;
		int GetConsAttr() const;
		void Pause();										// �ꎞ��~
		void Resume();										// �ꎞ��~����ĊJ
		void SetImgRotMode( int mode );						// �G�e�̉摜��]���[�h��ݒ�
		void SetImgRotAnglePerFrame( float angle );			// ���t���[�����������]�p�x��ݒ�
		void SetAlphaBlendingMode( int mode );				// ���u�����f�B���O�̕��@��ݒ�
		bool DoesColideWithPlayer( float x, float y, float radius );	// �v���C���[�Ƃ̏Փ˔���
		void SetShape( int shape );							// �Փ˔���̌`��ݒ�
		void EnableInvincibleMode();				// �����Ȃ����[�h�ֈڍs
		void DisableInvincibleMode();			// �����郂�[�h�ֈڍs
		void EnableInvisibleMode();				// �����Ȃ����[�h�ֈڍs
		void DisableInvisibleMode();			// �����郂�[�h�ֈڍs
		std::string GetMasterEnemyName() const;
		void SetDrawingMultiplicity( int num );
		void Delete( int by );
		void DeleteWhen( int when );
		void NotDeleteWhen( int when );		// �폜�^�C�~���O�̐ݒ�
		virtual void SetLength( const GameUnit& length );
		virtual void SetTextureColor( int color );
	};

}

#endif
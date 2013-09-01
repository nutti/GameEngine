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

#if defined ( USE_FLOATING_POINT )
		float								m_PosX;						// �ʒu�iX���W�j
		float								m_PosY;						// �ʒu�iY���W�j
		float								m_Angle;					// �p�x
		float								m_Speed;					// ���x
		float								m_ColRadius;				// �Փ˔���̔��a
#elif defined ( USE_GAME_UNIT )
		struct GameUnitData
		{
			GameUnit		m_PosX;
			GameUnit		m_PosY;
			GameUnit		m_Angle;
			GameUnit		m_Speed;
			GameUnit		m_ColRadius;

		};
		GameUnitData			m_GUData;
#endif


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

		int				m_ShotShape;	// �V���b�g�̌`
		Circle			m_Circle;
		Line			m_Line;

		// �t���O�Ǘ�
		enum StatusFlag
		{
			COLLIDED					= 0,	// �Փ˂������H
			DEAD						= 1,	// ����ł�����true
			HAS_CONS_ATTR				= 2,	// �ӎ��Z�p�̒e�̏ꍇtrue
			HAS_BLENDING_MODE_CHAGE		= 3,	// �A���t�@�u�����h�̕ω�������ꍇ�Atrue
			PAUSED						= 4,	// �ꎞ��~���̏ꍇtrue
			INVINCIBLE					= 5,	// �{����v���C���[�_���[�W�ŏ����Ȃ��ꍇ�Atrue
			IMG_SCALE_CHANGED			= 6,	// �摜�̊g�嗦��ύX�����ꍇ�Atrue
			INVISIBLE					= 7,	// �����郂�[�h�̏ꍇ�Atrue
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
		int GetPower() const;								// �e�̍U���͂��擾
		void SetPower( int power );							// �e�̍U���͂�ݒ�
		void SetImage( int id );							// �摜��ݒ�
		void SetAtlasImage( int id );
		void SetImageScale( float scale );					// �摜�̊g�嗦��ݒ�
		void SetConsAttr( int attr );						// �ӎ��Z��p�e�ɐݒ�
		void JoinShotGroup( int id, EnemyShotGroup* pGroup );
		void ProcessCollision( Player* pPlayer );			// �Փˎ��̏����i�v���C���[�j
#if defined ( USE_FLOATING_POINT )
		void SetPos( float posX, float posY );
		void SetLinePos( float x1, float y1, float x2, float y2, float thickness );		// ���̒l��ݒ�
		void GetPos( float* pX, float* pY );
		void SetAngle( float angle );						// �p�x��ݒ�
		void SetSpeed( float speed );						// ���x��ݒ�
		void SetCollisionRadius( float radius );			// �Փ˔���̔��a��ݒ�
		void AddPos( float x, float y );					// �ʒu�����Z
		void AddAngle( float angle );						// �p�x�����Z
		void AddSpeed( float speed );						// ���x�����Z
		bool DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius );
		float GetCollisionRadius() const;
#elif defined ( USE_GAME_UNIT )
		void SetPos( const GameUnit& posX, const GameUnit& posY );
		void SetLinePos(	const GameUnit& x1,
							const GameUnit& y1,
							const GameUnit& x2,
							const GameUnit& y2,
							const GameUnit& thickness );		// ���̒l��ݒ�
		void GetPos( GameUnit* pX, GameUnit* pY );
		void SetAngle( const GameUnit& angle );						// �p�x��ݒ�
		void SetSpeed( const GameUnit& speed );						// ���x��ݒ�
		void SetCollisionRadius( const GameUnit& radius );			// �Փ˔���̔��a��ݒ�
		void AddPos( const GameUnit& x, const GameUnit& y );					// �ʒu�����Z
		void AddAngle( const GameUnit& angle );						// �p�x�����Z
		void AddSpeed( const GameUnit& speed );						// ���x�����Z
		bool DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius );
		GameUnit GetCollisionRadius() const;
		GameUnit GetAngle() const;
		GameUnit GetSpeed() const;
#endif
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
	};

	EnemyShot::Impl::Impl( std::shared_ptr < ResourceMap > pMap, int id ) :	m_pResourceMap( pMap ),
																			m_ShotID( id ),

																			m_ShotShape( SHOT_SHAPE_CIRCLE ),
																			m_Circle(),
																			m_Line()
	{
		m_Counter = 0;
		m_Attr = ENEMY_SHOT_ATTR_NORMAL;
		m_StatusFlags.reset();
		MAPIL::ZeroObject( &m_ShotGroupData, sizeof( m_ShotGroupData ) );
		m_Power = 1;
		m_AlphaBlendingMode = MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT;
		m_ImgRotAnglePerFrame = 0.0f;
		m_ImgRotMode = IMG_ROT_MODE_SYNC;
		m_ImgRotAngle = 0.0f;
		m_ImgRotAnglePerFrame = 0.0f;
		m_ImgScale = 1.0f;
		m_AtlasImgID = -1;
	}

	EnemyShot::Impl::~Impl()
	{
		if( m_ShotGroupData.m_pShotGroup ){
			m_ShotGroupData.m_pShotGroup->DeleteShot( m_ShotGroupData.m_ID );
		}
		MAPIL::ZeroObject( &m_ShotGroupData, sizeof( m_ShotGroupData ) );
	}

#if defined ( USE_FLOATING_POINT )

	void EnemyShot::Impl::Draw()
	{
		if( m_StatusFlags[ INVISIBLE ] ){
			return;
		}

		if( m_StatusFlags[ DEAD ] ){
			MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
								m_PosX, m_PosY,
								m_DeadCounter * 0.05f + m_ImgScale, m_DeadCounter * 0.05f + m_ImgScale,
								m_ImgRotAngle,
								true, ( ( 20 - m_DeadCounter ) * 5 ) << 24 | 0xFFFFFF );
		}
		else{
			//if( m_Counter >= 6 ){
			if( m_ShotShape == SHOT_SHAPE_CIRCLE ){
				if( m_StatusFlags[ HAS_CONS_ATTR ] ){
					AddToSpriteBatch(	MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT,
										m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
										m_PosX, m_PosY, m_ImgRotAngle );
					if( ( m_Counter / 4 ) % 2 == 0 ){
						AddToSpriteBatch(	MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT,
										m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
										m_PosX, m_PosY, m_ImgRotAngle );
					}
				}
				else if( m_AlphaBlendingMode != MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT ){
					if( m_StatusFlags[ IMG_SCALE_CHANGED ] ){
						AddToSpriteBatch(	m_AlphaBlendingMode,
											m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
											m_PosX, m_PosY, m_ImgScale, m_ImgScale, m_ImgRotAngle );
					}
					else{
						AddToSpriteBatch(	m_AlphaBlendingMode,
											m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
											m_PosX, m_PosY, m_ImgRotAngle );
					}
				}
				else{
					if( m_StatusFlags[ IMG_SCALE_CHANGED ] ){
						MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
											m_PosX, m_PosY, m_ImgScale, m_ImgScale, m_ImgRotAngle );
					}
					else{
						MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
											m_PosX, m_PosY, m_ImgRotAngle );
					}
				}
			}
			else if( m_ShotShape == SHOT_SHAPE_LINE ){
				float angle = ::atan2( m_Line.GetEndY() - m_Line.GetStartY(), -m_Line.GetEndX() + m_Line.GetStartX() ) - MAPIL::DegToRad( 90.0f );
				float length = std::sqrt(	( m_Line.GetEndX() - m_Line.GetStartX() ) * ( m_Line.GetEndX() - m_Line.GetStartX() ) + 
											( m_Line.GetEndY() - m_Line.GetStartY() ) * ( m_Line.GetEndY() - m_Line.GetStartY() ) );
				unsigned int texSizeX;
				unsigned int texSizeY;
				MAPIL::GetTextureSize( m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ], &texSizeX, &texSizeY );
				MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
									m_Line.GetStartX() - texSizeX / 2,
									m_Line.GetStartY(),
									1.0f,
									length / texSizeY,
									angle,
									false );
			}
		}
	}

#elif defined ( USE_GAME_UNIT )
	void EnemyShot::Impl::Draw()
	{
		if( m_StatusFlags[ INVISIBLE ] ){
			return;
		}

		float posX = m_GUData.m_PosX.GetFloat();
		float posY = m_GUData.m_PosY.GetFloat();

		if( m_StatusFlags[ DEAD ] ){
			if( m_AtlasImgID == -1 ){
				MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
									posX, posY,
									m_DeadCounter * 0.05f + m_ImgScale, m_DeadCounter * 0.05f + m_ImgScale,
									m_ImgRotAngle,
									true, ( ( 20 - m_DeadCounter ) * 5 ) << 24 | 0xFFFFFF );
			}
			else{
				ResourceMap::TextureAtlas atlas;
				atlas = m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ m_AtlasImgID ];
				MAPIL::DrawClipedTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ atlas.m_TexID ],
											posX, posY, m_DeadCounter * 0.05f + m_ImgScale, m_DeadCounter * 0.05f + m_ImgScale, m_ImgRotAngle,
											atlas.m_X, atlas.m_Y, atlas.m_X + atlas.m_Width, atlas.m_Y + atlas.m_Height, true,
											( ( 20 - m_DeadCounter ) * 5 ) << 24 | 0xFFFFFF );
			}
		}
		else{
			if( m_ShotShape == SHOT_SHAPE_CIRCLE ){
				if( m_StatusFlags[ HAS_CONS_ATTR ] ){
					if( m_AtlasImgID == -1 ){
						AddToSpriteBatch(	MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT,
											m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
											posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle );
						if( ( m_Counter / 4 ) % 2 == 0 ){
							AddToSpriteBatch(	MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT,
												m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
												posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle );
						}
					}
					else{
						AddToAtlasSpriteBatch(	MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT,
												m_AtlasImgID,
												posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle );
						if( ( m_Counter / 4 ) % 2 == 0 ){
							AddToAtlasSpriteBatch(	MAPIL::ALPHA_BLEND_MODE_ADD_SEMI_TRANSPARENT,
													m_AtlasImgID,
													posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle );
						}
					}
				}
				else if( m_AlphaBlendingMode != MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT ){
					if( m_AtlasImgID == -1 ){
						AddToSpriteBatch(	m_AlphaBlendingMode,
											m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
											posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle );
					}
					else{
						AddToAtlasSpriteBatch(	m_AlphaBlendingMode,
												m_AtlasImgID,
												posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle );
					}
				}
				else{
					if( m_AtlasImgID == -1 ){
						MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
											posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle );
					}
					else{
						ResourceMap::TextureAtlas atlas;
						atlas = m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ m_AtlasImgID ];
						MAPIL::DrawClipedTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ atlas.m_TexID ],
													posX, posY, m_ImgScale, m_ImgScale, m_ImgRotAngle,
													atlas.m_X, atlas.m_Y, atlas.m_X + atlas.m_Width, atlas.m_Y + atlas.m_Height );
					}
				}
			}
			else if( m_ShotShape == SHOT_SHAPE_LINE ){
				float angle = ::atan2( m_Line.GetEndY() - m_Line.GetStartY(), -m_Line.GetEndX() + m_Line.GetStartX() ) - MAPIL::DegToRad( 90.0f );
				float length = std::sqrt(	( m_Line.GetEndX() - m_Line.GetStartX() ) * ( m_Line.GetEndX() - m_Line.GetStartX() ) + 
											( m_Line.GetEndY() - m_Line.GetStartY() ) * ( m_Line.GetEndY() - m_Line.GetStartY() ) );
				unsigned int texSizeX;
				unsigned int texSizeY;
				if( m_AtlasImgID == -1 ){
					MAPIL::GetTextureSize( m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ], &texSizeX, &texSizeY );
					MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
										m_Line.GetStartX() - ( texSizeX / 2.0f ) * cos( angle ),
										m_Line.GetStartY() + ( texSizeX / 2.0f ) * sin( angle ),
										1.0f,
										length / texSizeY,
										angle,
										false );
				}
				else{
					ResourceMap::TextureAtlas atlas;
					atlas = m_pResourceMap->m_pStageResourceMap->m_TexAtlasMap[ m_AtlasImgID ];
					texSizeX = atlas.m_Width;
					texSizeY = atlas.m_Height;
					MAPIL::DrawClipedTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ atlas.m_TexID ],
												m_Line.GetStartX() - ( texSizeX / 2.0f ) * cos( angle ),
												m_Line.GetStartY() + ( texSizeX / 2.0f ) * sin( angle ),
												1.0f,
												length / texSizeY,
												angle,
												atlas.m_X, atlas.m_Y, atlas.m_X + atlas.m_Width, atlas.m_Y + atlas.m_Height,
												false );
				}
			}
		}
	}
#endif

#if defined ( USE_FLOATING_POINT )
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

			if( m_ShotShape == SHOT_SHAPE_CIRCLE ){
				m_PosX += m_Speed * ::cos( m_Angle );
				m_PosY -= m_Speed * ::sin( m_Angle );
				m_Circle.SetCenterX( m_PosX );
				m_Circle.SetCenterY( m_PosY );
			}
			else if( m_ShotShape == SHOT_SHAPE_LINE ){
				m_PosX = m_Line.GetStartX();
				m_PosY = m_Line.GetStartY();
			}


			if( m_PosX < 0.0f || m_PosX > 640.0f || m_PosY < -30.0f || m_PosY > 500.0f ){
				return false;
			}
		}

		if( m_ImgRotMode == IMG_ROT_MODE_SYNC ){
			m_ImgRotAngle = m_Angle + static_cast < float > ( MAPIL::DegToRad( 90.0f ) );
		}
		else if( m_ImgRotMode == IMG_ROT_MODE_AUTO ){
			m_ImgRotAngle += m_ImgRotAnglePerFrame;
		}

		++m_Counter;

		return true;
	}

#elif defined ( USE_GAME_UNIT )
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

			if( m_ShotShape == SHOT_SHAPE_CIRCLE ){
				m_GUData.m_PosX += m_GUData.m_Speed * CosGU( m_GUData.m_Angle );
				m_GUData.m_PosY -= m_GUData.m_Speed * SinGU( m_GUData.m_Angle );
				m_Circle.SetCenterX( m_GUData.m_PosX.GetFloat() );
				m_Circle.SetCenterY( m_GUData.m_PosY.GetFloat() );
			}
			else if( m_ShotShape == SHOT_SHAPE_LINE ){
				m_GUData.m_PosX = m_Line.GetStartX();
				m_GUData.m_PosY = m_Line.GetStartY();
			}

			if(	m_GUData.m_PosX < GameUnit( 0 ) ||
				m_GUData.m_PosX > GameUnit( 640 ) ||
				m_GUData.m_PosY < GameUnit( -30 ) ||
				m_GUData.m_PosY > GameUnit( 500 ) ){
				return false;
			}
		}

		if( m_ImgRotMode == IMG_ROT_MODE_SYNC ){
			m_ImgRotAngle = MAPIL::DegToRad( m_GUData.m_Angle.GetFloat() + 90.0f );
		}
		else if( m_ImgRotMode == IMG_ROT_MODE_AUTO ){
			m_ImgRotAngle += m_ImgRotAnglePerFrame;
		}

		++m_Counter;

		return true;
	}
#endif
	

	inline int EnemyShot::Impl::GetPower() const
	{
		return m_Power;
	}

	

	inline void EnemyShot::Impl::SetPower( int power )
	{
		m_Power = power;
	}

	

	inline void EnemyShot::Impl::SetImage( int id )
	{
		m_ImgID = id;
	}

	inline void EnemyShot::Impl::SetAtlasImage( int id )
	{
		m_AtlasImgID = id;
	}

	inline void EnemyShot::Impl::SetImageScale( float scale )
	{
		m_ImgScale = scale;
		m_StatusFlags.set( IMG_SCALE_CHANGED );
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

	

	inline void EnemyShot::Impl::JoinShotGroup( int id, EnemyShotGroup* pGroup )
	{
		m_ShotGroupData.m_ID = id;
		m_ShotGroupData.m_pShotGroup = pGroup;
	}

	inline int EnemyShot::Impl::GetCounter() const
	{
		return m_Counter;
	}

#if defined ( USE_FLOATING_POINT )

	inline void EnemyShot::Impl::GetPos( float* pX, float* pY )
	{
		*pX = m_PosX;
		*pY = m_PosY;
	}

	inline void EnemyShot::Impl::SetPos( float posX, float posY )
	{
		m_PosX = posX;
		m_PosY = posY;
	}

	inline void EnemyShot::Impl::SetAngle( float angle )
	{
		m_Angle = angle;
	}

	inline void EnemyShot::Impl::SetSpeed( float speed )
	{
		m_Speed = speed;
	}

	inline void EnemyShot::Impl::SetCollisionRadius( float radius )
	{
		m_ColRadius = radius;
		if( m_ShotShape == SHOT_SHAPE_CIRCLE ){
			m_Circle.SetRadius( radius );
		}
	}

	inline float EnemyShot::Impl::GetCollisionRadius() const
	{
		return m_ColRadius;
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

	inline bool EnemyShot::Impl::DoesColideWithPlayer( float x, float y, float radius )
	{
		Circle c;
		c.SetCenterX( x );
		c.SetCenterY( y );
		c.SetRadius( radius );

		if( m_ShotShape == SHOT_SHAPE_CIRCLE ){
			return m_Circle.Colided( &c );
		}
		else if( m_ShotShape == SHOT_SHAPE_LINE ){
			return m_Line.Colided( &c );
		}

		return false;
	}

#elif defined ( USE_GAME_UNIT )
	inline void EnemyShot::Impl::GetPos( GameUnit* pX, GameUnit* pY )
	{
		*pX = m_GUData.m_PosX;
		*pY = m_GUData.m_PosY;
	}

	inline GameUnit EnemyShot::Impl::GetAngle() const
	{
		return m_GUData.m_Angle;
	}

	inline void EnemyShot::Impl::SetPos( const GameUnit& posX, const GameUnit& posY )
	{
		m_GUData.m_PosX = posX;
		m_GUData.m_PosY = posY;
	}

	inline void EnemyShot::Impl::SetAngle( const GameUnit& angle )
	{
		m_GUData.m_Angle = angle;
	}

	inline void EnemyShot::Impl::SetSpeed( const GameUnit& speed )
	{
		m_GUData.m_Speed = speed;
	}

	inline void EnemyShot::Impl::SetCollisionRadius( const GameUnit& radius )
	{
		m_GUData.m_ColRadius = radius;
		if( m_ShotShape == SHOT_SHAPE_CIRCLE ){
			m_Circle.SetRadius( radius.GetFloat() );
		}
	}

	inline GameUnit EnemyShot::Impl::GetCollisionRadius() const
	{
		return m_GUData.m_ColRadius;
	}

	inline void EnemyShot::Impl::AddPos( const GameUnit& x, const GameUnit& y )
	{
		m_GUData.m_PosX += x;
		m_GUData.m_PosY += y;
	}

	inline void EnemyShot::Impl::AddAngle( const GameUnit& angle )
	{
		m_GUData.m_Angle += angle;
	}

	inline void EnemyShot::Impl::AddSpeed( const GameUnit& speed )
	{
		m_GUData.m_Speed += speed;
	}

	inline bool EnemyShot::Impl::DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius )
	{
		Circle c;
		c.SetCenterX( x.GetFloat() );
		c.SetCenterY( y.GetFloat() );
		c.SetRadius( radius.GetFloat() );

		if( m_ShotShape == SHOT_SHAPE_CIRCLE ){
			return m_Circle.Colided( &c );
		}
		else if( m_ShotShape == SHOT_SHAPE_LINE ){
			return m_Line.Colided( &c );
		}

		return false;
	}
#endif
	
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

	inline void EnemyShot::Impl::SetImgRotMode( int mode )
	{
		m_ImgRotMode = mode;
	}

	inline void EnemyShot::Impl::SetImgRotAnglePerFrame( float angle )
	{
		m_ImgRotAnglePerFrame = angle;
	}

	inline void EnemyShot::Impl::SetAlphaBlendingMode( int mode )
	{
		m_AlphaBlendingMode = mode;
	}

	void EnemyShot::Impl::SetShape( int shape )
	{
		m_ShotShape = shape;
	}

#if defined ( USE_FLOATING_POINT )
	void EnemyShot::Impl::SetLinePos( float x1, float y1, float x2, float y2, float thickness )
	{
		m_Line.SetStartX( x1 );
		m_Line.SetStartY( y1 );
		m_Line.SetEndX( x2 );
		m_Line.SetEndY( y2 );
		m_Line.SetThickness( thickness );
	}
#elif defined ( USE_GAME_UNIT )
	void EnemyShot::Impl::SetLinePos(	const GameUnit& x1,
										const GameUnit& y1,
										const GameUnit& x2,
										const GameUnit& y2,
										const GameUnit& thickness )
	{
		m_Line.SetStartX( x1.GetFloat() );
		m_Line.SetStartY( y1.GetFloat() );
		m_Line.SetEndX( x2.GetFloat() );
		m_Line.SetEndY( y2.GetFloat() );
		m_Line.SetThickness( thickness.GetFloat() );
	}
#endif

	inline void EnemyShot::Impl::EnableInvincibleMode()
	{
		m_StatusFlags.set( INVINCIBLE );
	}

	inline void EnemyShot::Impl::DisableInvincibleMode()
	{

		m_StatusFlags.reset( INVINCIBLE );
	}

	inline void EnemyShot::Impl::EnableInvisibleMode()
	{
		m_StatusFlags.set( INVISIBLE );
	}

	inline void EnemyShot::Impl::DisableInvisibleMode()
	{
		m_StatusFlags.reset( INVISIBLE );
	}

	inline GameUnit EnemyShot::Impl::GetSpeed() const
	{
		return m_GUData.m_Speed;
	}

	inline std::string EnemyShot::Impl::GetMasterEnemyName() const
	{
		if( m_ShotGroupData.m_pShotGroup != NULL ){
			return m_ShotGroupData.m_pShotGroup->GetMasterEnemyName();
		}
		else{
			MAPIL::Assert( 0, CURRENT_POSITION, TSTR( "shot group is null pointer!!" ), -1 );
			return "";
		}
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

	

	void EnemyShot::Draw()
	{
		m_pImpl->Draw();
	}

	bool EnemyShot::Update()
	{
		return m_pImpl->Update();
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

	

	int EnemyShot::GetPower() const
	{
		return m_pImpl->GetPower();
	}

	

	void EnemyShot::SetImage( int id )
	{
		m_pImpl->SetImage( id );
	}

	void EnemyShot::SetAtlasImage( int id )
	{
		m_pImpl->SetAtlasImage( id );
	}

	void EnemyShot::SetImageScale( float scale )
	{
		m_pImpl->SetImageScale( scale );
	}

	

	void EnemyShot::JoinShotGroup( int id, EnemyShotGroup* pGroup )
	{
		m_pImpl->JoinShotGroup( id, pGroup );
	}

	int EnemyShot::GetCounter() const
	{
		return m_pImpl->GetCounter();
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

	void EnemyShot::SetImgRotMode( int mode )
	{
		m_pImpl->SetImgRotMode( mode );
	}

	void EnemyShot::SetImgRotAnglePerFrame( float angle )
	{
		m_pImpl->SetImgRotAnglePerFrame( angle );
	}
	
	void EnemyShot::SetAlphaBlendingMode( int mode )
	{
		m_pImpl->SetAlphaBlendingMode( mode );
	}

#if defined ( USE_FLOATING_POINT )

	void EnemyShot::Init( float posX, float posY )
	{
	}

	void EnemyShot::SetPos( float posX, float posY )
	{
		m_pImpl->SetPos( posX, posY );
	}

	void EnemyShot::SetLinePos( float x1, float y1, float x2, float y2, float thickness )
	{
		m_pImpl->SetLinePos( x1, y1, x2, y2, thickness );
	}

	void EnemyShot::GetPos( float* pPosX, float* pPosY )
	{
		m_pImpl->GetPos( pPosX, pPosY );
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

	void EnemyShot::SetCollisionRadius( float radius )
	{
		m_pImpl->SetCollisionRadius( radius );
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

	bool EnemyShot::DoesColideWithPlayer( float x, float y, float radius )
	{
		return m_pImpl->DoesColideWithPlayer( x, y, radius );
	}

#elif defined ( USE_GAME_UNIT )
	void EnemyShot::Init( const GameUnit& posX, const GameUnit& posY )
	{
	}

	void EnemyShot::SetPos( const GameUnit& posX, const GameUnit& posY )
	{
		m_pImpl->SetPos( posX, posY );
	}

	void EnemyShot::SetLinePos(	const GameUnit& x1,
								const GameUnit& y1,
								const GameUnit& x2,
								const GameUnit& y2,
								const GameUnit& thickness )
	{
		m_pImpl->SetLinePos( x1, y1, x2, y2, thickness );
	}

	void EnemyShot::GetPos( GameUnit* pPosX, GameUnit* pPosY )
	{
		m_pImpl->GetPos( pPosX, pPosY );
	}

	GameUnit EnemyShot::GetCollisionRadius()
	{
		return m_pImpl->GetCollisionRadius();
	}

	void EnemyShot::SetAngle( const GameUnit& angle )
	{
		m_pImpl->SetAngle( angle );
	}

	void EnemyShot::SetSpeed( const GameUnit& speed )
	{
		m_pImpl->SetSpeed( speed );
	}

	void EnemyShot::SetCollisionRadius( const GameUnit& radius )
	{
		m_pImpl->SetCollisionRadius( radius );
	}

	void EnemyShot::AddPos( const GameUnit& x, const GameUnit& y )
	{
		m_pImpl->AddPos( x, y );
	}

	void EnemyShot::AddAngle( const GameUnit& angle )
	{
		m_pImpl->AddAngle( angle );
	}

	void EnemyShot::AddSpeed( const GameUnit& speed )
	{
		m_pImpl->AddSpeed( speed );
	}

	bool EnemyShot::DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius )
	{
		return m_pImpl->DoesColideWithPlayer( x, y, radius );
	}
	
	GameUnit EnemyShot::GetAngle() const
	{
		return m_pImpl->GetAngle();
	}

	GameUnit EnemyShot::GetSpeed() const
	{
		return m_pImpl->GetSpeed();
	}

#endif

	void EnemyShot::SetShape( int shape )
	{
		m_pImpl->SetShape( shape );
	}

	

	void EnemyShot::EnableInvincibleMode()
	{
		m_pImpl->EnableInvincibleMode();
	}

	void EnemyShot::DisableInvincibleMode()
	{
		m_pImpl->DisableInvincibleMode();
	}

	void EnemyShot::EnableInvisibleMode()
	{
		m_pImpl->EnableInvisibleMode();
	}

	void EnemyShot::DisableInvisibleMode()
	{
		m_pImpl->DisableInvisibleMode();
	}

	std::string EnemyShot::GetMasterEnemyName() const
	{
		return m_pImpl->GetMasterEnemyName();
	}
}
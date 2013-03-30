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
		EnemyShotGroup*						m_pShotGroup;		// 所属しているショットグループ
		int									m_ID;				// ショットグループID
	};

	typedef std::queue < EnemyShotMessage >			EnemyShotMessageQueue;


	class EnemyShot::Impl : public GameObjectImplBase
	{
	private:

		enum ImgRotMode
		{
			IMG_ROT_MODE_SYNC	= 0,			// 進行方向と同期
			IMG_ROT_MODE_AUTO	= 1,			// 毎フレーム指定した角度分だけ増加
			IMG_ROT_MODE_MANUAL	= 2,			// 自分で全て指定
			IMG_ROT_MODE_TOTAL,
		};


		// 衝突判定
		enum ShotShape
		{
			SHOT_SHAPE_CIRCLE	= 0,		// 円
			SHOT_SHAPE_LINE		= 1,		// 直線
			SHOT_SHAPE_TOTAL,
		};

		class Circle;
		class Line;
		class Shape
		{
		public:
			Shape(){}
			virtual ~Shape(){}
			virtual bool Colided( Shape* pObject ) = 0;				 // 衝突判定の処理 ディスパッチャ
			virtual bool JudgeCollision( Circle* pCircle ) = 0;		// 衝突判定の処理（円）
			virtual bool JudgeCollision( Line* pLine ) = 0;			// 衝突判定の処理（線）
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

				// 円と直線の距離
				float d = std::abs( vL.GetOuterProduct( v1 ) ) / vL.GetNorm();
				if( d <= m_Radius + pLine->GetThickness() ){
					// 衝突
					if( v1.GetInnerProduct( vL ) * v2.GetInnerProduct( vL ) <= 0 ){
						return true;
					}
					else{
						// 特殊な場合の衝突
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

				// 円と直線の距離
				float d = std::abs( vL.GetOuterProduct( v1 ) ) / vL.GetNorm();
				if( d <= pCircle->GetRadius() ){
					// 衝突
					if( v1.GetInnerProduct( vL ) * v2.GetInnerProduct( vL ) <= 0 ){
						return true;
					}
					else{
						// 特殊な場合の衝突
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

		std::shared_ptr < ResourceMap >		m_pResourceMap;		// リソース管理データ
		ShotGroupData						m_ShotGroupData;	// ショットグループデータ

		// 状態関連
		int									m_ShotID;					// ショットID
		float								m_PosX;						// 位置（X座標）
		float								m_PosY;						// 位置（Y座標）
		float								m_Angle;					// 角度
		float								m_Speed;					// 速度
		float								m_ColRadius;				// 衝突判定の半径
		int									m_ImgID;					// 画像ID
		float								m_ImgScale;					// 画像拡大率
		int									m_Counter;					// カウンタ
		int									m_DeadCounter;				// 死亡カウンタ
		int									m_Attr;						// 属性
		int									m_Power;					// 攻撃力
		int									m_ImgRotMode;				// 画像回転モード
		int									m_AlphaBlendingMode;		// αブレンディングモード
		float								m_ImgRotAnglePerFrame;		// 毎フレーム増加する回転角度
		float								m_ImgRotAngle;				// 画像の向き

		int				m_ShotShape;	// ショットの形
		Circle			m_Circle;
		Line			m_Line;

		// フラグ管理
		enum StatusFlag
		{
			COLLIDED					= 0,	// 衝突したか？
			DEAD						= 1,	// 死んでいたらtrue
			HAS_CONS_ATTR				= 2,	// 意識技用の弾の場合true
			HAS_BLENDING_MODE_CHAGE		= 3,	// アルファブレンドの変化がある場合、true
			PAUSED						= 4,	// 一時停止中の場合true
			STATUS_FLAG_TOTAL,
		};
		std::bitset < STATUS_FLAG_TOTAL >	m_StatusFlags;		// 状態管理フラグ

		// メッセージ関連
		EnemyShotMessageQueue				m_MsgQueue;			// メッセージキュー
		
	public:
		Impl( std::shared_ptr < ResourceMap > pMap, int id );
		~Impl();
		void Draw();										// 描画
		bool Update();										// 更新
		void GetPos( float* pX, float* pY );
		int GetPower() const;								// 弾の攻撃力を取得
		void SetPos( float posX, float posY );
		void SetPower( int power );							// 弾の攻撃力を設定
		void SetAngle( float angle );						// 角度を設定
		void SetSpeed( float speed );						// 速度を設定
		void SetImage( int id );							// 画像を設定
		void SetImageScale( float scale );					// 画像の拡大率を設定
		void SetCollisionRadius( float radius );			// 衝突判定の半径を設定
		void SetConsAttr( int attr );						// 意識技専用弾に設定
		void AddPos( float x, float y );					// 位置を加算
		void AddAngle( float angle );						// 角度を加算
		void AddSpeed( float speed );						// 速度を加算
		void JoinShotGroup( int id, EnemyShotGroup* pGroup );
		void ProcessCollision( Player* pPlayer );			// 衝突時の処理（プレイヤー）
		float GetCollisionRadius() const;
		int GetCounter() const;
		void ProcessMessages();								// 溜まっていたメッセージの処理
		void PostMessage( int msgID );						// メッセージの追加
		void PrepDestroy();									// 削除前処理
		bool IsDead() const;
		int GetConsAttr() const;
		void Pause();										// 一時停止
		void Resume();										// 一時停止から再開
		void SetImgRotMode( int mode );						// 敵弾の画像回転モードを設定
		void SetImgRotAnglePerFrame( float angle );			// 毎フレーム増加する回転角度を設定
		void SetAlphaBlendingMode( int mode );				// αブレンディングの方法を設定
		bool DoesColideWithPlayer( float x, float y, float radius );	// プレイヤーとの衝突判定
		void SetShape( int shape );							// 衝突判定の形を設定
		void SetLinePos( float x1, float y1, float x2, float y2, float thickness );		// 線の値を設定
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
					AddToSpriteBatch(	m_AlphaBlendingMode,
										m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
										m_PosX, m_PosY, m_ImgRotAngle );
				}
				else{
					MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
										m_PosX, m_PosY, m_ImgRotAngle );
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
			//}
			/*else{
				MAPIL::DrawTexture(	m_pResourceMap->m_pStageResourceMap->m_TextureMap[ m_ImgID ],
									m_PosX, m_PosY,
									( 30 - m_Counter ) * 0.1f, ( 30 - m_Counter ) * 0.1f,
									m_ImgRotAngle,
									true, ( m_Counter * 10 + 100 ) << 24 | 0xFFFFFF );
			}*/
		}
	}

	bool EnemyShot::Impl::Update()
	{
		if( m_StatusFlags[ PAUSED ] ){
			return true;
		}

		// メッセージ処理
		ProcessMessages();
		
		// 死亡判定処理
		if( m_StatusFlags[ DEAD ] ){
			++m_DeadCounter;
			if( m_DeadCounter >= 20 ){
				return false;
			}
		}
		else{
		//	if( m_Counter < 20 ){
		//		m_Speed -= 0.1f;
		//	}

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
		//if( m_Counter < 20 ){
		//	m_Speed = speed + 2.0f - m_Counter * 0.1f;
		//}
		//else{
			m_Speed = speed;
		//}
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
		if( m_ShotShape == SHOT_SHAPE_CIRCLE ){
			m_Circle.SetRadius( radius );
		}
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

	void EnemyShot::Impl::SetShape( int shape )
	{
		m_ShotShape = shape;
	}

	void EnemyShot::Impl::SetLinePos( float x1, float y1, float x2, float y2, float thickness )
	{
		m_Line.SetStartX( x1 );
		m_Line.SetStartY( y1 );
		m_Line.SetEndX( x2 );
		m_Line.SetEndY( y2 );
		m_Line.SetThickness( thickness );
	}

	// ----------------------------------
	// 実装クラスの呼び出し
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

	bool EnemyShot::DoesColideWithPlayer( float x, float y, float radius )
	{
		return m_pImpl->DoesColideWithPlayer( x, y, radius );
	}

	void EnemyShot::SetShape( int shape )
	{
		m_pImpl->SetShape( shape );
	}

	void EnemyShot::SetLinePos( float x1, float y1, float x2, float y2, float thickness )
	{
		m_pImpl->SetLinePos( x1, y1, x2, y2, thickness );
	}
}
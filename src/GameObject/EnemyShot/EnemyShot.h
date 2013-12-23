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
			ENEMY_SHOT_MESSAGE_ID_PLAYER_DAMAGED			= 0,		// プレイヤーがダメージを受けた
			ENEMY_SHOT_MESSAGE_ID_BOSS_MODE_STARTED			= 1,		// ボスモードへ移行した
			ENEMY_SHOT_MESSAGE_ID_BOSS_INVOKE_CONS_SKILL	= 2,		// ボスがスキルを使用した
			ENEMY_SHOT_MESSAGE_ID_PLAYER_BOMBED				= 3,		// プレイヤーがボムを発動
		};

		int		m_MsgID;
	};

	
	struct ShotGroupData
	{
		EnemyShotGroup*						m_pShotGroup;		// 所属しているショットグループ
		int									m_ID;				// ショットグループID
	};

	typedef std::queue < EnemyShotMessage >			EnemyShotMessageQueue;


	class EnemyShot : public CollisionObject
	{
	protected:

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


		struct GameUnitData
		{
			GameUnit		m_PosX;
			GameUnit		m_PosY;
			GameUnit		m_Angle;
			GameUnit		m_Speed;
			GameUnit		m_ColRadius;

		};
		GameUnitData			m_GUData;


		// 状態関連
		int									m_ShotID;					// ショットID
		int									m_ImgID;					// 画像ID
		int									m_AtlasImgID;				// アトラスID(-1が標準)
		float								m_ImgScale;					// 画像拡大率
		int									m_Counter;					// カウンタ
		int									m_DeadCounter;				// 死亡カウンタ
		int									m_Attr;						// 属性
		int									m_Power;					// 攻撃力
		int									m_ImgRotMode;				// 画像回転モード
		int									m_AlphaBlendingMode;		// αブレンディングモード
		float								m_ImgRotAnglePerFrame;		// 毎フレーム増加する回転角度
		float								m_ImgRotAngle;				// 画像の向き
		int									m_DrawingMultiplicity;		// 描画多重度
		int									m_TexColor;					// 画像の色

		int				m_ShotShape;	// ショットの形
		Circle			m_Circle;
		Line			m_Line;

		// フラグ管理
		enum StatusFlag
		{
			COLLIDED						= 0,	// 衝突したか？
			DEAD							= 1,	// 死んでいたらtrue
			HAS_CONS_ATTR					= 2,	// 意識技用の弾の場合true
			HAS_BLENDING_MODE_CHAGE			= 3,	// アルファブレンドの変化がある場合、true
			PAUSED							= 4,	// 一時停止中の場合true
			INVINCIBLE						= 5,	// ボムやプレイヤーダメージで消えない場合、true
			IMG_SCALE_CHANGED				= 6,	// 画像の拡大率を変更した場合、true
			INVISIBLE						= 7,	// 見えるモードの場合、true
			NOT_DELETE_BY_PLAYER_DAMAGE		= 8,	// プレイヤーダメージ時に消えない場合、true
			NOT_DELETE_BY_PLAYER_SKILL		= 9,	// プレイヤーのスキルによって消えない場合、true
			STATUS_FLAG_TOTAL,
		};
		std::bitset < STATUS_FLAG_TOTAL >	m_StatusFlags;		// 状態管理フラグ

		// メッセージ関連
		EnemyShotMessageQueue				m_MsgQueue;			// メッセージキュー

		
		
	public:
		EnemyShot( std::shared_ptr < ResourceMap > pMap, int id );
		virtual ~EnemyShot();
		virtual void Draw();										// 描画
		virtual bool Update();										// 更新
		virtual void Init( const GameUnit& posX, const GameUnit& posY );

		int GetPower() const;								// 弾の攻撃力を取得
		void SetPower( int power );							// 弾の攻撃力を設定
		void SetImage( int id );							// 画像を設定
		void SetAtlasImage( int id );
		void SetImageScale( float scale );					// 画像の拡大率を設定
		void SetConsAttr( int attr );						// 意識技専用弾に設定
		void JoinShotGroup( int id, EnemyShotGroup* pGroup );
		void LeaveFromShotGroup();

		virtual void Colided( CollisionObject* pObject );			// 衝突時の処理 ディスパッチャ
		void ProcessCollision( Player* pPlayer );			// 衝突時の処理（プレイヤー）
		void ProcessCollision( Enemy* pEnemy );				// 衝突時の処理（敵）
		void ProcessCollision( PlayerShot* pPlayerShot );	// 衝突時の処理（プレイヤーショット）
		void ProcessCollision( EnemyShot* pEnemyShot );		// 衝突時の処理（敵弾）
		void ProcessCollision( Item* pItem );				// 衝突時の処理（アイテム）

		virtual void SetPos( const GameUnit& posX, const GameUnit& posY );
		virtual void SetLinePos(	const GameUnit& x1,
									const GameUnit& y1,
									const GameUnit& x2,
									const GameUnit& y2,
									const GameUnit& thickness );		// 線の値を設定
		virtual void GetPos( GameUnit* pX, GameUnit* pY );
		void SetAngle( const GameUnit& angle );						// 角度を設定
		void SetSpeed( const GameUnit& speed );						// 速度を設定
		void SetCollisionRadius( const GameUnit& radius );			// 衝突判定の半径を設定
		virtual void AddPos( const GameUnit& x, const GameUnit& y );					// 位置を加算
		void AddAngle( const GameUnit& angle );						// 角度を加算
		void AddSpeed( const GameUnit& speed );						// 速度を加算
		virtual bool DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius );
		GameUnit GetCollisionRadius();
		GameUnit GetAngle() const;
		GameUnit GetSpeed() const;
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
		void EnableInvincibleMode();				// 消えないモードへ移行
		void DisableInvincibleMode();			// 消えるモードへ移行
		void EnableInvisibleMode();				// 見えないモードへ移行
		void DisableInvisibleMode();			// 見えるモードへ移行
		std::string GetMasterEnemyName() const;
		void SetDrawingMultiplicity( int num );
		void Delete( int by );
		void DeleteWhen( int when );
		void NotDeleteWhen( int when );		// 削除タイミングの設定
		virtual void SetLength( const GameUnit& length );
		virtual void SetTextureColor( int color );
	};

}

#endif
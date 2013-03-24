#ifndef INCLUDED_GAMEENGINE_ENEMYSHOTGROUP_H
#define INCLUDED_GAMEENGINE_ENEMYSHOTGROUP_H

#include <memory>



namespace GameEngine
{
	struct ResourceMap;
	struct EnemyData;
	struct StageData;
	class EnemyShot;
	class EnemyShotGroup;
	struct EnemyShotGroupScriptData;

	struct EnemyShotGroupData
	{
		EnemyShot*							m_pShots[ 1000 ];	// ショット
		int									m_ShotTotal;		// ショット数
		int									m_Counter;			// カウンタ
		EnemyData*							m_pEnemyData;		// 敵データ
		StageData*							m_pStageData;		// ステージデータ
		EnemyShotGroup*						m_pShotGroup;		// ショットグループ
		bool								m_IsNew;			// 新しいか？
		bool								m_EnemyControlled;	// 敵の管理下にあるか？
		int									m_Reg;				// レジスタ
		float								m_FReg;				// 浮動小数点レジスタ
	};
	
	class EnemyShotGroup
	{
	private:
		class Impl;
		std::auto_ptr < EnemyShotGroup::Impl >		m_pImpl;
	public:
		EnemyShotGroup( std::shared_ptr < ResourceMap > pMap,
						std::shared_ptr < EnemyShotGroupScriptData > pData,
						int id,
						EnemyData* pEnemyData );
		~EnemyShotGroup();
		void Init();
		bool Update();
		int CreateShot();
		void DeleteShot( int id );
		void SetReg( int reg );
		void SetFReg( float reg );
		int GetReg() const;
		float GetFReg() const;
		void SetShotPos( int id, float x, float y );
		void SetShotAngle( int id, float angle );
		void SetShotSpeed( int id, float speed );
		void SetShotImage( int id, int imgID );
		void SetCollisionRadius( int id, float radius );
		void SetShotMovement( int id, float angle, float speed );
		void SetShotStatus( int id, float x, float y, float angle, float speed, float radius, int imgID );
		bool IsEmpty() const;			// 全部消去済みか？
		void DetachEnemyControl();		// 敵の管理下でなくなるようにする。
	};
}

#endif
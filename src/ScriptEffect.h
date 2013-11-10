#ifndef INCLUDED_GAMEENGINE_SCRIPTEFFECT_H
#define INCLUDED_GAMEENGINE_SCRIPTEFFECT_H

#include <memory>
#include <bitset>

#include "NonCollisionObject.h"
#include "EffectVCPU.h"
#include "ScriptTypes.h"

namespace GameEngine
{
	struct ResourceMap;
	struct EnemyData;
	class Enemy;

	

	struct ScriptEffectData
	{
		enum StatusFlag
		{
			TERMINATED		= 0,
			STATUS_FLAG_TOTAL,
		};

		int			m_EffectID;
		int			m_EffectSubID;
		int			m_AlphaBlendingMode;
		int			m_Counter;
		float		m_PosX;
		float		m_PosY;
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		int			m_PointSprite;
		int			m_Regs[ 10 ];
		std::weak_ptr < Enemy >				m_ParentEnemy;
		std::bitset < STATUS_FLAG_TOTAL >		m_StatusFlags;
	};

	class ScriptEffect : public NonCollisionObject
	{
	private:
		ScriptEffectData			m_EffectData;
		EffectVCPU			m_VM;
		std::shared_ptr < EffectScriptData >		m_pScriptData;
	public:
		ScriptEffect(	std::shared_ptr < ResourceMap > pMap,
						std::shared_ptr < EffectScriptData > pData,
						std::shared_ptr < Enemy > pEnemy,
						int id );
		~ScriptEffect();
		void Init( float posX, float posY );				// 初期化
		void SetPos( float posX, float posY );				// 位置を設定
		void Draw();										// 描画
		bool Update();										// 更新
		void SetReg( int no, int val );						// レジスタ
		void SetAlphaBlendingMode( int mode );				// アルファブレンドモードの変更
	};
}

#endif
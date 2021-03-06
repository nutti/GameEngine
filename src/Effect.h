#ifndef INCLUDED_GAMEENGINE_EFFECT_H
#define INCLUDED_GAMEENGINE_EFFECT_H

#include <memory>

#include "NonCollisionObject.h"

namespace GameEngine
{
	struct ResourceMap;

	struct EffectData
	{
		int			m_EffectID;
		int			m_EffectSubID;
		int			m_Counter;
		float		m_PosX;
		float		m_PosY;
		std::shared_ptr < ResourceMap >		m_pResourceMap;
		int			m_PointSprite;
		int			m_Regs[ 10 ];
	};

	enum EffectID
	{
		EFFECT_ID_PLAYER_SHOT_COLLIDED	= 0,
		EFFECT_ID_ENEMY_DESTORYED		= 1,
		EFFECT_ID_SPRAY					= 2,
		EFFECT_ID_SCRIPT_EFFECT			= 100,
		EFFECT_ID_OBTAIN_ITEM_CRYSTAL	= 200,
	};

	class Effect : public NonCollisionObject
	{
	private:
		EffectData			m_EffectData;
	public:
		Effect( std::shared_ptr < ResourceMap > pMap, int id, int subID );
		~Effect();
		void Init( float posX, float posY );				// 初期化
		void SetPos( float posX, float posY );				// 位置を設定
		void Draw();										// 描画
		bool Update();										// 更新
		void SetReg( int no, int val );
	};
}

#endif
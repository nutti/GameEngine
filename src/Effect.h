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
	};

	enum EffectID
	{
		EFFECT_ID_PLAYER_SHOT_COLLIDED	= 0,
		EFFECT_ID_ENEMY_DESTORYED		= 1,
		EFFECT_ID_SCRIPT_EFFECT			= 100,
	};

	class Effect : public NonCollisionObject
	{
	private:
		EffectData			m_EffectData;
	public:
		Effect( std::shared_ptr < ResourceMap > pMap, int id, int subID );
		~Effect();
		void Init( float posX, float posY );				// èâä˙âª
		void SetPos( float posX, float posY );				// à íuÇê›íË
		void Draw();										// ï`âÊ
		bool Update();										// çXêV
	};
}

#endif
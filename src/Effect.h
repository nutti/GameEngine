#ifndef INCLUDED_GAMEENGINE_EFFECT_H
#define INCLUDED_GAMEENGINE_EFFECT_H

#include "NonCollisionObject.h"

namespace GameEngine
{

	class Effect : public NonCollisionObject
	{
	public:
		Effect();
		~Effect();
		void Init( float posX, float posY );				// 初期化
		void Draw();										// 描画
		bool Update();										// 更新
	};
}

#endif
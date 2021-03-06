#ifndef INCLUDED_GAMEENGINE_NONCOLLISIONOBJECT_H
#define INCLUDED_GAMEENGINE_NONCOLLISIONOBJECT_H

#include "GameObject.h"

namespace GameEngine
{

	class NonCollisionObject : public GameObject
	{
	public:
		NonCollisionObject();
		virtual ~NonCollisionObject();
		virtual void Init( float posX, float posY ) = 0;				// 初期化
		virtual void Draw() = 0;										// 描画
		virtual bool Update() = 0;										// 更新
	};
}

#endif
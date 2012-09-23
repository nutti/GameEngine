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
		virtual void Init( float posX, float posY ) = 0;				// ������
		virtual void Draw() = 0;										// �`��
		virtual bool Update() = 0;										// �X�V
	};
}

#endif
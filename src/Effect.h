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
		void Init( float posX, float posY );				// ������
		void Draw();										// �`��
		bool Update();										// �X�V
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_LASERSHOTIMPL_H
#define INCLUDED_GAMEENGINE_LASERSHOTIMPL_H

#include <memory>

#include "../../CollisionObject.h"
#include "LaserShot.h"
#include "EnemyShotImpl.h"

namespace GameEngine
{

	class LaserShotImpl : public EnemyShotImpl
	{
	private:
		GameUnit			m_Length;
	public:
		LaserShotImpl( std::shared_ptr < ResourceMap > pMap, int id );
		~LaserShotImpl();
		void Draw();										// �`��
		bool Update();										// �X�V
		void ProcessCollision( Player* pPlayer );			// �Փˎ��̏����i�v���C���[�j
		void SetPos( const GameUnit& posX, const GameUnit& posY );
		void SetLinePos(	const GameUnit& x1,
							const GameUnit& y1,
							const GameUnit& x2,
							const GameUnit& y2,
							const GameUnit& thickness );		// ���̒l��ݒ�
		void GetPos( GameUnit* pX, GameUnit* pY );
		void AddPos( const GameUnit& x, const GameUnit& y );					// �ʒu�����Z
		bool DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius );
		void SetLength( const GameUnit& length );
	};
}

#endif
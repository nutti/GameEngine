#ifndef INCLUDED_GAMEENGINE_LASERSHOT_H
#define INCLUDED_GAMEENGINE_LASERSHOT_H

#include <memory>

#include "EnemyShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class LaserShotImpl;
	class LaserShot : public EnemyShot
	{
	private:
		std::auto_ptr < LaserShotImpl >		m_pImpl;
	public:
		LaserShot( std::shared_ptr < ResourceMap > pMap, int id );
		~LaserShot();
		void Draw();										// �`��
		bool Update();										// �X�V
		void Colided( CollisionObject* pObject );			// �Փˎ��̏��� �f�B�X�p�b�`��
		void Init( const GameUnit& posX, const GameUnit& posY );				// ������
		void AddPos( const GameUnit& x, const GameUnit& y );					// �ʒu�����Z
		void GetPos( GameUnit* pPosX, GameUnit* pPosY );
		void SetPos( const GameUnit& posX, const GameUnit& posY );				// �ʒu��ݒ�
		void SetLinePos(	const GameUnit& x1,
							const GameUnit& y1,
							const GameUnit& x2,
							const GameUnit& y2,
							const GameUnit& thickness );		// ���̒l��ݒ�
		void SetLength( const GameUnit& length );
	};
}

#endif
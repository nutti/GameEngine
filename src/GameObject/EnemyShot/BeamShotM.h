#ifndef INCLUDED_GAMEENGINE_BEAMSHOTM_H
#define INCLUDED_GAMEENGINE_BEAMSHOTM_H

#include <memory>

#include "EnemyShot.h"

namespace GameEngine
{

	struct ResourceMap;
	class EnemyShotGroup;
	class BeamShotM : public EnemyShot
	{
	private:
		GameUnit			m_Length;
		GameUnit			m_BeginPosX;
		GameUnit			m_BeginPosY;
		void DrawEffect();
	public:
		BeamShotM( std::shared_ptr < ResourceMap > pMap, int id );
		~BeamShotM();
		void Draw();										// �`��
		bool Update();										// �X�V
		void Init( const GameUnit& posX, const GameUnit& posY );				// ������
		void SetLinePos(	const GameUnit& x1,
							const GameUnit& y1,
							const GameUnit& x2,
							const GameUnit& y2,
							const GameUnit& thickness );		// ���̒l��ݒ�
		bool DoesColideWithPlayer( const GameUnit& x, const GameUnit& y, const GameUnit& radius );
		void SetLength( const GameUnit& length );
		void SetTextureColor( int color );
	};
}

#endif
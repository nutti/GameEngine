#ifndef INCLUDED_GAMEENGINE_GAMESTATETYPES_H
#define INCLUDED_GAMEENGINE_GAMESTATETYPES_H

namespace GameEngine
{
	// ���b�Z�[�W�����p�Q�[���f�[�^
	struct GameDataMsg
	{
		int		m_Score;			// �X�R�A
		int		m_HIScore;			// �n�C�X�R�A
		int		m_Killed;			// �|�����G��
		int		m_CrystalTotal;		// �N���X�^����
	};
}

#endif
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

	// �X�e�[�W�i�s�x
	enum StageProgress
	{
		STAGE_PROGRESS_NONE			= 0,
		STAGE_PROGRESS_STAGE_1		= 1,
		STAGE_PROGRESS_STAGE_2		= 2,
		STAGE_PROGRESS_STAGE_3		= 3,
		STAGE_PROGRESS_STAGE_4		= 4,
		STAGE_PROGRESS_STAGE_5		= 5,
		STAGE_PROGRESS_ALL_CLEAR	= 6,
	};

	// ��Փx
	enum GameDifficulty
	{
		GAME_DIFFICULTY_UNKNOWN		= -1,
		GAME_DIFFICULTY_EASY		= 1,
		GAME_DIFFICULTY_NORMAL		= 2,
		GAME_DIFFICULTY_HARD		= 3,
		GAME_DIFFICULTY_HAZARD		= 4,
	};

	// �Z�[�u�f�[�^�p���R�[�h
	struct SaveDataRecord
	{
		struct Time
		{
			int		m_Year;
			char	m_Month;
			char	m_Day;
			char	m_Hour;
			char	m_Min;
			char	m_Sec;
		};
		struct StageData
		{
			int		m_Score;			// �X�R�A
			int		m_Killed;			// ���j��
			int		m_Crystal;			// �擾�N���X�^����
			int		m_Progress;			// �X�e�[�W�i�s�x
		};

		char		m_Name[ 10 ];			// �G���g����
		Time		m_Date;					// �o�^����
		StageData	m_StageData[ 5 ];		// �X�e�[�W�f�[�^
		int			m_Score;				// �X�R�A
		int			m_Progress;				// �i�s��
		int			m_Killed;				// ���j��
		int			m_Crystal;				// �N���X�^����
	};

	// �X�R�A�\���p�Z�[�u�f�[�^
	struct DisplayedSaveData
	{
		struct Difficulty
		{
			SaveDataRecord		m_Record[ 25 ];			// 25�G���g���܂ŃX�R�A�̋L�^���\
			int					m_AllClear;				// �S�N����
			int					m_PlayTime;				// �v���C����
		};
		int			m_PlayTime;
		Difficulty	m_Difficulty[ 4 ];				// ��Փx�ʃQ�[�����
	};
}

#endif
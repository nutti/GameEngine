#ifndef INCLUDED_GAMEENGINE_GAMESTATETYPES_H
#define INCLUDED_GAMEENGINE_GAMESTATETYPES_H

#include "Date.h"

namespace GameEngine
{
	// ���b�Z�[�W�����p�Q�[���f�[�^
	struct GameDataMsg
	{
		int		m_Score;			// �X�R�A
		//int		m_HIScore;			// �n�C�X�R�A
		int		m_Killed;			// �|�����G��
		int		m_CrystalTotal;		// �N���X�^����
		int		m_CrystalUsed;		// �c��̃N���X�^����
	};

	// �X�e�[�W�i�s�x
	//enum StageProgress
	//{
	//	STAGE_PROGRESS_NONE			= 0,
	//	STAGE_PROGRESS_STAGE_1		= 1,
	//	STAGE_PROGRESS_STAGE_2		= 2,
	//	STAGE_PROGRESS_STAGE_3		= 3,
	//	STAGE_PROGRESS_STAGE_4		= 4,
	//	STAGE_PROGRESS_STAGE_5		= 5,
	//	STAGE_PROGRESS_ALL_CLEAR	= 6,
	//};

	// ��Փx
	enum GameDifficulty
	{
		GAME_DIFFICULTY_UNKNOWN		= -1,
		GAME_DIFFICULTY_EASY		= 0,
		GAME_DIFFICULTY_NORMAL		= 1,
		GAME_DIFFICULTY_HARD		= 2,
		GAME_DIFFICULTY_HAZARD		= 3,
	};

	// �Z�[�u�f�[�^�p���R�[�h
	struct SaveDataRecord
	{
		struct StageData
		{
			int		m_Score;			// �X�R�A
			int		m_Killed;			// ���j��
			int		m_Crystal;			// �擾�N���X�^����
			int		m_CrystalUsed;		// �g�p�����N���X�^���̑���
			int		m_Progress;			// �X�e�[�W�i�s�x
		};

		char		m_Name[ 10 ];			// �G���g����
		Date		m_Date;					// �o�^����
		StageData	m_StageData[ 5 ];		// �X�e�[�W�f�[�^
		int			m_Score;				// �X�R�A
		int			m_Progress;				// �i�s��
		int			m_Killed;				// ���j��
		int			m_Crystal;				// �N���X�^����
		int			m_CrystalUsed;			// �g�p�����N���X�^���̑���
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

	// �\���p���v���C���
	struct DisplayedReplayInfo
	{
		struct Entry
		{
			int						m_EntryNo;				// �G���g���ԍ�
			char					m_Name[ 10 ];			// ���O
			int						m_Progress;				// �i�s�x
			int						m_Score;				// �X�R�A
			int						m_Crystal;				// �N���X�^���l����
			int						m_Killed;				// ���j��
			int						m_Difficulty;			// ��Փx
			Date					m_Date;					// �쐬����
		};
		Entry		m_Entries[ 25 ];
	};


}

#endif
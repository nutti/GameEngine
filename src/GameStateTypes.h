#ifndef INCLUDED_GAMEENGINE_GAMESTATETYPES_H
#define INCLUDED_GAMEENGINE_GAMESTATETYPES_H

#include <vector>

#include "Date.h"
#include "InputTypes.h"

namespace GameEngine
{
	const int MAX_REPLAY_ENTRY		= 15;		// �ő僊�v���C�G���g����
	const int MAX_SCORE_ENTRY		= 15;		// �ő�X�R�A�G���g����
	const int STAGE_TOTAL			= 5;		// �X�e�[�W����

	// ���b�Z�[�W�����p�Q�[���f�[�^
	struct GameDataMsg
	{
		int		m_Score;			// �X�R�A
		//int		m_HIScore;			// �n�C�X�R�A
		int		m_Killed;			// �|�����G��
		int		m_CrystalTotal;		// �N���X�^����
		int		m_CrystalUsed;		// �c��̃N���X�^����
	};

	// ��Փx
	enum GameDifficulty
	{
		GAME_DIFFICULTY_CALM		= 0,
		GAME_DIFFICULTY_EASY		= 1,
		GAME_DIFFICULTY_NORMAL		= 2,
		GAME_DIFFICULTY_HARD		= 3,
		GAME_DIFFICULTY_HAZARD		= 4,
		GAME_DIFFICULTY_TOTAL,
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
		StageData	m_StageData[ STAGE_TOTAL ];		// �X�e�[�W�f�[�^
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
			SaveDataRecord		m_Record[ MAX_SCORE_ENTRY ];	// 15�G���g���܂ŃX�R�A�̋L�^���\
			int					m_AllClear;						// �S�N����
			int					m_PlayTime;						// �v���C����
		};
		int			m_PlayTime;
		Difficulty	m_Difficulty[ GAME_DIFFICULTY_TOTAL ];				// ��Փx�ʃQ�[�����
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
			int						m_CrystalUsed;			// �N���X�^���g�p��
			int						m_Killed;				// ���j��
			int						m_Difficulty;			// ��Փx
			Date					m_Date;					// �쐬����
		};
		Entry		m_Entries[ MAX_REPLAY_ENTRY ];
	};

	// ���v���C�f�[�^
	struct ReplayDataRecord
	{
		struct StageDataInfo
		{
			int		m_IniPosX;				// ����X���W
			int		m_IniPosY;				// ����Y���W
			int		m_IniHP;				// ����HP
			int		m_IniShotPower;			// �����V���b�g�p���[
			int		m_IniScore;				// �����̓��_
			int		m_IniKilled;			// �����̌��j��
			int		m_IniCrystal;			// �����̃N���X�^���l����
			int		m_IniCrystalUsed;		// �N���X�^���g�p��
			int		m_IniCons;				// �����̈ӎ����
			int		m_IniConsGauge[ 3 ];	// �����̈ӎ��Q�[�W
			int		m_IniConsLevel[ 3 ];	// �����̈ӎ����x��
		};
		struct StageKeyStates
		{
			std::vector < ButtonPushedStatus >		m_StatusList;
		};

		char					m_Name[ 10 ];			// ���O
		int						m_Progress;				// �i�s�x
		int						m_Score;				// �X�R�A
		int						m_Crystal;				// �N���X�^���l����
		int						m_CrystalUsed;			// �g�p�����N���X�^���̑���
		int						m_Killed;				// ���j��
		int						m_Difficulty;			// ��Փx
		Date					m_Date;					// �쐬����

		int						m_EntryNo;				// �G���g���ԍ�

		StageDataInfo			m_StageDataInfo[ STAGE_TOTAL ];		// �����X�e�[�W�f�[�^
		StageKeyStates			m_StageKeyStatusList[ STAGE_TOTAL ];	// �X�e�[�W�̃L�[���X�g
	};

	// �����Q�[���f�[�^
	struct InitialGameData
	{
		int		m_HIScore;				// �n�C�X�R�A
		int		m_Score;				// �O��̃X�e�[�W���N���A�������̃X�R�A
		int		m_Crystal;				// �O��̃X�e�[�W�܂łŎ擾�N���X�^���̑���
		int		m_CrystalUsed;			// �O��̃X�e�[�W�܂łŎg�p�����N���X�^������
		int		m_Killed;				// �O��̃X�e�[�W�܂łœ|�����G�̐�
		int		m_PosX;					// �O��̃X�e�[�W���N���A�������̍Ō�ɂ����ꏊ�iX���W�j
		int		m_PosY;					// �O��̃X�e�[�W���N���A�������̍Ō�ɂ����ꏊ�iY���W�j
		int		m_HP;					// �O��̃X�e�[�W���N���A�������̎c��HP
		int		m_ShotPower;			// �O��̃X�e�[�W���N���A�������̃V���b�g�p���[
		int		m_Cons;					// �����̈ӎ����
		int		m_ConsGauge[ 3 ];		// �����̈ӎ��Q�[�W
		int		m_ConsLevel[ 3 ];		// �����̈ӎ����x��
		int		m_Progress;				// �X�e�[�W�i�s�x
	};

	// �Q�[���X�s�[�h
	enum GamePlaySpeed
	{
		GAME_PLAY_SPEED_X1_4	= 0,
		GAME_PLAY_SPEED_X1_3	= 1,
		GAME_PLAY_SPEED_X1_2	= 2,
		GAME_PLAY_SPEED_NORMAL	= 3,
		GAME_PLAY_SPEED_X2		= 4,
		GAME_PLAY_SPEED_X3		= 5,
		GAME_PLAY_SPEED_X4		= 6,
		GAME_PLAY_SPEED_TOTAL
	};

	const int MAX_BGM_VOLUME	= 100;
	const int MIN_BGM_VOLUME	= 0;
	const int MAX_SE_VOLUME		= 100;
	const int MIN_SE_VOLUME		= 0;

	// �Q�[���ݒ�f�[�^
	struct GameConfigData
	{
		int		m_BGMVolume;		// BGM�̉���
		int		m_SEVolume;			// ���ʉ��̉���
		int		m_PlaySpeed;		// �v���C���x
	};
}

#endif
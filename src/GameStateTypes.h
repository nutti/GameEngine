#ifndef INCLUDED_GAMEENGINE_GAMESTATETYPES_H
#define INCLUDED_GAMEENGINE_GAMESTATETYPES_H

#include "Date.h"

namespace GameEngine
{
	// メッセージ交換用ゲームデータ
	struct GameDataMsg
	{
		int		m_Score;			// スコア
		//int		m_HIScore;			// ハイスコア
		int		m_Killed;			// 倒した敵数
		int		m_CrystalTotal;		// クリスタル数
		int		m_CrystalUsed;		// 残りのクリスタル数
	};

	// ステージ進行度
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

	// 難易度
	enum GameDifficulty
	{
		GAME_DIFFICULTY_UNKNOWN		= -1,
		GAME_DIFFICULTY_EASY		= 0,
		GAME_DIFFICULTY_NORMAL		= 1,
		GAME_DIFFICULTY_HARD		= 2,
		GAME_DIFFICULTY_HAZARD		= 3,
	};

	// セーブデータ用レコード
	struct SaveDataRecord
	{
		struct StageData
		{
			int		m_Score;			// スコア
			int		m_Killed;			// 撃破数
			int		m_Crystal;			// 取得クリスタル数
			int		m_CrystalUsed;		// 使用したクリスタルの総数
			int		m_Progress;			// ステージ進行度
		};

		char		m_Name[ 10 ];			// エントリ名
		Date		m_Date;					// 登録日時
		StageData	m_StageData[ 5 ];		// ステージデータ
		int			m_Score;				// スコア
		int			m_Progress;				// 進行状況
		int			m_Killed;				// 撃破数
		int			m_Crystal;				// クリスタル数
		int			m_CrystalUsed;			// 使用したクリスタルの総数
	};

	// スコア表示用セーブデータ
	struct DisplayedSaveData
	{
		struct Difficulty
		{
			SaveDataRecord		m_Record[ 25 ];			// 25エントリまでスコアの記録が可能
			int					m_AllClear;				// 全クリ回数
			int					m_PlayTime;				// プレイ時間
		};
		int			m_PlayTime;
		Difficulty	m_Difficulty[ 4 ];				// 難易度別ゲーム状態
	};

	// 表示用リプレイ情報
	struct DisplayedReplayInfo
	{
		struct Entry
		{
			int						m_EntryNo;				// エントリ番号
			char					m_Name[ 10 ];			// 名前
			int						m_Progress;				// 進行度
			int						m_Score;				// スコア
			int						m_Crystal;				// クリスタル獲得数
			int						m_Killed;				// 撃破数
			int						m_Difficulty;			// 難易度
			Date					m_Date;					// 作成日時
		};
		Entry		m_Entries[ 25 ];
	};


}

#endif
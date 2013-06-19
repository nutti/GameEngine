#ifndef INCLUDED_GAMEENGINE_GAMESTATETYPES_H
#define INCLUDED_GAMEENGINE_GAMESTATETYPES_H

#include <vector>

#include "Date.h"
#include "InputTypes.h"

namespace GameEngine
{
	const int MAX_REPLAY_ENTRY		= 15;		// 最大リプレイエントリ数
	const int MAX_SCORE_ENTRY		= 15;		// 最大スコアエントリ数
	const int STAGE_TOTAL			= 5;		// ステージ総数

	// メッセージ交換用ゲームデータ
	struct GameDataMsg
	{
		int		m_Score;			// スコア
		//int		m_HIScore;			// ハイスコア
		int		m_Killed;			// 倒した敵数
		int		m_CrystalTotal;		// クリスタル数
		int		m_CrystalUsed;		// 残りのクリスタル数
	};

	// 難易度
	enum GameDifficulty
	{
		GAME_DIFFICULTY_CALM		= 0,
		GAME_DIFFICULTY_EASY		= 1,
		GAME_DIFFICULTY_NORMAL		= 2,
		GAME_DIFFICULTY_HARD		= 3,
		GAME_DIFFICULTY_HAZARD		= 4,
		GAME_DIFFICULTY_TOTAL,
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
		StageData	m_StageData[ STAGE_TOTAL ];		// ステージデータ
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
			SaveDataRecord		m_Record[ MAX_SCORE_ENTRY ];	// 15エントリまでスコアの記録が可能
			int					m_AllClear;						// 全クリ回数
			int					m_PlayTime;						// プレイ時間
		};
		int			m_PlayTime;
		Difficulty	m_Difficulty[ GAME_DIFFICULTY_TOTAL ];				// 難易度別ゲーム状態
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
			int						m_CrystalUsed;			// クリスタル使用数
			int						m_Killed;				// 撃破数
			int						m_Difficulty;			// 難易度
			Date					m_Date;					// 作成日時
		};
		Entry		m_Entries[ MAX_REPLAY_ENTRY ];
	};

	// リプレイデータ
	struct ReplayDataRecord
	{
		struct StageDataInfo
		{
			int		m_IniPosX;				// 初期X座標
			int		m_IniPosY;				// 初期Y座標
			int		m_IniHP;				// 初期HP
			int		m_IniShotPower;			// 初期ショットパワー
			int		m_IniScore;				// 初期の得点
			int		m_IniKilled;			// 初期の撃破数
			int		m_IniCrystal;			// 初期のクリスタル獲得数
			int		m_IniCrystalUsed;		// クリスタル使用数
			int		m_IniCons;				// 初期の意識状態
			int		m_IniConsGauge[ 3 ];	// 初期の意識ゲージ
			int		m_IniConsLevel[ 3 ];	// 初期の意識レベル
		};
		struct StageKeyStates
		{
			std::vector < ButtonPushedStatus >		m_StatusList;
		};

		char					m_Name[ 10 ];			// 名前
		int						m_Progress;				// 進行度
		int						m_Score;				// スコア
		int						m_Crystal;				// クリスタル獲得数
		int						m_CrystalUsed;			// 使用したクリスタルの総数
		int						m_Killed;				// 撃破数
		int						m_Difficulty;			// 難易度
		Date					m_Date;					// 作成日時

		int						m_EntryNo;				// エントリ番号

		StageDataInfo			m_StageDataInfo[ STAGE_TOTAL ];		// 初期ステージデータ
		StageKeyStates			m_StageKeyStatusList[ STAGE_TOTAL ];	// ステージのキーリスト
	};

	// 初期ゲームデータ
	struct InitialGameData
	{
		int		m_HIScore;				// ハイスコア
		int		m_Score;				// 前回のステージをクリアした時のスコア
		int		m_Crystal;				// 前回のステージまでで取得クリスタルの総数
		int		m_CrystalUsed;			// 前回のステージまでで使用したクリスタル総数
		int		m_Killed;				// 前回のステージまでで倒した敵の数
		int		m_PosX;					// 前回のステージをクリアした時の最後にいた場所（X座標）
		int		m_PosY;					// 前回のステージをクリアした時の最後にいた場所（Y座標）
		int		m_HP;					// 前回のステージをクリアした時の残りHP
		int		m_ShotPower;			// 前回のステージをクリアした時のショットパワー
		int		m_Cons;					// 初期の意識状態
		int		m_ConsGauge[ 3 ];		// 初期の意識ゲージ
		int		m_ConsLevel[ 3 ];		// 初期の意識レベル
		int		m_Progress;				// ステージ進行度
	};

	// ゲームスピード
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

	// ゲーム設定データ
	struct GameConfigData
	{
		int		m_BGMVolume;		// BGMの音量
		int		m_SEVolume;			// 効果音の音量
		int		m_PlaySpeed;		// プレイ速度
	};
}

#endif
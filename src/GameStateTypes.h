#ifndef INCLUDED_GAMEENGINE_GAMESTATETYPES_H
#define INCLUDED_GAMEENGINE_GAMESTATETYPES_H

namespace GameEngine
{
	// メッセージ交換用ゲームデータ
	struct GameDataMsg
	{
		int		m_Score;			// スコア
		int		m_HIScore;			// ハイスコア
		int		m_Killed;			// 倒した敵数
		int		m_CrystalTotal;		// クリスタル数
	};
}

#endif
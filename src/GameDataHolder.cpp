#include <MAPIL/MAPIL.h>

#include <time.h>

#include "GameDataHolder.h"
#include "Util.h"

namespace GameEngine
{

	struct GameFileData
	{
		struct Difficulty
		{
			SaveDataRecord		m_Record[ 25 ];			// 25エントリまでスコアの記録が可能
			int					m_AllClear;				// 全クリ回数
			int					m_PlayTime;				// プレイ時間
			int					m_StageProgress;		// 各ステージ進行度（1:Stage1クリア）
		};

		int			m_PlayTime;						// プレイ時間（秒単位）
		int			m_Progress;						// 進行度（0:未プレイ）
		Difficulty	m_Difficulty[ 4 ];				// 難易度別ゲーム状態
	};

	class GameDataHolder::Impl
	{
	private:
		GameDataMsg			m_GameData;				// 現在のゲームデータ
		GameFileData		m_GameFileData;			// セーブファイルに書き出すデータ
		std::string			m_SaveDataFileName;		// セーブファイル名

		::time_t			m_TimeStamp;			// 最終更新日時
	public:
		Impl();
		~Impl(){}
		void StartRecording();
		void EndRecording();
		void Flush();
		void Load( const std::string& fileName );
		int GetPlayTime() const;
		void UpdatePlayTime();
		const SaveDataRecord& GetRecord( int difficulty, int rank ) const;
		int GetAllClearCount( int difficulty ) const;
		int GetProgress( int difficulty ) const;
		int GetProgress() const;
		int GetPlayTime( int difficulty ) const;
		void SetRecord( int difficulty, const SaveDataRecord& record );
		int GetRank( int difficulty, const SaveDataRecord& record ) const;
	};

	GameDataHolder::Impl::Impl() : /*m_ScoreManager(),*/ m_SaveDataFileName()
	{
		MAPIL::ZeroObject( &m_GameData, sizeof( m_GameData ) );
		MAPIL::ZeroObject( &m_GameFileData, sizeof( m_GameFileData ) );
		m_SaveDataFileName = "save/data.dat";
	}

	void GameDataHolder::Impl::StartRecording()
	{
		MAPIL::ZeroObject( &m_GameData, sizeof( m_GameData ) );

		Load( m_SaveDataFileName );

		// 現在時刻を取得
		m_TimeStamp = ::time( NULL );
	}

	void GameDataHolder::Impl::EndRecording()
	{
		Flush();
	}

	void GameDataHolder::Impl::Flush()
	{
		UpdatePlayTime();

		std::ofstream fOut( m_SaveDataFileName, std::ios::binary | std::ios::out );

		WriteInt( &fOut, m_GameFileData.m_PlayTime );
		WriteInt( &fOut, m_GameFileData.m_Progress );
		for( int i = 0; i < 4; ++i ){
			WriteInt( &fOut, m_GameFileData.m_Difficulty[ i ].m_AllClear );
			WriteInt( &fOut, m_GameFileData.m_Difficulty[ i ].m_PlayTime );
			WriteInt( &fOut, m_GameFileData.m_Difficulty[ i ].m_StageProgress );
			for( int j = 0; j < 25; ++j ){
				SaveDataRecord record = m_GameFileData.m_Difficulty[ i ].m_Record[ j ];
				fOut.write( record.m_Name, sizeof( record.m_Name ) );
				WriteInt( &fOut, record.m_Date.m_Year );
				fOut.write( &record.m_Date.m_Month, sizeof( char ) );
				fOut.write( &record.m_Date.m_Day, sizeof( char ) );
				fOut.write( &record.m_Date.m_Hour, sizeof( char ) );
				fOut.write( &record.m_Date.m_Min, sizeof( char ) );
				fOut.write( &record.m_Date.m_Sec, sizeof( char ) );
				for( int k = 0; k < 5; ++k ){
					SaveDataRecord::StageData stage = record.m_StageData[ k ];
					WriteInt( &fOut, stage.m_Score );
					WriteInt( &fOut, stage.m_Killed );
					WriteInt( &fOut, stage.m_Crystal );
					WriteInt( &fOut, stage.m_Progress );
				}
				WriteInt( &fOut, record.m_Score );
				WriteInt( &fOut, record.m_Progress );
				WriteInt( &fOut, record.m_Killed );
				WriteInt( &fOut, record.m_Crystal );
			}
		}
	}

	void GameDataHolder::Impl::Load( const std::string& fileName )
	{
		std::ifstream fIn( m_SaveDataFileName, std::ios::binary | std::ios::out );
		if( !fIn ){
			return;
		}

		m_GameFileData.m_PlayTime = ReadInt( &fIn );
		m_GameFileData.m_Progress = ReadInt( &fIn );
		for( int i = 0; i < 4; ++i ){
			GameFileData::Difficulty difficulty;
			difficulty.m_AllClear = ReadInt( &fIn );
			difficulty.m_PlayTime = ReadInt( &fIn );
			difficulty.m_StageProgress = ReadInt( &fIn );
			for( int j = 0; j < 25; ++j ){
				SaveDataRecord record;
				fIn.read( record.m_Name, sizeof( record.m_Name ) );
				record.m_Date.m_Year = ReadInt( &fIn );
				fIn.read( &record.m_Date.m_Month, sizeof( char ) );
				fIn.read( &record.m_Date.m_Day, sizeof( char ) );
				fIn.read( &record.m_Date.m_Hour, sizeof( char ) );
				fIn.read( &record.m_Date.m_Min, sizeof( char ) );
				fIn.read( &record.m_Date.m_Sec, sizeof( char ) );
				for( int k = 0; k < 5; ++k ){
					SaveDataRecord::StageData stage;
					stage.m_Score = ReadInt( &fIn );
					stage.m_Killed = ReadInt( &fIn );
					stage.m_Crystal = ReadInt( &fIn );
					stage.m_Progress = ReadInt( &fIn );
					record.m_StageData[ k ] = stage;
				}
				record.m_Score = ReadInt( &fIn );
				record.m_Progress = ReadInt( &fIn );
				record.m_Killed = ReadInt( &fIn );
				record.m_Crystal = ReadInt( &fIn );
				difficulty.m_Record[ j ] = record;
			}
			m_GameFileData.m_Difficulty[ i ] = difficulty;
		}
	}

	int GameDataHolder::Impl::GetPlayTime() const
	{
		return m_GameFileData.m_PlayTime;
	}

	void GameDataHolder::Impl::UpdatePlayTime()
	{
		int time = ::time( NULL );
		m_GameFileData.m_PlayTime += time - m_TimeStamp;
		m_TimeStamp = time;
	}

	const SaveDataRecord& GameDataHolder::Impl::GetRecord( int difficulty, int rank ) const
	{
		return m_GameFileData.m_Difficulty[ difficulty ].m_Record[ rank ];
	}

	int GameDataHolder::Impl::GetAllClearCount( int difficulty ) const
	{
		return m_GameFileData.m_Difficulty[ difficulty ].m_AllClear;
	}

	int GameDataHolder::Impl::GetProgress( int difficulty ) const
	{
		return m_GameFileData.m_Difficulty[ difficulty ].m_StageProgress;
	}

	int GameDataHolder::Impl::GetProgress() const
	{
		return m_GameFileData.m_Progress;
	}

	int GameDataHolder::Impl::GetPlayTime( int difficulty ) const
	{
		return m_GameFileData.m_Difficulty[ difficulty ].m_PlayTime;
	}

	void GameDataHolder::Impl::SetRecord( int difficulty, const SaveDataRecord& record )
	{
		int newScore = record.m_Score;
		for( int i = 0; i < sizeof( m_GameFileData.m_Difficulty[ difficulty ].m_Record ) / sizeof( m_GameFileData.m_Difficulty[ difficulty ].m_Record[ 0 ] ); ++i ){
			if( m_GameFileData.m_Difficulty[ difficulty ].m_Record[ i ].m_Score < newScore ){
				int newRank = i;
				// 消去されるものの全コピー
				for( int j = sizeof( m_GameFileData.m_Difficulty[ difficulty ].m_Record ) / sizeof( m_GameFileData.m_Difficulty[ difficulty ].m_Record[ 0 ] ) - 1; j > i; --j ){
					m_GameFileData.m_Difficulty[ difficulty ].m_Record[ j ] = m_GameFileData.m_Difficulty[ difficulty ].m_Record[ j - 1 ];
				}
				m_GameFileData.m_Difficulty[ difficulty ].m_Record[ newRank ] = record;
				break;
			}
		}
	}

	int GameDataHolder::Impl::GetRank( int difficulty, const SaveDataRecord& record ) const
	{
		int newScore = record.m_Score;
		for( int i = 0; i < sizeof( m_GameFileData.m_Difficulty[ difficulty ].m_Record ) / sizeof( m_GameFileData.m_Difficulty[ difficulty ].m_Record[ 0 ] ); ++i ){
			if( m_GameFileData.m_Difficulty[ difficulty ].m_Record[ i ].m_Score < newScore ){
				return i;		// ランクイン
			}
		}

		return -1;		// ランク外
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	GameDataHolder::GameDataHolder() : m_pImpl( new GameDataHolder::Impl )
	{
	}

	GameDataHolder::~GameDataHolder()
	{
	}

	void GameDataHolder::StartRecording()
	{
		m_pImpl->StartRecording();
	}

	void GameDataHolder::EndRecording()
	{
		m_pImpl->EndRecording();
	}

	void GameDataHolder::Flush()
	{
		m_pImpl->Flush();
	}

	int GameDataHolder::GetPlayTime() const
	{
		return m_pImpl->GetPlayTime();
	}

	void GameDataHolder::UpdatePlayTime()
	{
		m_pImpl->UpdatePlayTime();
	}

	const SaveDataRecord& GameDataHolder::GetRecord( int difficulty, int rank ) const
	{
		return m_pImpl->GetRecord( difficulty, rank );
	}

	int GameDataHolder::GetAllClearCount( int difficulty ) const
	{
		return m_pImpl->GetAllClearCount( difficulty );
	}

	int GameDataHolder::GetProgress( int difficulty ) const
	{
		return m_pImpl->GetProgress( difficulty );
	}

	int GameDataHolder::GetProgress() const
	{
		return m_pImpl->GetProgress();
	}

	int GameDataHolder::GetPlayTime( int difficulty ) const
	{
		return m_pImpl->GetPlayTime( difficulty );
	}

	void GameDataHolder::SetRecord( int difficulty, const SaveDataRecord& record )
	{
		m_pImpl->SetRecord( difficulty, record );
	}

	int GameDataHolder::GetRank( int difficulty, const SaveDataRecord& record ) const
	{
		return m_pImpl->GetRank( difficulty, record );
	}
}
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
			SaveDataRecord				m_Record[ MAX_SCORE_ENTRY ];	// 25エントリまでスコアの記録が可能
			NormalPlayStat				m_NormalPlayStat;				// 通常プレイの統計情報
			StageSelectionPlayStat		m_StageSelectionPlayStat;		// ステージ選択プレイの統計情報
			Difficulty()
			{
				MAPIL::ZeroObject( m_Record, sizeof( m_Record ) );
			}
			~Difficulty()
			{
				MAPIL::ZeroObject( m_Record, sizeof( m_Record ) );
			}
		};

		int			m_PlayTime;						// プレイ時間（秒単位）
		int			m_Progress;						// 進行度（0:未プレイ）
		Difficulty	m_Difficulty[ GAME_DIFFICULTY_TOTAL ];				// 難易度別ゲーム状態

		GameFileData() : m_PlayTime( 0 ), m_Progress( 0 )
		{
		}
		~GameFileData()
		{
			m_PlayTime = 0;
			m_Progress = 0;
		}
	};

	class GameDataHolder::Impl
	{
	private:
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
		int GetHIScore( int difficulty ) const;
		int GetPlayCount( int difficulty ) const;

		const NormalPlayStat& GetNormalPlayStat( int difficulty ) const;
		const StageSelectionPlayStat& GetStageSelectionPlayStat( int difficulty ) const;

		void SetNormalPlayStat( int difficulty, const NormalPlayStat& stat );
		void SetStageSelectionPlayStat( int difficulty, const StageSelectionPlayStat& stat );
	};

	GameDataHolder::Impl::Impl() : m_SaveDataFileName(), m_GameFileData()
	{
		m_SaveDataFileName = "save/data.dat";
	}

	void GameDataHolder::Impl::StartRecording()
	{
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
		if( !FileExist( "save" ) ){
			CreateDirectory( "save" );
		}

		UpdatePlayTime();

		std::vector < char > data;
		data.reserve( 30000 );

		std::ofstream fOut( m_SaveDataFileName, std::ios::binary | std::ios::out );
		if( !fOut ){
			return;
		}

		CopyInt( &data, m_GameFileData.m_PlayTime );
		CopyInt( &data, m_GameFileData.m_Progress );
		for( int i = 0; i < GAME_DIFFICULTY_TOTAL; ++i ){
			// 通常プレイの統計情報の保存
			CopyInt( &data, m_GameFileData.m_Difficulty[ i ].m_NormalPlayStat.m_Play );
			CopyInt( &data, m_GameFileData.m_Difficulty[ i ].m_NormalPlayStat.m_AllClear );
			CopyInt( &data, m_GameFileData.m_Difficulty[ i ].m_NormalPlayStat.m_PlayTime );
			CopyInt( &data, m_GameFileData.m_Difficulty[ i ].m_NormalPlayStat.m_Progress );
			for( int j = 0; j < STAGE_TOTAL; ++j ){
				// 敵情報の保存
				const StageStat& stage = m_GameFileData.m_Difficulty[ i ].m_NormalPlayStat.m_StageStat[ j ];
				StageStat::EnemyStatMap::const_iterator it = stage.m_EnemyStat.begin();
				// エントリ数の保存
				CopyInt( &data, stage.m_EnemyStat.size() );
				for( ; it != stage.m_EnemyStat.end(); ++it ){
					// 敵の名前の長さ保存
					CopyInt( &data, it->first.size() );
					// 敵の名前の保存
					CopyArray( &data, it->first.c_str(), it->first.size() );
					// 情報の保存
					CopyInt( &data, it->second.m_Destroy );
					CopyInt( &data, it->second.m_Damaged );
					CopyInt( &data, it->second.m_KO );
				}
			}

			// ステージ選択プレイの統計情報の保存
			CopyInt( &data, m_GameFileData.m_Difficulty[ i ].m_StageSelectionPlayStat.m_Play );
			CopyInt( &data, m_GameFileData.m_Difficulty[ i ].m_StageSelectionPlayStat.m_Clear );
			CopyInt( &data, m_GameFileData.m_Difficulty[ i ].m_StageSelectionPlayStat.m_PlayTime );
			for( int j = 0; j < STAGE_TOTAL; ++j ){
				// 敵情報の保存
				const StageStat& stage = m_GameFileData.m_Difficulty[ i ].m_StageSelectionPlayStat.m_StageStat[ j ];
				StageStat::EnemyStatMap::const_iterator it = stage.m_EnemyStat.begin();
				// エントリ数の保存
				CopyInt( &data, stage.m_EnemyStat.size() );
				for( ; it != stage.m_EnemyStat.end(); ++it ){
					// 敵の名前の長さ保存
					CopyInt( &data, it->first.size() );
					// 敵の名前の保存
					CopyArray( &data, it->first.c_str(), it->first.size() );
					// 情報の保存
					CopyInt( &data, it->second.m_Destroy );
					CopyInt( &data, it->second.m_Damaged );
					CopyInt( &data, it->second.m_KO );
				}
			}

			// スコアの保存
			for( int j = 0; j < MAX_SCORE_ENTRY; ++j ){
				SaveDataRecord record = m_GameFileData.m_Difficulty[ i ].m_Record[ j ];
				CopyArray( &data, record.m_Name, sizeof( record.m_Name ) );
				CopyInt( &data, record.m_Date.m_Year );
				data.push_back( record.m_Date.m_Month );
				data.push_back( record.m_Date.m_Day );
				data.push_back( record.m_Date.m_Hour );
				data.push_back( record.m_Date.m_Min );
				data.push_back( record.m_Date.m_Sec );
				for( int k = 0; k < STAGE_TOTAL; ++k ){
					SaveDataRecord::StageData stage = record.m_StageData[ k ];
					CopyInt( &data, stage.m_Score );
					CopyInt( &data, stage.m_Killed );
					CopyInt( &data, stage.m_Crystal );
					CopyInt( &data, stage.m_CrystalUsed );
					CopyInt( &data, stage.m_Progress );
				}
				CopyInt( &data, record.m_Score );
				CopyInt( &data, record.m_Progress );
				CopyInt( &data, record.m_Killed );
				CopyInt( &data, record.m_Crystal );
				CopyInt( &data, record.m_CrystalUsed );
			}
		}

		// 圧縮
		char* pBuf = new char [ data.size() * 2 ];
		int compSize = 0;
		MAPIL::LZ lz( 200, 3 );
		lz.Compress( &data[ 0 ], data.size(), &pBuf, data.size() * 2, &compSize );
		// シーザ暗号化
		MAPIL::Caesar caesar( 10 );
		caesar.Encrypt( pBuf, compSize );
		// XOR暗号化
		MAPIL::XOR xor( 60 );
		xor.Encrypt( pBuf, compSize );
		fOut.write( pBuf, compSize );
		fOut.close();
		MAPIL::SafeDeleteArray( pBuf );
	}

	void GameDataHolder::Impl::Load( const std::string& fileName )
	{
		// 現在時刻を取得
		m_TimeStamp = ::time( NULL );

		std::fstream fIn( m_SaveDataFileName, std::ios::binary | std::ios::in );
		if( !fIn ){
			return;
		}
		int size = GetFileSize( fIn );
		char* pBuf = new char [ size ];
		fIn.read( pBuf, size * sizeof( char ) );
		fIn.close();

		// XOR暗号復号化
		MAPIL::XOR xor( 60 );
		xor.Decrypt( pBuf, size );
		// シーザ暗号復号化
		MAPIL::Caesar caesar( 10 );
		caesar.Decrypt( pBuf, size );
		// 解凍
		MAPIL::LZ lz( 200, 3 );
		char* pData = new char [ size * 1000 ];
		int dataSize = 0;
		lz.Expand( pBuf, size, &pData, size * 1000, &dataSize );
		MAPIL::SafeDeleteArray( pBuf );

		// データ設定
		char* p = pData;
		m_GameFileData.m_PlayTime = GetInt( &p );
		m_GameFileData.m_Progress = GetInt( &p );
		for( int i = 0; i < GAME_DIFFICULTY_TOTAL; ++i ){
			GameFileData::Difficulty difficulty;

			// 通常プレイのデータを取得
			difficulty.m_NormalPlayStat.m_Play = GetInt( &p );
			difficulty.m_NormalPlayStat.m_AllClear = GetInt( &p );
			difficulty.m_NormalPlayStat.m_PlayTime = GetInt( &p );
			difficulty.m_NormalPlayStat.m_Progress = GetInt( &p );
			for( int j = 0; j < STAGE_TOTAL; ++j ){
				// 敵情報の取得
				StageStat& stage = difficulty.m_NormalPlayStat.m_StageStat[ j ];
				// エントリ数の取得
				int entry = GetInt( &p );
				for( int k = 0; k < entry; ++k ){
					// 敵の名前の長さを取得
					int length = GetInt( &p );
					// 敵の名前の取得
					char* pEnemyName = new char[ length + 1 ];
					::memcpy( pEnemyName, p, sizeof( length ) );
					pEnemyName[ length ] = '\0';
					p += length;
					// 情報の取得
					EnemyStat enemyStat;
					enemyStat.m_Destroy = GetInt( &p );
					enemyStat.m_Damaged = GetInt( &p );
					enemyStat.m_KO = GetInt( &p );
					stage.m_EnemyStat[ pEnemyName ] = enemyStat;
					MAPIL::SafeDeleteArray( pEnemyName );
				}
			}

			// ステージ選択プレイのデータを取得
			difficulty.m_StageSelectionPlayStat.m_Play = GetInt( &p );
			difficulty.m_StageSelectionPlayStat.m_Clear = GetInt( &p );
			difficulty.m_StageSelectionPlayStat.m_PlayTime = GetInt( &p );
			for( int j = 0; j < STAGE_TOTAL; ++j ){
				// 敵情報の取得
				StageStat& stage = difficulty.m_StageSelectionPlayStat.m_StageStat[ j ];
				// エントリ数の取得
				int entry = GetInt( &p );
				for( int k = 0; k < entry; ++k ){
					// 敵の名前の長さを取得
					int length = GetInt( &p );
					// 敵の名前の取得
					char* pEnemyName = new char[ length + 1 ];
					::memcpy( pEnemyName, p, sizeof( length ) );
					pEnemyName[ length ] = '\0';
					p += length;
					// 情報の取得
					EnemyStat enemyStat;
					enemyStat.m_Destroy = GetInt( &p );
					enemyStat.m_Damaged = GetInt( &p );
					enemyStat.m_KO = GetInt( &p );
					stage.m_EnemyStat[ pEnemyName ] = enemyStat;
					MAPIL::SafeDeleteArray( pEnemyName );
				}
			}


			// スコアのロード
			for( int j = 0; j < MAX_SCORE_ENTRY; ++j ){
				SaveDataRecord record;
				::memcpy( record.m_Name, p, sizeof( record.m_Name ) );
				p += sizeof( record.m_Name );
				record.m_Date.m_Year = GetInt( &p );
				record.m_Date.m_Month = *p++;
				record.m_Date.m_Day = *p++;
				record.m_Date.m_Hour = *p++;
				record.m_Date.m_Min = *p++;
				record.m_Date.m_Sec = *p++;
				for( int k = 0; k < STAGE_TOTAL; ++k ){
					SaveDataRecord::StageData stage;
					stage.m_Score = GetInt( &p );
					stage.m_Killed = GetInt( &p );
					stage.m_Crystal = GetInt( &p );
					stage.m_CrystalUsed = GetInt( &p );
					stage.m_Progress = GetInt( &p );
					record.m_StageData[ k ] = stage;
				}
				record.m_Score = GetInt( &p );
				record.m_Progress = GetInt( &p );
				record.m_Killed = GetInt( &p );
				record.m_Crystal = GetInt( &p );
				record.m_CrystalUsed = GetInt( &p );
				difficulty.m_Record[ j ] = record;
			}
			m_GameFileData.m_Difficulty[ i ] = difficulty;
		}

		MAPIL::SafeDeleteArray( pData );
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
		return m_GameFileData.m_Difficulty[ difficulty ].m_NormalPlayStat.m_AllClear;
	}

	int GameDataHolder::Impl::GetProgress( int difficulty ) const
	{
		return m_GameFileData.m_Difficulty[ difficulty ].m_NormalPlayStat.m_Progress;
	}

	int GameDataHolder::Impl::GetProgress() const
	{
		return m_GameFileData.m_Progress;
	}

	int GameDataHolder::Impl::GetPlayTime( int difficulty ) const
	{
		return m_GameFileData.m_Difficulty[ difficulty ].m_NormalPlayStat.m_PlayTime;
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

	int GameDataHolder::Impl::GetHIScore( int difficulty ) const
	{
		return m_GameFileData.m_Difficulty[ difficulty ].m_Record[ 0 ].m_Score;
	}

	int GameDataHolder::Impl::GetPlayCount( int difficulty ) const
	{
		return m_GameFileData.m_Difficulty[ difficulty ].m_NormalPlayStat.m_Play;
	}

	const NormalPlayStat& GameDataHolder::Impl::GetNormalPlayStat( int difficulty ) const
	{
		return m_GameFileData.m_Difficulty[ difficulty ].m_NormalPlayStat;
	}

	const StageSelectionPlayStat& GameDataHolder::Impl::GetStageSelectionPlayStat( int difficulty ) const
	{
		return m_GameFileData.m_Difficulty[ difficulty ].m_StageSelectionPlayStat;
	}

	void GameDataHolder::Impl::SetNormalPlayStat( int difficulty, const NormalPlayStat& stat )
	{
		m_GameFileData.m_Difficulty[ difficulty ].m_NormalPlayStat = stat;
	}

	void GameDataHolder::Impl::SetStageSelectionPlayStat( int difficulty, const StageSelectionPlayStat& stat )
	{
		m_GameFileData.m_Difficulty[ difficulty ].m_StageSelectionPlayStat = stat;
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

	int GameDataHolder::GetHIScore( int difficulty ) const
	{
		return m_pImpl->GetHIScore( difficulty );
	}

	void GameDataHolder::Load( const std::string& fileName )
	{
		m_pImpl->Load( fileName );
	}

	int GameDataHolder::GetPlayCount( int difficulty ) const
	{
		return m_pImpl->GetPlayCount( difficulty );
	}

	const NormalPlayStat& GameDataHolder::GetNormalPlayStat( int difficulty ) const
	{
		return m_pImpl->GetNormalPlayStat( difficulty );
	}

	const StageSelectionPlayStat& GameDataHolder::GetStageSelectionPlayStat( int difficulty ) const
	{
		return m_pImpl->GetStageSelectionPlayStat( difficulty );
	}

	void GameDataHolder::SetNormalPlayStat( int difficulty, const NormalPlayStat& stat )
	{
		m_pImpl->SetNormalPlayStat( difficulty, stat );
	}

	void GameDataHolder::SetStageSelectionPlayStat( int difficulty, const StageSelectionPlayStat& stat )
	{
		m_pImpl->SetStageSelectionPlayStat( difficulty, stat );
	}
}
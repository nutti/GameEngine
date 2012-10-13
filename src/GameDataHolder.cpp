#include <MAPIL/MAPIL.h>

#include <time.h>

#include "GameDataHolder.h"
#include "ScoreManager.h"
#include "Util.h"

namespace GameEngine
{

	struct GameFileData
	{
		struct Difficulty
		{
			struct Record
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
					int		m_ConsGauge[ 3 ];	// �����ӎ��Q�[�W
					int		m_ConsLevel[ 3 ];	// �����ӎ����x��
					int		m_Cons;				// �����ӎ����
					int		m_ShotPower;		// �����V���b�g�p���[
					int		m_HP;				// ����HP
				};

				char		m_Name[ 10 ];			// �G���g����
				Time		m_Date;					// �o�^����
				StageData	m_StageData[ 5 ];		// �X�e�[�W�f�[�^
			};
			Record		m_Record[ 25 ];			// 25�G���g���܂ŃX�R�A�̋L�^���\
			int			m_AllClear;				// �S�N����
			int			m_PlayTime;				// �v���C����
			int			m_StageProgress;		// �e�X�e�[�W�i�s�x�i1:Stage1�N���A�j
		};

		int			m_PlayTime;						// �v���C���ԁi�b�P�ʁj
		int			m_Progress;						// �i�s�x�i0:���v���C�j
		Difficulty	m_Difficulty[ 4 ];				// ��Փx�ʃQ�[�����
	};

	class GameDataHolder::Impl
	{
	private:
		GameDataMsg			m_GameData;				// ���݂̃Q�[���f�[�^
		GameFileData		m_GameFileData;			// �Z�[�u�t�@�C���ɏ����o���f�[�^
		std::string			m_SaveDataFileName;		// �Z�[�u�t�@�C����

		::time_t			m_TimeStamp;			// �ŏI�X�V����

		ScoreManager		m_ScoreManager;			// �X�R�A�Ǘ��N���X
	public:
		Impl();
		~Impl(){}
		void StartRecording();
		void EndRecording();
		void Update();
		GameDataMsg GetScoreData() const;
		void Add( const GameDataMsg& data );
		void Flush();
		void Load( const std::string& fileName );
		int GetPlayTime() const;
		void UpdatePlayTime();
	};

	GameDataHolder::Impl::Impl() : m_ScoreManager(), m_SaveDataFileName()
	{
		MAPIL::ZeroObject( &m_GameData, sizeof( m_GameData ) );
		MAPIL::ZeroObject( &m_GameFileData, sizeof( m_GameFileData ) );
		m_SaveDataFileName = "save/data.dat";
	}

	void GameDataHolder::Impl::StartRecording()
	{
		MAPIL::ZeroObject( &m_GameData, sizeof( m_GameData ) );

		Load( m_SaveDataFileName );

		// ���ݎ������擾
		m_TimeStamp = ::time( NULL );
	}

	void GameDataHolder::Impl::EndRecording()
	{
		int time = ::time( NULL );
		m_GameFileData.m_PlayTime += time - m_TimeStamp;
		m_TimeStamp = time;
		Flush();
	}

	void GameDataHolder::Impl::Update()
	{
		m_ScoreManager.Update();
		m_GameData.m_Score = m_ScoreManager.GetScore();
		if( m_GameData.m_HIScore <= m_GameData.m_Score ){
			m_GameData.m_HIScore = m_GameData.m_Score;
		}
	}

	GameDataMsg GameDataHolder::Impl::GetScoreData() const
	{
		return m_GameData;
	}

	void GameDataHolder::Impl::Add( const GameDataMsg& data )
	{
		m_ScoreManager.Add( data.m_Score );
		m_GameData.m_Killed += data.m_Killed;
		m_GameData.m_CrystalTotal += data.m_CrystalTotal;
	}

	void GameDataHolder::Impl::Flush()
	{
		std::ofstream fOut( m_SaveDataFileName, std::ios::binary | std::ios::out );

		WriteInt( &fOut, m_GameFileData.m_PlayTime );
		WriteInt( &fOut, m_GameFileData.m_Progress );
		for( int i = 0; i < 4; ++i ){
			WriteInt( &fOut, m_GameFileData.m_Difficulty[ i ].m_AllClear );
			WriteInt( &fOut, m_GameFileData.m_Difficulty[ i ].m_PlayTime );
			WriteInt( &fOut, m_GameFileData.m_Difficulty[ i ].m_StageProgress );
			for( int j = 0; j < 25; ++j ){
				GameFileData::Difficulty::Record record = m_GameFileData.m_Difficulty[ i ].m_Record[ j ];
				fOut.write( record.m_Name, sizeof( record.m_Name ) );
				WriteInt( &fOut, record.m_Date.m_Year );
				fOut.write( &record.m_Date.m_Month, sizeof( char ) );
				fOut.write( &record.m_Date.m_Day, sizeof( char ) );
				fOut.write( &record.m_Date.m_Hour, sizeof( char ) );
				fOut.write( &record.m_Date.m_Min, sizeof( char ) );
				fOut.write( &record.m_Date.m_Sec, sizeof( char ) );
				for( int k = 0; k < 5; ++k ){
					GameFileData::Difficulty::Record::StageData stage = record.m_StageData[ k ];
					WriteInt( &fOut, stage.m_Score );
					WriteInt( &fOut, stage.m_Killed );
					WriteInt( &fOut, stage.m_Crystal );
					for( int l = 0; l < 3; ++l ){
						WriteInt( &fOut, stage.m_ConsGauge[ l ] );
						WriteInt( &fOut, stage.m_ConsLevel[ l ] );
					}
					WriteInt( &fOut, stage.m_Cons );
					WriteInt( &fOut, stage.m_ShotPower );
					WriteInt( &fOut, stage.m_HP );
				}		
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
				GameFileData::Difficulty::Record record;
				fIn.read( record.m_Name, sizeof( record.m_Name ) );
				record.m_Date.m_Year = ReadInt( &fIn );
				fIn.read( &record.m_Date.m_Month, sizeof( char ) );
				fIn.read( &record.m_Date.m_Day, sizeof( char ) );
				fIn.read( &record.m_Date.m_Hour, sizeof( char ) );
				fIn.read( &record.m_Date.m_Min, sizeof( char ) );
				fIn.read( &record.m_Date.m_Sec, sizeof( char ) );
				for( int k = 0; k < 5; ++k ){
					GameFileData::Difficulty::Record::StageData stage;
					stage.m_Score = ReadInt( &fIn );
					stage.m_Killed = ReadInt( &fIn );
					stage.m_Crystal = ReadInt( &fIn );
					for( int l = 0; l < 3; ++l ){
						stage.m_ConsGauge[ l ] = ReadInt( &fIn );
						stage.m_ConsLevel[ l ] = ReadInt( &fIn );
					}
					stage.m_Cons = ReadInt( &fIn );
					stage.m_ShotPower = ReadInt( &fIn );
					stage.m_HP = ReadInt( &fIn );
					record.m_StageData[ k ] = stage;
				}
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

	// ----------------------------------
	// �����N���X�̌Ăяo��
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

	void GameDataHolder::Update()
	{
		m_pImpl->Update();
	}

	GameDataMsg GameDataHolder::GetScoreData() const
	{
		return m_pImpl->GetScoreData();
	}

	void GameDataHolder::Add( const GameDataMsg& data )
	{
		m_pImpl->Add( data );
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
}
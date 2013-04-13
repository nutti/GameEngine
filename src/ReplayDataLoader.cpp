#include "ReplayDataLoader.h"

#include <MAPIL/MAPIL.h>
#include <vector>

#include "Util.h"

namespace GameEngine
{
	const char* REPLAY_FILE_DIR = "replay";
	const char* REPLAY_FILE_NAME_PREFIX = "replay";
	const char* REPLAY_FILE_NAME_SUFFIX = ".rpy";

	/*struct ReplayDataInfo
	{
		struct StageData
		{
			int		m_IniPosX;				// ����X���W
			int		m_IniPosY;				// ����Y���W
			int		m_IniHP;				// ����HP
			int		m_IniShotPower;			// �����V���b�g�p���[
			int		m_IniScore;				// �����̓��_
			int		m_IniKilled;			// �����̌��j��
			int		m_IniCrystal;			// �����̃N���X�^���l����
			int		m_IniCons;				// �����̈ӎ����
			int		m_IniConsGauge[ 3 ];	// �����̈ӎ��Q�[�W
			int		m_IniConsLevel[ 3 ];	// �����̈ӎ����x��
			int		m_FrameTotal;			// �X�e�[�W�̃t���[����
		};
		struct Date
		{
			int		m_Year;
			char	m_Month;
			char	m_Day;
			char	m_Hour;
			char	m_Min;
			char	m_Sec;
		};

		char					m_Name[ 10 ];			// ���O
		int						m_Progress;				// �i�s�x
		int						m_Score;				// �X�R�A
		int						m_Crystal;				// �N���X�^���l����
		int						m_Killed;				// ���j��
		int						m_Difficulty;			// ��Փx
		Date					m_Date;					// �쐬����

		StageData				m_StageData[ 5 ];		// �����X�e�[�W�f�[�^
	};*/

	class ReplayDataLoader::Impl
	{
	private:
		//std::vector < ButtonPushedStatus >		m_ButtonList;			// �ߋ��̃{�^���̗���
		//ReplayDataInfo							m_ReplayDataInfo;		// ���v���C�f�[�^�̏��
		ReplayDataRecord						m_ReplayDataRecord;		// ���v���C�f�[�^
	public:
		Impl();
		~Impl(){}
		ButtonPushedStatus GetButtonState( int stageNo, int frame ) const;
		//void Load( const std::string& fileName );
		void Load( const std::string& fileName );
		void Cleanup();
		DisplayedReplayInfo::Entry GetDisplayedInfo( const std::string& fileName ) const;
		const ReplayDataRecord& GetReplayDataRecord() const;
	};

	ReplayDataLoader::Impl::Impl()
	{
		Cleanup();
	}

	ButtonPushedStatus ReplayDataLoader::Impl::GetButtonState( int stageNo, int frame ) const
	{
		return m_ReplayDataRecord.m_StageKeyStatusList[ stageNo - 1 ].m_StatusList[ frame ];
	}

	/*void ReplayDataLoader::Impl::Load( const std::string& fileName )
	{
		// �t�@�C���̓ǂݍ���
		std::fstream fIn( fileName, std::ios::binary | std::ios::in );
		int size = GetFileSize( fIn );
		char* pBuf = new char [ size ];
		fIn.read( pBuf, size * sizeof( char ) );
		fIn.close();

		// XOR�Í�������
		MAPIL::XOR xor( 60 );
		xor.Decrypt( pBuf, size );
		// �V�[�U�Í�������
		MAPIL::Caesar caesar( 10 );
		caesar.Decrypt( pBuf, size );
		// ��
		MAPIL::LZ lz( 200, 5 );
		char* pData = new char [ size * 1000 ];
		int dataSize = 0;
		lz.Expand( pBuf, size, &pData, size * 1000, &dataSize );
		MAPIL::SafeDeleteArray( pBuf );

		// �f�[�^�ݒ�
		char* p = pData;
		::memcpy( m_ReplayDataInfo.m_Name, p, sizeof( m_ReplayDataInfo.m_Name ) );
		p += sizeof( m_ReplayDataInfo.m_Name );
		m_ReplayDataInfo.m_Progress = GetInt( &p );
		m_ReplayDataInfo.m_Score = GetInt( &p );
		m_ReplayDataInfo.m_Crystal = GetInt( &p );
		m_ReplayDataInfo.m_Killed = GetInt( &p );
		m_ReplayDataInfo.m_Difficulty = GetInt( &p );
		m_ReplayDataInfo.m_Date.m_Year = GetInt( &p );
		m_ReplayDataInfo.m_Date.m_Month = *p++;
		m_ReplayDataInfo.m_Date.m_Day = *p++;
		m_ReplayDataInfo.m_Date.m_Hour = *p++;
		m_ReplayDataInfo.m_Date.m_Min = *p++;
		m_ReplayDataInfo.m_Date.m_Sec = *p++;
		// �e�X�e�[�W�f�[�^�J�n���̃f�[�^���擾
		for( int i = 0; i < 5; ++i ){
			ReplayDataInfo::StageData stage;
			stage.m_IniPosX = GetInt( &p );
			stage.m_IniPosY = GetInt( &p );
			stage.m_IniHP = GetInt( &p );
			stage.m_IniShotPower = GetInt( &p );
			stage.m_IniScore = GetInt( &p );
			stage.m_IniKilled = GetInt( &p );
			stage.m_IniCrystal = GetInt( &p );
			stage.m_IniCons = GetInt( &p );
			for( int j = 0; j < 3; ++j ){
				stage.m_IniConsGauge[ j ] = GetInt( &p );
			}
			for( int j = 0; j < 3; ++j ){
				stage.m_IniConsLevel[ j ] = GetInt( &p );
			}
			stage.m_FrameTotal = GetInt( &p );
		}

		// ���̓{�^���̃��[�h
		int frame = GetInt( &p );
		m_ButtonList.resize( frame + 50, 0 );
		for( unsigned int i = 0; i < m_ButtonList.size(); ++i ){
			m_ButtonList[ i ] = *p++;
		}

		MAPIL::SafeDeleteArray( pData );
	}*/

	void ReplayDataLoader::Impl::Cleanup()
	{
		MAPIL::ZeroObject( &m_ReplayDataRecord.m_StageDataInfo, sizeof( m_ReplayDataRecord.m_StageDataInfo ) );
		m_ReplayDataRecord.m_Crystal = 0;
		m_ReplayDataRecord.m_CrystalUsed = 0;
		MAPIL::ZeroObject( &m_ReplayDataRecord.m_Date, sizeof( m_ReplayDataRecord.m_Date ) );
		m_ReplayDataRecord.m_Difficulty = 0;
		m_ReplayDataRecord.m_Killed = 0;
		MAPIL::ZeroObject( m_ReplayDataRecord.m_Name, sizeof( m_ReplayDataRecord.m_Name ) );
		m_ReplayDataRecord.m_Progress = 0;
		m_ReplayDataRecord.m_Score = 0;
		for( int i = 0; i < 5; ++i ){
			m_ReplayDataRecord.m_StageKeyStatusList[ i ].m_StatusList.clear();
		}
	}

	DisplayedReplayInfo::Entry ReplayDataLoader::Impl::GetDisplayedInfo( const std::string& fileName ) const
	{
		DisplayedReplayInfo::Entry entry;

		// �t�@�C���̓ǂݍ���
		std::fstream fIn( fileName, std::ios::binary | std::ios::in );
		if( !fIn ){
			MAPIL::ZeroObject( &entry, sizeof( entry ) );
			entry.m_Progress = -1;
			return entry;
		}
		int size = GetFileSize( fIn );
		char* pBuf = new char [ size ];
		fIn.read( pBuf, size * sizeof( char ) );
		fIn.close();

		// XOR�Í�������
		MAPIL::XOR xor( 60 );
		xor.Decrypt( pBuf, size );
		// �V�[�U�Í�������
		MAPIL::Caesar caesar( 10 );
		caesar.Decrypt( pBuf, size );
		// ��
		MAPIL::LZ lz( 200, 5 );
		char* pData = new char [ size * 10 ];
		int dataSize = 0;
		lz.Expand( pBuf, size, &pData, size * 10, &dataSize );
		MAPIL::SafeDeleteArray( pBuf );


		// �f�[�^�̐ݒ�
		char* p = pData;

		::memcpy( entry.m_Name, p, sizeof( entry.m_Name ) );
		p += sizeof( entry.m_Name );
		entry.m_Progress = GetInt( &p );
		entry.m_Score = GetInt( &p );
		entry.m_Crystal = GetInt( &p );
		entry.m_CrystalUsed = GetInt( &p );
		entry.m_Killed = GetInt( &p );
		entry.m_Difficulty = GetInt( &p );
		entry.m_Date.m_Year = GetInt( &p );
		entry.m_Date.m_Month = *p++;
		entry.m_Date.m_Day = *p++;
		entry.m_Date.m_Hour = *p++;
		entry.m_Date.m_Min = *p++;
		entry.m_Date.m_Sec = *p++;

		MAPIL::SafeDeleteArray( pData );

		return entry;
	}

	void ReplayDataLoader::Impl::Load( const std::string& fileName )
	{
		// �t�@�C���̓ǂݍ���
		std::fstream fIn( fileName, std::ios::binary | std::ios::in );
		int size = GetFileSize( fIn );
		char* pBuf = new char [ size ];
		fIn.read( pBuf, size * sizeof( char ) );
		fIn.close();

		// XOR�Í�������
		MAPIL::XOR xor( 60 );
		xor.Decrypt( pBuf, size );
		// �V�[�U�Í�������
		MAPIL::Caesar caesar( 10 );
		caesar.Decrypt( pBuf, size );
		// ��
		MAPIL::LZ lz( 200, 5 );
		char* pData = new char [ size * 1000 ];
		int dataSize = 0;
		lz.Expand( pBuf, size, &pData, size * 1000, &dataSize );
		MAPIL::SafeDeleteArray( pBuf );

		// �f�[�^�ݒ�
		char* p = pData;
		::memcpy( m_ReplayDataRecord.m_Name, p, sizeof( m_ReplayDataRecord.m_Name ) );
		p += sizeof( m_ReplayDataRecord.m_Name );
		m_ReplayDataRecord.m_Progress = GetInt( &p );
		m_ReplayDataRecord.m_Score = GetInt( &p );
		m_ReplayDataRecord.m_Crystal = GetInt( &p );
		m_ReplayDataRecord.m_CrystalUsed = GetInt( &p );
		m_ReplayDataRecord.m_Killed = GetInt( &p );
		m_ReplayDataRecord.m_Difficulty = GetInt( &p );
		m_ReplayDataRecord.m_Date.m_Year = GetInt( &p );
		m_ReplayDataRecord.m_Date.m_Month = *p++;
		m_ReplayDataRecord.m_Date.m_Day = *p++;
		m_ReplayDataRecord.m_Date.m_Hour = *p++;
		m_ReplayDataRecord.m_Date.m_Min = *p++;
		m_ReplayDataRecord.m_Date.m_Sec = *p++;
		// �e�X�e�[�W�f�[�^�J�n���̃f�[�^���擾
		for( int i = 0; i < 5; ++i ){
			ReplayDataRecord::StageDataInfo stage;
			stage.m_IniPosX = GetInt( &p );
			stage.m_IniPosY = GetInt( &p );
			stage.m_IniHP = GetInt( &p );
			stage.m_IniShotPower = GetInt( &p );
			stage.m_IniScore = GetInt( &p );
			stage.m_IniKilled = GetInt( &p );
			stage.m_IniCrystal = GetInt( &p );
			stage.m_IniCrystalUsed = GetInt( &p );
			stage.m_IniCons = GetInt( &p );
			for( int j = 0; j < 3; ++j ){
				stage.m_IniConsGauge[ j ] = GetInt( &p );
			}
			for( int j = 0; j < 3; ++j ){
				stage.m_IniConsLevel[ j ] = GetInt( &p );
			}
			m_ReplayDataRecord.m_StageDataInfo[ i ] = stage;

			int frameTotal = GetInt( &p );
			// ���̓{�^���̃��[�h
			m_ReplayDataRecord.m_StageKeyStatusList[ i ].m_StatusList.resize( frameTotal + 50, 0 );
			for( unsigned int j = 0; j < frameTotal; ++j ){
				m_ReplayDataRecord.m_StageKeyStatusList[ i ].m_StatusList[ j ] = *p++;
			}
		}

		

		MAPIL::SafeDeleteArray( pData );
	}

	const ReplayDataRecord& ReplayDataLoader::Impl::GetReplayDataRecord() const
	{
		return m_ReplayDataRecord;
	}

	// ----------------------------------
	// �����N���X�̌Ăяo��
	// ----------------------------------

	ReplayDataLoader::ReplayDataLoader() : m_pImpl( new ReplayDataLoader::Impl )
	{
	}

	ReplayDataLoader::~ReplayDataLoader()
	{
	}

	ButtonPushedStatus ReplayDataLoader::GetButtonState( int stageNo, int frame ) const
	{
		return m_pImpl->GetButtonState( stageNo, frame );
	}
	
	void ReplayDataLoader::Load( const std::string& fileName )
	{
		m_pImpl->Load( fileName );
	}

	void ReplayDataLoader::Cleanup()
	{
		m_pImpl->Cleanup();
	}

	DisplayedReplayInfo::Entry ReplayDataLoader::GetDisplayedInfo( const std::string& fileName ) const
	{
		return m_pImpl->GetDisplayedInfo( fileName );
	}

	const ReplayDataRecord& ReplayDataLoader::GetReplayDataRecord() const
	{
		return m_pImpl->GetReplayDataRecord();
	}
}
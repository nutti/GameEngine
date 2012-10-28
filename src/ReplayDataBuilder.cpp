#include "ReplayDataBuilder.h"

#include <MAPIL/MAPIL.h>
#include <vector>

#include "Util.h"

namespace GameEngine
{

	struct ReplayDataInfo
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

		char					m_Name[ 10 ];			// ���O
		int						m_Progress;				// �i�s�x
		int						m_Score;				// �X�R�A
		int						m_Crystal;				// �N���X�^���l����
		int						m_Killed;				// ���j��
		int						m_Difficulty;			// ��Փx
		Date					m_Date;					// �쐬����
		
		StageData				m_StageData[ 5 ];		// �����X�e�[�W�f�[�^
	};

	class ReplayDataBuilder::Impl
	{
	private:
		std::vector < ButtonPushedStatus >		m_ButtonList;			// �ߋ��̃{�^���̗���
		ReplayDataInfo							m_ReplayDataInfo;		// ���v���C�f�[�^�̏��
	public:
		Impl();
		~Impl(){}
		void AddButtonState( ButtonPushedStatus status );
		void Save( const std::string& fileName );
		void Cleanup();
		void SetName( const char* pName );
		void SetProgress( int progress );
		void SetScore( int score );
		void SetCrystal( int crystal );
		void SetKilled( int killed );
		void SetDifficulty( int difficulty );
		void SetDate( const Date& date );
	};

	ReplayDataBuilder::Impl::Impl()
	{
		MAPIL::ZeroObject( &m_ReplayDataInfo, sizeof( m_ReplayDataInfo ) );
		m_ButtonList.clear();
	}

	inline void ReplayDataBuilder::Impl::AddButtonState( ButtonPushedStatus status )
	{
		m_ButtonList.push_back( status );
	}
	
	void ReplayDataBuilder::Impl::Save( const std::string& fileName )
	{
		std::vector < char > data;
		data.reserve( 30000 );		// 30,000�o�C�g��\��

		std::ofstream fOut( fileName, std::ios::binary | std::ios::out );
		if( !fOut ){
			return;
		}

		CopyArray( &data, m_ReplayDataInfo.m_Name, sizeof( m_ReplayDataInfo.m_Name ) );
		CopyInt( &data, m_ReplayDataInfo.m_Progress );
		CopyInt( &data, m_ReplayDataInfo.m_Score );
		CopyInt( &data, m_ReplayDataInfo.m_Crystal );
		CopyInt( &data, m_ReplayDataInfo.m_Killed );
		CopyInt( &data, m_ReplayDataInfo.m_Difficulty );
		CopyInt( &data, m_ReplayDataInfo.m_Date.m_Year );
		data.push_back( m_ReplayDataInfo.m_Date.m_Month );
		data.push_back( m_ReplayDataInfo.m_Date.m_Day );
		data.push_back( m_ReplayDataInfo.m_Date.m_Hour );
		data.push_back( m_ReplayDataInfo.m_Date.m_Min );
		data.push_back( m_ReplayDataInfo.m_Date.m_Sec );
		// �e�X�e�[�W�J�n���̃f�[�^��ۑ�
		for( int i = 0; i < 5; ++i ){
			ReplayDataInfo::StageData stage = m_ReplayDataInfo.m_StageData[ i ];
			CopyInt( &data, stage.m_IniPosX );
			CopyInt( &data, stage.m_IniPosY );
			CopyInt( &data, stage.m_IniHP );
			CopyInt( &data, stage.m_IniShotPower );
			CopyInt( &data, stage.m_IniScore );
			CopyInt( &data, stage.m_IniKilled );
			CopyInt( &data, stage.m_IniCrystal );
			CopyInt( &data, stage.m_IniCons );
			for( int j = 0; j < 3; ++j ){
				CopyInt( &data, stage.m_IniConsGauge[ j ] );
			}
			for( int j = 0; j < 3; ++j ){
				CopyInt( &data, stage.m_IniConsLevel[ j ] );
			}
			CopyInt( &data, stage.m_FrameTotal );
		}

		// ���̓{�^���̌���ۑ�
		int frame = 0;
		for( int i = 0; i < 5; ++i ){
			frame += m_ReplayDataInfo.m_StageData[ i ].m_FrameTotal;
		}
		CopyInt( &data, m_ButtonList.size() );
		// ���̓{�^���̕ۑ�
		for( unsigned int i = 0; i < m_ButtonList.size(); ++i ){
			data.push_back( m_ButtonList[ i ] );
		}

		// ���k
		char* pBuf = new char [ data.size() * 2 ];
		int compSize = 0;
		MAPIL::LZ lz( 200, 5 );
		lz.Compress( &data[ 0 ], data.size(), &pBuf, data.size() * 2, &compSize );
		// �V�[�U�Í���
		MAPIL::Caesar caesar( 10 );
		caesar.Encrypt( pBuf, compSize );
		// XOR�Í���
		MAPIL::XOR xor( 60 );
		xor.Encrypt( pBuf, compSize );
		fOut.write( pBuf, compSize );
		fOut.close();
		MAPIL::SafeDeleteArray( pBuf );
	}

	inline void ReplayDataBuilder::Impl::Cleanup()
	{
		MAPIL::ZeroObject( &m_ReplayDataInfo, sizeof( m_ReplayDataInfo ) );
		m_ButtonList.clear();
	}

	inline void ReplayDataBuilder::Impl::SetName( const char* pName )
	{
		memcpy( m_ReplayDataInfo.m_Name, pName, sizeof( char ) * 9 );
		m_ReplayDataInfo.m_Name[ 9 ] = '\0';
	}

	inline void ReplayDataBuilder::Impl::SetProgress( int progress )
	{
		m_ReplayDataInfo.m_Progress = progress;
	}

	inline void ReplayDataBuilder::Impl::SetScore( int score )
	{
		m_ReplayDataInfo.m_Score = score;
	}

	inline void ReplayDataBuilder::Impl::SetCrystal( int crystal )
	{
		m_ReplayDataInfo.m_Crystal = crystal;
	}

	inline void ReplayDataBuilder::Impl::SetKilled( int killed )
	{
		m_ReplayDataInfo.m_Killed = killed;
	}

	inline void ReplayDataBuilder::Impl::SetDifficulty( int difficulty )
	{
		m_ReplayDataInfo.m_Difficulty = difficulty;
	}

	inline void ReplayDataBuilder::Impl::SetDate( const Date& date )
	{
		m_ReplayDataInfo.m_Date = date;
	}

	// ----------------------------------
	// �����N���X�̌Ăяo��
	// ----------------------------------

	ReplayDataBuilder::ReplayDataBuilder() : m_pImpl( new ReplayDataBuilder::Impl )
	{
	}

	ReplayDataBuilder::~ReplayDataBuilder()
	{
	}

	void ReplayDataBuilder::AddButtonState( ButtonPushedStatus status )
	{
		m_pImpl->AddButtonState( status );
	}

	void ReplayDataBuilder::Save( const std::string& fileName )
	{
		m_pImpl->Save( fileName );
	}

	void ReplayDataBuilder::Cleanup()
	{
		m_pImpl->Cleanup();
	}

	void ReplayDataBuilder::SetName( const char* pName )
	{
		m_pImpl->SetName( pName );
	}

	void ReplayDataBuilder::SetProgress( int progress )
	{
		m_pImpl->SetProgress( progress );
	}

	void ReplayDataBuilder::SetScore( int score )
	{
		m_pImpl->SetScore( score );
	}

	void ReplayDataBuilder::SetCrystal( int crystal )
	{
		m_pImpl->SetCrystal( crystal );
	}

	void ReplayDataBuilder::SetKilled( int killed )
	{
		m_pImpl->SetKilled( killed );
	}

	void ReplayDataBuilder::SetDifficulty( int difficulty )
	{
		m_pImpl->SetDifficulty( difficulty );
	}

	void ReplayDataBuilder::SetDate( const Date& date )
	{
		m_pImpl->SetDate( date );
	}

}
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
	};

	ReplayDataBuilder::Impl::Impl()
	{
		MAPIL::ZeroObject( &m_ReplayDataInfo, sizeof( m_ReplayDataInfo ) );
		m_ButtonList.clear();
	}

	void ReplayDataBuilder::Impl::AddButtonState( ButtonPushedStatus status )
	{
		m_ButtonList.push_back( status );
	}
	
	void ReplayDataBuilder::Impl::Save( const std::string& fileName )
	{
		std::ofstream fOut( fileName, std::ios::binary | std::ios::out );

		// �X�R�A�S�̏���ۑ�
		fOut.write( m_ReplayDataInfo.m_Name, sizeof( m_ReplayDataInfo.m_Name ) );
		WriteInt( &fOut, m_ReplayDataInfo.m_Progress );
		WriteInt( &fOut, m_ReplayDataInfo.m_Score );
		WriteInt( &fOut, m_ReplayDataInfo.m_Crystal );
		WriteInt( &fOut, m_ReplayDataInfo.m_Killed );
		WriteInt( &fOut, m_ReplayDataInfo.m_Difficulty );
		WriteInt( &fOut, m_ReplayDataInfo.m_Date.m_Year );
		WriteInt( &fOut, m_ReplayDataInfo.m_Date.m_Month );
		WriteInt( &fOut, m_ReplayDataInfo.m_Date.m_Day );
		WriteInt( &fOut, m_ReplayDataInfo.m_Date.m_Hour );
		WriteInt( &fOut, m_ReplayDataInfo.m_Date.m_Min );
		WriteInt( &fOut, m_ReplayDataInfo.m_Date.m_Sec );
		// �e�X�e�[�W�J�n���̃f�[�^��ۑ�
		for( int i = 0; i < 5; ++i ){
			ReplayDataInfo::StageData stage = m_ReplayDataInfo.m_StageData[ i ];
			WriteInt( &fOut, stage.m_IniPosX );
			WriteInt( &fOut, stage.m_IniPosY );
			WriteInt( &fOut, stage.m_IniHP );
			WriteInt( &fOut, stage.m_IniShotPower );
			WriteInt( &fOut, stage.m_IniScore );
			WriteInt( &fOut, stage.m_IniKilled );
			WriteInt( &fOut, stage.m_IniCrystal );
			WriteInt( &fOut, stage.m_IniCons );
			for( int j = 0; j < 3; ++j ){
				WriteInt( &fOut, stage.m_IniConsGauge[ j ] );
			}
			for( int j = 0; j < 3; ++j ){
				WriteInt( &fOut, stage.m_IniConsLevel[ j ] );
			}
			WriteInt( &fOut, stage.m_FrameTotal );
		}

		// ���̓{�^���̌���ۑ�
		int frame = 0;
		for( int i = 0; i < 5; ++i ){
			frame += m_ReplayDataInfo.m_StageData[ i ].m_FrameTotal;
		}
		WriteInt( &fOut, m_ButtonList.size() );
		// ���̓{�^���̕ۑ�
		for( unsigned int i = 0; i < m_ButtonList.size(); ++i ){
			fOut.write( &m_ButtonList[ i ], sizeof( m_ButtonList[ i ] ) );
		}

		fOut.close();
	}

	void ReplayDataBuilder::Impl::Cleanup()
	{
		MAPIL::ZeroObject( &m_ReplayDataInfo, sizeof( m_ReplayDataInfo ) );
		m_ButtonList.clear();
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

}
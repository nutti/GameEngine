#include "EnemyPatternFileLoader.h"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <fstream>

#include <MAPIL/MAPIL.h>

#include "Util.h"

namespace GameEngine
{
	class EnemyPatternFile
	{
	private:
		typedef std::map < int, EnemyPattern >	EnemyPatternList;
		EnemyPatternList		m_EnemyPatternList;
		void Analyze( char* pData, int len );
	public:
		EnemyPatternFile();
		~EnemyPatternFile();
		void Load( const std::string& fileName );
		void Load( int archiveHandle, const std::string& filePath );
		void GetPattern( int frame, EnemyPattern* pInfo );
		bool HasPattern( int frame );
	};

	EnemyPatternFile::EnemyPatternFile() : m_EnemyPatternList()
	{
		m_EnemyPatternList.clear();
	}

	EnemyPatternFile::~EnemyPatternFile()
	{
		m_EnemyPatternList.clear();
	}

	void EnemyPatternFile::Analyze( char* pData, int len )
	{
		char* pEnd = pData + len;

		char str[ 1024 ];

		// �t�@�C�����
		while( pData != pEnd ){
			GetLineFromString( &pData, pEnd, str, sizeof( str ) );
			int count = 0;
			int frame = -100;
			EnemyPatternInfo info;
			MAPIL::ZeroObject( &info, sizeof( info ) );
			for( const char* pToken = ::strtok( str, "," ); pToken; pToken = ::strtok( 0, "," ) ){
				char s[ 1024 ];
				DeleteChar( s, sizeof( s ), pToken, ' ' );
				switch( count ){
					// �t���[��
					case 0:
						frame = ::atoi( s );
						if( frame <= 0 ){
							frame = -100;
						}
						break;
					// �GID
					case 1:
						info.m_EnemyID = ::atoi( s );
						break;
					// X���W
					case 2:
						info.m_PosX = static_cast < float > ( ::atof( s ) );
						break;
					// Y���W
					case 3:
						info.m_PosY = static_cast < float > ( ::atof( s ) );
						break;
					// ���W�X�^
					default:
						if( count < MAX_ENEMY_REGS + 3 ){
							info.m_Regs[ count - 4 ] = ::atoi( s );
						}
						break;
				}
				++count;
			}
			m_EnemyPatternList[ frame ].m_InfoList.push_back( info );
		}
	}

	void EnemyPatternFile::Load( const std::string& fileName )
	{
		// �t�@�C���ǂݍ���
		std::fstream fIn( fileName, std::ios::in );
		if( !fIn ){
			return;
		}
		int size = GetFileSize( fIn );
		char* pBuf = new char[ size ];
		LoadFile( fIn, pBuf, size );
		
		// �t�@�C�����
		Analyze( pBuf, size );

		MAPIL::SafeDeleteArray( pBuf );
		fIn.close();
	}

	void EnemyPatternFile::Load( int archiveHandle, const std::string& filePath )
	{
		// �A�[�J�C�u����t�@�C����ǂݍ���
		int size = MAPIL::GetContentsSizeOnArchiveFile( archiveHandle, filePath.c_str() );
		char* pBuf = new char [ size ];
		MAPIL::LoadDataFromArchiveFile( archiveHandle, filePath.c_str(), pBuf );

		// �t�@�C�����
		Analyze( pBuf, size );

		MAPIL::SafeDeleteArray( pBuf );
	}

	void EnemyPatternFile::GetPattern( int frame, EnemyPattern* pInfo )
	{
		*pInfo = m_EnemyPatternList[ frame ];
	}

	bool EnemyPatternFile::HasPattern( int frame )
	{
		EnemyPatternList::iterator it;
		it = m_EnemyPatternList.find( frame );
		if( it == m_EnemyPatternList.end() ){
			return false;
		}
		else{
			return true;
		}
	}


	struct EnemyPatternFileTag
	{
		std::shared_ptr < EnemyPatternFile >	m_File;
		bool									m_IsUsed;
	};

	class EnemyPatternFileLoader
	{
	private:
		std::vector < EnemyPatternFileTag >		m_FileList;
	public:
		EnemyPatternFileLoader();
		~EnemyPatternFileLoader();
		int CreateEnemyPattern( const std::string& fileName );
		int CreateEnemyPattern( int archiveHandle, const std::string& filePath );
		void DeleteEnemyPattern( int pattern );
		void GetEnemyInfo( int pattern, int frame, EnemyPattern* pInfo ) const;
		bool HasEnemyInfo( int pattern, int frame ) const;
	};

	static EnemyPatternFileLoader		g_Loader;

	EnemyPatternFileLoader::EnemyPatternFileLoader() : m_FileList()
	{
		m_FileList.clear();
	}

	EnemyPatternFileLoader::~EnemyPatternFileLoader()
	{
		m_FileList.clear();
	}

	int EnemyPatternFileLoader::CreateEnemyPattern( const std::string& fileName )
	{
		// �G�o���p�^�[���t�@�C���̓ǂݍ���
		std::shared_ptr < EnemyPatternFile > file( new EnemyPatternFile );
		file->Load( fileName );
		EnemyPatternFileTag tag;
		tag.m_File = file;
		tag.m_IsUsed = true;

		// �g���܂킹��ID���m�F
		for( int i = 0; i < m_FileList.size(); ++i ){
			if( !m_FileList[ i ].m_IsUsed ){
				m_FileList[ i ] = tag;
				return i;		// �g���܂킹��ID�𔭌�
			}
		}

		// �V�����ꏊ���m��
		int id = m_FileList.size();
		m_FileList.push_back( tag );

		return id;
	}

	int EnemyPatternFileLoader::CreateEnemyPattern( int archiveHandle, const std::string& filePath )
	{
		// �G�o���p�^�[���t�@�C���̓ǂݍ���
		std::shared_ptr < EnemyPatternFile > file( new EnemyPatternFile );
		file->Load( archiveHandle, filePath );
		EnemyPatternFileTag tag;
		tag.m_File = file;
		tag.m_IsUsed = true;

		// �g���܂킹��ID���m�F
		for( int i = 0; i < m_FileList.size(); ++i ){
			if( !m_FileList[ i ].m_IsUsed ){
				m_FileList[ i ] = tag;
				return i;		// �g���܂킹��ID�𔭌�
			}
		}

		// �V�����ꏊ���m��
		int id = m_FileList.size();
		m_FileList.push_back( tag );

		return id;
	}

	void EnemyPatternFileLoader::DeleteEnemyPattern( int id )
	{
		if( m_FileList.size() <= id ){
			return;
		}

		m_FileList[ id ].m_File = NULL;
		m_FileList[ id ].m_IsUsed = false;
	}

	void EnemyPatternFileLoader::GetEnemyInfo( int id, int frame, EnemyPattern* pInfo ) const
	{
		// �z��͈̔͊O���Q�Ƃ���
		if( m_FileList.size() <= id ){
			return;
		}
		// �w�肳�ꂽID�͑��݂��Ȃ�
		if( !m_FileList[ id ].m_IsUsed ){
			return;
		}

		m_FileList[ id ].m_File->GetPattern( frame, pInfo );
	}

	bool EnemyPatternFileLoader::HasEnemyInfo( int id, int frame ) const
	{
		// �z��͈̔͊O���Q�Ƃ���
		if( m_FileList.size() <= id ){
			return false;
		}
		// �w�肳�ꂽID�͑��݂��Ȃ�
		if( !m_FileList[ id ].m_IsUsed ){
			return false;
		}

		return m_FileList[ id ].m_File->HasPattern( frame );
	}


	int CreateStageEnemyPattern( const std::string& fileName )
	{
		return g_Loader.CreateEnemyPattern( fileName );
	}

	void DeleteStageEnemyPattern( int id )
	{
		g_Loader.DeleteEnemyPattern( id );
	}

	bool ExistEnemyPattern( int id, int frame )
	{
		return g_Loader.HasEnemyInfo( id, frame );
	}

	void GetEnemyPattern( int id, int frame, EnemyPattern* pPattern )
	{
		g_Loader.GetEnemyInfo( id, frame, pPattern );
	}

	int CreateStageEnemyPattern( int archiveHandle, const std::string& filePath )
	{
		return g_Loader.CreateEnemyPattern( archiveHandle, filePath );
	}
}
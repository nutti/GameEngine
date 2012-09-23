#include "ScriptManager.h"

#include <MAPIL/MAPIL.h>
#include <vector>
#include <fstream>

#include "ScriptCompiler.h"

namespace GameEngine
{
	// �X�N���v�g�t�@�C���\���}
	//
	//	Stage�\�����t�@�C�� ------> Stage1�\���t�@�C�� --------> ���\�[�X�X�N���v�g�t�@�C��
	//							|							|
	//							|							---> �G�X�N���v�g�t�@�C�� 0 ...
	//							|							|
	//							|							---> �G�e�X�N���v�g�t�@�C�� 0 ...
	//							|							|
	//							|							---> �X�e�[�W�X�N���v�g�t�@�C��
	//							|
	//							---> Stage2�\���t�@�C��
	//							|

	// �X�e�[�W�^�O
	struct StageTag
	{
		int				m_StageNo;		// �X�e�[�W�ԍ�
		std::string		m_FileName;		// �t�@�C����
	};

	// ScriptManager�����N���X
	class ScriptManager::Impl
	{
	private:
		std::weak_ptr < EventMediator >			m_pEventMediator;		// �C�x���g������N���X
		ScriptCompiler							m_Compiler;				// �R���p�C��

		std::vector < StageTag >				m_StageFileList;		// �X�e�[�W�t�@�C�������X�g

		int GetInt( char** p );				// �f�[�^�񂩂�A�������擾����
		int GetID( const char* pStr );		// �f�[�^�񂩂�AID���擾����
		char* GetFileName( char* pStr );	// �f�[�^�񂩂�A�t�@�C�������擾����
	public:
		Impl( std::shared_ptr < EventMediator > pEventMediator );
		~Impl(){}
		void BuildFileStructure( const std::string& fileName );
		void BuildScriptData( int stage );							// �X�N���v�g�f�[�^�̍\�z
		ScriptData GetScriptData();
	};

	int ScriptManager::Impl::GetInt( char** p )
	{
		int i = MAPIL::TransformCharIntoInt32( *p, MAPIL::BYTE_ORDER_LITTLE );
		*p += 4;

		return i;
	}

	int ScriptManager::Impl::GetID( const char* pStr )
	{
		char buf[ 5 ];		// 1000�܂Ŋi�[�\
		int pos = 0;

		while( *pStr ){
			if( *pStr == ' ' || pos > 3 ){
				break;
			}
			buf[ pos++ ] = *pStr++;
		}
		buf[ pos ] = '\0';

		return ::atoi( buf );
	}

	char* ScriptManager::Impl::GetFileName( char* pStr )
	{
		while( *pStr++ != ' ' ){
		}

		return pStr;
	}

	ScriptManager::Impl::Impl( std::shared_ptr < EventMediator > pEventMediator ) :	m_pEventMediator( pEventMediator ),
																					m_Compiler()
	{
	}

	void ScriptManager::Impl::BuildFileStructure( const std::string& fileName )
	{
		std::ifstream fIn( fileName, std::ios::in );
		if( !fIn ){
			exit( 1 );
		}

		while( !fIn.eof() ){
			char buf[ 1024 ];
			fIn.getline( buf, sizeof( buf ) );
			StageTag tag;
			tag.m_StageNo = GetID( buf );			// ID�擾
			tag.m_FileName = GetFileName( buf );	// �t�@�C�����擾
			m_StageFileList.push_back( tag );
		}

		fIn.close();
	}

	void ScriptManager::Impl::BuildScriptData( int stage )
	{
		// �t�@�C���\���X�N���v�g�̌���
		unsigned int cur = 0;
		for( ; cur < m_StageFileList.size(); ++cur ){
			if( m_StageFileList[ cur ].m_StageNo == stage ){
				break;
			}
		}
		if( cur == m_StageFileList.size() ){
			exit( 1 );	// �w�肳�ꂽ�X�e�[�W�̃t�@�C���\���X�N���v�g�͑��݂��Ȃ�
		}

		m_Compiler.Compile( m_StageFileList[ cur ].m_FileName );
	}

	ScriptData ScriptManager::Impl::GetScriptData()
	{
		return m_Compiler.GetScriptData();
	}


	// ----------------------------------
	// �����N���X�̌Ăяo��
	// ----------------------------------


	ScriptManager::ScriptManager( std::shared_ptr < EventMediator > pEventMediator ) : m_pImpl( new ScriptManager::Impl( pEventMediator ) )
	{
	}

	ScriptManager::~ScriptManager()
	{
	}

	void ScriptManager::BuildFileStructure( const std::string& fileName )
	{
		m_pImpl->BuildFileStructure( fileName );
	}

	void ScriptManager::BuildScriptData( int stage )
	{
		m_pImpl->BuildScriptData( stage );
	}

	ScriptData ScriptManager::GetScriptData()
	{
		return m_pImpl->GetScriptData();
	}
}
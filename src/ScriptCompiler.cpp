#include "ScriptCompiler.h"

#include <fstream>

#include "ScriptTypes.h"
#include "ScriptManager/Compiler.h"

namespace GameEngine
{
	// �ʂ̃X�N���v�g�t�@�C���̂��߂̍\����
	struct ScriptFileTag
	{
		int				m_ID;			// ���ʔԍ�
		std::string		m_FileName;		// �t�@�C����
	};

	// �X�N���v�g�t�@�C���̎��
	enum ScriptType
	{
		SCRIPT_TYPE_ENEMY				= 0,
		SCRIPT_TYPE_ENEMY_SHOT			= 1,
		SCRIPT_TYPE_STAGE				= 2,
		SCRIPT_TYPE_STAGE_BACKGROUND	= 3,
		SCRIPT_TYPE_RESOURCE			= 4,
	};

	// ���\�[�X�X�N���v�g�̃��\�[�X�̎��
	enum ResourceType
	{
		RESOURCE_TYPE_BGM		= 0,
		RESOURCE_TYPE_SE		= 1,
		RESOURCE_TYPE_TEXTURE	= 2,
	};

	// ScriptCompiler�����N���X
	class ScriptCompiler::Impl
	{
	private:
		std::shared_ptr < StageScriptData >					m_pStageScriptData;				// �X�e�[�W�X�N���v�g�f�[�^
		std::shared_ptr < StageBackgroundScriptData >		m_pStageBackgroundScriptData;	// �X�e�[�W�w�i�X�N���v�g�f�[�^
		std::shared_ptr < ResourceScriptData >				m_pResourceScriptData;			// ���\�[�X�X�N���v�g�f�[�^
		std::shared_ptr < EnemyScriptData >					m_pEnemyScriptData;				// �G�X�N���v�g�f�[�^
		std::shared_ptr < EnemyShotGroupScriptData >		m_pEnemyShotGroupScriptData;	// �G�V���b�g�X�N���v�g�f�[�^

		int GetID( const char* pStr );		// �f�[�^�񂩂�AID���擾����
		char* GetFileName( char* pStr );	// �f�[�^�񂩂�A�t�@�C�������擾����

		void CompileStageScript( const std::string& fileName );				// �X�e�[�W�̃X�N���v�g�̃R���p�C��
		void CompileStageBackgroundScript( const std::string& fileName );	// �X�e�[�W�w�i�̃X�N���v�g�̃R���p�C��
		void CompileResourceScript( const std::string& fileName );			// ���\�[�X�X�N���v�g�̃R���p�C��
		void CompileEnemyScript( int id, const std::string& fileName );		// �G�̃X�N���v�g�̃R���p�C��
		void CompileEnemyShotGroupScript( int id, const std::string& fileName );	// �G�e�̃X�N���v�g�̃R���p�C��
	public:
		Impl();
		~Impl(){}
		void Compile( const std::string& fileName );
		ScriptData GetScriptData();

	};

	int ScriptCompiler::Impl::GetID( const char* pStr )
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

	char* ScriptCompiler::Impl::GetFileName( char* pStr )
	{
		while( *pStr++ != ' ' ){
		}

		return pStr;
	}

	void ScriptCompiler::Impl::CompileStageScript( const std::string& fileName )
	{
		Compiler compiler;
		m_pStageScriptData.reset( new StageScriptData );
		compiler.Compile( fileName, m_pStageScriptData->m_Data );
	}

	void ScriptCompiler::Impl::CompileStageBackgroundScript( const std::string& fileName )
	{
		Compiler compiler;
		m_pStageBackgroundScriptData.reset( new StageBackgroundScriptData );
		compiler.Compile( fileName, m_pStageScriptData->m_Data );
	}

	void ScriptCompiler::Impl::CompileResourceScript( const std::string& fileName )
	{
		m_pResourceScriptData.reset( new ResourceScriptData );

		std::ifstream fIn( fileName, std::ios::in );

		while( !fIn.eof() ){
			char buf[ 1024 ];
			int type;		// �X�N���v�g�^�C�v
			fIn.getline( buf, sizeof( buf ) );
			if( !strcmp( buf, "[BGM]" ) ){
				type = RESOURCE_TYPE_BGM;
			}
			else if( !strcmp( buf, "[SE]" ) ){
				type = RESOURCE_TYPE_SE;
			}
			else if( !strcmp( buf, "[Texture]" ) ){
				type = RESOURCE_TYPE_TEXTURE;
			}
			else if( !strcmp( buf, "" ) ){
				// ����
			}
			else{
				// �t�@�C�����擾
				if( type == RESOURCE_TYPE_BGM ){
					m_pResourceScriptData->m_BGMList.insert( std::pair < int, std::string > ( GetID( buf ), GetFileName( buf ) ) );
				}
				else if( type == RESOURCE_TYPE_SE ){
					m_pResourceScriptData->m_SEList.insert( std::pair < int, std::string > ( GetID( buf ), GetFileName( buf ) ) );
				}
				else if( type == RESOURCE_TYPE_TEXTURE ){
					m_pResourceScriptData->m_TextureList.insert( std::pair < int, std::string > ( GetID( buf ), GetFileName( buf ) ) );
				}
			}
		}
	}

	void ScriptCompiler::Impl::CompileEnemyScript( int id, const std::string& fileName )
	{
		m_pEnemyScriptData->m_pElm[ id ].m_ID = id;
		Compiler compiler;
		compiler.Compile( fileName, m_pEnemyScriptData->m_pElm[ id ].m_Data );
	}

	void ScriptCompiler::Impl::CompileEnemyShotGroupScript( int id, const std::string& fileName )
	{
		m_pEnemyShotGroupScriptData->m_pElm[ id ].m_ID = id;
		Compiler compiler;
		compiler.Compile( fileName, m_pEnemyShotGroupScriptData->m_pElm[ id ].m_Data );
	}

	ScriptCompiler::Impl::Impl() : m_pStageScriptData()
	{
	}

	void ScriptCompiler::Impl::Compile( const std::string& fileName )
	{
		std::fstream fIn( fileName, std::ios::in );
		if( !fIn ){
			exit( 1 );
		}

		// �t�@�C���\���X�N���v�g����X�N���v�g�t�@�C�������擾����
		std::vector < ScriptFileTag > enemyScriptList;		// �G�̃X�N���v�g�t�@�C�����X�g
		std::vector < ScriptFileTag > enemyShotScriptList;	// �G�e�̃X�N���v�g�t�@�C�����X�g
		std::string stageScriptFileName;					// �X�e�[�W�X�N���v�g�̃t�@�C����
		std::string stageBGScriptFileName;					// �X�e�[�W�w�i�X�N���v�g�̃t�@�C����
		std::string resourceScriptFileName;					// ���\�[�X�X�N���v�g�̃t�@�C����
		while( !fIn.eof() ){
			char buf[ 1024 ];
			int type;
			fIn.getline( buf, sizeof( buf ) );
			if( !strcmp( buf, "[Enemy]" ) ){
				type = SCRIPT_TYPE_ENEMY;
			}
			else if( !strcmp( buf, "[EnemyShot]" ) ){
				type = SCRIPT_TYPE_ENEMY_SHOT;
			}
			else if( !strcmp( buf, "[Stage]" ) ){
				type = SCRIPT_TYPE_STAGE;
			}
			else if( !strcmp( buf, "[StageBackground]" ) ){
				type = SCRIPT_TYPE_STAGE_BACKGROUND;
			}
			else if( !strcmp( buf, "[Resource]" ) ){
				type = SCRIPT_TYPE_RESOURCE;
			}
			else if( !strcmp( buf, "" ) ){
				// ����
			}
			else{
				// �t�@�C�����擾
				if( type == SCRIPT_TYPE_STAGE ){
					stageScriptFileName = buf;
				}
				else if( type == SCRIPT_TYPE_STAGE_BACKGROUND ){
					stageBGScriptFileName = buf;
				}
				else if( type == SCRIPT_TYPE_RESOURCE ){
					resourceScriptFileName = buf;
				}
				else{
					ScriptFileTag tag;
					tag.m_ID = GetID( buf );
					tag.m_FileName = GetFileName( buf );
					if( type == SCRIPT_TYPE_ENEMY ){
						enemyScriptList.push_back( tag );
					}
					else if( type == SCRIPT_TYPE_ENEMY_SHOT ){
						enemyShotScriptList.push_back( tag );
					}
				}
			}
		}

		// �X�e�[�W�X�N���v�g�̓ǂݍ���
		CompileStageScript( stageScriptFileName );

		// �X�e�[�W�w�i�X�N���v�g�̓ǂݍ���
		CompileStageBackgroundScript( stageBGScriptFileName );

		// ���\�[�X�X�N���v�g�̓ǂݍ���
		CompileResourceScript( resourceScriptFileName );

		// �G�̍s���p�^�[���X�N���v�g�̓ǂݍ���
		m_pEnemyScriptData.reset( new EnemyScriptData );
		m_pEnemyScriptData->m_pElm = new EnemyScriptData::EnemyScriptDataElm[ enemyScriptList.size() ];
		for( unsigned int i = 0; i < enemyScriptList.size(); ++i ){
			CompileEnemyScript( i, enemyScriptList[ i ].m_FileName );
		}

		// �G�e�̃X�N���v�g�̓ǂݍ���
		m_pEnemyShotGroupScriptData.reset( new EnemyShotGroupScriptData );
		m_pEnemyShotGroupScriptData->m_pElm = new EnemyShotGroupScriptData::EnemyShotGroupScriptDataElm[ enemyShotScriptList.size() ];
		for( unsigned int i = 0; i < enemyShotScriptList.size(); ++i ){
			CompileEnemyShotGroupScript( i, enemyShotScriptList[ i ].m_FileName );
		}
	}

	ScriptData ScriptCompiler::Impl::GetScriptData()
	{
		ScriptData data;

		data.m_pStageScriptData = m_pStageScriptData;
		data.m_pStageBackgroundScriptData = m_pStageBackgroundScriptData;
		data.m_pEnemyScriptData = m_pEnemyScriptData;
		data.m_pEnemyShotGroupScriptData = m_pEnemyShotGroupScriptData;
		data.m_pResourceScriptData = m_pResourceScriptData;

		return data;
	}

	// ----------------------------------
	// �����N���X�̌Ăяo��
	// ----------------------------------


	ScriptCompiler::ScriptCompiler() : m_pImpl( new ScriptCompiler::Impl )
	{
	}

	ScriptCompiler::~ScriptCompiler()
	{
	}

	void ScriptCompiler::Compile( const std::string& fileName )
	{
		m_pImpl->Compile( fileName );
	}

	ScriptData ScriptCompiler::GetScriptData()
	{
		return m_pImpl->GetScriptData();
	}
}
#include "ScriptCompiler.h"

#include <fstream>

#include "ScriptTypes.h"
#include "ScriptManager/Compiler.h"

namespace GameEngine
{
	// 個別のスクリプトファイルのための構造体
	struct ScriptFileTag
	{
		int				m_ID;			// 識別番号
		std::string		m_FileName;		// ファイル名
	};

	// スクリプトファイルの種類
	enum ScriptType
	{
		SCRIPT_TYPE_ENEMY				= 0,
		SCRIPT_TYPE_ENEMY_SHOT			= 1,
		SCRIPT_TYPE_STAGE				= 2,
		SCRIPT_TYPE_STAGE_BACKGROUND	= 3,
		SCRIPT_TYPE_RESOURCE			= 4,
	};

	// リソーススクリプトのリソースの種類
	enum ResourceType
	{
		RESOURCE_TYPE_BGM		= 0,
		RESOURCE_TYPE_SE		= 1,
		RESOURCE_TYPE_TEXTURE	= 2,
	};

	// ScriptCompiler実装クラス
	class ScriptCompiler::Impl
	{
	private:
		std::shared_ptr < StageScriptData >					m_pStageScriptData;				// ステージスクリプトデータ
		std::shared_ptr < StageBackgroundScriptData >		m_pStageBackgroundScriptData;	// ステージ背景スクリプトデータ
		std::shared_ptr < ResourceScriptData >				m_pResourceScriptData;			// リソーススクリプトデータ
		std::shared_ptr < EnemyScriptData >					m_pEnemyScriptData;				// 敵スクリプトデータ
		std::shared_ptr < EnemyShotGroupScriptData >		m_pEnemyShotGroupScriptData;	// 敵ショットスクリプトデータ

		int GetID( const char* pStr );		// データ列から、IDを取得する
		char* GetFileName( char* pStr );	// データ列から、ファイル名を取得する

		void CompileStageScript( const std::string& fileName );				// ステージのスクリプトのコンパイル
		void CompileStageBackgroundScript( const std::string& fileName );	// ステージ背景のスクリプトのコンパイル
		void CompileResourceScript( const std::string& fileName );			// リソーススクリプトのコンパイル
		void CompileEnemyScript( int id, const std::string& fileName );		// 敵のスクリプトのコンパイル
		void CompileEnemyShotGroupScript( int id, const std::string& fileName );	// 敵弾のスクリプトのコンパイル
	public:
		Impl();
		~Impl(){}
		void Compile( const std::string& fileName );
		ScriptData GetScriptData();

	};

	int ScriptCompiler::Impl::GetID( const char* pStr )
	{
		char buf[ 5 ];		// 1000まで格納可能
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
			int type;		// スクリプトタイプ
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
				// 無視
			}
			else{
				// ファイル名取得
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

		// ファイル構成スクリプトからスクリプトファイル名を取得する
		std::vector < ScriptFileTag > enemyScriptList;		// 敵のスクリプトファイルリスト
		std::vector < ScriptFileTag > enemyShotScriptList;	// 敵弾のスクリプトファイルリスト
		std::string stageScriptFileName;					// ステージスクリプトのファイル名
		std::string stageBGScriptFileName;					// ステージ背景スクリプトのファイル名
		std::string resourceScriptFileName;					// リソーススクリプトのファイル名
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
				// 無視
			}
			else{
				// ファイル名取得
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

		// ステージスクリプトの読み込み
		CompileStageScript( stageScriptFileName );

		// ステージ背景スクリプトの読み込み
		CompileStageBackgroundScript( stageBGScriptFileName );

		// リソーススクリプトの読み込み
		CompileResourceScript( resourceScriptFileName );

		// 敵の行動パターンスクリプトの読み込み
		m_pEnemyScriptData.reset( new EnemyScriptData );
		m_pEnemyScriptData->m_pElm = new EnemyScriptData::EnemyScriptDataElm[ enemyScriptList.size() ];
		for( unsigned int i = 0; i < enemyScriptList.size(); ++i ){
			CompileEnemyScript( i, enemyScriptList[ i ].m_FileName );
		}

		// 敵弾のスクリプトの読み込み
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
	// 実装クラスの呼び出し
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
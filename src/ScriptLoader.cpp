#include "ScriptLoader.h"

#include <fstream>

#include "Util.h"

#include "TextureAtlasPatternFileLoader.h"

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
		SCRIPT_TYPE_EFFECT				= 5,
	};

	// リソーススクリプトのリソースの種類
	enum ScriptResourceType
	{
		SCRIPT_RESOURCE_TYPE_BGM					= 0,
		SCRIPT_RESOURCE_TYPE_SE						= 1,
		SCRIPT_RESOURCE_TYPE_TEXTURE				= 2,
		SCRIPT_RESOURCE_TYPE_MODEL					= 3,
		SCRIPT_RESOURCE_TYPE_ENEMY_PATTERN_FILE		= 4,
		SCRIPT_RESOURCE_TYPE_TEXTURE_ATLAS			= 5,
		SCRIPT_RESOURCE_TYPE_SKIN_MODEL				= 6,
	};

	// ScriptCompiler実装クラス
	class ScriptLoader::Impl
	{
	private:
		std::shared_ptr < StageScriptData >					m_pStageScriptData;		// ステージスクリプトデータ
		std::shared_ptr < StageBackgroundScriptData >		m_pStageBackgroundScriptData;	// ステージ背景スクリプトデータ
		std::shared_ptr < ResourceScriptData >				m_pResourceScriptData;	// リソーススクリプトデータ
		std::shared_ptr < EnemyScriptData >					m_pEnemyScriptData;		// 敵スクリプトデータ
		std::shared_ptr < EnemyShotGroupScriptData >		m_pEnemyShotGroupScriptData;	// 敵ショットスクリプトデータ
		std::shared_ptr < EffectScriptData >				m_pEffectScriptData;	// エフェクトスクリプトデータ

		int GetID( const char* pStr );					// データ列から、IDを取得する
		char* GetFileName( char* pStr, int column );	// データ列から、ファイル名を取得する
		int GetValue( const char* pStr, int column );

		void LoadScript( const std::string& fileName, VM::Data* pVMData );
		void LoadScript( int archiveHandle, const std::string& filePath, VM::Data* pVMData );

		void LoadStageScript( const std::string& fileName );					// ステージのスクリプトの読み込み
		void LoadStageScript( int archiveHandle, const std::string& fileName );
		void LoadStageBGScript( const std::string& fileName );					// ステージ背景のスクリプトの読み込み
		void LoadStageBGScript( int archiveHandle, const std::string& fileName );
		void LoadResourceScript( const std::string& fileName );					// リソーススクリプトの読み込み
		void LoadResourceScript( int archiveHandle, const std::string& fileName );
		void LoadEnemyScript( int id, const std::string& fileName );			// 敵のスクリプトの読み込み
		void LoadEnemyScript( int archiveHandle, int id, const std::string& fileName );
		void LoadEnemyShotGroupScript( int id, const std::string& fileName );	// 敵弾のスクリプトの読み込み
		void LoadEnemyShotGroupScript( int archiveHandle, int id, const std::string& fileName );
		void LoadEffectScript( int id, const std::string& fileName );	// エフェクトスクリプトの読み込み
		void LoadEffectScript( int archiveHandle, int id, const std::string& fileName );
		
	public:
		Impl();
		~Impl(){}
		void Load( const std::string& fileName );
		void Load( int archiveHandle, const std::string& filePath );
		ScriptData GetScriptData();

	};

	int ScriptLoader::Impl::GetID( const char* pStr )
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

	char* ScriptLoader::Impl::GetFileName( char* pStr, int column )
	{
		static char str[ 1024 ];
		int count = 0;

		for( int i = 0; i <= column; ++i ){
			count = 0;
			while( *pStr == ' ' ){
				++pStr;
			}
			while( *pStr != ' ' && *pStr != '\0' ){
				str[ count++ ] = *pStr++;
				if( count >= sizeof( str ) ){
					::exit( 1 );
				}
			}
		}

		str[ count ] = '\0';

		return str;
	}

	int ScriptLoader::Impl::GetValue( const char* pStr, int column )
	{
		char buf[ 5 ];		// 1000まで格納可能
		int pos = 0;
		int col = 0;

		while( col < column ){
			MAPIL::Assert( *pStr != '\0', -1 );
			if( *pStr == ' ' ){
				++col;
			}
			++pStr;
		}

		while( *pStr ){
			if( *pStr == ' ' || pos > 3 ){
				break;
			}
			buf[ pos++ ] = *pStr++;
		}
		buf[ pos ] = '\0';

		return ::atoi( buf );
	}

	void ScriptLoader::Impl::LoadScript( const std::string& fileName, VM::Data* pVMData )
	{
		std::fstream fStgScrIn( fileName, std::ios::binary | std::ios::in );
		int dataSize = GetFileSize( fStgScrIn );
		char* pData = new char [ dataSize ];
		fStgScrIn.read( pData, dataSize );
		// VM::Dataの再構築
		char* p = pData;
		int cmdSize = GetInt( &p );
		pVMData->m_CommandSize = cmdSize;
		pVMData->m_Command = new unsigned char [ cmdSize ];
		::memcpy( pVMData->m_Command, p, cmdSize );
		p += cmdSize;
		int entryPoint = GetInt( &p );
		pVMData->m_EntryPoint = entryPoint;
		int textSize = GetInt( &p );
		pVMData->m_TextSize = textSize;
		pVMData->m_TextBuffer = new char [ textSize ];
		::memcpy( pVMData->m_TextBuffer, p, textSize );
		p += textSize;
		int valueSize = GetInt( &p );
		pVMData->m_ValueSize = valueSize;
		MAPIL::SafeDeleteArray( pData );
	}

	void ScriptLoader::Impl::LoadScript( int archiveHandle, const std::string& filePath, VM::Data* pVMData )
	{
		// アーカイブからのデータ読み込み
		int dataSize = MAPIL::GetContentsSizeOnArchiveFile( archiveHandle, filePath.c_str() );
		char* pData = new char [ dataSize ];
		MAPIL::LoadDataFromArchiveFile( archiveHandle, filePath.c_str(), pData );
		// VM::Dataの再構築
		char* p = pData;
		int cmdSize = GetInt( &p );
		pVMData->m_CommandSize = cmdSize;
		pVMData->m_Command = new unsigned char [ cmdSize ];
		::memcpy( pVMData->m_Command, p, cmdSize );
		p += cmdSize;
		int entryPoint = GetInt( &p );
		pVMData->m_EntryPoint = entryPoint;
		int textSize = GetInt( &p );
		pVMData->m_TextSize = textSize;
		pVMData->m_TextBuffer = new char [ textSize ];
		::memcpy( pVMData->m_TextBuffer, p, textSize );
		p += textSize;
		int valueSize = GetInt( &p );
		pVMData->m_ValueSize = valueSize;
		MAPIL::SafeDeleteArray( pData );
	}

	void ScriptLoader::Impl::LoadStageScript( const std::string& fileName )
	{
		m_pStageScriptData.reset( new StageScriptData );
		LoadScript( fileName, &m_pStageScriptData->m_Data );
	}

	void ScriptLoader::Impl::LoadStageScript( int archiveHandle, const std::string& fileName )
	{
		m_pStageScriptData.reset( new StageScriptData );
		LoadScript( archiveHandle, fileName, &m_pStageScriptData->m_Data );
	}

	void ScriptLoader::Impl::LoadStageBGScript( const std::string& fileName )
	{
		m_pStageBackgroundScriptData.reset( new StageBackgroundScriptData );
		LoadScript( fileName, &m_pStageBackgroundScriptData->m_Data );
	}

	void ScriptLoader::Impl::LoadStageBGScript( int archiveHandle, const std::string& fileName )
	{
		m_pStageBackgroundScriptData.reset( new StageBackgroundScriptData );
		LoadScript( archiveHandle, fileName, &m_pStageBackgroundScriptData->m_Data );
	}

	void ScriptLoader::Impl::LoadResourceScript( const std::string& fileName )
	{
		m_pResourceScriptData.reset( new ResourceScriptData );

		std::ifstream fIn( fileName, std::ios::in );

		while( !fIn.eof() ){
			char buf[ 1024 ];
			int type;		// スクリプトタイプ
			fIn.getline( buf, sizeof( buf ) );
			if( !strcmp( buf, "[BGM]" ) ){
				type = SCRIPT_RESOURCE_TYPE_BGM;
			}
			else if( !strcmp( buf, "[SE]" ) ){
				type = SCRIPT_RESOURCE_TYPE_SE;
			}
			else if( !strcmp( buf, "[Texture]" ) ){
				type = SCRIPT_RESOURCE_TYPE_TEXTURE;
			}
			else if( !strcmp( buf, "[Model]" ) ){
				type = SCRIPT_RESOURCE_TYPE_MODEL;
			}
			else if( !strcmp( buf, "[EnemyPatternFile]" ) ){
				type = SCRIPT_RESOURCE_TYPE_ENEMY_PATTERN_FILE;
			}
			else if( !strcmp( buf, "[Texture Atlas]" ) ){
				type = SCRIPT_RESOURCE_TYPE_TEXTURE_ATLAS;
			}
			else if( !strcmp( buf, "[Skin Model]" ) ){
				type = SCRIPT_RESOURCE_TYPE_SKIN_MODEL;
			}
			else if( !strcmp( buf, "" ) ){
				// 無視
			}
			else{
				// ファイル名取得
				if( type == SCRIPT_RESOURCE_TYPE_BGM ){
					m_pResourceScriptData->m_BGMList.insert( std::pair < int, std::string > ( GetID( buf ), GetFileName( buf, 1 ) ) );
				}
				else if( type == SCRIPT_RESOURCE_TYPE_SE ){
					m_pResourceScriptData->m_SEList.insert( std::pair < int, std::string > ( GetID( buf ), GetFileName( buf, 1 ) ) );
				}
				else if( type == SCRIPT_RESOURCE_TYPE_TEXTURE ){
					m_pResourceScriptData->m_TextureList.insert( std::pair < int, std::string > ( GetID( buf ), GetFileName( buf, 1 ) ) );
				}
				else if( type == SCRIPT_RESOURCE_TYPE_TEXTURE_ATLAS ){
					TextureAtlasPatternFileLoader loader;
					std::string str = GetFileName( buf, 0 );
					loader.Load( str );
					int atlasTotal = loader.GetAtlasTotal();
					for( int i = 0; i < atlasTotal; ++i ){
						int id = loader.GetID( i );
						ResourceMap::TextureAtlas atlas;
						atlas.m_TexID = loader.GetTexID( i );
						atlas.m_X = loader.GetStartX( i );
						atlas.m_Y = loader.GetStartY( i );
						atlas.m_Width = loader.GetWidth( i );
						atlas.m_Height = loader.GetHeight( i );
						m_pResourceScriptData->m_TexAtlasList.insert( std::pair < int, ResourceMap::TextureAtlas > ( id, atlas ) );
					}
				}
				else if( type == SCRIPT_RESOURCE_TYPE_MODEL ){
					ResourceScriptData::ModelResourceData data;
					data.m_ModelFileName = GetFileName( buf, 1 );
					data.m_TextureFileName = GetFileName( buf, 2 );
					m_pResourceScriptData->m_ModelList.insert( std::pair < int, ResourceScriptData::ModelResourceData > ( GetID( buf ), data ) );
				}
				else if( type == SCRIPT_RESOURCE_TYPE_ENEMY_PATTERN_FILE ){
					m_pResourceScriptData->m_EnemyPatternList.insert( std::pair < int, std::string > ( GetID( buf ), GetFileName( buf, 1 ) ) );
				}
				else if( type == SCRIPT_RESOURCE_TYPE_SKIN_MODEL ){
					ResourceScriptData::SkinModelResourceData data;
					data.m_ModelFileName = GetFileName( buf, 1 );
					data.m_TextureFileName = GetFileName( buf, 2 );
					m_pResourceScriptData->m_SkinModelList.insert( std::pair < int, ResourceScriptData::SkinModelResourceData > ( GetID( buf ), data ) );
				}
			}
		}
	}

	void ScriptLoader::Impl::LoadResourceScript( int archiveHandle, const std::string& fileName )
	{
		m_pResourceScriptData.reset( new ResourceScriptData );

		int size = MAPIL::GetContentsSizeOnArchiveFile( archiveHandle, fileName.c_str() );
		char* pData = new char [ size ];
		MAPIL::LoadDataFromArchiveFile( archiveHandle, fileName.c_str(), pData );
		char* pBegin = pData;
		char* pEnd = pData + size;

		while( pData != pEnd ){
			char buf[ 1024 ];
			int type;		// スクリプトタイプ
			GetLineFromString( &pData, pEnd, buf, sizeof( buf ) );
			if( !strcmp( buf, "[BGM]" ) ){
				type = SCRIPT_RESOURCE_TYPE_BGM;
			}
			else if( !strcmp( buf, "[SE]" ) ){
				type = SCRIPT_RESOURCE_TYPE_SE;
			}
			else if( !strcmp( buf, "[Texture]" ) ){
				type = SCRIPT_RESOURCE_TYPE_TEXTURE;
			}
			else if( !strcmp( buf, "[Model]" ) ){
				type = SCRIPT_RESOURCE_TYPE_MODEL;
			}
			else if( !strcmp( buf, "[EnemyPatternFile]" ) ){
				type = SCRIPT_RESOURCE_TYPE_ENEMY_PATTERN_FILE;
			}
			else if( !strcmp( buf, "[Texture Atlas]" ) ){
				type = SCRIPT_RESOURCE_TYPE_TEXTURE_ATLAS;
			}
			else if( !strcmp( buf, "[Skin Model]" ) ){
				type = SCRIPT_RESOURCE_TYPE_SKIN_MODEL;
			}
			else if( !strcmp( buf, "" ) ){
				// 無視
			}
			else{
				// ファイル名取得
				if( type == SCRIPT_RESOURCE_TYPE_BGM ){
					m_pResourceScriptData->m_BGMList.insert( std::pair < int, std::string > ( GetID( buf ), GetFileName( buf, 1 ) ) );
				}
				else if( type == SCRIPT_RESOURCE_TYPE_SE ){
					m_pResourceScriptData->m_SEList.insert( std::pair < int, std::string > ( GetID( buf ), GetFileName( buf, 1 ) ) );
				}
				else if( type == SCRIPT_RESOURCE_TYPE_TEXTURE ){
					m_pResourceScriptData->m_TextureList.insert( std::pair < int, std::string > ( GetID( buf ), GetFileName( buf, 1 ) ) );
				}
				else if( type == SCRIPT_RESOURCE_TYPE_TEXTURE_ATLAS ){
					TextureAtlasPatternFileLoader loader;
					std::string str = GetFileName( buf, 0 );
					loader.Load( str );
					int atlasTotal = loader.GetAtlasTotal();
					for( int i = 0; i < atlasTotal; ++i ){
						int id = loader.GetID( i );
						ResourceMap::TextureAtlas atlas;
						atlas.m_TexID = loader.GetTexID( i );
						atlas.m_X = loader.GetStartX( i );
						atlas.m_Y = loader.GetStartY( i );
						atlas.m_Width = loader.GetWidth( i );
						atlas.m_Height = loader.GetHeight( i );
						m_pResourceScriptData->m_TexAtlasList.insert( std::pair < int, ResourceMap::TextureAtlas > ( id, atlas ) );
					}
				}
				else if( type == SCRIPT_RESOURCE_TYPE_MODEL ){
					ResourceScriptData::ModelResourceData data;
					data.m_ModelFileName = GetFileName( buf, 1 );
					data.m_TextureFileName = GetFileName( buf, 2 );
					m_pResourceScriptData->m_ModelList.insert( std::pair < int, ResourceScriptData::ModelResourceData > ( GetID( buf ), data ) );
				}
				else if( type == SCRIPT_RESOURCE_TYPE_ENEMY_PATTERN_FILE ){
					m_pResourceScriptData->m_EnemyPatternList.insert( std::pair < int, std::string > ( GetID( buf ), GetFileName( buf, 1 ) ) );
				}
				else if( type == SCRIPT_RESOURCE_TYPE_SKIN_MODEL ){
					ResourceScriptData::SkinModelResourceData data;
					data.m_ModelFileName = GetFileName( buf, 1 );
					data.m_TextureFileName = GetFileName( buf, 2 );
					m_pResourceScriptData->m_SkinModelList.insert( std::pair < int, ResourceScriptData::SkinModelResourceData > ( GetID( buf ), data ) );
				}
			}
		}

		MAPIL::SafeDeleteArray( pBegin );
	}

	void ScriptLoader::Impl::LoadEnemyScript( int id, const std::string& fileName )
	{
		m_pEnemyScriptData->m_pElm[ id ].m_ID = id;
		LoadScript( fileName, &m_pEnemyScriptData->m_pElm[ id ].m_Data );
	}

	void ScriptLoader::Impl::LoadEnemyScript( int archiveHandle, int id, const std::string& fileName )
	{
		m_pEnemyScriptData->m_pElm[ id ].m_ID = id;
		LoadScript( archiveHandle, fileName, &m_pEnemyScriptData->m_pElm[ id ].m_Data );
	}

	void ScriptLoader::Impl::LoadEnemyShotGroupScript( int id, const std::string& fileName )
	{
		m_pEnemyShotGroupScriptData->m_pElm[ id ].m_ID = id;
		LoadScript( fileName, &m_pEnemyShotGroupScriptData->m_pElm[ id ].m_Data );
	}

	void ScriptLoader::Impl::LoadEnemyShotGroupScript( int archiveHandle, int id, const std::string& fileName )
	{
		m_pEnemyShotGroupScriptData->m_pElm[ id ].m_ID = id;
		LoadScript( archiveHandle, fileName, &m_pEnemyShotGroupScriptData->m_pElm[ id ].m_Data );
	}

	void ScriptLoader::Impl::LoadEffectScript( int id, const std::string& fileName )
	{
		m_pEffectScriptData->m_pElm[ id ].m_ID = id;
		LoadScript( fileName, &m_pEffectScriptData->m_pElm[ id ].m_Data );
	}

	void ScriptLoader::Impl::LoadEffectScript( int archiveHandle, int id, const std::string& fileName )
	{
		m_pEffectScriptData->m_pElm[ id ].m_ID = id;
		LoadScript( archiveHandle, fileName, &m_pEffectScriptData->m_pElm[ id ].m_Data );
	}

	ScriptLoader::Impl::Impl()
	{
	}

	void ScriptLoader::Impl::Load( const std::string& fileName )
	{
		std::fstream fIn( fileName, std::ios::in );
		if( !fIn ){
			exit( 1 );
		}

		// ファイル構成スクリプトからスクリプトファイル名を取得する
		std::vector < ScriptFileTag > enemyScriptList;		// 敵のスクリプトファイルリスト
		std::vector < ScriptFileTag > enemyShotScriptList;	// 敵弾のスクリプトファイルリスト
		std::vector < ScriptFileTag > effectScriptList;		// エフェクトのスクリプトファイルリスト
		std::string stageScriptFileName;					// ステージスクリプトのファイル名
		std::string stageScriptBGFileName;					// ステージ背景スクリプトのファイル名
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
			else if( !strcmp( buf, "[Effect]" ) ){
				type = SCRIPT_TYPE_EFFECT;
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
					stageScriptBGFileName = buf;
				}
				else if( type == SCRIPT_TYPE_RESOURCE ){
					resourceScriptFileName = buf;
				}
				else{
					ScriptFileTag tag;
					tag.m_ID = GetID( buf );
					tag.m_FileName = GetFileName( buf, 1 );
					if( type == SCRIPT_TYPE_ENEMY ){
						enemyScriptList.push_back( tag );
					}
					else if( type == SCRIPT_TYPE_ENEMY_SHOT ){
						enemyShotScriptList.push_back( tag );
					}
					else if( type == SCRIPT_TYPE_EFFECT ){
						effectScriptList.push_back( tag );
					}
				}
			}
		}

		// ステージスクリプトの読み込み
		LoadStageScript( stageScriptFileName );

		// ステージ背景スクリプトの読み込み
		LoadStageBGScript( stageScriptBGFileName );

		// リソーススクリプトの読み込み
		LoadResourceScript( resourceScriptFileName );

		// 敵の行動パターンスクリプトの読み込み
		m_pEnemyScriptData.reset( new EnemyScriptData );
		m_pEnemyScriptData->m_pElm = new EnemyScriptData::EnemyScriptDataElm[ enemyScriptList.size() ];
		for( unsigned int i = 0; i < enemyScriptList.size(); ++i ){
			LoadEnemyScript( i, enemyScriptList[ i ].m_FileName );
		}

		// 敵弾のスクリプトの読み込み
		m_pEnemyShotGroupScriptData.reset( new EnemyShotGroupScriptData );
		m_pEnemyShotGroupScriptData->m_pElm = new EnemyShotGroupScriptData::EnemyShotGroupScriptDataElm[ enemyShotScriptList.size() ];
		for( unsigned int i = 0; i < enemyShotScriptList.size(); ++i ){
			LoadEnemyShotGroupScript( i, enemyShotScriptList[ i ].m_FileName );
		}

		// エフェクトのスクリプトの読み込み
		m_pEffectScriptData.reset( new EffectScriptData );
		m_pEffectScriptData->m_pElm = new EffectScriptData::EffectScriptDataElm[ effectScriptList.size() ];
		for( unsigned int i = 0; i < effectScriptList.size(); ++i ){
			LoadEffectScript( i, effectScriptList[ i ].m_FileName );
		}
	}

	void ScriptLoader::Impl::Load( int archiveHandle, const std::string& filePath )
	{
		int size = MAPIL::GetContentsSizeOnArchiveFile( archiveHandle, filePath.c_str() );
		char* pData = new char [ size ];
		MAPIL::LoadDataFromArchiveFile( archiveHandle, filePath.c_str(), pData );
		char* pBegin = pData;
		char* pEnd = pData + size;

		// ファイル構成スクリプトからスクリプトファイル名を取得する
		std::vector < ScriptFileTag > enemyScriptList;		// 敵のスクリプト
		std::vector < ScriptFileTag > enemyShotScriptList;	// 敵弾のスクリプト
		std::vector < ScriptFileTag > effectScriptList;		// エフェクトのスクリプトファイルリスト
		char stageScriptFileName[ 1024 ];					// ステージスクリプトのファイル名
		char stageScriptBGFileName[ 1024 ];					// ステージ背景スクリプトのファイル名
		char resourceScriptFileName[ 1024 ];				// リソーススクリプトのファイル名
		int type;		// スクリプトタイプ
		while( pData != pEnd ){
			char buf[ 1024 ];
			GetLineFromString( &pData, pEnd, buf, sizeof( buf ) );
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
			else if( !strcmp( buf, "[Effect]" ) ){
				type = SCRIPT_TYPE_EFFECT;
			}
			else if( !strcmp( buf, "" ) ){
				// 無視
			}
			else{
				// ファイル名取得
				if( type == SCRIPT_TYPE_STAGE ){
					::strcpy( stageScriptFileName, buf );
				}
				else if( type == SCRIPT_TYPE_STAGE_BACKGROUND ){
					::strcpy( stageScriptBGFileName, buf );
				}
				else if( type == SCRIPT_TYPE_RESOURCE ){
					::strcpy( resourceScriptFileName, buf );
				}
				else{
					ScriptFileTag tag;
					tag.m_ID = GetID( buf );
					tag.m_FileName = GetFileName( buf, 1 );
					if( type == SCRIPT_TYPE_ENEMY ){
						enemyScriptList.push_back( tag );
					}
					else if( type == SCRIPT_TYPE_ENEMY_SHOT ){
						enemyShotScriptList.push_back( tag );
					}
					else if( type == SCRIPT_TYPE_EFFECT ){
						effectScriptList.push_back( tag );
					}
				}
			}
		}

		

		// ステージスクリプトの読み込み
		LoadStageScript( archiveHandle, stageScriptFileName );

		// ステージ背景スクリプトの読み込み
		LoadStageBGScript( archiveHandle, stageScriptBGFileName );

		// リソーススクリプトの読み込み
		LoadResourceScript( archiveHandle, resourceScriptFileName );

		// 敵の行動パターンスクリプトの読み込み
		m_pEnemyScriptData.reset( new EnemyScriptData );
		m_pEnemyScriptData->m_pElm = new EnemyScriptData::EnemyScriptDataElm[ enemyScriptList.size() ];
		for( unsigned int i = 0; i < enemyScriptList.size(); ++i ){
			LoadEnemyScript( archiveHandle, i, enemyScriptList[ i ].m_FileName );
		}

		// 敵弾のスクリプトの読み込み
		m_pEnemyShotGroupScriptData.reset( new EnemyShotGroupScriptData );
		m_pEnemyShotGroupScriptData->m_pElm = new EnemyShotGroupScriptData::EnemyShotGroupScriptDataElm[ enemyShotScriptList.size() ];
		for( unsigned int i = 0; i < enemyShotScriptList.size(); ++i ){
			LoadEnemyShotGroupScript( archiveHandle, i, enemyShotScriptList[ i ].m_FileName );
		}

		// エフェクトのスクリプトの読み込み
		m_pEffectScriptData.reset( new EffectScriptData );
		m_pEffectScriptData->m_pElm = new EffectScriptData::EffectScriptDataElm[ effectScriptList.size() ];
		for( unsigned int i = 0; i < effectScriptList.size(); ++i ){
			LoadEffectScript( archiveHandle, i, effectScriptList[ i ].m_FileName );
		}

		MAPIL::SafeDeleteArray( pBegin );
	}

	ScriptData ScriptLoader::Impl::GetScriptData()
	{
		ScriptData data;

		data.m_pStageScriptData = m_pStageScriptData;
		data.m_pStageBackgroundScriptData = m_pStageBackgroundScriptData;
		data.m_pEnemyScriptData = m_pEnemyScriptData;
		data.m_pEnemyShotGroupScriptData = m_pEnemyShotGroupScriptData;
		data.m_pResourceScriptData = m_pResourceScriptData;
		data.m_pEffectScriptData = m_pEffectScriptData;

		return data;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------


	ScriptLoader::ScriptLoader() : m_pImpl( new ScriptLoader::Impl )
	{
	}

	ScriptLoader::~ScriptLoader()
	{
	}

	void ScriptLoader::Load( const std::string& fileName )
	{
		m_pImpl->Load( fileName );
	}

	void ScriptLoader::Load( int archiveHandle, const std::string& filePath )
	{
		m_pImpl->Load( archiveHandle, filePath );
	}

	ScriptData ScriptLoader::GetScriptData()
	{
		return m_pImpl->GetScriptData();
	}
}
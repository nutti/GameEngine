#include "ScriptManager.h"

#include <MAPIL/MAPIL.h>
#include <vector>
#include <fstream>

#include "ScriptCompiler.h"
#include "ScriptLoader.h"
#include "Util.h"

namespace GameEngine
{
	// スクリプトファイル構成図
	//
	//	Stage構成情報ファイル ------> Stage1構成ファイル --------> リソーススクリプトファイル
	//							|							|
	//							|							---> 敵スクリプトファイル 0 ...
	//							|							|
	//							|							---> 敵弾スクリプトファイル 0 ...
	//							|							|
	//							|							---> ステージスクリプトファイル
	//							|
	//							---> Stage2構成ファイル
	//							|

	// ステージタグ
	struct StageTag
	{
		int				m_StageNo;		// ステージ番号
		std::string		m_FileName;		// ファイル名
	};

	// ScriptManager実装クラス
	class ScriptManager::Impl
	{
	private:
		std::weak_ptr < EventMediator >			m_pEventMediator;		// イベント仲介役クラス
		ScriptCompiler							m_Compiler;				// コンパイラ
		ScriptLoader							m_Loader;				// ローダ
		ScriptFileType							m_ScriptFileType;		// スクリプトファイルタイプ

		std::vector < StageTag >				m_StageFileList;		// ステージファイル名リスト

		int GetInt( char** p );				// データ列から、整数を取得する
		int GetID( const char* pStr );		// データ列から、IDを取得する
		char* GetFileName( char* pStr );	// データ列から、ファイル名を取得する
	public:
		Impl( std::shared_ptr < EventMediator > pEventMediator );
		~Impl(){}
		void BuildFileStructure( const std::string& fileName, ScriptFileType type );
		void BuildScriptData( int stage );							// スクリプトデータの構築
		ScriptData GetScriptData();
		void BuildFileStructure( int archiveHandle, const std::string& fileName, ScriptFileType type );
		void BuildScriptData( int archiveHandle, int stage );
	};

	int ScriptManager::Impl::GetInt( char** p )
	{
		int i = MAPIL::TransformCharIntoInt32( *p, MAPIL::BYTE_ORDER_LITTLE );
		*p += 4;

		return i;
	}

	int ScriptManager::Impl::GetID( const char* pStr )
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

	char* ScriptManager::Impl::GetFileName( char* pStr )
	{
		while( *pStr++ != ' ' ){
		}

		return pStr;
	}

	ScriptManager::Impl::Impl( std::shared_ptr < EventMediator > pEventMediator ) :	m_pEventMediator( pEventMediator ),
																					m_Compiler(),
																					m_Loader(),
																					m_ScriptFileType( SCRIPT_FILE_TYPE_UNKNOWN )
	{
	}

	void ScriptManager::Impl::BuildFileStructure( const std::string& fileName, ScriptFileType type )
	{
		m_ScriptFileType = type;

		std::ifstream fIn( fileName, std::ios::in );
		if( !fIn ){
			exit( 1 );
		}

		while( !fIn.eof() ){
			char buf[ 1024 ];
			fIn.getline( buf, sizeof( buf ) );
			StageTag tag;
			tag.m_StageNo = GetID( buf );			// ID取得
			tag.m_FileName = GetFileName( buf );	// ファイル名取得
			m_StageFileList.push_back( tag );
		}

		fIn.close();
	}

	void ScriptManager::Impl::BuildScriptData( int stage )
	{
		// ファイル構成スクリプトの検索
		unsigned int cur = 0;
		for( ; cur < m_StageFileList.size(); ++cur ){
			if( m_StageFileList[ cur ].m_StageNo == stage ){
				break;
			}
		}
		if( cur == m_StageFileList.size() ){
			exit( 1 );	// 指定されたステージのファイル構成スクリプトは存在しない
		}

		if( m_ScriptFileType == SCRIPT_FILE_TYPE_SOURCE_CODE ){
			m_Compiler.Compile( m_StageFileList[ cur ].m_FileName );
		}
		else if( m_ScriptFileType == SCRIPT_FILE_TYPE_BINARY ){
			m_Loader.Load( m_StageFileList[ cur ].m_FileName );
		}
		else{
			exit( 1 );		// 不明なファイル形式
		}
	}

	ScriptData ScriptManager::Impl::GetScriptData()
	{
		if( m_ScriptFileType == SCRIPT_FILE_TYPE_SOURCE_CODE ){
			return m_Compiler.GetScriptData();
		}
		else if( m_ScriptFileType == SCRIPT_FILE_TYPE_BINARY ){
			return m_Loader.GetScriptData();
		}
		
		ScriptData data;
		return data;
		
	}

	void ScriptManager::Impl::BuildFileStructure( int archiveHandle, const std::string& fileName, ScriptFileType type )
	{
		m_ScriptFileType = type;
		if( m_ScriptFileType != SCRIPT_FILE_TYPE_BINARY ){
			exit( -1 );
		}

		int fileSize = MAPIL::GetContentsSizeOnArchiveFile( archiveHandle, fileName.c_str() );
		char* pData = new char [ fileSize ];
		char* pBegin = pData;
		char* pEnd = pData + fileSize;
		MAPIL::LoadDataFromArchiveFile( archiveHandle, fileName.c_str(), pData );

		while( pData != pEnd ){
			char buf[ 1024 ];
			GetLineFromString( &pData, pEnd, buf, sizeof( buf ) );
			StageTag tag;
			tag.m_StageNo = GetID( buf );
			tag.m_FileName = GetFileName( buf );
			m_StageFileList.push_back( tag );
		}

		MAPIL::SafeDeleteArray( pBegin );
	}

	void ScriptManager::Impl::BuildScriptData( int archiveHandle, int stage )
	{
		if( m_ScriptFileType != SCRIPT_FILE_TYPE_BINARY ){
			exit( -1 );
		}

		// ファイル構成スクリプトの検索
		unsigned int cur = 0;
		for( ; cur < m_StageFileList.size(); ++cur ){
			if( m_StageFileList[ cur ].m_StageNo == stage ){
				break;
			}
		}
		if( cur == m_StageFileList.size() ){
			exit( 1 );	// 指定されたステージのファイル構成スクリプトは存在しない
		}

		if( m_ScriptFileType == SCRIPT_FILE_TYPE_BINARY ){
			m_Loader.Load( archiveHandle, m_StageFileList[ cur ].m_FileName );
		}
	}


	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------


	ScriptManager::ScriptManager( std::shared_ptr < EventMediator > pEventMediator ) : m_pImpl( new ScriptManager::Impl( pEventMediator ) )
	{
	}

	ScriptManager::~ScriptManager()
	{
	}

	void ScriptManager::BuildFileStructure( const std::string& fileName, ScriptFileType type )
	{
		m_pImpl->BuildFileStructure( fileName, type );
	}

	void ScriptManager::BuildScriptData( int stage )
	{
		m_pImpl->BuildScriptData( stage );
	}

	ScriptData ScriptManager::GetScriptData()
	{
		return m_pImpl->GetScriptData();
	}

	void ScriptManager::BuildFileStructure( int archiveHandle, const std::string& fileName, ScriptFileType type )
	{
		return m_pImpl->BuildFileStructure( archiveHandle, fileName, type );
	}

	void ScriptManager::BuildScriptData( int archiveHandle, int stage )
	{
		m_pImpl->BuildScriptData( archiveHandle, stage );
	}
}
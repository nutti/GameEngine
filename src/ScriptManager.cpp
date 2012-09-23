#include "ScriptManager.h"

#include <MAPIL/MAPIL.h>
#include <vector>
#include <fstream>

#include "ScriptCompiler.h"

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

		std::vector < StageTag >				m_StageFileList;		// ステージファイル名リスト

		int GetInt( char** p );				// データ列から、整数を取得する
		int GetID( const char* pStr );		// データ列から、IDを取得する
		char* GetFileName( char* pStr );	// データ列から、ファイル名を取得する
	public:
		Impl( std::shared_ptr < EventMediator > pEventMediator );
		~Impl(){}
		void BuildFileStructure( const std::string& fileName );
		void BuildScriptData( int stage );							// スクリプトデータの構築
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

		m_Compiler.Compile( m_StageFileList[ cur ].m_FileName );
	}

	ScriptData ScriptManager::Impl::GetScriptData()
	{
		return m_Compiler.GetScriptData();
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
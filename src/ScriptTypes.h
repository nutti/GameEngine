#ifndef INCLUDED_GAMEENGINE_SCRIPTTYPES_H
#define INCLUDED_GAMEENGINE_SCRIPTTYPES_H

#include "ScriptManager/VM.h"
#include "ResourceTypes.h"

#include <MAPIL/MAPIL.h>
#include <vector>
#include <map>

namespace GameEngine
{
	

	struct StageScriptData
	{
		int				m_ID;		// 識別番号
		VM::Data		m_Data;		// スクリプトデータ本体
	};

	struct EnemyScriptData
	{
		struct EnemyScriptDataElm
		{
			int			m_ID;		// 識別番号
			VM::Data	m_Data;		// スクリプトデータ本体
		};
		//std::vector < EnemyScriptDataElm >		m_Elm;
		EnemyScriptDataElm*		m_pElm;
		~EnemyScriptData()
		{
			MAPIL::SafeDeleteArray( m_pElm );
		}
	};

	struct EnemyShotGroupScriptData
	{
		struct EnemyShotGroupScriptDataElm
		{
			int			m_ID;		// 識別番号
			VM::Data	m_Data;		// スクリプトデータ本体
		};
		EnemyShotGroupScriptDataElm*		m_pElm;
		~EnemyShotGroupScriptData()
		{
			MAPIL::SafeDeleteArray( m_pElm );
		}
	};
	
	struct EffectScriptData
	{
		struct EffectScriptDataElm
		{
			int			m_ID;		// 識別番号
			VM::Data	m_Data;		// スクリプトデータ本体
		};
		EffectScriptDataElm*		m_pElm;
		~EffectScriptData()
		{
			MAPIL::SafeDeleteArray( m_pElm );
		}
	};

	struct StageBackgroundScriptData
	{
		int				m_ID;		// 識別番号
		VM::Data		m_Data;		// スクリプトデータ本体
	};

	struct ResourceScriptData
	{
		struct ModelResourceData
		{
			std::string				m_ModelFileName;
			std::string				m_TextureFileName;
		};
		typedef ModelResourceData	SkinModelResourceData;
		std::map < int, std::string >			m_BGMList;
		std::map < int, std::string >			m_SEList;
		std::map < int, std::string >			m_TextureList;
		std::map < int, ModelResourceData >		m_ModelList;
		std::map < int, std::string >			m_EnemyPatternList;
		std::map < int, ResourceMap::TextureAtlas >		m_TexAtlasList;
		std::map < int, SkinModelResourceData >		m_SkinModelList;
	};

	// メッセージ通信用スクリプトデータ
	struct ScriptData
	{
		std::shared_ptr < StageScriptData >					m_pStageScriptData;
		std::shared_ptr < StageBackgroundScriptData >		m_pStageBackgroundScriptData;
		std::shared_ptr < EnemyScriptData >					m_pEnemyScriptData;
		std::shared_ptr < EnemyShotGroupScriptData >		m_pEnemyShotGroupScriptData;
		std::shared_ptr < ResourceScriptData >				m_pResourceScriptData;
		std::shared_ptr < EffectScriptData >				m_pEffectScriptData;
	};

	// スクリプトのファイル形式
	enum ScriptFileType
	{
		SCRIPT_FILE_TYPE_UNKNOWN			= 0,		// 不明な形式
		SCRIPT_FILE_TYPE_SOURCE_CODE		= 1,		// ソースコード形式
		SCRIPT_FILE_TYPE_BINARY				= 2,		// バイナリ形式
	};
}

#endif
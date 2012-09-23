#ifndef INCLUDED_GAMEENGINE_SCRIPTTYPES_H
#define INCLUDED_GAMEENGINE_SCRIPTTYPES_H

#include "ScriptManager/VM.h"

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

	struct EnemyShotScriptData
	{
		struct EnemyShotScriptDataElm
		{
			int			m_ID;		// 識別番号
			VM::Data	m_Data;		// スクリプトデータ本体
		};
		EnemyShotScriptDataElm*		m_pElm;
		~EnemyShotScriptData()
		{
			MAPIL::SafeDeleteArray( m_pElm );
		}
	};

	struct ResourceScriptData
	{
		std::map < int, std::string >		m_BGMList;
		std::map < int, std::string >		m_SEList;
		std::map < int, std::string >		m_TextureList;
	};

	// メッセージ通信用スクリプトデータ
	struct ScriptData
	{
		std::shared_ptr < StageScriptData >			m_pStageScriptData;
		std::shared_ptr < EnemyScriptData >			m_pEnemyScriptData;
		std::shared_ptr < EnemyShotScriptData >		m_pEnemyShotScriptData;
		std::shared_ptr < ResourceScriptData >		m_pResourceScriptData;
	};
}

#endif
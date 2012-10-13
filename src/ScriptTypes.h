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
		int				m_ID;		// ���ʔԍ�
		VM::Data		m_Data;		// �X�N���v�g�f�[�^�{��
	};

	struct EnemyScriptData
	{
		struct EnemyScriptDataElm
		{
			int			m_ID;		// ���ʔԍ�
			VM::Data	m_Data;		// �X�N���v�g�f�[�^�{��
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
			int			m_ID;		// ���ʔԍ�
			VM::Data	m_Data;		// �X�N���v�g�f�[�^�{��
		};
		EnemyShotGroupScriptDataElm*		m_pElm;
		~EnemyShotGroupScriptData()
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

	// ���b�Z�[�W�ʐM�p�X�N���v�g�f�[�^
	struct ScriptData
	{
		std::shared_ptr < StageScriptData >					m_pStageScriptData;
		std::shared_ptr < EnemyScriptData >					m_pEnemyScriptData;
		std::shared_ptr < EnemyShotGroupScriptData >		m_pEnemyShotGroupScriptData;
		std::shared_ptr < ResourceScriptData >				m_pResourceScriptData;
	};
}

#endif
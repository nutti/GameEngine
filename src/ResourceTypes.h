#ifndef INCLUDED_GAMEENGINE_RESOURCETYPES_H
#define INCLUDED_GAMEENGINE_RESOURCETYPES_H

#include <vector>
#include <memory>

namespace GameEngine
{
	enum GlobalResourceID
	{
		GLOBAL_RESOURCE_ID_ITEM_1_SE					= 0,
		GLOBAL_RESOURCE_ID_ITEM_2_SE					= 1,
		GLOBAL_RESOURCE_ID_SHOT_SE						= 2,
		GLOBAL_RESOURCE_ID_HP_BAR_TEXTURE				= 3,
		GLOBAL_RESOURCE_ID_CONS_BAR_TEXTURE				= 4,
		GLOBAL_RESOURCE_ID_BACKGROUND_TEXTURE			= 5,
		GLOBAL_RESOURCE_ID_FONT_TEXTURE_FIRST			= 6,
		GLOBAL_RESOURCE_ID_FONT_TEXTURE_LAST			= 42,
		GLOBAL_RESOURCE_ID_POWER_UP_ITEM_1_TEXTURE		= 43,
		GLOBAL_RESOURCE_ID_POWER_UP_ITEM_2_TEXTURE		= 44,
		GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE			= 45,
		GLOBAL_RESOURCE_ID_PLAYER_SHOT_1_TEXTURE		= 46,
		GLOBAL_RESOURCE_ID_PLAYER_SHOT_COLLIDED_TEXTURE	= 47,
		GLOBAL_RESOURCE_ID_ENEMY_DESTROYED_TEXTURE		= 48,
		GLOBAL_RESOURCE_ID_ENEMY_DESTROYED_SE			= 49,
	};

	enum ResourceType
	{
		RESOURCE_TYPE_BGM				= 0,
		RESOURCE_TYPE_SE				= 1,
		RESOURCE_TYPE_TEXTURE			= 2,
		RESOURCE_TYPE_MULTI_TEXTURE		= 3,
		RESOURCE_TYPE_MODEL				= 4,
	};

	// リソース対応関係
	struct ResourceMap
	{
		// ステージリソース
		struct StageResourceMapElm
		{
			std::vector < int >		m_BGMMap;		// BGM
			std::vector < int >		m_SEMap;		// SE
			std::vector < int >		m_TextureMap;	// テクスチャ
			std::vector < int >		m_ModelMap;		// 3Dモデル
		};
		// グローバルリソース
		struct GlobalResourceMapElm
		{
			std::vector < int >		m_BGMMap;		// BGM
			std::vector < int >		m_SEMap;		// SE
			std::vector < int >		m_TextureMap;	// テクスチャ
			std::vector < int >		m_ModelMap;		// 3Dモデル
		};
		std::shared_ptr < StageResourceMapElm >		m_pStageResourceMap;
		std::shared_ptr < GlobalResourceMapElm >	m_pGlobalResourceMap;
	};
}

#endif
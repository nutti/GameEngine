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
		GLOBAL_RESOURCE_ID_FONT_TEXTURE					= 6,
		/*GLOBAL_RESOURCE_ID_FONT_TEXTURE_FIRST			= 6,
		GLOBAL_RESOURCE_ID_FONT_TEXTURE_LAST			= 42,*/
		GLOBAL_RESOURCE_ID_POWER_UP_ITEM_1_TEXTURE		= 43,
		GLOBAL_RESOURCE_ID_POWER_UP_ITEM_2_TEXTURE		= 44,
		GLOBAL_RESOURCE_ID_CRYSTAL_ITEM_TEXTURE			= 45,
		GLOBAL_RESOURCE_ID_PLAYER_SHOT_1_TEXTURE		= 46,
		GLOBAL_RESOURCE_ID_PLAYER_SHOT_COLLIDED_TEXTURE	= 47,
		GLOBAL_RESOURCE_ID_ENEMY_DESTROYED_TEXTURE		= 48,
		GLOBAL_RESOURCE_ID_ENEMY_DESTROYED_SE			= 49,
	};

	enum GlobalTexAtlasID
	{
		GLOBAL_TEX_ATLAS_ID_FONT_FIRST		= 0,
		GLOBAL_TEX_ATLAS_ID_FONT_LAST		= 48,
	};

	enum ResourceType
	{
		RESOURCE_TYPE_BGM					= 0,
		RESOURCE_TYPE_SE					= 1,
		RESOURCE_TYPE_TEXTURE				= 2,
		RESOURCE_TYPE_MULTI_TEXTURE			= 3,
		RESOURCE_TYPE_MODEL					= 4,
		RESOURCE_TYPE_LIGHT					= 5,
		RESOURCE_TYPE_POINT_SPRITE			= 6,
		RESOURCE_TYPE_ENEMY_PATTERN_FILE	= 7,
		RESOURCE_TYPE_TEXTURE_ATLAS			= 8,
		RESOURCE_TYPE_SKIN_MODEL			= 9,
	};

	// ライトのタイプ
	enum LightType
	{
		LIGHT_TYPE_DIRECTIONAL		= 0,
		LIGHT_TYPE_POINT			= 1,
		LIGHT_TYPE_SPOT				= 2,
		LIGHT_TYPE_NONE				= 3,
	};

	// リソース対応関係
	struct ResourceMap
	{
		// テクスチャアトラス用
		struct TextureAtlas
		{
			int			m_TexID;		// テクスチャID
			int			m_X;			// X座標
			int			m_Y;			// Y座標
			int			m_Width;		// 幅
			int			m_Height;		// 高さ
		};

		// ステージリソース
		struct StageResourceMapElm
		{
			std::vector < int >		m_BGMMap;				// BGM
			std::vector < int >		m_SEMap;				// SE
			std::vector < int >		m_TextureMap;			// テクスチャ
			std::vector < int >		m_ModelMap;				// 3Dモデル
			std::vector < int >		m_EnemyPatternFileMap;	// 敵出現パターンファイル
			int						m_LightTypeMap[ 8 ];	// ライト(上位16バイトがライトタイプ、下位16バイトがハンドル)
			std::vector < int >		m_PointSpriteMap;		// ポイントスプライト
			std::vector < TextureAtlas >	m_TexAtlasMap;	// テクスチャアトラス
			std::vector < int >		m_SkinModelMap;			// スキンメッシュモデル
		};
		// グローバルリソース
		struct GlobalResourceMapElm
		{
			std::vector < int >		m_BGMMap;		// BGM
			std::vector < int >		m_SEMap;		// SE
			std::vector < int >		m_TextureMap;	// テクスチャ
			std::vector < int >		m_ModelMap;		// 3Dモデル
			std::vector < TextureAtlas >	m_TexAtlasMap;	// テクスチャアトラス
			std::vector < int >		m_SkinModelMap;
		};
		std::shared_ptr < StageResourceMapElm >		m_pStageResourceMap;
		std::shared_ptr < GlobalResourceMapElm >	m_pGlobalResourceMap;
	};
}

#endif
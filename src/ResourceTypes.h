#ifndef INCLUDED_GAMEENGINE_RESOURCETYPES_H
#define INCLUDED_GAMEENGINE_RESOURCETYPES_H

#include <vector>
#include <memory>

namespace GameEngine
{
	// リソース対応関係
	struct ResourceMap
	{
		struct ResourceMapElm
		{
			std::vector < int >		m_BGMMap;		// BGM
			std::vector < int >		m_SEMap;		// SE
			std::vector < int >		m_TextureMap;	// テクスチャ
		};
		std::shared_ptr < ResourceMapElm >		m_pElm;
	};
}

#endif
#ifndef INCLUDED_GAMEENGINE_SPRITEBATCH_H
#define INCLUDED_GAMEENGINE_SPRITEBATCH_H

#include "ResourceTypes.h"

namespace GameEngine
{
	void AddToSpriteBatch(	int blendingMode, int texID, float posX, float posY, float angle,
							bool centerized = true, int color = 0xFFFFFFFF );

	void AddToSpriteBatch(	int blendingMode, int texID, float posX, float posY, float scaleX, float scaleY, float angle,
							bool centerized = true, int color = 0xFFFFFFFF );

	void AddToAtlasSpriteBatch(	int blendingMode, int atlasID, float posX, float posY, float angle,
								bool centerized = true, int color = 0xFFFFFFFF );

	void AddToAtlasSpriteBatch(	int blendingMode, int atlasID, float posX, float posY, float scaleX, float scaleY, float angle,
								bool centerized = true, int color = 0xFFFFFFFF );

	void ProcAllBatchWorks( const ResourceMap& map );
}

#endif
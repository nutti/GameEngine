#ifndef INCLUDED_GAMEENGINE_SPRITEBATCH_H
#define INCLUDED_GAMEENGINE_SPRITEBATCH_H

#include "ResourceTypes.h"

namespace GameEngine
{
	void AddToSpriteBatch(	int blendingMode, int texID, float posX, float posY, float angle,
							bool centerized = true, int color = 0xFFFFFFFF );

	void AddToSpriteBatch(	int blendingMode, int texID, float posX, float posY, float scaleX, float scaleY, float angle,
							bool centerized = true, int color = 0xFFFFFFFF );

	void AddToAtlasSpriteBatch(	bool global, int blendingMode, int atlasID, float posX, float posY, float angle,
								bool centerized = true, int color = 0xFFFFFFFF );

	void AddToAtlasSpriteBatch(	bool global, int blendingMode, int atlasID, float posX, float posY, float scaleX, float scaleY, float angle,
								bool centerized = true, int color = 0xFFFFFFFF );

	void AddToAtlasSpriteBatch(	bool global, int blendingMode, int atlasID,
								float cx1, float cy1, float cx2, float cy2,
								float posX, float posY, float scaleX, float scaleY, float angle,
								bool centerized = true, int color = 0xFFFFFFFF );

	void ProcAllBatchWorks( const ResourceMap& map );
}

#endif
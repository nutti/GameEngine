#ifndef INCLUDED_GAMEENGINE_SPRITEBATCH_H
#define INCLUDED_GAMEENGINE_SPRITEBATCH_H


namespace GameEngine
{
	void AddToSpriteBatch(	int blendingMode, int texID, float posX, float posY, float angle,
							bool centerized = true, int color = 0xFFFFFFFF );

	void ProcAllBatchWorks();
}

#endif
#ifndef INCLUDED_GAMEENGINE_SCOREENTRY_H
#define INCLUDED_GAMEENGINE_SCOREENTRY_H

#include <memory>
#include "Scene.h"

namespace GameEngine
{
	class ScoreEntry : public Scene
	{
	private:
		class Impl;
		std::auto_ptr < ScoreEntry::Impl >		m_pImpl;
	public:
		ScoreEntry();
		~ScoreEntry();
		void Init();
		SceneType Update();
		void Draw();
		void AttachButtonState( ButtonStatusHolder* pHolder );
		void AttachResourceMap( const ResourceMap& map );
		void AttachDisplayedSaveData( const DisplayedSaveData& data );
		void AttachRecord( const SaveDataRecord& record );
		void SetDifficulty( int difficulty );
		const DisplayedSaveData& GetDisplayedSaveData() const;
		void SetRecordRank( int rank );
		const SaveDataRecord& GetRecord() const;
	};
}

#endif
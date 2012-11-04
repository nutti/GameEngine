#ifndef INCLUDED_GAMEENGINE_STAGEBACKGROUND_H
#define INCLUDED_GAMEENGINE_STAGEBACKGROUND_H

#include <memory>

namespace GameEngine
{
	struct ScriptData;
	struct StageData;

	struct StageBackgroundData
	{
		int				m_Counter;		// カウンタ
		int				m_Stage;		// ステージ
		StageData*		m_pStageData;	// ステージデータ
	};
	
	class StageBackground
	{
	private:
		class Impl;
		std::auto_ptr < StageBackground::Impl >		m_pImpl;
	public:
		StageBackground( int stageNo );
		~StageBackground();
		void Init();
		void Draw();
		void Update();
		void AttachStageData( StageData* pData );
		void AttachScriptData( const ScriptData& data );
		void Terminate();
	};
}

#endif
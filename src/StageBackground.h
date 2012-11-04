#ifndef INCLUDED_GAMEENGINE_STAGEBACKGROUND_H
#define INCLUDED_GAMEENGINE_STAGEBACKGROUND_H

#include <memory>

namespace GameEngine
{
	struct ScriptData;
	struct StageData;

	struct StageBackgroundData
	{
		int				m_Counter;		// �J�E���^
		int				m_Stage;		// �X�e�[�W
		StageData*		m_pStageData;	// �X�e�[�W�f�[�^
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
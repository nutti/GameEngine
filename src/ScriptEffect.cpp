#include "ScriptEffect.h"

#include <MAPIL/MAPIL.h>

#include "ResourceTypes.h"
#include "Enemy.h"
#include "Util.h"

namespace GameEngine
{
	ScriptEffect::ScriptEffect(	std::shared_ptr < ResourceMap > pMap,
								std::shared_ptr < EffectScriptData > pData,
								std::weak_ptr < Enemy > pEnemy,
								int id ) : m_VM(), m_pScriptData( pData )
	{
		m_EffectData.m_EffectID = id;
		m_EffectData.m_EffectSubID = -1;
		m_EffectData.m_Counter = 0;
		m_EffectData.m_pResourceMap = pMap;
		m_EffectData.m_PosX = 0.0f;
		m_EffectData.m_PosY = 0.0f;
		m_EffectData.m_ParentEnemy = pEnemy;
		//m_EffectData.m_pEnemyData = pEnemyData;
		m_EffectData.m_StatusFlags.reset();
		MAPIL::ZeroObject( m_EffectData.m_Regs, sizeof( m_EffectData.m_Regs ) );
	}

	ScriptEffect::~ScriptEffect()
	{
	}

	void ScriptEffect::Init( float posX, float posY )
	{
		m_EffectData.m_PosX = posX;
		m_EffectData.m_PosY = posY;
		m_VM.Init( &m_pScriptData->m_pElm[ m_EffectData.m_EffectID ].m_Data, &m_EffectData );
	}

	void ScriptEffect::SetPos( float posX, float posY )
	{
		m_EffectData.m_PosX = posX;
		m_EffectData.m_PosY = posY;
	}

	void ScriptEffect::Draw()
	{
		// スクリプトの実行
		m_VM.Run();
		if( m_VM.Terminated() ){
			//m_EffectData.m_StatusFlags.set( ScriptEffectData::TERMINATED );
			throw MAPIL::MapilException( CURRENT_POSITION, TSTR( "Effect script must not terminated." ), -1 );
		}
	}

	bool ScriptEffect::Update()
	{
		if( m_EffectData.m_StatusFlags[ ScriptEffectData::TERMINATED ] ){
			return false;
		}

		++m_EffectData.m_Counter;

		return true;
	}

	void ScriptEffect::SetReg( int no, int val )
	{
		m_EffectData.m_Regs[ no ] = val;
	}

	void ScriptEffect::SetAlphaBlendingMode( int mode )
	{
		m_EffectData.m_AlphaBlendingMode = mode;
	}
}
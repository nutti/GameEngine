#ifndef INCLUDED_GAMEENGINE_ENEMYSHOTGROUPVCPU_H
#define INCLUDED_GAMEENGINE_ENEMYSHOTGROUPVCPU_H

#include "ScriptManager/VM.h"

namespace GameEngine
{
	struct EnemyShotGroupData;
	class EnemyShotGroupVCPU : public VM::VCPU
	{
	private:
		EnemyShotGroupData*		m_pEnemyShotGroupData;

		void SysGetPlayerPosXGU();
		void SysGetPlayerPosYGU();
		void SysGetEnemyPosXGU();
		void SysGetEnemyPosYGU();


		void SysGetPlayerPosX();
		void SysGetPlayerPosY();
		
		void SysGetRandF();
		void SysGetRandGU();
		

		void SysGetEnemyPosX();
		void SysGetEnemyPosY();
		void SysGetEnemyHP();
		void SysGetEnemySpeed();
		void SysGetEnemyCounter();
		void SysGetEnemyCounterF();
		void SysGetEnemyRotAngle();
		void SysGetEnemyConsGauge();

		void SysSetEnemyShotStatusGU();
		void SysSetEnemyShotStatusAtlasGU();
		void SysGetEnemyShotPosXGU();
		void SysGetEnemyShotPosYGU();
		void SysSetEnemyShotSpeedGU();
		void SysSetEnemyShotAngleGU();
		void SysSetEnemyShotPosGU();
		void SysEnemyShotAddAngleGU();
		void SysEnemyShotSetLineStatusGU();
		void SysGetEnemyShotAngleGU();
		void SysEnemyShotAddSpeedGU();

		void SysCreateEnemyShot();
		void SysCreateConsEnemyShot();
		void SysCreateEnemyShotGroup();
		void SysCreateEnemyShotGroupReg();
		void SysCreateEnemyShotGroupGReg5();
		void SysSetEnemyShotPos();
		void SysSetEnemyShotPower();
		void SysSetEnemyShotAngle();
		void SysSetEnemyShotSpeed();
		void SysSetEnemyShotImage();
		void SysSetEnemyShotImageAtlas();
		void SysSetEnemyShotCollisionRadius();
		void SysSetEnemyShotCollisionRadiusGU();
		void SysSetEnemyShotStatus();
		void SysSetEnemyShotMovement();
		void SysGetEnemyShotCounter();
		void SysGetEnemyShotPosX();
		void SysGetEnemyShotPosY();
		void SysGetEnemyShotPower();
		void SysGetEnemyShotAngle();
		void SysGetEnemyShotSpeedGU();
		void SysSetEnemyShotImgRotMode();
		void SysSetEnemyShotImgRotAnglePerFrame();
		void SysSetEnemyShotDrawingMultiplicity();
		void SysSetEnemyShotAlphaBlendingMode();
		void SysEnemyShotDeleteByPlayerSkill();
		void SysEnemyShotDeleteByPlayerDamage();
		void SysEnemyShotNotDeleteByPlayerSkill();
		void SysEnemyShotNotDeleteByPlayerDamage();


		void SysEnemyShotAddAngle();
		void SysEnemyShotSetImgScale();
		void SysEnemyShotAddPos();
		void SysEnemyShotAddSpeed();
		void SysEnemyShotSetShape();
		void SysEnemyShotSetLineStatus();
		void SysEnableEnemyShotInvisibleMode();
		void SysDisableEnemyShotInvisibleMode();
		void SysEnableEnemyShotInvincibleMode();
		void SysDisableEnemyShotInvincibleMode();

		void SysGetStageFrame();
		void SysGetEnemyShotGroupReg();
		void SysGetEnemyShotGroupFReg();
		void SysGetEnemyShotGroupGReg();
		void SysGetEnemyShotGroupGRegIndex();

		void SysCreateScriptEffectReg();
		void SysCreateScriptEffectFReg5();

		void SysSendEventToEnemyShotGroup();
		void SysSetEnemyShotGroupSubID();
		void SysGetEvent();

		void SysGetDifficulty();

		void SysPlaySE();
		void SysStopSE();
	public:
		EnemyShotGroupVCPU();
		~EnemyShotGroupVCPU();
		void Init( VM::Data* pData, EnemyShotGroupData* pEnemyShotGroupData );
		void OpSysCall( int val );
	};
}

#endif

#include <iostream>
#include <iomanip>
#include <memory>
#include <memory.h>
#include "Compiler.h"
#include "Parser.hh"

Compiler::Compiler() : m_BreakIndex( -1 ), m_ErrorCount( 0 )
{
}

Compiler::~Compiler()
{
}

bool Compiler::Compile( const std::string& f, VM::Data& data )
{
	// System call
	AddFunction( VM::SYS_PRINT, TYPE_VOID, "print", "s" );
	AddFunction( VM::SYS_TOSTR, TYPE_STRING, "str", "i" );
	AddFunction( VM::SYS_PRINT_GU, TYPE_VOID, "printGU", "g" );

	AddFunction( VM::SYS_FLOAT_TO_INT, TYPE_INTEGER, "FtoI", "f" );		// int FtoI( float );
	AddFunction( VM::SYS_INT_TO_FLOAT, TYPE_FLOAT, "ItoF", "i" );		// float ItoF( int );
	AddFunction( VM::SYS_GU_TO_INT, TYPE_INTEGER, "GtoI", "g" );		// int GtoI( gu );
	AddFunction( VM::SYS_INT_TO_GU, TYPE_GU, "ItoG", "i" );				// gu ItoG( int );
	AddFunction( VM::SYS_GU_TO_FLOAT, TYPE_FLOAT, "GtoF", "g" );		// float GtoF( gu );
	AddFunction( VM::SYS_FLOAT_TO_GU, TYPE_GU, "FtoG", "f" );			// gu FtoG( float );

	AddFunction( VM::SYS_SIN, TYPE_FLOAT, "sin", "f" );					// float sin( rad );
	AddFunction( VM::SYS_COS, TYPE_FLOAT, "cos", "f" );					// float cos( rad );
	AddFunction( VM::SYS_ATAN2, TYPE_FLOAT, "atan2", "ff" );			// float atan2( y, x );
	AddFunction( VM::SYS_ABS, TYPE_INTEGER, "abs", "i" );				// int abs( val );
	AddFunction( VM::SYS_FABS, TYPE_FLOAT, "fabs", "f" );				// float fabs( val );
	AddFunction( VM::SYS_SQRT, TYPE_FLOAT, "sqrt", "f" );				// float sqrt( val );

	AddFunction( VM::SYS_SIN_GU, TYPE_GU, "sinGU", "g" );				// gu sinGU( deg );
	AddFunction( VM::SYS_COS_GU, TYPE_GU, "cosGU", "g" );				// gu cosGU( deg );
	AddFunction( VM::SYS_ATAN2_GU, TYPE_GU, "atan2GU", "gg" );			// gu atan2GU( y, x );
	AddFunction( VM::SYS_ABS_GU, TYPE_GU, "absGU", "g" );				// gu absGU( val );
	AddFunction( VM::SYS_SQRT_GU, TYPE_GU, "sqrtGU", "g" );				// gu sqrtGU( val );

	AddFunction( VM::SYS_DEG_TO_RAD, TYPE_FLOAT, "DegToRad", "f" );		// float DegToRad();
	AddFunction( VM::SYS_RAD_TO_DEG, TYPE_FLOAT, "RadToDeg", "f" );		// float RadToDeg();

	AddFunction( VM::SYS_GET_PLAYER_POSX_GU, TYPE_GU, "GetPlayerPosXGU", "" );			// gu GetPlayerPosXGU();
	AddFunction( VM::SYS_GET_PLAYER_POSY_GU, TYPE_GU, "GetPlayerPosYGU", "" );			// gu GetPlayerPosYGU();
	AddFunction( VM::SYS_GET_PLAYER_POSX, TYPE_FLOAT, "GetPlayerPosX", "" );				// float GetPlayerPosX();
	AddFunction( VM::SYS_GET_PLAYER_POSY, TYPE_FLOAT, "GetPlayerPosY", "" );				// float GetPlayerPosY();
	AddFunction( VM::SYS_GET_PLAYER_HP, TYPE_INTEGER, "GetPlayerHP", "" );					// int GetPlayerHP();
	AddFunction( VM::SYS_GET_PLAYER_CUR_CONS, TYPE_INTEGER, "GetPlayerCurCons", "" );		// int GetPlayerCurCons();
	AddFunction( VM::SYS_GET_PLAYER_CONS_GAUGE, TYPE_INTEGER, "GetPlayerConsGauge", "i" );	// int GetPlayerConsGauge( cons );
	AddFunction( VM::SYS_GET_PLAYER_SCORE_TOTAL, TYPE_INTEGER, "GetPlayerScoreTotal", "" );	// int GetPlayerScoreTotal();
	AddFunction( VM::SYS_GET_PLAYER_CRYSTAL_TOTAL, TYPE_INTEGER, "GetPlayerCrystalTotal", "" );		// int GetPlayerCrystalTotal();
	AddFunction( VM::SYS_GET_PLAYER_CRYSTAL_USED_TOTAL, TYPE_INTEGER, "GetPlayerCrystalUsedTotal", "" );	// int GetPlayerCrystalUsedTotal();
	AddFunction( VM::SYS_GET_PLAYER_KILLED_TOTAL, TYPE_INTEGER, "GetPlayerKilledTotal", "" );		// int GetPlayerKilledTotal();

	AddFunction( VM::SYS_GET_RANDOM_F, TYPE_FLOAT, "GetRandF", "" );							// float GatRand();
	AddFunction( VM::SYS_GET_RANDOM_GU, TYPE_GU, "GetRandGU", "" );						// gu GetRandGU();
	AddFunction( VM::SYS_ADD_SCORE, TYPE_VOID, "AddScore", "i" );							// void AddScore( score );

	AddFunction( VM::SYS_PLAY_SE, TYPE_VOID, "PlaySE", "i" );			// void PlaySE( id );
	AddFunction( VM::SYS_STOP_SE, TYPE_VOID, "StopSE", "i" );			// void StopSE( id );
	AddFunction( VM::SYS_PLAY_BGM, TYPE_VOID, "PlayBGM", "i" );	// void PlayBGM( id );
	AddFunction( VM::SYS_STOP_BGM, TYPE_VOID, "StopBGM", "i" );	// void StopBGM( id );

	

	// System calll for MAPIL function call.

	// System call for enemy.
	AddFunction( VM::SYS_ENEMY_GET_POSX_GU, TYPE_GU, "GetEnemyPosXGU", "" );							// gu GetEnemyPosXGU();
	AddFunction( VM::SYS_ENEMY_GET_POSY_GU, TYPE_GU, "GetEnemyPosYGU", "" );							// gu GetEnemyPosYGU();
	AddFunction( VM::SYS_ENEMY_SET_COLLISION_RADIUS_GU, TYPE_VOID, "SetEnemyCollisionRadiusGU", "g" );	// void SetEnemyCollisionRadiusGU( raidius );
	AddFunction( VM::SYS_ENEMY_SET_POS_GU, TYPE_VOID, "SetEnemyPosGU", "gg" );							// void SetEnemyPosGU( x, y );
	AddFunction( VM::SYS_ENEMY_SET_POS_3D_GU, TYPE_VOID, "SetEnemyPos3DGU", "ggg" );					// void SetEnemyPos3DGU( x, y, z );

	AddFunction( VM::SYS_ENEMY_GET_POSX, TYPE_FLOAT, "GetEnemyPosX", "" );				// float GetEnemyPosX();
	AddFunction( VM::SYS_ENEMY_GET_POSY, TYPE_FLOAT, "GetEnemyPosY", "" );				// float GetEnemyPosY();
	AddFunction( VM::SYS_ENEMY_GET_HP, TYPE_INTEGER, "GetEnemyHP", "" );					// int GetEnemyHP();
	AddFunction( VM::SYS_ENEMY_GET_SPEED, TYPE_INTEGER, "GetEnemySpeed", "" );				// int GetEnemySpeed();
	AddFunction( VM::SYS_ENEMY_GET_COUNTER, TYPE_INTEGER, "GetEnemyCounter", "" );			// int GetEnemyCounter();
	AddFunction( VM::SYS_ENEMY_GET_COUNTER_F, TYPE_FLOAT, "GetEnemyCounterF", "" );			// float GetEnemyCounterF();
	AddFunction( VM::SYS_ENEMY_SET_ANGLE, TYPE_INTEGER, "GetEnemyAngle", "" );				// int GetEnemyAngle();
	AddFunction( VM::SYS_ENEMY_GET_CONS_GAUGE, TYPE_INTEGER, "GetEnemyConsGauge", "" );		// int GetEnemyConsGauge();
	AddFunction( VM::SYS_ENEMY_GET_REG, TYPE_INTEGER, "GetEnemyReg", "i" );					// int GetEnemyReg( reg_no );
	AddFunction( VM::SYS_ENEMY_GET_ROT_ANGLE, TYPE_FLOAT, "GetEnemyRotAngle", "i" );		// float GetEnemyRotAngle( axis );

	AddFunction( VM::SYS_ENEMY_SET_NAME, TYPE_VOID, "SetEnemyName", "s" );					// void SetEnemyName( name );
	AddFunction( VM::SYS_ENEMY_SET_POS, TYPE_VOID, "SetEnemyPos", "ff" );					// void SetEnemyPos( x, y );
	AddFunction( VM::SYS_ENEMY_SET_POS_3D, TYPE_VOID, "SetEnemyPos3D", "fff" );				// void SetEnemyPos3D( x, y, z );
	AddFunction( VM::SYS_ENEMY_SET_ANGLE, TYPE_VOID, "SetEnemyAngle", "i" );	//integerバグ		// void SetEnemyAngle( angle );
	AddFunction( VM::SYS_ENEMY_SET_ANGLE_3D, TYPE_VOID, "SetEnemyAngle3D", "fff" );			// void SetEnemyAngle3D( rx, ry, rz );
	AddFunction( VM::SYS_ENEMY_SET_SCALE_3D, TYPE_VOID, "SetEnemyScale3D", "fff" );			// void SetEnemyScale3D( sx, sy, sz );
	AddFunction( VM::SYS_ENEMY_SET_SPEED, TYPE_VOID, "SetEnemySpeed", "i" );	//integerバグ			// void SetEnemySpeed( speed );
	AddFunction( VM::SYS_ENEMY_SET_HP, TYPE_VOID, "SetEnemyHP", "i" );						// void SetEnemyHP( hp );
	AddFunction( VM::SYS_DAMAGE_ENEMY, TYPE_VOID, "DamageEnemy", "i" );						// void DamageEnemy( hp );
	AddFunction( VM::SYS_ENEMY_SET_IMAGE, TYPE_VOID, "SetEnemyImgID", "i" );				// void SetEnemyImage( texture_id );
	AddFunction( VM::SYS_ENEMY_SET_MODEL, TYPE_VOID, "SetEnemyModelID", "i" );				// void SetEnemyModel( model_id );
	AddFunction( VM::SYS_ENEMY_SET_SKIN_MODEL, TYPE_VOID, "SetEnemySkinModelID", "i" );		// void SetEnemySkinModel( model_id );
	AddFunction( VM::SYS_ENEMY_SET_MODEL_ANIM_TIME, TYPE_VOID, "SetEnemyModelAnimTime", "f" );		// void SetEnemyModelAnimTime( time );
	AddFunction( VM::SYS_ENEMY_SET_COLLISION_RADIUS, TYPE_VOID, "SetEnemyCollisionRadius", "f" );		// void SetEnemyCollisionRadius( radius );
	AddFunction( VM::SYS_ENEMY_SET_SCORE, TYPE_VOID, "SetEnemyScore", "i" );							// void SetEnemyScore( score );
	AddFunction( VM::SYS_ENEMY_SET_CONS_GAUGE, TYPE_VOID, "SetEnemyConsGauge", "i" );					// void SetEnemyConsGauge( gauge );
	AddFunction( VM::SYS_ENEMY_SET_CONS_ATTR, TYPE_VOID, "SetEnemyConsAttr", "i" );						// void SetEnemyConsAttr( attr );
	AddFunction( VM::SYS_ENEMY_ENABLE_INVINCIBLE, TYPE_VOID, "EnemyEnableInvincible", "" );				// void EnemyEnableInvincible();
	AddFunction( VM::SYS_ENEMY_DISABLE_INVINCIBLE, TYPE_VOID, "EnemyDisableInvincible", "" );			// void EnemyDisableInvincible();
	AddFunction( VM::SYS_ENEMY_ENABLE_NON_COLLISION_MODE, TYPE_VOID, "EnemyEnableNonCollisionMode", "" );	// void EnemyEnableNonCollisionMode();
	AddFunction( VM::SYS_ENEMY_DISABLE_NON_COLLISION_MODE, TYPE_VOID, "EnemyDisableNonCollisionMode", "" );	// void EnemyDisableNonCollisionMode();
	AddFunction( VM::SYS_ENEMY_INVOKE_CONS_SKILL, TYPE_VOID, "EnemyInvokeConsSkill", "sii" );			// void EnemyInvokeConsSkill( skill_name, dec_gauge, attribute );
	AddFunction( VM::SYS_ENEMY_STOP_CONS_SKILL, TYPE_VOID, "EnemyStopConsSkill", "" );				// void EnemyStopConsSkill();
	AddFunction( VM::SYS_ENEMY_SHIFT_NEXT_MODE, TYPE_VOID, "EnemyShiftNextMode", "" );	// void EnemyShiftNextMode();
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_IMG_ROT_MODE, TYPE_VOID, "SetEnemyShotImgRotMode", "ii" );		// void SetImgRotMode( shot_id, mode );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_IMG_ROT_ANGLE_PER_FRAME, TYPE_VOID, "SetEnemyShotImgRotAnglePerFrame", "if" );		// void SetImgAnglePerFrame( shot_id, angle );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_DRAWING_MULTIPLICITY, TYPE_VOID, "SetEnemyShotDrawingMultiplicity", "ii" );		// void SetDrawingMultiplicity( shot_id, num );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_ALPHA_BLENDING_MODE, TYPE_VOID, "SetEnemyShotAlphaBlendingMode", "ii" );		// void SetImgAlphaBlendingMode( shot_id, mode );
	AddFunction( VM::SYS_ENEMY_DAMAGED_BY_CONS_SHOT, TYPE_INTEGER, "EnemyDamagedByConsShot", "" );		// int EnemyDamagedByConsShot();
	AddFunction( VM::SYS_ENEMY_DAMAGED_BY_CONS_SHOT_INDEX, TYPE_INTEGER, "EnemyDamagedByConsShotIndex", "i" );	// int EnemyDamagedByConsShotIndex( cons_idx );
	AddFunction( VM::SYS_SEARCH_ENEMY_IN_SKILL_MODE, TYPE_INTEGER, "SearchEnemyInSkillMode", "" );		// int SearchEnemyInSkillMode();
	AddFunction( VM::SYS_SEARCH_ENEMY_IN_SKILL_MODE_BY_NAME, TYPE_INTEGER, "SearchEnemyInSkillModeByName", "s" );		// int SearchEnemyInSkillModeByName( enemy_name );


	AddFunction( VM::SYS_ENEMY_CREATE_SHOT_1, TYPE_VOID, "CreateEnemyShot1", "fffffii" );	// void CreateEnemyShot1( x, y, speed, angle, radius, texture_id, power );
	AddFunction( VM::SYS_ENEMY_CREATE_SHOT_1_GU, TYPE_VOID, "CreateEnemyShot1GU", "gggggii" );	// void CreateEnemyShot1( x, y, speed, angle, radius, texture_id, power );
	AddFunction( VM::SYS_ENEMY_CREATE_EFFECT_1, TYPE_VOID, "CreateEffect1", "ffii" );		// void CreateEffect1( x, y, id, subid );
	AddFunction( VM::SYS_ENEMY_CREATE_EFFECT_1_GU, TYPE_VOID, "CreateEffect1GU", "ggii" );	// void CreateEffect1GU( x, y, id, subid );
	AddFunction( VM::SYS_ENEMY_SET_BOSS_FLAG, TYPE_VOID, "SetEnemyBossFlag", "i" );			// void SetEnemyBossFlag( flag );
	AddFunction( VM::SYS_CREATE_ITEM, TYPE_VOID, "CreateItem", "iiff" );			// void CreateItem( item_id, item_sub_id, x, y );
	AddFunction( VM::SYS_CREATE_ITEM_GU, TYPE_VOID, "CreateItemGU", "iigg" );		// void CreateItemGU( item_id, item_sub_id, x, y );
	AddFunction( VM::SYS_ENEMY_CREATE_SHOT_GROUP, TYPE_VOID, "CreateEnemyShotGroup", "i" );			// void CreateShotGroup( script_id );
	AddFunction( VM::SYS_ENEMY_CREATE_SHOT_GROUP_REG, TYPE_VOID, "CreateEnemyShotGroupReg", "ii" );	// void CreateShotGroupReg( script_id, reg1 );
	AddFunction( VM::SYS_ENEMY_CREATE_SHOT_GROUP_FREG, TYPE_VOID, "CreateEnemyShotGroupFReg", "if" );	// void CreateShotGroupFReg( script_id, reg1 );
	AddFunction( VM::SYS_ENEMY_CREATE_SHOT_GROUP_GREG, TYPE_VOID, "CreateEnemyShotGroupGReg", "ig" );	// void CreateShotGroupGReg( script_id, reg1 );
	AddFunction( VM::SYS_ENEMY_CREATE_SHOT_GROUP_GREG5, TYPE_VOID, "CreateEnemyShotGroupGReg5", "iggggg" );	// void CreateShotGroupGReg( script_id, reg1, reg2, reg3, reg4, reg5 );
	AddFunction( VM::SYS_ENEMY_SEARCH_ITEM, TYPE_VOID, "SearchItem", "" );			// void SearchItem();
	AddFunction( VM::SYS_ENEMY_GET_ITEM_POS_X, TYPE_FLOAT, "GetItemPosX", "" );		// float GetItemPosX();
	AddFunction( VM::SYS_ENEMY_GET_ITEM_POS_Y, TYPE_FLOAT, "GetItemPosY", "" );		// float GetItemPosY();
	AddFunction( VM::SYS_ENEMY_GET_ITEM_POS_X_GU, TYPE_GU, "GetItemPosXGU", "" );		// gu GetItemPosXGU();
	AddFunction( VM::SYS_ENEMY_GET_ITEM_POS_Y_GU, TYPE_GU, "GetItemPosYGU", "" );		// gu GetItemPosYGU();
	AddFunction( VM::SYS_ENEMY_ALIVE, TYPE_INTEGER, "EnemyAlive", "" );		// int EnemyAlive();
	AddFunction( VM::SYS_ENEMY_IN_SKILL_MODE, TYPE_INTEGER, "EnemyInSkillMode", "" );	// int EnemyInSkillMode();

	AddFunction( VM::SYS_SET_ENEMY_SUB_ID, TYPE_VOID, "SetEnemySubID", "i" );			// void SetEnemySubID( sub_id );
	AddFunction( VM::SYS_SEND_EVENT_TO_ENEMY, TYPE_VOID, "SendEventToEnemy", "sii" );	// void SetEventToEnemy( name, sub_id, event );

	// System call for enemy shot.
	AddFunction( VM::SYS_SET_ENEMY_SHOT_LENGTH, TYPE_VOID, "SetEnemyShotLength", "ig" );		// void SetEnemyShotLength( int id, gu length );
	AddFunction( VM::SYS_CREATE_ENEMY_SHOT_BY_ID_ORTHO, TYPE_INTEGER,  "CreateEnemyShotByIDOrtho", "iigg" );		// int CreateEnemyShotByIDOrtho( gu x, gu y, int id, int tex_color );
	AddFunction( VM::SYS_CREATE_ENEMY_SHOT_BY_ID_ORTHO, TYPE_INTEGER,  "CreateEnemyShotByIDPolar", "iigg" );		// int CreateEnemyShotByIDPolar( gu x, gu y, int id, int tex_color );
	AddFunction( VM::SYS_CREATE_ENENY_SHOT_BY_ID_LOCAL_ORTHO, TYPE_INTEGER, "CreateEnemyShotByIDLocalOrtho", "iigg" );		// int CreateEnemyShotByIDLocalOrtho( gu radius, gu angle, int id, int tex_color );
	AddFunction( VM::SYS_CREATE_ENEMY_SHOT_BY_ID_LOCAL_POLAR, TYPE_INTEGER,  "CreateEnemyShotByIDLocalPolar", "iigg" );		// int CreateEnemyShotByIDLocalPolar( gu x, gu y, int id, int tex_color );
	AddFunction( VM::SYS_CHANGE_ENEMY_SHOT_ID, TYPE_VOID, "ChangeEnemyShotID", "iii" );			// void ChangeEnemyShotID( int shot_id, int id, int tex_color );
	AddFunction( VM::SYS_DELETE_ENEMY_SHOT, TYPE_VOID, "DeleteEnemyShot", "i" );			// void DeleteEnemyShot( int shot_id );

	// System call for enemy shot group.
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_STATUS_GU, TYPE_VOID, "SetEnemyShotStatusGU", "igggggi" );	// void SetEnemyShotStatusGU( shot_id, x, y, angle, speed, radius, texture_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_STATUS_ATLAS_GU, TYPE_VOID, "SetEnemyShotStatusAtlasGU", "igggggi" );		// void SetEnemyShotStatusAtlasGU( shot_id, x, y, angle, speed, radius, texture_atlas_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_GET_POS_X_GU, TYPE_GU, "GetEnemyShotPosXGU", "i" );		// gu GetEnemyShotPosXGU( shot_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_GET_POS_Y_GU, TYPE_GU, "GetEnemyShotPosYGU", "i" );		// gu GetEnemyShotPosYGU( shot_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_SPEED_GU, TYPE_VOID, "SetEnemyShotSpeedGU", "ig" );	// void SetEnemyShotSetSpeedGU( shot_id, speed );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_ANGLE_GU, TYPE_VOID, "SetEnemyShotAngleGU", "ig" );	// void SetEnemyShotSetAngleGU( shot_id, angle );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_POS_GU, TYPE_VOID, "SetEnemyShotPosGU", "igg" );		// void SetEnemyShotPosGU( shot_id, x, y );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_ADD_ANGLE_GU, TYPE_VOID, "EnemyShotAddAngleGU", "ig" );			// void EnemyShotAddAngleGU( id, angle );
	AddFunction( VM::SYS_ENEMY_SHOT_SET_LINE_SHOT_STATUS_GU, TYPE_VOID, "EnemyShotSetLineShotStatusGU", "iggggg" );			// void EnemyShotSetLineShotStatusGU( id, x1, y1, x2, y2, thickness );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_GET_ANGLE_GU, TYPE_GU, "GetEnemyShotAngleGU", "i" );	// gu GetEnemyShotAngleGU( shot_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_ADD_SPEED_GU, TYPE_VOID, "EnemyShotAddSpeedGU", "ig" );			// void EnemyShotAddSpeedGU( id, speed );
	
	AddFunction( VM::SYS_CREATE_ENEMY_INI_POS_GU, TYPE_VOID, "CreateEnemyIniPosGU", "igg" );	// void CreateEnemyIniPosGU( id, x, y );
	AddFunction( VM::SYS_CREATE_ENEMY_INI_POS_REG_GU, TYPE_VOID, "CreateEnemyIniPosRegGU", "iggi" );	// void CreateEnemyIniPosRegGU( id, x, y, reg1 );
	AddFunction( VM::SYS_CREATE_ENEMY_INI_POS_REG5_GU, TYPE_VOID, "CreateEnemyIniPosReg5GU", "iggiiiii" );	// void CreateEnemyIniPosReg5GU( script_id, x, y, r1, r2, r3, r4, r5 );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_CREATE_SHOT, TYPE_INTEGER, "CreateEnemyShot", "" );	// int CreateEnemyShot();
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_CREATE_CONS_SHOT, TYPE_INTEGER, "CreateConsEnemyShot", "i" );	// int CreateConsEnemyShot( shot_attr );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_POS, TYPE_VOID, "SetEnemyShotPos", "iff" );		// void SetEnemyShotPos( shot_id, x, y );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_POWER, TYPE_VOID, "SetEnemyShotPower", "ii" );		// void SetEnemyShotPower( shot_id, power );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_ANGLE, TYPE_VOID, "SetEnemyShotAngle", "if" );		// void SetEnemyShotAngle( shot_id, angle );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_SPEED, TYPE_VOID, "SetEnemyShotSpeed", "if" );		// void SetEnemyShotSpeed( shot_id, speed );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_IMAGE, TYPE_VOID, "SetEnemyShotImage", "ii" );		// void SetEnemyShotImage( shot_id, texture_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_IMAGE_ATLAS, TYPE_VOID, "SetEnemyShotImageAtlas", "ii" );		// void SetEnemyShotImageAtlas( shot_id, texture_atlas_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_COLLISION_RADIUS, TYPE_VOID, "SetEnemyShotCollisionRadius", "if" );	// void SetEnemyShotCollisionRadius( id, radius );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_COLLISION_RADIUS_GU, TYPE_VOID, "SetEnemyShotCollisionRadiusGU", "ig" );	// void SetEnemyShotCollisionRadiusGU( id, radius );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_STATUS, TYPE_VOID, "SetEnemyShotStatus", "ifffffi" );		// void SetEnemyShotStatus( shot_id, x, y, angle, speed, radius, texture_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_MOVEMENT, TYPE_VOID, "SetEnemyShotMovement", "iff" );		// void SetEnemyShotMovement( shot_id, angle, speed );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_GET_COUNTER, TYPE_INTEGER, "GetEnemyShotCounter", "i" );		// int GetEnemyShotCounter( shot_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_GET_POS_X, TYPE_FLOAT, "GetEnemyShotPosX", "i" );				// float GetEnemyShotPosX( shot_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_GET_POS_Y, TYPE_FLOAT, "GetEnemyShotPosY", "i" );				// float GetEnemyShotPosY( shot_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_GET_POWER, TYPE_INTEGER, "GetEnemyShotPower", "i" );			// int GetEnemyShotPower( shot_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_GET_ANGLE, TYPE_FLOAT, "GetEnemyShotAngle", "i" );			// int GetEnemyShotAngle( shot_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_GET_SPEED_GU, TYPE_GU, "GetEnemyShotSpeedGU", "i" );			// gu GetEnemyShotSpeedGU( shot_id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_GET_REG, TYPE_INTEGER, "GetEnemyShotGroupReg", "" );			// int GetEnemyShotGroupReg();
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_GET_FREG, TYPE_FLOAT, "GetEnemyShotGroupFReg", "" );			// float GetEnemyShotGroupFReg();
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_GET_GREG, TYPE_GU, "GetEnemyShotGroupGReg", "" );			// gu GetEnemyShotGroupGReg();
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_GET_GREG_INDEX, TYPE_GU, "GetEnemyShotGroupGRegIndex", "i" );			// gu GetEnemyShotGroupGRegIndex( reg_index );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_ADD_ANGLE, TYPE_VOID, "EnemyShotAddAngle", "if" );			// void EnemyShotAddAngle( id, angle );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_IMAGE_SCALE, TYPE_VOID, "EnemyShotSetImgScale", "if" );	// void EnemyShotSetImgScale( id, scale );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_ADD_POS, TYPE_VOID, "EnemyShotAddPos", "iff" );				// void EnemyShotAddPos( id, x, y );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_ADD_SPEED, TYPE_VOID, "EnemyShotAddSpeed", "if" );			// void EnemyShotAddSpeed( id, speed );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_SET_SHAPE, TYPE_VOID, "EnemyShotSetShape", "ii" );			// void EnemyShotAddSpeed( id, shape );
	AddFunction( VM::SYS_ENEMY_SHOT_SET_LINE_SHOT_STATUS, TYPE_VOID, "EnemyShotSetLineShotStatus", "ifffff" );			// void EnemyShotSetLineShotStatus( id, x1, y1, x2, y2, thickness );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_ENABLE_INVISIBLE_MODE, TYPE_VOID, "EnableEnemyShotInvisibleMode", "i" );			// void EnableEnemyShotInvisibleMode( id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_DISABLE_INVISIBLE_MODE, TYPE_VOID, "DisableEnemyShotInvisibleMode", "i" );			// void DisableEnemyShotInvisibleMode( id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_ENABLE_INVINCIBLE_MODE, TYPE_VOID, "EnableEnemyShotInvincibleMode", "i" );			// void EnableEnemyShotInvincibleMode( id );
	AddFunction( VM::SYS_ENEMY_SHOT_GROUP_DISABLE_INVINCIBLE_MODE, TYPE_VOID, "DisableEnemyShotInvincibleMode", "i" );			// void DisableEnemyShotInvincibleMode( id );
	AddFunction( VM::SYS_SEND_EVENT_TO_ENEMY_SHOT_GROUP, TYPE_VOID, "SendEventToEnemyShotGroup", "ii" );			// void SendEventToEnemyShotGroup( id, event );
	AddFunction( VM::SYS_SET_ENEMY_SHOT_GROUP_SUB_ID, TYPE_VOID, "SetEnemyShotGroupSubID", "i" );			// void SetEnemyShotGroupSubID( id );
	AddFunction( VM::SYS_GET_EVENT, TYPE_INTEGER, "GetEvent", "" );										// int GetEvent();
	AddFunction( VM::SYS_ENEMY_SHOT_DELETE_BY_PLAYER_SKILL, TYPE_VOID, "EnemyShotDeleteByPlayerSkill", "i" );			// void EnemyShotDeleteByPlayerSkill( id );
	AddFunction( VM::SYS_ENEMY_SHOT_DELETE_BY_PLAYER_DAMAGE, TYPE_VOID, "EnemyShotDeleteByPlayerDamage", "i" );			// void EnemyShotDeleteByPlayerDamage( id );
	AddFunction( VM::SYS_ENEMY_SHOT_NOT_DELETE_BY_PLAYER_SKILL, TYPE_VOID, "EnemyShotNotDeleteByPlayerSkill", "i" );	// void EnemyShotNotDeleteByPlayerSkill( id );
	AddFunction( VM::SYS_ENEMY_SHOT_NOT_DELETE_BY_PLAYER_DAMAGE, TYPE_VOID, "EnemyShotNotDeleteByPlayerDamage", "i" );	// void EnemyShotNotDeleteByPlayerDamage( id );


	// System call for stage.
	AddFunction( VM::SYS_STAGE_ADD_ENEMY, TYPE_VOID, "AddEnemy", "i" );					// void AddEnemy( script_id );
	AddFunction( VM::SYS_STAGE_ADD_ENEMY_INIPOS, TYPE_VOID, "AddEnemyIniPos", "iff" );	// void AddEnemyIniPos( script_id, x, y );
	AddFunction( VM::SYS_STAGE_GET_FRAME, TYPE_INTEGER, "GetFrame", "" );				// int GetFrame();
	AddFunction( VM::SYS_STAGE_SET_FRAME, TYPE_VOID, "SetFrame", "i" );					// void SetFrame( frame );
	AddFunction( VM::SYS_UPDATE, TYPE_VOID, "Update", "" );								// void Update();
	AddFunction( VM::SYS_STAGE_PROC_ENEMY_PATTERN_FILE, TYPE_VOID, "ProcEnemyPatternFile", "ii" );	// void ProcEnemyPatternFile( file_id, frame );
	AddFunction( VM::SYS_STAGE_SET_BOSS_FLAG, TYPE_VOID, "SetStageBossFlag", "i" );			// void SetBossFlag( flag );
	AddFunction( VM::SYS_STAGE_GET_BOSS_FLAG, TYPE_INTEGER, "GetStageBossFlag", "" );		// int GetBossFlag();
	AddFunction( VM::SYS_STAGE_CREATE_BOSS, TYPE_INTEGER, "StageCreateBoss", "iff" );			// void CreateBoss( script_id, x, y );
	AddFunction( VM::SYS_STAGE_SET_FRAME_TOTAL, TYPE_VOID, "SetFrameTotal", "i" );				// void SetFrameTotal( frame );
	AddFunction( VM::SYS_STAGE_SWITCH_TO_NEXT_STAGE, TYPE_VOID, "SwitchToNextStage", "i" );		// void SwitchToNextStage( stage_no );
	AddFunction( VM::SYS_STAGE_GET_DIFFICULTY, TYPE_INTEGER, "GetDifficulty", "" );		// int GetDifficulty();
	AddFunction( VM::SYS_STAGE_SET_BOSS_PHASE_TOTAL, TYPE_VOID, "SetBossPhaseTotal", "i" );		// void SetBossPhaseTotal( phase_total );
	AddFunction( VM::SYS_STAGE_SET_BOSS_PHASE_SHIFT_HP, TYPE_VOID, "SetBossPhaseShiftHP", "ii" );		// void SetBossPhaseShiftHP( phase, hp );
	AddFunction( VM::SYS_SET_STAGE_BASE_CONS_LEVEL, TYPE_VOID, "SetStageBaseConsLevel", "i" );			// void SetStageBaseConsLevel( level );

	// System call for stage background.
	AddFunction( VM::SYS_STAGE_BACKGROUND_GET_COUNTER, TYPE_INTEGER, "StageBackgroundGetCounter", "" );	// int StageBackgroundGetCounter();

	// System call for graphics.
	AddFunction( VM::SYS_ENABLE_CAMERA, TYPE_VOID, "EnableCamera", "" );				// void EnableCamera();
	AddFunction( VM::SYS_SET_CAMERA_PROJ, TYPE_VOID, "SetCameraProj", "ffff" );			// void SetCameraProj( fovy, aspect, near, far );
	AddFunction( VM::SYS_SET_CAMERA_VIEW, TYPE_VOID, "SetCameraView", "fffffffff" );	// void SetCameraView( ex, ey, ez, lx, ly, lz, ux, uy, uz );
	// void DrawRect3DP( texID, x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4 );
	AddFunction( VM::SYS_DRAW_RECTANGLE_3D_FIXED_COLOR_TEXCOORD, TYPE_VOID, "DrawRect3DP", "iffffffffffff" );
	AddFunction( VM::SYS_BEGIN_3D_GRAPHICS, TYPE_VOID, "Begin3DGraphics", "" );		// void Begin3DGraphics();
	AddFunction( VM::SYS_END_3D_GRAPHICS, TYPE_VOID, "End3DGraphics", "" );			// void End3DGraphics();
	AddFunction( VM::SYS_ENABLE_FOG, TYPE_VOID, "EnableFog", "" );					// void EnableFog();
	AddFunction( VM::SYS_SET_FOG_PARAM, TYPE_VOID, "SetFogParam", "iiiiifff" );		// void SetFogParam( color_a, color_r, color_g, color_b, mode, begin, end, density );
	AddFunction( VM::SYS_DRAW_MODEL_FIXED_SCALE_ROT, TYPE_VOID, "DrawModelP", "ifff" );		// void DrawModelP( index, x, y, z );
	AddFunction( VM::SYS_SET_DIRLIGHT_DIR, TYPE_VOID, "SetDirLightDir", "ifff" );		// void SetDirLightDirection( index, x, y, z );
	AddFunction( VM::SYS_SET_DIRLIGHT_DIF_COLOR, TYPE_VOID, "SetDirLightDifColor", "iiiii" );	// void SetDirLightDifColor( idnex, a, r, g, b );
	AddFunction( VM::SYS_SET_DIRLIGHT_AMB_COLOR, TYPE_VOID, "SetDirLightAmbColor", "iiiii" );	// void SetDirLightAmbColor( idnex, a, r, g, b );
	AddFunction( VM::SYS_SET_DIRLIGHT_SPC_COLOR, TYPE_VOID, "SetDirLightSpcColor", "iiiii" );	// void SetDirLightSpcColor( idnex, a, r, g, b );
	AddFunction( VM::SYS_SET_DIRLIGHT_ATTEN, TYPE_VOID, "SetDirLightAtten", "ifff" );			// void SetDirLightAtten( index, atten0, atten1, atten2 );
	AddFunction( VM::SYS_ENABLE_DIRLIGHT, TYPE_VOID, "EnableDirLight", "i" );					// void EnableDirLight( index );
	AddFunction( VM::SYS_ENABLE_ALPHA_BLENDING, TYPE_VOID, "EnableAlphaBlending", "" );			// void EnableAlphaBlending();
	AddFunction( VM::SYS_DISABLE_ALPHA_BLENDING, TYPE_VOID, "DisableAlphaBlending", "" );		// void DisableAlphaBlending();
	AddFunction( VM::SYS_DRAW_MODEL_FIXED_ROT, TYPE_VOID, "DrawModelPS", "iffffff" );		// void DrawModelP( index, x, y, z, sx, sy, sz );
	AddFunction( VM::SYS_BEGIN_2D_GRAPHICS, TYPE_VOID, "Begin2DGraphics", "" );				// void Begin2DGraphics();
	AddFunction( VM::SYS_END_2D_GRAPHICS, TYPE_VOID, "End2DGraphics", "" );					// void End2DGraphics();
	AddFunction( VM::SYS_DRAW_TEXTURE_FIXED_ROT, TYPE_VOID, "DrawTexturePS", "iffffii" );	// void DrawTexture( index, x, y, sx, sy, centerize, color );
	AddFunction( VM::SYS_COLOR_ARGB, TYPE_INTEGER, "ColorARGB", "iiii" );					// int ColorARGB( a, r, g, b );

	// For effect.
	AddFunction( VM::SYS_CREATE_SCRIPT_EFFECT, TYPE_INTEGER, "CreateScriptEffect", "iff" );	// int CreateScriptEffect( id, x, y );
	AddFunction( VM::SYS_CREATE_SCRIPT_EFFECT_REG, TYPE_INTEGER, "CreateScriptEffectReg", "iffi" );		// int CreateScriptEffectReg( id, x, y, reg );
	AddFunction( VM::SYS_CREATE_SCRIPT_EFFECT_FREG5, TYPE_INTEGER, "CreateScriptEffectFReg5", "ifffffff" );	// int CreateScriptEffectFReg5( id, x, y, reg1, reg2, reg3, reg4, reg5 );
	AddFunction( VM::SYS_DRAW_TEXTURE_ATLAS_PSR, TYPE_VOID, "DrawTextureAtlasPSR", "ifffff" );	// void DrawTextureAtlasPSR( id, x, y, sx, sy, angle );
	AddFunction( VM::SYS_DRAW_TEXTURE_ATLAS_PSRC, TYPE_VOID, "DrawTextureAtlasPSRC", "ifffffi" );	// void DrawTextureAtlasPSRC( id, x, y, sx, sy, angle, color );
	AddFunction( VM::SYS_DRAW_TEXTURE_ATLAS_BLENDING_PSR, TYPE_VOID, "DrawTextureAtlasBlendingPSR", "iifffff" );	// void DrawTextureAtlasBlendingPSR( id, alpha_mode, x, y, sx, sy, angle );
	AddFunction( VM::SYS_DRAW_TEXTURE_ATLAS_BLENDING_PSRC, TYPE_VOID, "DrawTextureAtlasBlendingPSRC", "iifffffi" );		// void DrawTextureAtlasBlendingPSRC( id, alpha_mode, x, y, sx, sy, angle, color );
	AddFunction( VM::SYS_DRAW_CLIPED_TEXTURE_ATLAS_P, TYPE_VOID, "DrawClipedTextureAtlasP", "iffffff" );		// void DrawClipedTextureAtlasP( id, x, y, cx1, cy1, cx2, cy2 );
	AddFunction( VM::SYS_DRAW_CLIPED_TEXTURE_ATLAS_PSR, TYPE_VOID, "DrawClipedTextureAtlasPSR", "ifffffffff" );	// void DrawClipedTextureAtlasPSR( id, x, y, sx, sy, angle, cx1, cy1, cx2, cy2 );
	AddFunction( VM::SYS_DRAW_CLIPED_TEXTURE_ATLAS_PSRC, TYPE_VOID, "DrawClipedTextureAtlasPSRC", "ifffffiffff" ); // void DrawClipedTextureAtlasPSR( id, x, y, sx, sy, angle, color, cx1, cy1, cx2, cy2 );
	AddFunction( VM::SYS_DRAW_CLIPED_TEXTURE_ATLAS_BLENDING_P, TYPE_VOID, "DrawClipedTextureAtlasBlendingP", "iiffffff" );		// void DrawClipedTextureAtlasBlendingP( id, alpha_mode, x, y, cx1, cy1, cx2, cy2 );
	AddFunction( VM::SYS_DRAW_CLIPED_TEXTURE_ATLAS_BLENDING_PSR, TYPE_VOID, "DrawClipedTextureAtlasBlendingPSR", "iifffffffff" );	// void DrawClipedTextureAtlasBlendingPSR( id, alpha_mode, x, y, sx, sy, angle, cx1, cy1, cx2, cy2 );
	AddFunction( VM::SYS_DRAW_CLIPED_TEXTURE_ATLAS_BLENDING_PSRC, TYPE_VOID, "DrawClipedTextureAtlasBlendingPSRC", "iifffffiffff" );	// void DrawClipedTextureAtlasBlendingPSR( id, alpha_mode, x, y, sx, sy, angle, color cx1, cy1, cx2, cy2 );

	AddFunction( VM::SYS_GET_SCRIPT_EFFECT_REG, TYPE_INTEGER, "GetScriptEffectReg", "i" );		// int GetScriptEffectReg( no );
	AddFunction( VM::SYS_GET_SCRIPT_EFFECT_COUNTER, TYPE_INTEGER, "GetScriptEffectCounter", "" );	// int GetScriptEffectCounter();
	AddFunction( VM::SYS_TERMINATE_SCRIPT_EFFECT, TYPE_VOID, "TerminateScriptEffect", "" );		// void TerminateScriptEffect();
	AddFunction( VM::SYS_GET_SCRIPT_EFFECT_POS_X, TYPE_FLOAT, "GetScriptEffectPosX", "" );		// float GetScriptEffectPosX();
	AddFunction( VM::SYS_GET_SCRIPT_EFFECT_POS_Y, TYPE_FLOAT, "GetScriptEffectPosY", "" );		// float GetScriptEffectPosY();

	// Global variables.
	m_Variables.push_back( ValueTable() );
	m_Variables[ 0 ].SetGlobal();

	// HALT operation is first operation.
	OpHalt();

	m_File = f;
	ScanBegin();						// Initialize scanner.
	yy::Parser parser( *this );			// Build parser.
	int result = parser.parse();		// Analyze grammer.
	ScanEnd();							// End scanner.

	if( result != 0 ){
		return false;
	}

	int codeSize = LabelSetting();		// Set address to label.
	CreateData( data, codeSize );		// Create binary.

	return m_ErrorCount == 0;
}

void Compiler::error( const yy::location& location, const std::string& m )
{
	std::cerr << location << ": " << m << std::endl;
	++m_ErrorCount;
}

void Compiler::error( const std::string& m )
{
	std::cerr << m << std::endl;
	++m_ErrorCount;
}

bool Compiler::AddFunction( int index, int type, const char* pName, const char* pArgs )
{
	FunctionTag func( type );
	// Set argument
	if( !func.SetArgs( pArgs ) ){
		return false;
	}
	func.SetDeclaration();		// Set declared flag.
	func.SetSystem();			// System call flag.
	func.SetIndex( index );		// Set system call number.
	if( m_Functions.Add( pName, func ) == 0 ){
		return false;
	}
	return true;
}

struct DefValue
{
	Compiler*		m_pCompiler;
	int				m_Type;
	DefValue( Compiler* pCompiler, int type ) :	m_pCompiler( pCompiler ),
												m_Type( type )
	{
	}
	void operator()( ValueNode* pNode ) const
	{
		m_pCompiler->AddValue( pNode->GetLocation(), m_Type, pNode->GetString(), pNode->GetLeft() );
	}
};

void Compiler::DefineValue( const yy::location& location, int type, ValueList* pValueList )
{
	std::auto_ptr < ValueList > valueList( pValueList );
	pValueList->ForEach( DefValue( this, type ) );
}

void Compiler::DefineFunction( const yy::location& location, int type, const std::string* pName, ArgList* pArgs )
{
	std::auto_ptr < const std::string > name( pName );
	std::auto_ptr < ArgList > args( pArgs );

	const FunctionTag* pTag = m_Functions.Find( *pName );
	if( pTag ){
		if( !pTag->CheckArgList( pArgs ) ){
			error( location, "Function " + *pName + " has different type of argument." );
			return;
		}
	}
	else{
		FunctionTag func( type );
		func.SetArgs( pArgs );			// Set arguments.
		func.SetDeclaration();			// Set declared flag.
		func.SetIndex( MakeLabel() );	// Set label.
		if( m_Functions.Add( *pName, func ) == 0 ){
			error( location, "Internal Error : Failed to register to function table." );
		}
	}
}

struct AddVal
{
	Compiler*		m_pCompiler;
	ValueTable&		m_ValueTable;
	mutable int		m_Addr;
	AddVal( Compiler* pCompiler, ValueTable& values ) :	m_pCompiler( pCompiler ),
															m_ValueTable( values ),
															m_Addr( -4 )
	{
	}
	void operator()( ArgDef* pArg ) const
	{
		if( !m_ValueTable.AddArg( pArg->GetType(), pArg->GetName(), m_Addr ) ){
			m_pCompiler->error( pArg->GetLocation(), "Argument " + pArg->GetName() + " is already registered." );
		}
		--m_Addr;
	}
};

void Compiler::AddFunction( const yy::location& location, int type, const std::string* pName, ArgList* pArgs, StateBlock* pBlock )
{
	std::auto_ptr < const std::string > name( pName );
	std::auto_ptr < ArgList > args( pArgs );
	std::auto_ptr < StateBlock > block( pBlock );

	FunctionTag* pTag = m_Functions.Find( *pName );
	if( pTag ){
		if( pTag->IsDefinition() ){
			error( location, "Function : " + *pName + " is already registered." );
			return;
		}
		if( pTag->IsDeclaration() && !pTag->CheckArgList( pArgs ) ){
			error( location, "Function : " + *pName + " has different argument type." );
			return;
		}
		pTag->SetDefinition();
	}
	else{
		FunctionTag func( type );
		func.SetArgs( pArgs );
		func.SetDefinition();
		func.SetIndex( MakeLabel() );
		pTag = m_Functions.Add( *pName, func );
		if( pTag == 0 ){
			error( location, "Internal error : The function can not be registered." );
		}
	}
	m_CurFunctionName = *pName;
	m_CurFunctionType = type;

	SetLabel( pTag->GetIndex() );	// Entry point of the function.
	
	BlockIn();		// Increase the stack for variables.

	// Register arguments.
	if( pArgs ){
		pArgs->ForEachReverse( AddVal( this, m_Variables.back() ) );
	}

	// Analyze block.
	if( pBlock ){
		pBlock->Analyze( this );
	}

	const VMCode& code = m_Statement.back();
	if( type == TYPE_VOID ){
		if( code.m_OP != VM_RETURN ){
			OpReturn();
		}
	}
	else{
		if( code.m_OP != VM_RETURNV ){
			error( location, "Function : " + *pName + " has no return." );
		}
	}

	BlockOut();

	m_CurFunctionName.clear();
}

void Compiler::AddValue( const yy::location& location, int type, const std::string& name, const Node* pNode )
{
	int size = 1;
	if( pNode ){
		if( pNode->GetOP() != OP_INT_CONST /*|| pNode->GetOP() != OP_FLOAT_CONST*/ ){
			error( location, "Array size must be constant." );
		}
		else if( pNode->GetValue() <= 0 ){
			error( location, "Array size must be positive." );
		}
		size = pNode->GetValue();
	}

	ValueTable& values = m_Variables.back();
	if( !values.Add( type, name, size ) ){
		error( location, "Variable : " + name + "is already registered." );
	}
}

int Compiler::MakeLabel()
{
	int index = (int)m_Labels.size();
	m_Labels.push_back( Label( index ) );
	return index;
}

void Compiler::SetLabel( int label )
{
	m_Statement.push_back( VMCode( VM_MAXCOMMAND, label ) );
}

void Compiler::PushString( const std::string& str )
{
	PushString( (int)m_TextTable.size() );
	m_TextTable.insert( m_TextTable.end(), str.begin(), str.end() );
	m_TextTable.push_back( '\0' );
}

void Compiler::PushGU( const GameEngine::GameUnit& gu )
{
	PushString( (int) gu.GetRawValue() );
}

bool Compiler::JmpBreakLabel()
{
	if( m_BreakIndex < 0 ){
		return false;
	}
	OpJmp( m_BreakIndex );
	return true;
}

void Compiler::BlockIn()
{
	int startAddr = 0;
	if( m_Variables.size() > 1 ){
		startAddr = m_Variables.back().GetSize();
	}
	m_Variables.push_back( ValueTable( startAddr ) );
}

void Compiler::BlockOut()
{
	m_Variables.pop_back();
}

void Compiler::AllocStack()
{
	OpAllocStack( m_Variables.back().GetSize() );
}

// Calculate address to the label.
struct CalcAddr
{
	std::vector < Label >&		m_Labels;
	int&						m_Pos;
	CalcAddr( std::vector < Label >& labels, int& pos ) : m_Labels( labels ), m_Pos( pos )
	{
	}
	void operator()( const VMCode& code )
	{
		if( code.m_OP == VM_MAXCOMMAND ){
			m_Labels[ code.m_Arg1 ].m_Pos = m_Pos;
		}
		else{
			m_Pos += code.m_Size;
		}
	}
};

// Set address to the label.
struct SetAddr
{
	std::vector < Label >&	m_Labels;
	SetAddr( std::vector < Label >& labels ) : m_Labels( labels )
	{
	}
	void operator()( VMCode& code )
	{
		switch( code.m_OP ){
			case VM_JMP:
			case VM_JMPC:
			case VM_JMPNC:
			case VM_TEST:
			case VM_CALL:
				code.m_Arg1 = m_Labels[ code.m_Arg1 ].m_Pos;
				break;
		}
	}
};

int Compiler::LabelSetting()
{
	int pos = 0;
	std::for_each( m_Statement.begin(), m_Statement.end(), CalcAddr( m_Labels, pos ) );
	std::for_each( m_Statement.begin(), m_Statement.end(), SetAddr( m_Labels ) );

	return pos;
}

struct CopyCode
{
	unsigned char* p;
	CopyCode( unsigned char* pCode ) : p( pCode )
	{
	}
	void operator()( const VMCode& code )
	{
		p = code.Get( p );
	}
};

bool Compiler::CreateData( VM::Data& data, int codeSize )
{
	// Entry point for the program is main function.
	const FunctionTag* pTag = GetFunctionTag( "main" );
	if( pTag == 0 ){
		error( "Function : main is not found." );
		return false;
	}

	data.m_Command = new unsigned char[ codeSize ];		// Binary data.
	data.m_TextBuffer = new char[ m_TextTable.size() ];	// Source code.
	data.m_CommandSize = codeSize;						// Binary size.
	data.m_TextSize = (int)m_TextTable.size();			// Source code size.
	data.m_ValueSize = (int)m_Variables[ 0 ].GetSize();
	data.m_EntryPoint = m_Labels[ pTag->m_Index ].m_Pos;	// Entry point.(main function)

	if( data.m_TextSize != 0 ){
		memcpy( data.m_TextBuffer, &m_TextTable[ 0 ], data.m_TextSize );
	}

	std::for_each( m_Statement.begin(), m_Statement.end(), CopyCode( data.m_Command ) );
	
	return true;
}


#ifdef _DEBUG
void Compiler::DebugDump()
{
	std::cout << "---variables---" << std::endl;
	size_t vsize = m_Variables.size();
	std::cout << "value stack = " << vsize << std::endl;
	for( size_t i = 0; i < vsize; ++i ){
		m_Variables[ i ].Dump();
	}
	std::cout << "---code---" << std::endl;

	static const char* opName[] = {
#define VM_NAMETABLE
#include "VMCode.h"
#undef VM_NAMETABLE
		"LABEL",
	};

	int pos = 0;
	size_t size = m_Statement.size();
	for( size_t i = 0; i < size; ++i ){
		std::cout << std::setw( 6 ) << pos << ": " << opName[ m_Statement[ i ].m_OP ];//opName[ m_Statement[ i ].m_OP ];
		if( m_Statement[ i ].m_Size > 1 ){
			std::cout << ", " << m_Statement[ i ].m_Arg1;
		}
		std::cout << std::endl;
		if( m_Statement[ i ].m_OP != VM_MAXCOMMAND ){
			pos += m_Statement[ i ].m_Size;
		}
	}
}

#endif


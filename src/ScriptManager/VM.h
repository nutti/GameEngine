#ifndef INCLUDED_VM_H
#define INCLUDED_VM_H

#include <stdio.h>
#include <vector>
#include <Windows.h>
#include "VMValue.h"

#include "../Math.hpp"

#ifdef _MSC_VER
#pragma warning( disable: 4996 )
#endif

#define VM_ENUMDEF
enum
{
#include "VMCode.h"
	VM_MAXCOMMAND,
};
#undef VM_ENUMDEF

namespace VM
{

	enum
	{
		SYS_PRINT,
		SYS_TOSTR,
		SYS_PRINT_GU,

		SYS_UPDATE,					// Force updating.

		SYS_INT_TO_FLOAT,			// Convert int to float.
		SYS_FLOAT_TO_INT,			// Convert float to int.
		SYS_GU_TO_INT,				// Convert gu to int.
		SYS_INT_TO_GU,				// Convert int to gu.
		SYS_GU_TO_FLOAT,			// Convert gu to float.
		SYS_FLOAT_TO_GU,			// Convert float to gu.

		SYS_SIN,					// Call math library sin().
		SYS_COS,					// Call math library cos().
		SYS_ATAN2,					// Call math library atan2().
		SYS_ABS,					// Call math library std::abs( int ).
		SYS_FABS,					// Call math library std::fabs( float ).
		SYS_SQRT,					// Call math library std::sqrt( float ).

		// GU version math library.
		SYS_SIN_GU,
		SYS_COS_GU,
		SYS_ATAN2_GU,
		SYS_ABS_GU,
		SYS_FABS_GU,
		SYS_SQRT_GU,

		SYS_DEG_TO_RAD,					// Convert degree to radius.
		SYS_RAD_TO_DEG,					// Convert radius to degree.

		SYS_GET_PLAYER_POSX_GU,			// Get player position. (X, gameunit)
		SYS_GET_PLAYER_POSY_GU,			// Get player position. (Y, gameunit)
		SYS_GET_PLAYER_POSX,			// Get player position. ( X )
		SYS_GET_PLAYER_POSY,			// Get player position. ( Y )
		SYS_GET_PLAYER_HP,				// Get player HP.
		SYS_GET_PLAYER_CUR_CONS,		// Get current cons.
		SYS_GET_PLAYER_CONS_GAUGE,		// Get cons gauge.
		SYS_GET_PLAYER_SCORE_TOTAL,		// Get total score.
		SYS_GET_PLAYER_CRYSTAL_TOTAL,	// Get total crystal.
		SYS_GET_PLAYER_CRYSTAL_USED_TOTAL,	// Get used total crystal.
		SYS_GET_PLAYER_KILLED_TOTAL,	// Get total killed.

		SYS_GET_RANDOM_F,			// Get random. (float)
		SYS_GET_RANDOM_GU,			// Get random. (gameunit)
		SYS_ADD_SCORE,				// Add score.

		SYS_PLAY_SE,		// Play SE.
		SYS_STOP_SE,		// Stop SE.
		SYS_PLAY_BGM,		// Play BGM.
		SYS_STOP_BGM,		// Stop BGM.

		// For enemy script.
		SYS_ENEMY_GET_POSX_GU,				// Get position. (X, gameunit)
		SYS_ENEMY_GET_POSY_GU,				// Get position. (Y, gameunit)
		SYS_ENEMY_SET_COLLISION_RADIUS_GU,	// Set collision radius. (gameunit)
		SYS_ENEMY_SET_POS_GU,				// Set position. (gameunit)
		SYS_ENEMY_SET_POS_3D_GU,	// Set position. (3D coordinate, gameunit)
		SYS_ENEMY_GET_POSX,			// Get position. ( X )
		SYS_ENEMY_GET_POSY,			// Get position. ( Y )
		SYS_ENEMY_GET_HP,			// Get HP.
		SYS_ENEMY_GET_SPEED,		// Get speed.
		SYS_ENEMY_GET_COUNTER,		// Get counter.
		SYS_ENEMY_GET_COUNTER_F,	// Get counter. ( float )
		SYS_ENEMY_GET_ROT_ANGLE,	// Get angle.
		SYS_ENEMY_GET_CONS_GAUGE,	// Get conscious gauge.
		SYS_ENEMY_SET_NAME,			// Set name.
		SYS_ENEMY_SET_POS,			// Set position.
		SYS_ENEMY_SET_POS_3D,		// Set position. (3D coordinate)
		SYS_ENEMY_SET_ANGLE,		// Set angle.
		SYS_ENEMY_SET_ANGLE_3D,		// Set angle. (3D)
		SYS_ENEMY_SET_SCALE_3D,		// Set scaling. (3D)
		SYS_ENEMY_SET_SPEED,		// Set speed.
		SYS_ENEMY_SET_HP,			// Set HP.
		SYS_ENEMY_SET_IMAGE,		// Set image.
		SYS_ENEMY_SET_MODEL,		// Set model.
		SYS_ENEMY_SET_SKIN_MODEL,	// Set skin model.
		SYS_ENEMY_SET_MODEL_ANIM_TIME,		// Set animation time of model.
		SYS_ENEMY_SET_COLLISION_RADIUS,		// Set collision radius.
		SYS_ENEMY_SET_SCORE,				// Set score.
		SYS_ENEMY_SET_BOSS_FLAG,			// Set boss flag.
		SYS_ENEMY_SET_CONS_GAUGE,			// Set conscious gauge.
		SYS_ENEMY_SET_CONS_ATTR,			// Set conscious attr.
		SYS_ENEMY_ENABLE_INVINCIBLE,		// Enable invincible mode.
		SYS_ENEMY_DISABLE_INVINCIBLE,			// Disable invincible mode.
		SYS_ENEMY_ENABLE_NON_COLLISION_MODE,	// Enable non collision mode.
		SYS_ENEMY_DISABLE_NON_COLLISION_MODE,	// Disable non collision mode.
		SYS_ENEMY_INVOKE_CONS_SKILL,		// Invoke skill.
		SYS_ENEMY_STOP_CONS_SKILL,			// Stop skill.
		SYS_ENEMY_SHIFT_NEXT_MODE,			// Shift next mode.
		SYS_ENEMY_GET_REG,					// Get registers.
		SYS_ENEMY_GET_FREG,					// Get float register.
		SYS_ENEMY_SEARCH_ITEM,				// Search item.
		SYS_ENEMY_GET_ITEM_POS_X,			// Get item position x.
		SYS_ENEMY_GET_ITEM_POS_Y,			// Get item position y.
		SYS_ENEMY_DAMAGED_BY_CONS_SHOT,		// Damaged by cons shot?
		SYS_ENEMY_DAMAGED_BY_CONS_SHOT_INDEX,	// Damaged by cons shot? (index)
		SYS_SEARCH_ENEMY_IN_SKILL_MODE,		// Search enemy in skill mode.
		SYS_SEARCH_ENEMY_IN_SKILL_MODE_BY_NAME,		// Search enemy in skill mode by name.
		SYS_DAMAGE_ENEMY,					// Damage enemy.
		SYS_ENEMY_ALIVE,					// Enemy alive?
		SYS_ENEMY_IN_SKILL_MODE,			// Enemy in skill mode?

		SYS_ENEMY_GET_ITEM_POS_X_GU,		// Get item position x. (gameunit)
		SYS_ENEMY_GET_ITEM_POS_Y_GU,		// Get item position y. (gameunit)

		SYS_CREATE_ENEMY_INI_POS_GU,		// Create enemy. (Initial position with gu.)
		SYS_CREATE_ENEMY_INI_POS_REG_GU,	// Create enemy. (Initial position with gu. 1 register.)
		SYS_CREATE_ENEMY_INI_POS_REG5_GU,	// Create enemy. (Initial position with gu. 5 register.)
		SYS_ENEMY_CREATE_SHOT_1,			// Create enemy shot. (Linear)
		SYS_ENEMY_CREATE_SHOT_1_GU,			// Create enemy shot. (Linear. gameunit)
		SYS_ENEMY_CREATE_EFFECT_1,			// Create effect. (Bombbed Effect.)
		SYS_ENEMY_CREATE_EFFECT_1_GU,		// Create effect. (Bombbed Effect. gameunit)
		SYS_ENEMY_CREATE_SHOT_GROUP,		// Create shot group.
		SYS_ENEMY_CREATE_SHOT_GROUP_REG,	// Create shot group. (1 Register)
		SYS_ENEMY_CREATE_SHOT_GROUP_FREG,	// Create shot group. (1 Floating-point register)
		SYS_ENEMY_CREATE_SHOT_GROUP_GREG,	// Create shot group. (1 gameunit register)
		SYS_ENEMY_CREATE_SHOT_GROUP_GREG5,	// Create shot group. (5 gameunit register)
		SYS_CREATE_ITEM,		// Create item.
		SYS_CREATE_ITEM_GU,		// Create item. (gameunit)
		
		SYS_SET_ENEMY_SUB_ID,		// Set enemy sub ID.
		SYS_SEND_EVENT_TO_ENEMY,	// Send event to enemy

		// For enemy shot.
		SYS_SET_ENEMY_SHOT_LENGTH,
		SYS_CREATE_ENEMY_SHOT_BY_ID_ORTHO,
		SYS_CREATE_ENEMY_SHOT_BY_ID_POLAR,
		SYS_CREATE_ENENY_SHOT_BY_ID_LOCAL_ORTHO,
		SYS_CREATE_ENEMY_SHOT_BY_ID_LOCAL_POLAR,
		SYS_CHANGE_ENEMY_SHOT_ID,
		SYS_DELETE_ENEMY_SHOT,
		SYS_GET_ENEMY_SHOT_TEX_COLOR,
		SYS_SET_ENEMY_SHOT_TEX_COLOR,
		SYS_GET_ENEMY_SHOT_ID,

		// For enemy shot group.
		SYS_ENEMY_SHOT_GROUP_SET_STATUS_GU,
		SYS_ENEMY_SHOT_GROUP_SET_STATUS_ATLAS_GU,
		SYS_ENEMY_SHOT_GROUP_GET_POS_X_GU,
		SYS_ENEMY_SHOT_GROUP_GET_POS_Y_GU,
		SYS_ENEMY_SHOT_GROUP_SET_SPEED_GU,
		SYS_ENEMY_SHOT_GROUP_SET_ANGLE_GU,
		SYS_ENEMY_SHOT_GROUP_SET_POS_GU,
		SYS_ENEMY_SHOT_GROUP_ADD_ANGLE_GU,
		SYS_ENEMY_SHOT_GROUP_ADD_SPEED_GU,
		SYS_ENEMY_SHOT_SET_LINE_SHOT_STATUS_GU,
		SYS_ENEMY_SHOT_GROUP_GET_ANGLE_GU,

		
		SYS_ENEMY_SHOT_GROUP_CREATE_SHOT,
		SYS_ENEMY_SHOT_GROUP_CREATE_CONS_SHOT,
		SYS_ENEMY_SHOT_GROUP_SET_POS,
		SYS_ENEMY_SHOT_GROUP_SET_POWER,
		SYS_ENEMY_SHOT_GROUP_SET_ANGLE,
		SYS_ENEMY_SHOT_GROUP_SET_SPEED,
		SYS_ENEMY_SHOT_GROUP_SET_IMAGE,
		SYS_ENEMY_SHOT_GROUP_SET_IMAGE_ATLAS,
		SYS_ENEMY_SHOT_GROUP_SET_COLLISION_RADIUS,
		SYS_ENEMY_SHOT_GROUP_SET_COLLISION_RADIUS_GU,
		SYS_ENEMY_SHOT_GROUP_SET_STATUS,
		SYS_ENEMY_SHOT_GROUP_SET_MOVEMENT,
		SYS_ENEMY_SHOT_GROUP_GET_COUNTER,
		SYS_ENEMY_SHOT_GROUP_GET_POS_X,
		SYS_ENEMY_SHOT_GROUP_GET_POS_Y,
		SYS_ENEMY_SHOT_GROUP_GET_POWER,
		SYS_ENEMY_SHOT_GROUP_GET_ANGLE,
		SYS_ENEMY_SHOT_GROUP_GET_SPEED_GU,
		SYS_ENEMY_SHOT_GROUP_GET_REG,
		SYS_ENEMY_SHOT_GROUP_GET_FREG,
		SYS_ENEMY_SHOT_GROUP_GET_GREG,
		SYS_ENEMY_SHOT_GROUP_GET_GREG_INDEX,
		SYS_ENEMY_SHOT_GROUP_ADD_ANGLE,
		SYS_ENEMY_SHOT_GROUP_SET_IMAGE_SCALE,
		SYS_ENEMY_SHOT_GROUP_ADD_POS,
		SYS_ENEMY_SHOT_GROUP_ADD_SPEED,
		SYS_ENEMY_SHOT_GROUP_SET_IMG_ROT_MODE,
		SYS_ENEMY_SHOT_GROUP_SET_ALPHA_BLENDING_MODE,
		SYS_ENEMY_SHOT_GROUP_SET_IMG_ROT_ANGLE_PER_FRAME,
		SYS_ENEMY_SHOT_GROUP_SET_DRAWING_MULTIPLICITY,
		SYS_ENEMY_SHOT_GROUP_SET_SHAPE,
		SYS_ENEMY_SHOT_SET_LINE_SHOT_STATUS,
		SYS_ENEMY_SHOT_GROUP_ENABLE_INVISIBLE_MODE,
		SYS_ENEMY_SHOT_GROUP_DISABLE_INVISIBLE_MODE,
		SYS_ENEMY_SHOT_GROUP_ENABLE_INVINCIBLE_MODE,
		SYS_ENEMY_SHOT_GROUP_DISABLE_INVINCIBLE_MODE,
		SYS_SET_ENEMY_SHOT_GROUP_SUB_ID,
		SYS_SEND_EVENT_TO_ENEMY_SHOT_GROUP,
		SYS_GET_EVENT,
		SYS_ENEMY_SHOT_DELETE_BY_PLAYER_SKILL,
		SYS_ENEMY_SHOT_DELETE_BY_PLAYER_DAMAGE,
		SYS_ENEMY_SHOT_NOT_DELETE_BY_PLAYER_SKILL,
		SYS_ENEMY_SHOT_NOT_DELETE_BY_PLAYER_DAMAGE,

		// For stage script.
		SYS_STAGE_ADD_ENEMY,				// Add enemy.
		SYS_STAGE_ADD_ENEMY_INIPOS,			// Add enemy with initial position.
		SYS_STAGE_GET_FRAME,				// Get frame count.
		SYS_STAGE_SET_FRAME,				// Set frame count.
		SYS_STAGE_PROC_ENEMY_PATTERN_FILE,	// Process enemy pattern file.
		SYS_STAGE_SET_BOSS_FLAG,			// Set boss flag.
		SYS_STAGE_GET_BOSS_FLAG,			// Get boss flag.
		SYS_STAGE_CREATE_BOSS,				// Create boss.
		SYS_STAGE_SET_FRAME_TOTAL,			// Set frame total.
		SYS_STAGE_SWITCH_TO_NEXT_STAGE,		// Switch to next stage.
		SYS_STAGE_GET_DIFFICULTY,			// Get difficulty.
		SYS_STAGE_SET_BOSS_PHASE_TOTAL,		// Set boss phase total.
		SYS_STAGE_SET_BOSS_PHASE_SHIFT_HP,	// Set boss phase shift hp.
		SYS_SET_STAGE_BASE_CONS_LEVEL,		// Set stage base cons level.

		// For stage background.
		SYS_STAGE_BACKGROUND_GET_COUNTER,	// Get count.

		// For graphics.
		SYS_ENABLE_CAMERA,								// Enable camera.
		SYS_SET_CAMERA_PROJ,							// Set camera projection matrix.
		SYS_SET_CAMERA_VIEW,							// Set camera view matrix.
		SYS_DRAW_RECTANGLE_3D_FIXED_COLOR_TEXCOORD,		// Draw polygon 3D. (Fixed color and texture coordinate.)
		SYS_BEGIN_3D_GRAPHICS,							// Begin 3D Graphics.
		SYS_END_3D_GRAPHICS,							// End 3D Graphics.
		SYS_ENABLE_FOG,									// Enable fog effect.
		SYS_SET_FOG_PARAM,								// Set fog parameter.
		SYS_DRAW_MODEL_FIXED_SCALE_ROT,					// Draw 3D model.
		SYS_SET_DIRLIGHT_DIR,							// Set direction for directional light.
		SYS_SET_DIRLIGHT_AMB_COLOR,						// Set ambient color for directional light.
		SYS_SET_DIRLIGHT_DIF_COLOR,						// Set diffuse color for directional light.
		SYS_SET_DIRLIGHT_SPC_COLOR,						// Set specular color for directional light.
		SYS_SET_DIRLIGHT_ATTEN,							// Set attenuation for directional light.
		SYS_ENABLE_DIRLIGHT,							// Enable directional light.
		SYS_ENABLE_ALPHA_BLENDING,						// Enable alpha blending.
		SYS_DISABLE_ALPHA_BLENDING,						// Disable alpha blending.
		SYS_DRAW_MODEL_FIXED_ROT,						// Draw 3D model (with scale).
		SYS_BEGIN_2D_GRAPHICS,							// Begin 2D Graphics.
		SYS_END_2D_GRAPHICS,							// End 2D Graphics.
		SYS_DRAW_TEXTURE_FIXED_ROT,						// Draw 2D texture (with texture).
		SYS_COLOR_ARGB,									// Get ARGB color.

		// For effect.
		SYS_CREATE_SCRIPT_EFFECT,						// Create script effect.
		SYS_CREATE_SCRIPT_EFFECT_REG,					// Create script effect. (1 reg)
		SYS_CREATE_SCRIPT_EFFECT_FREG5,					// Create script effect. (5 reg, float)
		SYS_DRAW_TEXTURE_ATLAS_PSR,						// Draw texture atlas (pos/scale/rot)
		SYS_DRAW_TEXTURE_ATLAS_PSRC,					// Draw texture atlas (pos/scale/rot/color)
		SYS_DRAW_TEXTURE_ATLAS_BLENDING_PSR,			// Draw texture atlas (pos/scale/rot, blending)
		SYS_DRAW_TEXTURE_ATLAS_BLENDING_PSRC,			// Draw texture atlas (pos/scale/rot/color, blending)

		SYS_DRAW_CLIPED_TEXTURE_ATLAS_P,				// Draw cliped texture atlas (pos)
		SYS_DRAW_CLIPED_TEXTURE_ATLAS_PSR,				// Draw cliped texture atlas (pos/scale/rot)
		SYS_DRAW_CLIPED_TEXTURE_ATLAS_PSRC,				// Draw cliped texture atlas (pos/scale/rot/color)
		SYS_DRAW_CLIPED_TEXTURE_ATLAS_BLENDING_P,		// Draw cliped texture atlas (pos, blending)
		SYS_DRAW_CLIPED_TEXTURE_ATLAS_BLENDING_PSR,		// Draw cliped texture atlas (pos/sclae/rot, blending)
		SYS_DRAW_CLIPED_TEXTURE_ATLAS_BLENDING_PSRC,	// Draw cliped texture atlas (pos/sclae/rot/color, blending)


		SYS_GET_SCRIPT_EFFECT_REG,						// Get script effect register.
		SYS_GET_SCRIPT_EFFECT_COUNTER,					// Get script effect counter.
		SYS_TERMINATE_SCRIPT_EFFECT,					// Terminate script effect.
		SYS_GET_SCRIPT_EFFECT_POS_X,					// Get script effect position X. (float)
		SYS_GET_SCRIPT_EFFECT_POS_Y,					// Get script effect position Y. (float)
	};

	class Data
	{
	public:
		unsigned char*	m_Command;			// Binary command
		char*			m_TextBuffer;		// Source code
		int				m_CommandSize;		// Binary size
		int				m_TextSize;			// Source code size
		int				m_EntryPoint;		// Entry point.( main function )
		int				m_ValueSize;
	public:
		Data() : m_Command( 0 ), m_TextBuffer( 0 )
		{
		}
		~Data()
		{
			delete [] m_Command;
			delete [] m_TextBuffer;
		}
	};

	class DivideByZero : public std::exception
	{
	public:
		const char* what() const throw()
		{
			return "divide by zero.";
		}
	};

	class VCPU
	{
	public:
		const static int STACK_SIZE = 400;			// Stack size.
		const static int GLOBAL_FLAG = 0x4000000;	// Flag for the global variable.
		const static int GLOBAL_MASK = 0x3ffffff;
	protected:
		Data*				m_pData;
		unsigned char*		m_pCommand;
		unsigned char*		m_pCommandPtr;
		int					m_CommandSize;
		char*				m_pTextBuffer;
		int					m_TextSize;
		VM::Stack < VM::Value, STACK_SIZE >		m_Stack;
		std::vector < VM::Value >				m_GlobalValue;
		int										m_StackBase;
		bool				m_HasUpdateReq;		// アップデート要求を受け取っているか
		bool				m_Terminated;		// プログラムは終了したか
	public:
		VCPU( Data* data ) : m_pData( data ), m_HasUpdateReq( false ), m_Terminated( false )
		{
		}
		VCPU() : m_pData( NULL ), m_HasUpdateReq( false ), m_Terminated( false )
		{
		}
		virtual ~VCPU()
		{
		}
		int Run();					// 実行開始/再開
		void Init();				// 仮想CPUの初期化
		void Init( Data* pData );	// 仮想CPUの初期化
		bool Terminated() const;	// プログラムは終了したか
	protected:
		void PushConst( int val )
		{
			Push( val );
		}
		void PushString( int val )
		{
			Push( std::string( m_pTextBuffer + val ) );
		}
		void PushValue( int val )
		{
			Push( m_GlobalValue[ val ] );
		}
		void PushLocal( int val )
		{
			Push( m_Stack[ val + m_StackBase ] );
		}
		void PushArray( int val )
		{
			int index = Top().m_Integer;
			Pop();
			Push( m_GlobalValue[ val + index ] );
		}
		void PushLocalArray( int val )
		{
			int index = Top().m_Integer;
			Pop();
			Push( m_Stack[ val + m_StackBase + index ] );
		}
		void PushLocalRef( int val )
		{
			int addr = m_Stack[ val + m_StackBase ].m_Integer;
			Push( RefToValue( addr ) );
		}
		void PushLocalArrayRef( int val )
		{
			int addr = m_Stack[ val + m_StackBase ].m_Integer;
			int index = Top().m_Integer;
			Pop();
			Push( RefToValue( addr + index ) );
		}
		void PushAddr( int val )
		{
			if( ( val & GLOBAL_FLAG ) == 0 ){
				val += +m_StackBase;
			}
			Push( val );
		}
		void PushArrayAddr( int val )
		{
			if( ( val & GLOBAL_FLAG ) == 0 ){
				val += +m_StackBase;
			}
			int index = Top().m_Integer;
			Pop();
			Push( val + index );
		}
		void PopValue( int val )
		{
			m_GlobalValue[ val ] = Top();
			Pop();
		}
		void PopLocal( int val )
		{
			m_Stack[ val + m_StackBase ] = Top();
			Pop();
		}
		void PopArray( int val )
		{
			int index = Top().m_Integer;
			Pop();
			m_GlobalValue[ val + index ] = Top();
			Pop();
		}
		void PopLocalArray( int val )
		{
			int index = Top().m_Integer;
			Pop();
			m_Stack[ val + m_StackBase + index ] = Top();
			Pop();
		}
		void PopLocalRef( int val )
		{
			int addr = m_Stack[ val + m_StackBase ].m_Integer;
			SetRef( addr, Top() );
			Pop();
		}
		void PopLocalArrayRef( int val )
		{
			int addr = m_Stack[ val + m_StackBase ].m_Integer;
			int index = Top().m_Integer;
			Pop();
			SetRef( addr + index, Top() );
			Pop();
		}

		void OpAllocStack( int val )
		{
			m_Stack.Resize( m_StackBase + val );
		}
		void OpPop()
		{
			Pop();
		}
		void OpNeg()
		{
			Top().m_Integer = -Top().m_Integer;
		}
		void OpEq()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs == rhs );
		}
		void OpNe()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs != rhs );
		}
		void OpGt()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs > rhs );
		}
		void OpGe()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs >= rhs );
		}
		void OpLt()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs < rhs );
		}
		void OpLe()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs <= rhs );
		}
		void OpLogAnd()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs && rhs );
		}
		void OpLogOr()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs || rhs );
		}
		void OpAnd()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs & rhs );
		}
		void OpOr()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs | rhs );
		}
		void OpLShift()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs << rhs );
		}
		void OpRShift()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs >> rhs );
		}
		void OpAdd()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs + rhs );
		}
		void OpSub()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs - rhs );
		}
		void OpMul()
		{
			int rhs = Top().m_Integer;
			Pop();
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs * rhs );
		}
		void OpDiv()
		{
			int rhs = Top().m_Integer;
			Pop();
			if( rhs == 0 ){
				throw DivideByZero();
			}
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs / rhs );
		}
		void OpMod()
		{
			int rhs = Top().m_Integer;
			Pop();
			if( rhs == 0 ){
				throw DivideByZero();
			}
			int lhs = Top().m_Integer;
			Pop();
			Push( lhs % rhs );
		}
		void OpStrEq()
		{
			const std::string& rhs = Text( Top() );
			Pop();
			const std::string& lhs = Text( Top() );
			Pop();

			Push( lhs == rhs );
		}
		void OpStrNe()
		{
			const std::string& rhs = Text( Top() );
			Pop();
			const std::string& lhs = Text( Top() );
			Pop();

			Push( lhs != rhs );
		}
		void OpStrGt()
		{
			const std::string& rhs = Text( Top() );
			Pop();
			const std::string& lhs = Text( Top() );
			Pop();

			Push( lhs > rhs );
		}
		void OpStrGe()
		{
			const std::string& rhs = Text( Top() );
			Pop();
			const std::string& lhs = Text( Top() );
			Pop();

			Push( lhs >= rhs );
		}
		void OpStrLt()
		{
			const std::string& rhs = Text( Top() );
			Pop();
			const std::string& lhs = Text( Top() );
			Pop();

			Push( lhs < rhs );
		}
		void OpStrLe()
		{
			const std::string& rhs = Text( Top() );
			Pop();
			const std::string& lhs = Text( Top() );
			Pop();

			Push( lhs <= rhs );
		}
		void OpStrAdd()
		{
			const std::string& rhs = Text( Top() );
			Pop();
			const std::string& lhs = Text( Top() );
			Pop();

			Push( lhs + rhs );
		}
		void OpJmp( int val )
		{
			jmp( val );
		}
		void OpJmpC( int val )
		{
			int cond = Top().m_Integer;
			Pop();
			if( cond ){
				jmp( val );
			}
		}
		void OpJmpNC( int val )
		{
			int cond = Top().m_Integer;
			Pop();
			if( !cond ){
				jmp( val );
			}
		}
		void OpTest( int val )
		{
			int value = Top().m_Integer;
			Pop();
			if( value == Top().m_Integer ){
				Pop();
				jmp( val );
			}
		}
		void OpCall( int val )
		{
			Push( m_StackBase );			// Previous stack baseline.
			Push( Addr() );					// Return address.
			m_StackBase = m_Stack.Size();	// New stack baseline.
			jmp( val );
		}
		void OpReturn()
		{
			m_Stack.Resize( m_StackBase );	// Release allocated stack.
			int addr = Top().m_Integer;		// Return address.
			Pop();
			m_StackBase = Top().m_Integer;	// Restore previous stack baseline.
			Pop();
			int argCount = Top().m_Integer;	// Get number of arguments.
			Pop();
			m_Stack.Pop( argCount );		// Remove all arguments.
			jmp( addr );
		}
		void OpReturnV()
		{
			VM::Value result = Top();		// Get return value.
			Pop();
			m_Stack.Resize( m_StackBase );
			int addr = Top().m_Integer;
			Pop();
			m_StackBase = Top().m_Integer;
			Pop();
			int argCount = Top().m_Integer;
			Pop();
			m_Stack.Pop( argCount );
			Push( result );					// Store return value.
			jmp( addr );
		}
		void OpHalt()
		{
		}

		void SysUpdate()
		{
			m_HasUpdateReq = true;
		}

		void SysFloatToInt();
		void SysIntToFloat();
		void SysIntToGU();
		void SysGUToInt();
		void SysGUToFloat();
		void SysFloatToGU();
		void SysAddScore();
		void SysColorARGB();

		void SysSin();
		void SysCos();
		void SysAtan2();
		void SysAbs();
		void SysFabs();
		void SysDegToRad();
		void SysRadToDeg();
		void SysSqrt();

		void SysSinGU();
		void SysCosGU();
		void SysAtan2GU();
		void SysAbsGU();
		void SysSqrtGU();


		void SysPlaySE();
		void SysStopSE();
		void SysPlayBGM();
		void SysStopBGM();

		void OpFNeg();
		void OpFEq();
		void OpFNe();
		void OpFGt();
		void OpFGe();
		void OpFLt();
		void OpFLe();
		void OpFSub();
		void OpFAdd();
		void OpFMul();
		void OpFDiv();

		void OpGUNeg();
		void OpGUEq();
		void OpGUNe();
		void OpGUGt();
		void OpGUGe();
		void OpGULt();
		void OpGULe();
		void OpGUSub();
		void OpGUAdd();
		void OpGUMul();
		void OpGUDiv();

		

		virtual void OpSysCall( int val );

		void SysPrint()
		{
			std::cout << Text( Top() );
			Pop();
		}
		void SysToStr()
		{
			int v = Top().m_Integer;
			Pop();
			char str[ 16 ];
			sprintf( str, "%d", v );
			Push( std::string( str ) );
		}

		void SysPrintGU();
	protected:
		int Value()
		{
			int v = *(int*) m_pCommandPtr;
			m_pCommandPtr += 4;
			return v;
		}
		int Addr() const
		{
			return (int)( m_pCommandPtr - m_pCommand );
		}
		void jmp( int addr )
		{
			m_pCommandPtr = m_pCommand + addr ;
		}
		void Push( int v )
		{
			m_Stack.Push( VM::Value( v ) );
		}
		//void Push( float v );
		void Push( float v )
		{
			m_Stack.Push( VM::Value( v ) );
		}
		void Push( const std::string& v )
		{
			m_Stack.Push( VM::Value( v ) );
		}
		void Push( const VM::Value& v )
		{
			m_Stack.Push( ( v ) );
		}
		void Pop()
		{
			m_Stack.Pop();
		}
		const VM::Value& Top() const
		{
			return m_Stack.Top();
		}
		VM::Value& Top()
		{
			return m_Stack.Top();
		}
		/*const VM::Value& RetPop()
		{
			const VM::Value& ret = m_Stack.Top();
			m_Stack.Pop();
			return ret;
		}*/
		VM::Value& RetPop()
		{
			VM::Value& ret = m_Stack.Top();
			m_Stack.Pop();
			return ret;
		}
		std::string Text( const VM::Value& v )
		{
			return v.m_pString->m_Str;
		}
		const VM::Value& RefToValue( int addr ) const
		{
			if( addr & GLOBAL_FLAG ){
				return m_GlobalValue[ addr & GLOBAL_MASK ];
			}
			return m_Stack[ addr ];
		}
		void SetRef( int addr, const VM::Value& v )
		{
			if( addr & GLOBAL_FLAG ){
				m_GlobalValue[ addr & GLOBAL_MASK ] = v;
			}
			else{
				m_Stack[ addr ] = v;
			}
		}
	};
}

#endif

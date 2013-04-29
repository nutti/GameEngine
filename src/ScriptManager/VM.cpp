#include <exception>
#include "VM.h"

#include <MAPIL/MAPIL.h>

void VM::VCPU::Init()
{
	m_pCommand = m_pData->m_Command;
	m_pTextBuffer = m_pData->m_TextBuffer;
	m_CommandSize = m_pData->m_CommandSize;
	m_TextSize = m_pData->m_TextSize;

	m_GlobalValue.resize( m_pData->m_ValueSize );
	m_pCommandPtr = m_pCommand + m_pData->m_EntryPoint;

	// Call main
	Push( 0 );		// Arguments count of main
	Push( 0 );		// Initial stack base
	Push( 0 );		// Exit point
	m_StackBase = m_Stack.Size();
}

void VM::VCPU::Init( Data* pData )
{
	m_pData = pData;
	Init();
}



bool VM::VCPU::Terminated() const
{
	return m_Terminated;
}

int VM::VCPU::Run()
{
	try{
		int op;
		while( ( op = *m_pCommandPtr++ ) != VM_HALT ){
			switch( op ){
				#define VM_SWITCHTABLE
				#include "VMCode.h"
				#undef VM_SWITCHTABLE
			}
			// アップデート要求が生じた場合はその場所で一度停止する。
			if( m_HasUpdateReq ){
				m_HasUpdateReq = false;
				return -1;
			}
		}
	}
	catch( const std::exception& e )
	{
		std::cerr << "VM Error : " << e.what() << std::endl;
		return -1;
	}
	m_Terminated = true;
	return Top().m_Integer;		// Return value of main function.
}

void VM::VCPU::OpSysCall( int val )
{
	Pop();
	switch( val ){
		case SYS_PRINT:
			SysPrint();
			break;
		case SYS_TOSTR:
			SysToStr();
			break;
		case SYS_PRINT_GU:
			SysPrintGU();
			break;
		case SYS_UPDATE:
			SysUpdate();
			break;
		case SYS_INT_TO_FLOAT:
			SysIntToFloat();
			break;
		case SYS_FLOAT_TO_INT:
			SysFloatToInt();
			break;
		case SYS_INT_TO_GU:
			SysIntToGU();
			break;
		case SYS_GU_TO_INT:
			SysGUToInt();
			break;
		case SYS_GU_TO_FLOAT:
			SysGUToFloat();
			break;
		case SYS_ADD_SCORE:
			SysAddScore();
			break;
		case SYS_SIN:
			SysSin();
			break;
		case SYS_COS:
			SysCos();
			break;
		case SYS_ATAN2:
			SysAtan2();
			break;
		case SYS_ABS:
			SysAbs();
			break;
		case SYS_FABS:
			SysFabs();
			break;
		case SYS_SQRT:
			SysSqrt();
			break;

		case SYS_SIN_GU:
			SysSinGU();
			break;
		case SYS_COS_GU:
			SysCosGU();
			break;
		case SYS_ATAN2_GU:
			SysAtan2GU();
			break;
		case SYS_ABS_GU:
			SysAbsGU();
			break;
		case SYS_SQRT_GU:
			SysSqrtGU();
			break;

		case SYS_DEG_TO_RAD:
			SysDegToRad();
			break;
		case SYS_PLAY_SE:
			SysPlaySE();
			break;
		case SYS_STOP_SE:
			SysStopSE();
			break;
		case SYS_PLAY_BGM:
			SysPlayBGM();
			break;
		case SYS_STOP_BGM:
			SysStopBGM();
			break;

		default:
			throw new MAPIL::MapilException( CURRENT_POSITION, TSTR( "Invalid system call." ), -1 );
			break;
	}
}

void VM::VCPU::SysPrintGU()
{
	int gu = Top().m_Integer;
	Pop();

	int value = 0;

	for( int i = 0; i < GameEngine::SCRIPT_GU_DECIMAL_SHIFT; ++i ){
		value |= ( 1 << i );
	}

	//printf( "%d.%d", gu >> GameEngine::SCRIPT_GU_DECIMAL_SHIFT, gu & ( value ) );
	printf( "%f", gu / 1024.0f );
}

void VM::VCPU::SysFloatToInt()
{
	float f = Top().m_Float;
	Pop();
	Push( static_cast < int > ( f ) );
}

void VM::VCPU::SysIntToFloat()
{
	int i = Top().m_Integer;
	Pop();
	Push( static_cast < float > ( i ) );
}

void VM::VCPU::SysIntToGU()
{
	int i = Top().m_Integer;
	Pop();
	Push( GameEngine::ToGU( i ) );
}

void VM::VCPU::SysGUToInt()
{
	GameEngine::ScriptGU gu = Top().m_Integer;
	Pop();
	Push( GameEngine::ToInt( gu ) );
}

void VM::VCPU::SysGUToFloat()
{
	GameEngine::ScriptGU gu = Top().m_Integer;
	Pop();
	Push( GameEngine::ToFloat( gu ) );
}

void VM::VCPU::SysFloatToGU()
{
	float f = Top().m_Float;
	Pop();
	Push( GameEngine::ToGU( f ) );
}

void VM::VCPU::SysAddScore()
{
	int score = Top().m_Integer;
	Pop();
	//RTG::ResourceHandler* p = RTG::ResourceHandler::GetInst();
	//p->m_pScore->Add( score );
}

void VM::VCPU::SysSin()
{
	float deg = Top().m_Float;
	Pop();
	Push( ::sin( deg ) );
}

void VM::VCPU::SysCos()
{
	float deg = Top().m_Float;
	Pop();
	Push( ::cos( deg ) );
}

void VM::VCPU::SysAtan2()
{
	float x = Top().m_Float;
	Pop();
	float y = Top().m_Float;
	Pop();
	Push( ::atan2( y, x ) );
}

void VM::VCPU::SysSqrt()
{
	float val = Top().m_Float;
	Pop();
	Push( std::sqrtf( val ) );
}

void VM::VCPU::SysSinGU()
{
	GameEngine::GameUnit deg;
	deg.SetRawValue( Top().m_Integer );
	Pop();
	Push( GameEngine::SinGU( deg ).GetRawValue() );
}

void VM::VCPU::SysCosGU()
{
	GameEngine::GameUnit deg;
	deg.SetRawValue( Top().m_Integer );
	Pop();
	Push( GameEngine::CosGU( deg ).GetRawValue() );
}

void VM::VCPU::SysAtan2GU()
{
	GameEngine::GameUnit x;
	GameEngine::GameUnit y;
	x.SetRawValue( Top().m_Integer );
	Pop();
	y.SetRawValue( Top().m_Integer );
	Pop();
	Push( GameEngine::Atan2GU( y, x ).GetRawValue() );
}

void VM::VCPU::SysAbsGU()
{
	GameEngine::GameUnit value;
	value.SetRawValue( Top().m_Integer );
	Pop();
	Push( GameEngine::AbsGU( value ).GetRawValue() );
}

void VM::VCPU::SysSqrtGU()
{
	GameEngine::GameUnit gu;
	gu.SetRawValue( Top().m_Integer );
	Pop();
	Push( GameEngine::ToGU( std::sqrt( gu.GetFloat() ) ) );
}

void VM::VCPU::SysAbs()
{
	int val = Top().m_Integer;
	Pop();
	Push( std::abs( val ) );
}

void VM::VCPU::SysFabs()
{
	float val = Top().m_Float;
	Pop();
	Push( std::abs( val ) );
}

void VM::VCPU::SysDegToRad()
{
	float deg = Top().m_Float;
	Pop();
	Push( VM::Value( static_cast < float > ( MAPIL::DegToRad( deg ) ) ) );
}

void VM::VCPU::SysPlaySE()
{
	//RTG::ResourceHandler* p = RTG::ResourceHandler::GetInst();
	//int id = p->GetSEHandle( Top().m_Integer );
	Pop();
	//MAPIL::PlayStaticBuffer( id );
}

void VM::VCPU::SysStopSE()
{
	//RTG::ResourceHandler* p = RTG::ResourceHandler::GetInst();
	//int id = p->GetSEHandle( Top().m_Integer );
	Pop();
	//MAPIL::StopStaticBuffer( id );
}

void VM::VCPU::SysPlayBGM()
{
	//RTG::ResourceHandler* p = RTG::ResourceHandler::GetInst();
	//int id = p->GetBGMHandle( Top().m_Integer );
	Pop();
	//MAPIL::PlayStreamingBuffer( id );
}

void VM::VCPU::SysStopBGM()
{
	//RTG::ResourceHandler* p = RTG::ResourceHandler::GetInst();
	//int id = p->GetBGMHandle( Top().m_Integer );
	Pop();
	//MAPIL::StopStreamingBuffer( id );
}


void VM::VCPU::OpFNeg()
{
	Top().m_Float = -Top().m_Float;
}

void VM::VCPU::OpFEq()
{
	float rhs = Top().m_Float;
	Pop();
	float lhs = Top().m_Float;
	Pop();
	Push( (int)(lhs == rhs) );
}

void VM::VCPU::OpFNe()
{
	float rhs = Top().m_Float;
	Pop();
	float lhs = Top().m_Float;
	Pop();
	Push( (int)(lhs != rhs) );
}

void VM::VCPU::OpFGt()
{
	float rhs = Top().m_Float;
	Pop();
	float lhs = Top().m_Float;
	Pop();
	Push( (int)(lhs > rhs) );
}

void VM::VCPU::OpFGe()
{
	float rhs = Top().m_Float;
	Pop();
	float lhs = Top().m_Float;
	Pop();
	Push( (int)(lhs >= rhs) );
}

void VM::VCPU::OpFLt()
{
	float rhs = Top().m_Float;
	Pop();
	float lhs = Top().m_Float;
	Pop();
	Push( (int)(lhs < rhs) );
}

void VM::VCPU::OpFLe()
{
	float rhs = Top().m_Float;
	Pop();
	float lhs = Top().m_Float;
	Pop();
	Push( (int)(lhs <= rhs) );
}

void VM::VCPU::OpFSub()
{
	float rhs = Top().m_Float;
	Pop();
	float lhs = Top().m_Float;
	Pop();
	Push( lhs - rhs );
}

void VM::VCPU::OpFAdd()
{
	float rhs = Top().m_Float;
	Pop();
	float lhs = Top().m_Float;
	Pop();
	Push( lhs + rhs );
}

void VM::VCPU::OpFMul()
{
	float rhs = Top().m_Float;
	Pop();
	float lhs = Top().m_Float;
	Pop();
	Push( lhs * rhs );
}

void VM::VCPU::OpFDiv()
{
	float rhs = Top().m_Float;
	Pop();
	if( rhs == 0.0f ){
		throw DivideByZero();
	}
	float lhs = Top().m_Float;
	Pop();
	Push( lhs / rhs );
}

void VM::VCPU::OpGUNeg()
{
	Top().m_Integer = GameEngine::GUNeg( Top().m_Integer );
}

void VM::VCPU::OpGUEq()
{
	GameEngine::ScriptGU rhs = Top().m_Integer;
	Pop();
	GameEngine::ScriptGU lhs = Top().m_Integer;
	Pop();
	Push( GameEngine::GUEq( lhs, rhs ) );
}

void VM::VCPU::OpGUNe()
{
	GameEngine::ScriptGU rhs = Top().m_Integer;
	Pop();
	GameEngine::ScriptGU lhs = Top().m_Integer;
	Pop();
	Push( GameEngine::GUNe( lhs, rhs ) );
}

void VM::VCPU::OpGUGt()
{
	GameEngine::ScriptGU rhs = Top().m_Integer;
	Pop();
	GameEngine::ScriptGU lhs = Top().m_Integer;
	Pop();
	Push( GameEngine::GUGt( lhs, rhs ) );
}

void VM::VCPU::OpGUGe()
{
	GameEngine::ScriptGU rhs = Top().m_Integer;
	Pop();
	GameEngine::ScriptGU lhs = Top().m_Integer;
	Pop();
	Push( GameEngine::GUGe( lhs, rhs ) );
}

void VM::VCPU::OpGULt()
{
	GameEngine::ScriptGU rhs = Top().m_Integer;
	Pop();
	GameEngine::ScriptGU lhs = Top().m_Integer;
	Pop();
	Push( GameEngine::GULt( lhs, rhs ) );
}

void VM::VCPU::OpGULe()
{
	GameEngine::ScriptGU rhs = Top().m_Integer;
	Pop();
	GameEngine::ScriptGU lhs = Top().m_Integer;
	Pop();
	Push( GameEngine::GULe( lhs, rhs ) );
}

void VM::VCPU::OpGUSub()
{
	GameEngine::ScriptGU rhs = Top().m_Integer;
	Pop();
	GameEngine::ScriptGU lhs = Top().m_Integer;
	Pop();
	Push( GameEngine::GUSub( lhs, rhs ) );
}

void VM::VCPU::OpGUAdd()
{
	GameEngine::ScriptGU rhs = Top().m_Integer;
	Pop();
	GameEngine::ScriptGU lhs = Top().m_Integer;
	Pop();
	Push( GameEngine::GUAdd( lhs, rhs ) );
}

void VM::VCPU::OpGUMul()
{
	GameEngine::ScriptGU rhs = Top().m_Integer;
	Pop();
	GameEngine::ScriptGU lhs = Top().m_Integer;
	Pop();
	Push( GameEngine::GUMul( lhs, rhs ) );
}

void VM::VCPU::OpGUDiv()
{
	GameEngine::ScriptGU rhs = Top().m_Integer;
	Pop();
	GameEngine::ScriptGU lhs = Top().m_Integer;
	Pop();
	Push( GameEngine::GUDiv( lhs, rhs ) );
}
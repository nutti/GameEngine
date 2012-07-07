#ifndef INCLUDED_VM_H
#define INCLUDED_VM_H

#include <stdio.h>
#include <vector>
#include "VMValue.h"

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
		SYS_DUMP,
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
		const static int STACK_SIZE = 1000;			// Stack size.
		const static int GLOBAL_FLAG = 0x4000000;	// Flag for the global variable.
		const static int GLOBAL_MASK = 0x3ffffff;
	private:
		Data&				m_Data;
		unsigned char*		m_pCommand;
		unsigned char*		m_pCommandPtr;
		int					m_CommandSize;
		char*				m_pTextBuffer;
		int					m_TextSize;
		VM::Stack < VM::Value, STACK_SIZE >		m_Stack;
		std::vector < VM::Value >				m_GlobalValue;
		int										m_StackBase;
	public:
		VCPU( Data& data ) : m_Data( data )
		{
		}
		~VCPU()
		{
		}
		int Run();
	private:
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
		void OpSysCall( int val )
		{
			Pop();
			switch( val ){
				case SYS_PRINT:
					SysPrint();
					break;
				case SYS_TOSTR:
					SysToStr();
					break;
				case SYS_DUMP:
					SysDump();
					break;
			}
		}
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
		void SysDump()
		{
			std::cout << "test" << std::endl;
		}
	private:
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

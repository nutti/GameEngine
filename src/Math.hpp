#ifndef INCLUDED_GAMEENGINE_MATH_HPP
#define INCLUDED_GAMEENGINE_MATH_HPP

#include <cmath>
#include <climits>

namespace GameEngine
{
	typedef int		ScriptGU;
	const int SCRIPT_GU_DECIMAL_SHIFT	= 10;

	inline int ToInt( int gu )
	{
		return gu >> SCRIPT_GU_DECIMAL_SHIFT;
	}

	inline ScriptGU ToGU( int i )
	{
		return i << SCRIPT_GU_DECIMAL_SHIFT;
	}

	inline ScriptGU ToGU( float f )
	{
		return static_cast < ScriptGU > ( f * ( 1 << SCRIPT_GU_DECIMAL_SHIFT ) );
	}

	inline float ToFloat( int gu )
	{
		return gu > 0 ? gu * 1.0f / ( 1 << SCRIPT_GU_DECIMAL_SHIFT ) : -( (-gu) * 1.0f / ( 1 << SCRIPT_GU_DECIMAL_SHIFT ) );
	}

	inline ScriptGU MakeGU( int i, int d )
	{
		return ( i << SCRIPT_GU_DECIMAL_SHIFT ) + d;
	}

	inline ScriptGU GUAdd( const ScriptGU& lhs, const ScriptGU& rhs )
	{
		return lhs + rhs;
	}

	inline ScriptGU GUSub( const ScriptGU& lhs, const ScriptGU& rhs )
	{
		return lhs - rhs;
	}

	inline ScriptGU GUMul( const ScriptGU& lhs, const ScriptGU& rhs )
	{
		long long value = ( static_cast < long long > ( lhs ) * rhs );
		return static_cast < int > ( value >> SCRIPT_GU_DECIMAL_SHIFT );
	}

	inline ScriptGU GUDiv( const ScriptGU& lhs, const ScriptGU& rhs )
	{
		ScriptGU value = ( static_cast < long long > ( lhs << SCRIPT_GU_DECIMAL_SHIFT ) / rhs );
		return value;
	}

	inline ScriptGU GUNeg( const ScriptGU& gu )
	{
		return -gu;
	}

	inline ScriptGU GUEq( const ScriptGU& lhs, const ScriptGU& rhs )
	{
		return lhs == rhs;
	}

	inline ScriptGU GUNe( const ScriptGU& lhs, const ScriptGU& rhs )
	{
		return lhs != rhs;
	}

	inline ScriptGU GUGt( const ScriptGU& lhs, const ScriptGU& rhs )
	{
		return lhs > rhs;
	}

	inline ScriptGU GUGe( const ScriptGU& lhs, const ScriptGU& rhs )
	{
		return lhs >= rhs;
	}

	inline ScriptGU GULt( const ScriptGU& lhs, const ScriptGU& rhs )
	{
		return lhs < rhs;
	}

	inline ScriptGU GULe( const ScriptGU& lhs, const ScriptGU& rhs )
	{
		return lhs <= rhs;
	}

	class Int64
	{
	private:
		int			m_Hi;
		int			m_Lo;
	public:
		Int64() : m_Hi( 0 ), m_Lo( 0 )
		{
		}
		Int64( int i ) : m_Hi( 0 ), m_Lo( i )
		{
		}
		Int64( const Int64& i )
		{
			m_Hi = i.m_Hi;
			m_Lo = i.m_Lo;
		}
		~Int64()
		{
			m_Hi = 0;
			m_Lo = 0;
		}
		Int64& operator=( const Int64& i )
		{
			m_Hi = i.m_Hi;
			m_Lo = i.m_Lo;

			return *this;
		}
		Int64& operator+=( const Int64& i )
		{
			m_Hi += i.m_Hi;
			m_Lo += i.m_Lo;

			return *this;
		}
		Int64& operator++()
		{
			if( m_Lo == INT_MAX ){
				++m_Hi;
				m_Lo = 0;
			}
			else{
				++m_Lo;
			}

			return *this;
		}
		Int64 operator++( int i )
		{
			Int64 i64 = *this;
			if( m_Lo == INT_MAX ){
				++m_Hi;
				m_Lo = 0;
			}
			else{
				++m_Lo;
			}

			return i64;
		}
	};

	/*struct ScriptGU
	{
		int		m_Value;

	};*/
	
	/*
	class GameUnit
	{
	private:
		enum
		{
			UNIT		= 1000,
		};
		int			m_Value;
	public:
		GameUnit();
		GameUnit( int i );
		GameUnit( int u, int l );
		GameUnit( float f );
		GameUnit( const GameUnit& u );
		~GameUnit();
		GameUnit& operator=( const GameUnit& u );
		GameUnit operator+( const GameUnit& u );
		GameUnit operator-( const GameUnit& u );
		GameUnit operator*( const GameUnit& u );
		GameUnit operator/( const GameUnit& u );
		GameUnit operator>>( int shift );
		GameUnit operator<<( int shift );
		GameUnit& operator+=( const GameUnit& u );
		GameUnit& operator-=( const GameUnit& u );
		GameUnit& operator*=( const GameUnit& u );
		GameUnit& operator/=( const GameUnit& u );
		GameUnit& operator>>=( int shift );
		GameUnit& operator<<=( int shift );
		bool operator<( const GameUnit& u );
		bool operator>( const GameUnit& u );
		bool operator<=( const GameUnit& u );
		bool operator>=( const GameUnit& u );
		bool operator==( const GameUnit& u );
		bool operator!=( const GameUnit& u );

		int GetRawValue() const;
		int GetUnit() const;
		int GetInt() const;
		float GetFloat() const;
	};

	inline GameUnit::GameUnit() : m_Value( 0 )
	{
	}

	inline GameUnit::GameUnit( int i ) : m_Value( i * UNIT )
	{
	}

	inline GameUnit::GameUnit( int u, int l ) : m_Value( u * UNIT + l )
	{
	}

	inline GameUnit::GameUnit( float f )
	{
		m_Value = static_cast < int > ( f * UNIT );
	}

	inline GameUnit::GameUnit( const GameUnit& u )
	{
		m_Value = u.m_Value;
	}

	inline GameUnit::~GameUnit()
	{
		m_Value = 0;
	}

	inline GameUnit& GameUnit::operator=( const GameUnit& u )
	{
		m_Value = u.m_Value;
		return *this;
	}

	inline GameUnit GameUnit::operator+( const GameUnit& u )
	{
		GameUnit tmp( *this );
		tmp += u;
		return tmp;
	}

	inline GameUnit GameUnit::operator-( const GameUnit& u )
	{
		GameUnit tmp( *this );
		tmp -= u;
		return tmp;
	}

	inline GameUnit GameUnit::operator*( const GameUnit& u )
	{
		GameUnit tmp( *this );
		tmp *= u;
		return tmp;
	}

	inline GameUnit GameUnit::operator/( const GameUnit& u )
	{
		GameUnit tmp( *this );
		tmp /= u;
		return tmp;
	}

	inline GameUnit GameUnit::operator>>( int shift )
	{
		GameUnit tmp( *this );
		tmp >>= shift;
		return tmp;
	}

	inline GameUnit GameUnit::operator<<( int shift )
	{
		GameUnit tmp( *this );
		tmp <<= shift;
		return tmp;
	}

		
	inline GameUnit& GameUnit::operator+=( const GameUnit& u )
	{
		m_Value += u.m_Value;
		return *this;
	}

	inline GameUnit& GameUnit::operator-=( const GameUnit& u )
	{
		m_Value -= u.m_Value;
		return *this;
	}

	inline GameUnit& GameUnit::operator*=( const GameUnit& u )
	{
		m_Value = ( static_cast < long long > ( m_Value ) * u.m_Value ) / UNIT;
		return *this;
	}

	inline GameUnit& GameUnit::operator/=( const GameUnit& u )
	{
		m_Value = ( static_cast < long long > ( m_Value * UNIT ) / u.m_Value );
		return *this;
	}

	inline GameUnit& GameUnit::operator>>=( int shift )
	{
		m_Value >>= shift;
		return *this;
	}

	inline GameUnit& GameUnit::operator<<=( int shift )
	{
		m_Value <<= shift;
		return *this;
	}


	inline bool GameUnit::operator<( const GameUnit& u )
	{
		return m_Value < u.m_Value;
	}

	inline bool GameUnit::operator>( const GameUnit& u )
	{
		return m_Value > u.m_Value;
	}

	inline bool GameUnit::operator<=( const GameUnit& u )
	{
		return m_Value <= u.m_Value;
	}

	inline bool GameUnit::operator>=( const GameUnit& u )
	{
		return m_Value >= u.m_Value;
	}

	inline bool GameUnit::operator==( const GameUnit& u )
	{
		return m_Value == u.m_Value;
	}

	inline bool GameUnit::operator!=( const GameUnit& u )
	{
		return m_Value != u.m_Value;
	}

	inline int GameUnit::GetRawValue() const
	{
		return m_Value;
	}

	inline int GameUnit::GetUnit() const
	{
		return UNIT;
	}

	inline int GameUnit::GetInt() const
	{
		return m_Value / UNIT;
	}

	inline float GameUnit::GetFloat() const
	{
		return m_Value * 1.0f / UNIT;
	}
	*/
	




	class GameUnit
	{
	public:
		enum
		{
			SHIFT		= SCRIPT_GU_DECIMAL_SHIFT,
		};
	private:
		union
		{
			struct Bits
			{
				unsigned	m_Decimal : 10;
				signed		m_Integer : 22;
			};
			struct Sign
			{
				unsigned	m_Value : 31;
				unsigned	m_Sign : 1;
			};
			int			m_Value;
			Bits		m_Bits;
			Sign		m_Sign;
		};
	public:
		GameUnit();
		GameUnit( int i );
		GameUnit( int u, int l );
		GameUnit( float f );
		GameUnit( const GameUnit& u );
		~GameUnit();
		GameUnit& operator=( const GameUnit& u );
		GameUnit operator+( const GameUnit& u );
		GameUnit operator-( const GameUnit& u );
		GameUnit operator*( const GameUnit& u );
		GameUnit operator/( const GameUnit& u );
		GameUnit operator>>( int shift );
		GameUnit operator<<( int shift );
		GameUnit& operator+=( const GameUnit& u );
		GameUnit& operator-=( const GameUnit& u );
		GameUnit& operator*=( const GameUnit& u );
		GameUnit& operator/=( const GameUnit& u );
		GameUnit& operator>>=( int shift );
		GameUnit& operator<<=( int shift );
		bool operator<( const GameUnit& u );
		bool operator>( const GameUnit& u );
		bool operator<=( const GameUnit& u );
		bool operator>=( const GameUnit& u );
		bool operator==( const GameUnit& u );
		bool operator!=( const GameUnit& u );

		int GetRawValue() const;
		void SetRawValue( int value );
		int GetUnit() const;
		int GetInt() const;
		float GetFloat() const;
		int GetIntegerPart() const;
		int GetDecimalPart() const;
	};

	inline GameUnit::GameUnit() : m_Value( 0 )
	{
	}

	inline GameUnit::GameUnit( int i )/* : m_Value( i > 0 ? ( i << SHIFT ) : - ( (-i) << SHIFT ) )*/
	{
		/*int sign = ( i >> 31 ) & 0x1;
		if( sign < 0 ){
			int a = 60;
		}
		m_Value = ( ( i << SHIFT ) ^ ( ( 1 - sign ) ) ) + sign;*/

		m_Value = i << SHIFT;
	}

	inline GameUnit::GameUnit( int u, int l ) : m_Value( u > 0 ? ( u << SHIFT ) + l : - ( (-u) << SHIFT ) + l )
	{
	}

	inline GameUnit::GameUnit( float f )
	{
		m_Value = static_cast < int > ( f * ( 1 << SHIFT ) );
	}

	inline GameUnit::GameUnit( const GameUnit& u )
	{
		m_Value = u.m_Value;
	}

	inline GameUnit::~GameUnit()
	{
		m_Value = 0;
	}

	inline GameUnit& GameUnit::operator=( const GameUnit& u )
	{
		m_Value = u.m_Value;
		return *this;
	}

	inline GameUnit GameUnit::operator+( const GameUnit& u )
	{
		GameUnit tmp( *this );
		tmp += u;
		return tmp;
	}

	inline GameUnit GameUnit::operator-( const GameUnit& u )
	{
		GameUnit tmp( *this );
		tmp -= u;
		return tmp;
	}

	inline GameUnit GameUnit::operator*( const GameUnit& u )
	{
		GameUnit tmp( *this );
		tmp *= u;
		return tmp;
	}

	inline GameUnit GameUnit::operator/( const GameUnit& u )
	{
		GameUnit tmp( *this );
		tmp /= u;
		return tmp;
	}

	inline GameUnit GameUnit::operator>>( int shift )
	{
		GameUnit tmp( *this );
		tmp >>= shift;
		return tmp;
	}

	inline GameUnit GameUnit::operator<<( int shift )
	{
		GameUnit tmp( *this );
		tmp <<= shift;
		return tmp;
	}

		
	inline GameUnit& GameUnit::operator+=( const GameUnit& u )
	{
		m_Value += u.m_Value;
		return *this;
	}

	inline GameUnit& GameUnit::operator-=( const GameUnit& u )
	{
		m_Value -= u.m_Value;
		return *this;
	}

	inline GameUnit& GameUnit::operator*=( const GameUnit& u )
	{
		long long value = ( static_cast < long long > ( m_Value ) * u.m_Value );
		m_Value = static_cast < int > ( value >> SHIFT );
		return *this;
	}

	inline GameUnit& GameUnit::operator/=( const GameUnit& u )
	{
		int value = ( ( static_cast < long long > ( m_Value ) << SHIFT ) / u.m_Value );
		m_Value = value;
		//m_Value = ( ( static_cast < long long > ( m_Value << ( SHIFT * 2 ) ) / u.m_Value ) >> SHIFT );
		return *this;
	}

	inline GameUnit& GameUnit::operator>>=( int shift )
	{
		m_Value >>= shift;
		return *this;
	}

	inline GameUnit& GameUnit::operator<<=( int shift )
	{
		m_Value <<= shift;
		return *this;
	}


	inline bool GameUnit::operator<( const GameUnit& u )
	{
		return m_Value < u.m_Value;
	}

	inline bool GameUnit::operator>( const GameUnit& u )
	{
		return m_Value > u.m_Value;
	}

	inline bool GameUnit::operator<=( const GameUnit& u )
	{
		return m_Value <= u.m_Value;
	}

	inline bool GameUnit::operator>=( const GameUnit& u )
	{
		return m_Value >= u.m_Value;
	}

	inline bool GameUnit::operator==( const GameUnit& u )
	{
		return m_Value == u.m_Value;
	}

	inline bool GameUnit::operator!=( const GameUnit& u )
	{
		return m_Value != u.m_Value;
	}

	inline int GameUnit::GetRawValue() const
	{
		return m_Value;
	}

	inline void GameUnit::SetRawValue( int value )
	{
		m_Value = value;
	}

	inline int GameUnit::GetUnit() const
	{
		return 1 << SHIFT;
	}

	inline int GameUnit::GetInt() const
	{

		return m_Bits.m_Integer;
		//return m_Value > 0 ? m_Value >> SHIFT : -( (-m_Value) >> SHIFT );
	}

	inline float GameUnit::GetFloat() const
	{
		//return m_Bits.m_Integer * 1.0f;
		return m_Value > 0 ? m_Value * 1.0f / ( 1 << SHIFT ) : -( (-m_Value) * 1.0f / ( 1 << SHIFT ) );
	}

	inline int GameUnit::GetIntegerPart() const
	{
		return m_Bits.m_Integer;
	}

	inline int GameUnit::GetDecimalPart() const
	{
		return m_Bits.m_Decimal;
	}



	// int -> GameUnit
	GameUnit IntToGU( int val );

	// float -> GameUnit (¬”“_ˆÈ‰º3Œ…‚Ü‚Å•Û‘¶)
	GameUnit FloatToGU( float val );

	// GameUnit -> int
	int GUToInt( GameUnit val );

	// sin
	GameUnit SinGU( GameUnit deg );

	// cos
	GameUnit CosGU( GameUnit deg );

	// tan
	GameUnit TanGU( GameUnit deg );

	// atan2
	GameUnit Atan2GU( GameUnit y, GameUnit x );

	// abs
	GameUnit AbsGU( GameUnit val );

}

#endif
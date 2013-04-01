#ifndef INCLUDED_GAMEENGINE_MATH_HPP
#define INCLUDED_GAMEENGINE_MATH_HPP

#include <cmath>

namespace GameEngine
{
	// GameUnit = int * 1000
	// const int  UNIT		= 1000;
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
		operator int() const;
		operator float() const;
		int GetRawValue() const;
		int GetUnit() const;
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
		m_Value = ( m_Value / ( UNIT / 100 ) ) * ( u.m_Value / ( UNIT / 10 ) );
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

	inline GameUnit::operator int() const
	{
		return m_Value / UNIT;
	}

	inline GameUnit::operator float() const
	{
		return m_Value * 1.0f / UNIT;
	}

	inline int GameUnit::GetRawValue() const
	{
		return m_Value;
	}

	inline int GameUnit::GetUnit() const
	{
		return UNIT;
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

#if defined ( BUTTON_CODE_STR )
#define BUTTON_CODE( button ) MAPIL::KEYBOARD_KEY_ ## button ,
#elif defined ( BUTTON_CODE_ID )
#define BUTTON_CODE( button ) #button ,
#else
#define BUTTON_CODE( button )
#endif

BUTTON_CODE( BACK )		///< Backspace.
BUTTON_CODE( RETURN )	///< Enter.
BUTTON_CODE( ESCAPE )	///< Esc.
BUTTON_CODE( SPACE )		///< Space.
BUTTON_CODE( LEFT )		///< Left.
BUTTON_CODE( UP )		///< Up.
BUTTON_CODE( RIGHT )		///< Right.
BUTTON_CODE( DOWN )		///< Down.
BUTTON_CODE( SNAPSHOT )	///< Print Screen.
BUTTON_CODE( DELETE )	///< Delete.
BUTTON_CODE( 0 )			///< 0.
BUTTON_CODE( 1 )			///< 1.
BUTTON_CODE( 2 )		///< 2.
BUTTON_CODE( 3 )		///< 3.
BUTTON_CODE( 4 )		///< 4.
BUTTON_CODE( 5 )		///< 5.
BUTTON_CODE( 6 )		///< 6.
BUTTON_CODE( 7 )		///< 7.
BUTTON_CODE( 8 )		///< 8.
BUTTON_CODE( 9 )		///< 9.
BUTTON_CODE( A )		///< A.
BUTTON_CODE( B )		///< B.
BUTTON_CODE( C )		///< C.
BUTTON_CODE( D )		///< D.
BUTTON_CODE( E )		///< E.
BUTTON_CODE( F )		///< F.
BUTTON_CODE( G )		///< G.
BUTTON_CODE( H )		///< H.
BUTTON_CODE( I )		///< I.
BUTTON_CODE( J )		///< J.
BUTTON_CODE( K )		///< K.
BUTTON_CODE( L )		///< L.
BUTTON_CODE( M )		///< M.
BUTTON_CODE( N )		///< N.
BUTTON_CODE( O )		///< O.
BUTTON_CODE( P )		///< P.
BUTTON_CODE( Q )		///< Q.
BUTTON_CODE( R )			///< R.
BUTTON_CODE( S )			///< S.
BUTTON_CODE( T )			///< T.
BUTTON_CODE( U )			///< U.
BUTTON_CODE( V )			///< V.
BUTTON_CODE( W )			///< W.
BUTTON_CODE( X )			///< X.
BUTTON_CODE( Y )			///< Y.
BUTTON_CODE( Z )			///< Z.
BUTTON_CODE( F1 )		///< F1.
BUTTON_CODE( F2 )		///< F2.
BUTTON_CODE( F3 )		///< F3.
BUTTON_CODE( F4 )		///< F4.
BUTTON_CODE( F5 )		///< F5.
BUTTON_CODE( F6 )		///< F6.
BUTTON_CODE( F7 )		///< F7.
BUTTON_CODE( F8 )		///< F8.
BUTTON_CODE( F9 )		///< F9.
BUTTON_CODE( F10 )		///< F10.
BUTTON_CODE( LSHIFT )		///< Left Shift key.
BUTTON_CODE( RSHIFT )		///< Right Shift key.
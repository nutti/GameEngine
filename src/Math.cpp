#include "Math.hpp"

#include <MAPIL/MAPIL.h>

namespace GameEngine
{
	// int -> GameUnit
	GameUnit IntToGU( int val )
	{
		return GameUnit( val );
	}

	// float -> GameUnit (小数点以下3桁まで保存)
	GameUnit FloatToGU( float val )
	{
		return GameUnit( val );
	}

	// GameUnit -> int
	int GUToInt( GameUnit val )
	{
		return val.GetInt();
	}

	// sin
	GameUnit SinGU( GameUnit deg )
	{
		// sinテーブル
		const int SIN_TABLE_GU [ 360 ] = {
			0, 17, 35, 52, 70, 87, 105, 122, 
			139, 156, 174, 191, 208, 225, 242, 259, 
			276, 292, 309, 326, 342, 358, 375, 391, 
			407, 423, 438, 454, 469, 485, 500, 515, 
			530, 545, 559, 574, 588, 602, 616, 629, 
			643, 656, 669, 682, 695, 707, 719, 731, 
			743, 755, 766, 777, 788, 799, 809, 819, 
			829, 839, 848, 857, 866, 875, 883, 891, 
			899, 906, 914, 921, 927, 934, 940, 946, 
			951, 956, 961, 966, 970, 974, 978, 982, 
			985, 988, 990, 993, 995, 996, 998, 999, 
			999, 1000, 1000, 1000, 999, 999, 998, 996, 
			995, 993, 990, 988, 985, 982, 978, 974, 
			970, 966, 961, 956, 951, 946, 940, 934, 
			927, 921, 914, 906, 899, 891, 883, 875, 
			866, 857, 848, 839, 829, 819, 809, 799, 
			788, 777, 766, 755, 743, 731, 719, 707, 
			695, 682, 669, 656, 643, 629, 616, 602, 
			588, 574, 559, 545, 530, 515, 500, 485, 
			469, 454, 438, 423, 407, 391, 375, 358, 
			342, 326, 309, 292, 276, 259, 242, 225, 
			208, 191, 174, 156, 139, 122, 105, 87, 
			70, 52, 35, 17, 0, -16, -34, -51, 
			-69, -86, -104, -121, -138, -155, -173, -190, 
			-207, -224, -241, -258, -275, -291, -308, -325, 
			-341, -357, -374, -390, -406, -422, -437, -453, 
			-468, -484, -499, -514, -529, -544, -558, -573, 
			-587, -601, -615, -628, -642, -655, -668, -681, 
			-694, -706, -718, -730, -742, -754, -765, -776, 
			-787, -798, -808, -818, -828, -838, -847, -856, 
			-865, -874, -882, -890, -898, -905, -913, -920, 
			-926, -933, -939, -945, -950, -955, -960, -965, 
			-969, -973, -977, -981, -984, -987, -989, -992, 
			-994, -995, -997, -998, -998, -999, -999, -999, 
			-998, -998, -997, -995, -994, -992, -989, -987, 
			-984, -981, -977, -973, -969, -965, -960, -955, 
			-950, -945, -939, -933, -926, -920, -913, -905, 
			-898, -890, -882, -874, -865, -856, -847, -838, 
			-828, -818, -808, -798, -787, -776, -765, -754, 
			-742, -730, -718, -706, -694, -681, -668, -655, 
			-642, -628, -615, -601, -587, -573, -558, -544, 
			-529, -514, -499, -484, -468, -453, -437, -422, 
			-406, -390, -374, -357, -341, -325, -308, -291, 
			-275, -258, -241, -224, -207, -190, -173, -155, 
			-138, -121, -104, -86, -69, -51, -34, -16
		};

		int value = deg.GetInt();
		value = std::abs( value );

		int ratio = deg.GetRawValue() % deg.GetUnit();					// 線形補完率
		int offset1 = value % 360;		// テーブルオフセット１
		int offset2 = ( offset1 + 1 ) == 360 ? 0 : offset1 + 1;	// テーブルオフセット２

		// 補完
		// ( ( o1 - o2 ) * r + o2 ) / UNIT
		return GameUnit( 0, ( SIN_TABLE_GU[ offset2 ] - SIN_TABLE_GU[ offset1 ] ) * ratio / deg.GetUnit() + SIN_TABLE_GU[ offset1 ] );
	}

	// cos
	GameUnit CosGU( GameUnit deg )
	{
		// cosテーブル
		const int COS_TABLE_GU [ 360 ] = {
			1000, 1000, 999, 999, 998, 996, 995, 993, 
			990, 988, 985, 982, 978, 974, 970, 966, 
			961, 956, 951, 946, 940, 934, 927, 921, 
			914, 906, 899, 891, 883, 875, 866, 857, 
			848, 839, 829, 819, 809, 799, 788, 777, 
			766, 755, 743, 731, 719, 707, 695, 682, 
			669, 656, 643, 629, 616, 602, 588, 574, 
			559, 545, 530, 515, 500, 485, 469, 454, 
			438, 423, 407, 391, 375, 358, 342, 326, 
			309, 292, 276, 259, 242, 225, 208, 191, 
			174, 156, 139, 122, 105, 87, 70, 52, 
			35, 17, 0, -16, -34, -51, -69, -86, 
			-104, -121, -138, -155, -173, -190, -207, -224, 
			-241, -258, -275, -291, -308, -325, -341, -357, 
			-374, -390, -406, -422, -437, -453, -468, -484, 
			-499, -514, -529, -544, -558, -573, -587, -601, 
			-615, -628, -642, -655, -668, -681, -694, -706, 
			-718, -730, -742, -754, -765, -776, -787, -798, 
			-808, -818, -828, -838, -847, -856, -865, -874, 
			-882, -890, -898, -905, -913, -920, -926, -933, 
			-939, -945, -950, -955, -960, -965, -969, -973, 
			-977, -981, -984, -987, -989, -992, -994, -995, 
			-997, -998, -998, -999, -999, -999, -998, -998, 
			-997, -995, -994, -992, -989, -987, -984, -981, 
			-977, -973, -969, -965, -960, -955, -950, -945, 
			-939, -933, -926, -920, -913, -905, -898, -890, 
			-882, -874, -865, -856, -847, -838, -828, -818, 
			-808, -798, -787, -776, -765, -754, -742, -730, 
			-718, -706, -694, -681, -668, -655, -642, -628, 
			-615, -601, -587, -573, -558, -544, -529, -514, 
			-499, -484, -468, -453, -437, -422, -406, -390, 
			-374, -357, -341, -325, -308, -291, -275, -258, 
			-241, -224, -207, -190, -173, -155, -138, -121, 
			-104, -86, -69, -51, -34, -16, 0, 17, 
			35, 52, 70, 87, 105, 122, 139, 156, 
			174, 191, 208, 225, 242, 259, 276, 292, 
			309, 326, 342, 358, 375, 391, 407, 423, 
			438, 454, 469, 485, 500, 515, 530, 545, 
			559, 574, 588, 602, 616, 629, 643, 656, 
			669, 682, 695, 707, 719, 731, 743, 755, 
			766, 777, 788, 799, 809, 819, 829, 839, 
			848, 857, 866, 875, 883, 891, 899, 906, 
			914, 921, 927, 934, 940, 946, 951, 956, 
			961, 966, 970, 974, 978, 982, 985, 988, 
			990, 993, 995, 996, 998, 999, 999, 1000
		};

		int value = deg.GetInt();
		value = std::abs( value );

		int ratio = deg.GetRawValue() % deg.GetUnit();					// 線形補完率
		int offset1 = value % 360;		// テーブルオフセット１
		int offset2 = ( offset1 + 1 ) == 360 ? 0 : offset1 + 1;	// テーブルオフセット２

		// 補完
		// ( ( o1 - o2 ) * r + o2 ) / UNIT
		return GameUnit( 0, ( COS_TABLE_GU[ offset2 ] - COS_TABLE_GU[ offset1 ] ) * ratio / deg.GetUnit() + COS_TABLE_GU[ offset1 ] );
	}

	// tan
	GameUnit TanGU( GameUnit deg )
	{
		// tanテーブル
		const int TAN_TABLE_GU [ 360 ] = {
			0, 17, 35, 52, 70, 87, 105, 123, 
			141, 158, 176, 194, 213, 231, 249, 268, 
			287, 306, 325, 344, 364, 384, 404, 424, 
			445, 466, 488, 510, 532, 554, 577, 601, 
			625, 649, 675, 700, 727, 754, 781, 810, 
			839, 869, 900, 933, 966, 1000, 1036, 1072, 
			1111, 1150, 1192, 1235, 1280, 1327, 1376, 1428, 
			1483, 1540, 1600, 1664, 1732, 1804, 1881, 1963, 
			2050, 2145, 2246, 2356, 2475, 2605, 2747, 2904, 
			3078, 3271, 3487, 3732, 4011, 4331, 4705, 5145, 
			5671, 6314, 7115, 8144, 9514, 11430, 14301, 19081, 
			28636, 57290, -2147483648, -57289, -28635, -19080, -14300, -11429, 
			-9513, -8143, -7114, -6313, -5670, -5144, -4704, -4330, 
			-4010, -3731, -3486, -3270, -3077, -2903, -2746, -2604, 
			-2474, -2355, -2245, -2144, -2049, -1962, -1880, -1803, 
			-1731, -1663, -1599, -1539, -1482, -1427, -1375, -1326, 
			-1279, -1234, -1191, -1149, -1110, -1071, -1035, -999, 
			-965, -932, -899, -868, -838, -809, -780, -753, 
			-726, -699, -674, -648, -624, -600, -576, -553, 
			-531, -509, -487, -465, -444, -423, -403, -383, 
			-363, -343, -324, -305, -286, -267, -248, -230, 
			-212, -193, -175, -157, -140, -122, -104, -86, 
			-69, -51, -34, -16, 0, 17, 35, 52, 
			70, 87, 105, 123, 141, 158, 176, 194, 
			213, 231, 249, 268, 287, 306, 325, 344, 
			364, 384, 404, 424, 445, 466, 488, 510, 
			532, 554, 577, 601, 625, 649, 675, 700, 
			727, 754, 781, 810, 839, 869, 900, 933, 
			966, 1000, 1036, 1072, 1111, 1150, 1192, 1235, 
			1280, 1327, 1376, 1428, 1483, 1540, 1600, 1664, 
			1732, 1804, 1881, 1963, 2050, 2145, 2246, 2356, 
			2475, 2605, 2747, 2904, 3078, 3271, 3487, 3732, 
			4011, 4331, 4705, 5145, 5671, 6314, 7115, 8144, 
			9514, 11430, 14301, 19081, 28636, 57290, -2147483648, -57289, 
			-28635, -19080, -14300, -11429, -9513, -8143, -7114, -6313, 
			-5670, -5144, -4704, -4330, -4010, -3731, -3486, -3270, 
			-3077, -2903, -2746, -2604, -2474, -2355, -2245, -2144, 
			-2049, -1962, -1880, -1803, -1731, -1663, -1599, -1539, 
			-1482, -1427, -1375, -1326, -1279, -1234, -1191, -1149, 
			-1110, -1071, -1035, -999, -965, -932, -899, -868, 
			-838, -809, -780, -753, -726, -699, -674, -648, 
			-624, -600, -576, -553, -531, -509, -487, -465, 
			-444, -423, -403, -383, -363, -343, -324, -305, 
			-286, -267, -248, -230, -212, -193, -175, -157, 
			-140, -122, -104, -86, -69, -51, -34, -16
		};

		int value = deg.GetInt();
		value = std::abs( value );

		int ratio = deg.GetRawValue() % deg.GetUnit();					// 線形補完率
		int offset1 = value % 360;		// テーブルオフセット１
		int offset2 = ( offset1 + 1 ) == 360 ? 0 : offset1 + 1;	// テーブルオフセット２

		// 補完
		// ( ( o1 - o2 ) * r + o2 ) / UNIT
		return GameUnit( 0, ( TAN_TABLE_GU[ offset2 ] - TAN_TABLE_GU[ offset1 ] ) * ratio / deg.GetUnit() + TAN_TABLE_GU[ offset1 ] );
	}

	// atan2
	GameUnit Atan2GU( GameUnit y, GameUnit x )
	{
		return GameUnit( (float)MAPIL::RadToDeg( std::atan2( (float)y.GetRawValue(), (float)x.GetRawValue() ) ) );
	}

	// abs
	GameUnit AbsGU( GameUnit val )
	{
		if( val.GetRawValue() < 0 ){
			return GameUnit( 0, -val.GetRawValue() / val.GetUnit() );
		}
		else{
			return GameUnit( 0, val.GetRawValue() / val.GetUnit() );
		}
	}
}
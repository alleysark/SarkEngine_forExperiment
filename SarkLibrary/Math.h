#ifndef __MATH_H__
#define __MATH_H__

/**
sarklib redeclare whole methods in 'math.h' as namespace sarklib::math for adjective library use
and declare useful functions additionally which are not included in 'math.h'
*/
#include <math.h>

#include <float.h>
#include <stdint.h>

namespace sarklib{

	typedef uint8_t		ubyte;
	typedef int8_t		byte;

	typedef uint16_t	uint16;
	typedef int16_t		int16;

	typedef uint32_t	uint32;
	typedef int32_t		int32;

	typedef uint64_t	uint64;
	typedef int64_t		int64;


	//#define SARKLIB_USING_DOUBLE
#ifdef SARKLIB_USING_DOUBLE
	typedef double		real;
	typedef double		real_d; //is there any 128-bit floating point system??
#define REAL_MAX	DBL_MAX

	typedef uint64		uinteger;
	typedef int64		integer;
#else
	typedef float		real;
	typedef double		real_d;
#define REAL_MAX	FLT_MAX

	typedef uint32		uinteger;
	typedef int32		integer;
#endif



	// namespace of math library
	namespace math{
#ifdef SARKLIB_USING_DOUBLE
		const real		PI = 3.141592653589;
		const real_d	PI_d = 3.141592653589;
		const real		EPSILON = DBL_EPSILON;
		const real_d	EPSILON_d = DBL_EPSILON;
#else
		const real		PI = 3.141592f;
		const real_d	PI_d = 3.141592653589;
		const real		EPSILON = FLT_EPSILON;
		const real_d	EPSILON_d = DBL_EPSILON;
#endif

		inline real rad2deg(real rad){
			return rad * 180.0f / PI;
		}

		inline real deg2rad(real deg){
			return deg * PI / 180.0f;
		}

		inline byte sign(real value){
			return (value > 0.0 ? 1 : (value < 0.0 ? -1 : 0));
		}

		inline real sqre(real value){
			return value*value;
		}

		inline real clamp(real value, real min, real max){
			return (value < min ? min : (value > max ? max : value));
		}

#ifdef max
#undef max
#endif
		inline real max(real v1, real v2){
			return (v1 > v2 ? v1 : v2);
		}

#ifdef min
#undef min
#endif
		inline real min(real v1, real v2){
			return (v1 < v2 ? v1 : v2);
		}

		// approximately equal
		inline bool apprx_equal(real v1, real v2, byte precision = 3){
			while (precision){
				v1 *= 10.0f;
				v2 *= 10.0f;
				precision--;
			}
			return static_cast<integer>(v1) == static_cast<integer>(v2);
		}


		// for specifying the methods within math.h
		using ::abs;
		using ::ceil;	using ::floor;

		using ::cos;	using ::cosh;	using ::acos;
		using ::sin;	using ::sinh;	using ::asin;
		using ::tan;	using ::tanh;	using ::atan;	using ::atan2;

		using ::exp;
		using ::pow;
		using ::sqrt;

		using ::frexp;	using ::ldexp;
		using ::log;	using ::log10;

		using ::modf; //modulo
		using ::fmod; //remainder
		using ::round;


		// interpolations
		inline real lerp(real q0, real q1, real t){
			return t*(q1 - q0) + q0;
		}

	}
}
#endif
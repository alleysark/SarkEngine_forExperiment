#ifndef __MATH_H__
#define __MATH_H__

namespace sarklib{

	/**
		Mathematic methods and constants, 
		this namespace redeclare whole methods in 'math.h' as namespace sarklib::math
		and declare useful things additionally which are not in 'math.h'
	*/
	namespace math{
		const float PI = 3.1415926535897932384626433832f;

		inline int sign(double d){
			return ( d>0?1:(d<0?-1:0) );
		}
		inline int sign(float f){
			return ( f>0?1:(f<0?-1:0) );
		}

		inline double sqre(double d){
			return d*d;
		}
		inline float sqre(float f){
			return f*f;
		}

		#include<math.h>
		// for specifying the methods within meth.h
		using ::acosf;	using ::asinf;
		using ::atanf;	using ::atan2f;	using ::ceilf;
		using ::cosf;	using ::coshf;	using ::expf;
		using ::fabsf;	using ::floorf;	using ::fmodf;
		using ::frexpf;	using ::ldexpf;	using ::logf;
		using ::log10f;	using ::modff;	using ::powf;
		using ::sinf;	using ::sinhf;	using ::sqrtf;
		using ::tanf;	using ::tanhf;

		using ::acosl;	using ::asinl;
		using ::atanl;	using ::atan2l;	using ::ceill;
		using ::cosl;	using ::coshl;	using ::expl;
		using ::fabsl;	using ::floorl;	using ::fmodl;
		using ::frexpl;	using ::ldexpl;	using ::logl;
		using ::log10l;	using ::modfl;	using ::powl;
		using ::sinl;	using ::sinhl;	using ::sqrtl;
		using ::tanl;	using ::tanhl;

		using ::abs;

		using ::acos;	using ::asin;
		using ::atan;	using ::atan2;	using ::ceil;
		using ::cos;	using ::cosh;	using ::exp;
		using ::fabs;	using ::floor;	using ::fmod;
		using ::frexp;	using ::ldexp;	using ::log;
		using ::log10;	using ::modf;	using ::pow;
		using ::sin;	using ::sinh;	using ::sqrt;
		using ::tan;	using ::tanh;
	}
}
#endif
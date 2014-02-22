#ifndef __MATH_H__
#define __MATH_H__

namespace sarklib{

	/**
		Mathematic methods and constants, 
		this namespace redeclare whole methods in 'math.h' as namespace sarklib::math for adjective library use
		and declare useful functions additionally which are not included in 'math.h'
	*/
	namespace math{
		const float PI = 3.141592f;
		const double PI_d = 3.1415926535897932384626433832;

		inline int sign(double value){
			return ( value > 0.0 ? 1 :  (value < 0.0 ? -1 : 0) );
		}
		inline int sign(float value){
			return ( value > 0.0f ? 1 :  (value < 0.0f ? -1 : 0) );
		}

		inline double sqre(double value){
			return value*value;
		}
		inline float sqre(float value){
			return value*value;
		}

		inline double clamp(double value, double min, double max){
			return (value < min ? min :  (value > max ? max : value) );
		}
		inline float clamp(float value, float min, float max){
			return (value < min ? min :  (value > max ? max : value) );
		}

		#ifdef max
			#undef max
		#endif
		inline double max(double v1, double v2){
			return (v1>v2 ? v1 : v2);
		}
		inline float max(float v1, float v2){
			return (v1>v2 ? v1 : v2);
		}

		#ifdef min
			#undef min
		#endif
		inline double min(double v1, double v2){
			return (v1<v2 ? v1 : v2);
		}
		inline float min(float v1, float v2){
			return (v1<v2 ? v1 : v2);
		}

		inline double round(double value){
			return (value >= 0.0) ? (long)(value + 0.5) : (long)(value - 0.5);
		}
		inline float round(float value){
			return (value >= 0.0f) ? (int)(value + 0.5f) : (int)(value - 0.5f);
		}


		#include<math.h>
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
		
		// interpolations
		float lerp(float q0, float q1, float t){
			return t*(q1 - q0) + q0;
		}
	}
}
#endif
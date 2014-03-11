#ifndef __CORE_H__
#define __CORE_H__

/**
sarklib redeclare whole methods in 'math.h' as namespace sarklib::math for adjective library use
and declare useful functions additionally which are not included in 'math.h'
*/
#include <math.h>

#include <float.h>
#include <stdint.h>
#include <stdlib.h>

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



	//==============================================
	//			mathematic type definitions
	//==============================================


	//=============== Vector Types =================
	/**
	Type of vector 2D
	*/
	class Vector2{
	public:
		real x, y;

		Vector2();
		Vector2(real val);
		Vector2(real _x, real _y);
		Vector2(const Vector2& v);
		void operator=(const Vector2& v);

		void Set(real _x, real _y);

		// vector magnitude
		real Magnitude() const;

		real MagnitudeSq() const;

		// unary - operator
		const Vector2 operator-() const;


		// vector plus operator
		const Vector2 operator+(const Vector2& v) const;

		const Vector2 operator+(real fConstant) const;

		friend const Vector2 operator+(real fConstant, const Vector2& v);

		const Vector2& operator+=(const Vector2& v);

		const Vector2& operator+=(real fConstant);


		// vector minus operator
		const Vector2 operator-(const Vector2& v) const;

		const Vector2 operator-(real fConstant) const;

		const Vector2& operator-=(const Vector2& v);

		const Vector2& operator-=(real fConstant);


		// vector constant multiplication operator (it is not cross operation)
		const Vector2 operator*(real fConstant) const;

		friend const Vector2 operator*(real fConstant, const Vector2& v);

		const Vector2& operator*=(real fConstant);


		// vector constnat division operator
		const Vector2 operator/(real fConstant) const;

		const Vector2& operator/=(real fConstant);

		// boolean operators
		bool operator==(const Vector2& v) const;

		bool operator!=(const Vector2& v) const;


		// check if zero vector
		bool IsZero() const;

		// dot product
		real Dot(const Vector2& v) const;


		// get normal and normalize this
		const Vector2 Normal() const;
		void Normalize();


		// calculate Euclidean distance of given two vectors
		static real Distance(const Vector2& v1, const Vector2& v2);

		// calculate radians of given two vectors
		static real Angle(const Vector2& v1, const Vector2& v2);
	};
	typedef Vector2 Vertex2;
	typedef Vector2 Position2;
	typedef Vector2 Point2;


	/**
	Type of vector 3D
	*/
	class Vector3{
	public:
		real x, y, z;

		Vector3();
		Vector3(real val);
		Vector3(real _x, real _y, real _z);
		Vector3(const Vector3& v);
		void operator=(const Vector3& v);

		void Set(real _x, real _y, real _z);

		// explicitly type cast
		explicit Vector3(const Vector2& v);


		// vector magnitude
		real Magnitude() const;

		real MagnitudeSq() const;

		// unary - operator
		const Vector3 operator-() const;


		// vector plus operator
		const Vector3 operator+(const Vector3& v) const;

		const Vector3 operator+(real fConstant) const;

		friend const Vector3 operator+(real fConstant, const Vector3& v);

		const Vector3& operator+=(const Vector3& v);

		const Vector3& operator+=(real fConstant);


		// vector minus operator
		const Vector3 operator-(const Vector3& v) const;

		const Vector3 operator-(real fConstant) const;

		const Vector3& operator-=(const Vector3& v);

		const Vector3& operator-=(real fConstant);


		// vector constant multiplication operator (it is not cross operation)
		const Vector3 operator*(real fConstant) const;

		friend const Vector3 operator*(real fConstant, const Vector3& v);

		const Vector3& operator*=(real fConstant);


		// vector constnat division operator
		const Vector3 operator/(real fConstant) const;

		const Vector3& operator/=(real fConstant);

		// boolean operators
		bool operator==(const Vector3& v) const;

		bool operator!=(const Vector3& v) const;


		// check if zero vector
		bool IsZero() const;

		// dot product
		real Dot(const Vector3& v) const;

		// cross product
		const Vector3 Cross(const Vector3& v) const;


		// get normal and normalize this
		const Vector3 Normal() const;
		void Normalize();


		// calculate Euclidean distance of given two vectors
		static real Distance(const Vector3& v1, const Vector3& v2);

		// calculate radians of given two vectors
		static real Angle(const Vector3& v1, const Vector3& v2);
	};
	typedef Vector3 Vertex3;
	typedef Vector3 Position3;
	typedef Vector3 Point3;


	/**
	Type of vector 4D
	*/
	class Vector4{
	public:
		union{
			struct{
				Vector3 xyz;
				real w;
			};
			struct{
				real x, y, z, w;
			};
			struct{
				real r, g, b, a;
			};
		};

		Vector4();
		Vector4(real val);
		Vector4(real _x, real _y, real _z, real _w);
		Vector4(const Vector4& v);
		void operator=(const Vector4& v);

		void Set(real _x, real _y, real _z, real _w);

		// explicitly type cast
		explicit Vector4(const Vector2& v);
		explicit Vector4(const Vector3& v);


		// vector magnitude
		real Magnitude() const;

		real MagnitudeSq() const;

		// unary - operator
		const Vector4 operator-() const;


		// vector plus operator
		const Vector4 operator+(const Vector4& v) const;

		const Vector4 operator+(real fConstant) const;

		friend const Vector4 operator+(real fConstant, const Vector4& v);

		const Vector4& operator+=(const Vector4& v);

		const Vector4& operator+=(real fConstant);


		// vector minus operator
		const Vector4 operator-(const Vector4& v) const;

		const Vector4 operator-(real fConstant) const;

		const Vector4& operator-=(const Vector4& v);

		const Vector4& operator-=(real fConstant);


		// vector constant multiplication operator (it is not cross operation)
		const Vector4 operator*(real fConstant) const;

		friend const Vector4 operator*(real fConstant, const Vector4& v);

		const Vector4& operator*=(real fConstant);


		// vector constnat division operator
		const Vector4 operator/(real fConstant) const;

		const Vector4& operator/=(real fConstant);

		// boolean operators
		bool operator==(const Vector4& v) const;

		bool operator!=(const Vector4& v) const;

		// check if zero vector
		bool IsZero() const;

		// dot product
		real Dot(const Vector4& v) const;

		// get normal and normalize this
		const Vector4 Normal() const;
		void Normalize();


		// calculate Euclidean distance of given two vectors
		static real Distance(const Vector4& v1, const Vector4& v2);
	};
	typedef Vector4 Vertex4;
	typedef Vector4 Position4;
	typedef Vector4 Point4;
	typedef Vector4 Color;


	//=============== Matrix Types =================

	// matrix determinant inline functions
	//template<typedef type>
	//inline type det2x2(type e00, type e01, type e10, type e11){
	//	return (e00*e11 - e01*e10);
	//}
#define det2x2(a,b,c,d) ((a)*(d) - (b)*(c))

#define det3x3(	a, b, c, \
	d, e, f, \
	g, h, i) \
	((a)*det2x2(e, f, h, i) - (b)*det2x2(d, f, g, i) + (c)*det2x2(d, e, g, h))

#define det4x4(	a, b, c, d, \
	e, f, g, h, \
	i, j, k, l, \
	m, n, o, p) \
	((a)*det3x3(f, g, h, j, k, l, n, o, p) - (b)*det3x3(e, g, h, i, k, l, m, o, p) + (c)*det3x3(e, f, h, i, j, l, m, n, p) - (d)*det3x3(e, f, g, i, j, k, m, n, o))


	/**
	Type of 3x3 square matrix defined as row X col order
	*/
	class Matrix3{
	public:
		union{
			struct{
				Vector3 row[3];
			};
			struct{
				real m[3][3]; //row x col
			};
		};

		Matrix3();
		Matrix3(real _00, real _01, real _02,
			real _10, real _11, real _12,
			real _20, real _21, real _22);
		Matrix3(real** mat3);
		Matrix3(const Matrix3& mat3);
		void operator=(const Matrix3& mat3);

		// getting matrix as GL format pointer
		const real* GetRawMatrix();

		// matrix plus operator
		const Matrix3 operator+(const Matrix3& mat3) const;

		// matrix minus operator
		const Matrix3 operator-(const Matrix3& mat3) const;

		// matrix constant multiply operator
		const Matrix3 operator*(real fConstant) const;

		friend const Matrix3 operator*(real fConstant, const Matrix3& mat3);

		// matrix constant division operator
		const Matrix3 operator/(real fConstant) const;

		friend const Matrix3 operator/(real fConstant, const Matrix3& mat3);

		// matrix multiply operator
		const Matrix3 operator*(const Matrix3& mat3) const;

		// matrix multiply to vector operator
		const Vector3 operator*(const Vector3& vec3) const;

		// check if zero matrix
		bool IsZero() const;

		// make this to zero matrix
		void MakeZero();

		// check if identity matrix
		bool IsIdentity() const;

		// make this to identity matrix
		void MakeIdentity();

		// matrix transpose
		const Matrix3 Transpose() const;

		// matrix cofactor matrix
		const Matrix3 CofactorMatrix() const;

		// matrix adjugate(== C^t, but this method is coded hardly)
		const Matrix3 Adjugate() const;

		// matrix inverse
		const Matrix3 Inverse() const;

		// matrix determination
		real Determin() const;
	};


	/**
	Type of 4x4 square matrix defined as row X col order
	*/
	class Matrix4{
	public:
		union{
			struct{
				Vector4 row[4];
			};
			struct{
				real m[4][4]; //row x col
			};
		};

		Matrix4();
		Matrix4(real _00, real _01, real _02, real _03,
			real _10, real _11, real _12, real _13,
			real _20, real _21, real _22, real _23,
			real _30, real _31, real _32, real _33);
		Matrix4(real** mat4);
		Matrix4(const Matrix4& mat4);
		void operator=(const Matrix4& mat4);

		explicit Matrix4(const Matrix3& mat3);

		// getting matrix as GL format pointer
		const real* GetRawMatrix();

		// matrix plus operator
		const Matrix4 operator+(const Matrix4& mat4) const;

		// matrix minus operator
		const Matrix4 operator-(const Matrix4& mat4) const;

		// matrix constant multiply operator
		const Matrix4 operator*(real fConstant) const;

		friend const Matrix4 operator*(real fConstant, const Matrix4& mat4);

		// matrix constant division operator
		const Matrix4 operator/(real fConstant) const;

		friend const Matrix4 operator/(real fConstant, const Matrix4& mat4);

		// matrix multiply operator
		const Matrix4 operator*(const Matrix4& mat4) const;

		// matrix multiply to vector operator
		const Vector4 operator*(const Vector4& vec4) const;

		// check if zero matrix
		bool IsZero() const;

		// make this to zero matrix
		void MakeZero();

		// check if identity matrix
		bool IsIdentity() const;

		// make this to identity matrix
		void MakeIdentity();

		// matrix transpose
		const Matrix4 Transpose() const;

		// matrix cofactor matrix
		const Matrix4 CofactorMatrix() const;

		// matrix adjugate(== C^t, but this method is coded hardly)
		const Matrix4 Adjugate() const;

		// matrix inverse
		const Matrix4 Inverse() const;

		// matrix determination
		real Determin() const;
	};


	/**
	Type of quaternion
	*/
	class Quaternion{
	public:
		union{
			struct{ // q = [s + xi+yj+zk], s is scalar and (x,y,z) is vector part
				real s;
				real x, y, z;
			};
			struct{
				real s;
				Vector3 v;
			};
		};

		Quaternion();
		Quaternion(real _x, real _y, real _z, real _s = 0.0f);
		Quaternion(const Quaternion& q);
		void operator=(const Quaternion& q);

		explicit Quaternion(const Vector3& v, real _s = 0.0f);
		explicit Quaternion(const Vector2& v);


		void Set(real _x, real _y, real _z);
		void Set(real _s, real _x, real _y, real _z);


		// is scalar factor zero?
		bool IsPure() const;

		// make this into pure quaternion
		void MakePure();


		// quaternion plus operation
		const Quaternion operator+(const Quaternion& q) const;
		const Quaternion& operator+=(const Quaternion& q);


		// quaternion minus operation
		const Quaternion operator-(const Quaternion& q) const;
		const Quaternion& operator-=(const Quaternion& q);


		// Hamilton product: [s1, v1]*[s2, v2] = [s1*s2 - dot(v1,v2), s1*v2 + s2*v1 + cross(v1,v2)]
		const Quaternion operator*(const Quaternion& q) const;
		const Quaternion& operator*=(const Quaternion& q);


		// get conjugation and conjugate this
		const Quaternion Conjugation() const;
		const Quaternion& Conjugate();


		// magnitude which called 'norm' or 'the tensor of quaternion'
		real Magnitude() const;
		real MagnitudeSq() const;


		// get normal and normalize this
		const Quaternion Normal() const;
		const Quaternion& Normalize();


		// inverse, q^{-1} = conj(q)/(norm(q)^2)
		const Quaternion Inverse() const;

		// make this as the rotating quaternion from given axis vector and theta
		void MakeRotatingQuat(const Vector3& axis, real theta);

		// make this as the rotating quaternion from given each axis rotating factor
		// (roll: z, pitch: x, yaw: y axis rotating factor)
		void MakeRotatingQuat(const real roll, real pitch, real yaw);


		// convert quaternion to matrix 3D
		const Matrix3 ToMatrix3(bool isNormalized = false);

		// convert quaternion to matrix 4D
		const Matrix4 ToMatrix4(bool isNormalized = false);


		// rotate input vector from given axis vector and theta
		static void Rotate(Vector3& v, const Vector3& axis, real theta);

		// rotate input vector from given each axis rotating factor
		// (roll: z, pitch: x, yaw: y axis rotating factor)
		static void Rotate(Vector3& v, real roll, real pitch, real yaw);
	};

}
#endif
#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdlib.h>
#include <memory.h>
#include <math.h>

#define sqre(x) (x*x)
#define sign(x) (x>0?1:(x<0?-1:0))

namespace sarklib{

	//=============== Vector Types =================

	class Vector2{
	public:
		float x,y;

		Vector2() :x(0.0f), y(0.0f)
		{ }
		Vector2(float val) :x(val), y(val)
		{ }
		Vector2(float _x, float _y)
			: x(_x), y(_y)
		{ }
		Vector2(const Vector2& v){
			x=v.x; y=v.y;
		}
		void operator=(const Vector2& v){
			x=v.x; y=v.y;
		}


		// vector magnitude
		float Magnitude() const{
			return sqrtf( MagnitudeSq() );
		}

		float MagnitudeSq() const{
			return sqre(x) + sqre(y);
		}

		// unary - operator
		const Vector2 operator-() const{
			return Vector2(-x, -y);
		}


		// vector plus operator
		const Vector2 operator+(const Vector2& v) const{
			return Vector2(x + v.x, y + v.y);
		}

		const Vector2 operator+(float fConstant) const{
			return Vector2(x + fConstant, y + fConstant);
		}

		friend const Vector2 operator+(float fConstant, const Vector2& v);

		const Vector2& operator+=(const Vector2& v){
			x += v.x; y += v.y;
			return *this;
		}

		const Vector2& operator+=(float fConstant){
			x += fConstant; y += fConstant;
			return *this;
		}


		// vector minus operator
		const Vector2 operator-(const Vector2& v) const{
			return Vector2(x - v.x, y - v.y);
		}

		const Vector2 operator-(float fConstant) const{
			return Vector2(x - fConstant, y - fConstant);
		}

		const Vector2& operator-=(const Vector2& v){
			x -= v.x; y -= v.y;
			return *this;
		}

		const Vector2& operator-=(float fConstant){
			x -= fConstant; y -= fConstant;
			return *this;
		}


		// vector constant multiplication operator (it is not cross operation)
		const Vector2 operator*(float fConstant) const{
			return Vector2(x*fConstant, y*fConstant);
		}

		friend const Vector2 operator*(float fConstant, const Vector2& v);

		const Vector2& operator*=(float fConstant){
			x *= fConstant; y *= fConstant;
			return *this;
		}


		// vector constnat division operator
		const Vector2 operator/(float fConstant) const{
			return Vector2(x/fConstant, y/fConstant);
		}

		const Vector2& operator/=(float fConstant){
			x /= fConstant; y /= fConstant;
			return *this;
		}

		// boolean operators
		bool operator==(const Vector2& v) const{
			return (x==v.x && y==v.y);
		}

		bool operator!=(const Vector2& v) const{
			return (x!=v.x || y!=v.y);
		}


		// dot product
		float Dot(const Vector2& v) const{
			return x*v.x + y*v.y;
		}


		// normalization
		void Normalize(){
			float mag = Magnitude();
			if( mag == 0 )
				return;
			x /= mag;
			y /= mag;
		}
	};
	typedef Vector2 Vertex2;
	typedef Vector2 Position2;

	// Vector2 friend operators
	const Vector2 operator+(float fConstant, const Vector2& v){
		return Vector2(fConstant + v.x, fConstant + v.y);
	}

	const Vector2 operator*(float fConstant, const Vector2& v){
		return Vector2(fConstant * v.x, fConstant * v.y);
	}




	class Vector3{
	public:
		float x,y,z;

		Vector3() :x(0.0f), y(0.0f), z(0.0f)
		{ }
		Vector3(float val) :x(val), y(val), z(val)
		{ }
		Vector3(float _x, float _y, float _z)
			: x(_x), y(_y), z(_z)
		{ }
		Vector3(const Vector3& v){
			x=v.x; y=v.y; z=v.z;
		}
		void operator=(const Vector3& v){
			x=v.x; y=v.y; z=v.z;
		}

		// explicitly type cast
		explicit Vector3(const Vector2& v){
			x=v.x; y=v.y; z=0.0f;
		}


		// vector magnitude
		float Magnitude() const{
			return sqrtf( MagnitudeSq() );
		}

		float MagnitudeSq() const{
			return sqre(x) + sqre(y) + sqre(z);
		}

		// unary - operator
		const Vector3 operator-() const{
			return Vector3(-x, -y, -z);
		}


		// vector plus operator
		const Vector3 operator+(const Vector3& v) const{
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		const Vector3 operator+(float fConstant) const{
			return Vector3(x + fConstant, y + fConstant, z + fConstant);
		}

		friend const Vector3 operator+(float fConstant, const Vector3& v);

		const Vector3& operator+=(const Vector3& v){
			x += v.x; y += v.y; z += v.z;
			return *this;
		}

		const Vector3& operator+=(float fConstant){
			x += fConstant; y += fConstant; z += fConstant;
			return *this;
		}


		// vector minus operator
		const Vector3 operator-(const Vector3& v) const{
			return Vector3(x - v.x, y - v.y, z - v.z);
		}

		const Vector3 operator-(float fConstant) const{
			return Vector3(x - fConstant, y - fConstant, z - fConstant);
		}

		const Vector3& operator-=(const Vector3& v){
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}

		const Vector3& operator-=(float fConstant){
			x -= fConstant; y -= fConstant; z -= fConstant;
			return *this;
		}


		// vector constant multiplication operator (it is not cross operation)
		const Vector3 operator*(float fConstant) const{
			return Vector3(x*fConstant, y*fConstant, z*fConstant);
		}

		friend const Vector3 operator*(float fConstant, const Vector3& v);

		const Vector3& operator*=(float fConstant){
			x *= fConstant; y *= fConstant; z *= fConstant;
			return *this;
		}


		// vector constnat division operator
		const Vector3 operator/(float fConstant) const{
			return Vector3(x/fConstant, y/fConstant, z/fConstant);
		}

		const Vector3& operator/=(float fConstant){
			x /= fConstant; y /= fConstant; z /= fConstant;
			return *this;
		}

		// boolean operators
		bool operator==(const Vector3& v) const{
			return (x==v.x && y==v.y && z==v.z);
		}

		bool operator!=(const Vector3& v) const{
			return (x!=v.x || y!=v.y || z!=v.z);
		}


		// dot product
		float Dot(const Vector3& v) const{
			return x*v.x + y*v.y + z*v.z;
		}

		// cross product
		const Vector3 Cross(const Vector3& v) const{
			return Vector3(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
		}

		// normalization
		void Normalize(){
			float mag = Magnitude();
			if( mag == 0 )
				return;
			x /= mag;
			y /= mag;
			z /= mag;
		}
	};
	typedef Vector3 Vertex3;
	typedef Vector3 Position3;

	// Vector3 friend operators
	const Vector3 operator+(float fConstant, const Vector3& v){
		return Vector3(fConstant + v.x, fConstant + v.y, fConstant + v.z);
	}

	const Vector3 operator*(float fConstant, const Vector3& v){
		return Vector3(fConstant * v.x, fConstant * v.y, fConstant * v.z);
	}




	//=============== Matrix Types =================

	// matrix determinant inline functions
	#define det2x2(a,b,c,d) ((a)*(d) - (b)*(c))

	#define det3x3(	a, b, c, \
					d, e, f, \
					g, h, i ) \
			((a)*det2x2(e,f,h,i) - (b)*det2x2(d,f,g,i) + (c)*det2x2(d,e,g,h))

	#define det4x4(	a, b, c, d, \
					e, f, g, h, \
					i, j, k, l, \
					m, n, o, p ) \
			((a)*det3x3(f,g,h,j,k,l,n,o,p) - (b)*det3x3(e,g,h,i,k,l,m,o,p) + (c)*det3x3(e,f,h,i,j,l,m,n,p) - (d)*det3x3(e,f,g,i,j,k,m,n,o))


	// 3x3 square matrix
	// defined row X col
	class Matrix3{
	public:
		float m[3][3]; //row x col

		Matrix3(){
			memset(m, 0, sizeof(float)*9);
		}
		Matrix3(float _00, float _01, float _02,
				float _10, float _11, float _12,
				float _20, float _21, float _22)
		{
			m[0][0]=_00; m[0][1]=_01; m[0][2]=_02;
			m[1][0]=_10; m[1][1]=_11; m[1][2]=_12;
			m[2][0]=_20; m[2][1]=_21; m[2][2]=_22;
		}
		Matrix3(float** mat3){
			memcpy(m, mat3, sizeof(float)*9);
		}
		Matrix3(const Matrix3& mat3){
			memcpy(m, mat3.m, sizeof(float)*9);
		}
		void operator=(const Matrix3& mat3){
			memcpy(m, mat3.m, sizeof(float)*9);
		}

		
		// matrix plus operator
		const Matrix3 operator+(const Matrix3& mat3) const{
			return Matrix3(
				m[0][0]+mat3.m[0][0], m[0][1]+mat3.m[0][1], m[0][2]+mat3.m[0][2],
				m[1][0]+mat3.m[1][0], m[1][1]+mat3.m[1][1], m[1][2]+mat3.m[1][2],
				m[2][0]+mat3.m[2][0], m[2][1]+mat3.m[2][1], m[2][2]+mat3.m[2][2] );
		}

		// matrix minus operator
		const Matrix3 operator-(const Matrix3& mat3) const{
			return Matrix3(
				m[0][0]-mat3.m[0][0], m[0][1]-mat3.m[0][1], m[0][2]-mat3.m[0][2],
				m[1][0]-mat3.m[1][0], m[1][1]-mat3.m[1][1], m[1][2]-mat3.m[1][2],
				m[2][0]-mat3.m[2][0], m[2][1]-mat3.m[2][1], m[2][2]-mat3.m[2][2] );
		}

		// matrix constant multiply operator
		const Matrix3 operator*(float fConstant) const{
			return Matrix3(
				m[0][0]*fConstant, m[0][1]*fConstant, m[0][2]*fConstant,
				m[1][0]*fConstant, m[1][1]*fConstant, m[1][2]*fConstant,
				m[2][0]*fConstant, m[2][1]*fConstant, m[2][2]*fConstant );
		}

		friend const Matrix3 operator*(float fConstant, const Matrix3& mat3);

		// matrix constant division operator
		const Matrix3 operator/(float fConstant) const{
			return Matrix3(
				m[0][0]/fConstant, m[0][1]/fConstant, m[0][2]/fConstant,
				m[1][0]/fConstant, m[1][1]/fConstant, m[1][2]/fConstant,
				m[2][0]/fConstant, m[2][1]/fConstant, m[2][2]/fConstant );
		}

		friend const Matrix3 operator/(float fConstant, const Matrix3& mat3);

		// matrix multiply operator
		const Matrix3 operator*(const Matrix3& mat3) const{
			return Matrix3(
				m[0][0]*mat3.m[0][0] + m[0][1]*mat3.m[1][0] + m[0][2]*mat3.m[2][0],
				m[0][0]*mat3.m[0][1] + m[0][1]*mat3.m[1][1] + m[0][2]*mat3.m[2][1],
				m[0][0]*mat3.m[0][2] + m[0][1]*mat3.m[1][2] + m[0][2]*mat3.m[2][2],

				m[1][0]*mat3.m[0][0] + m[1][1]*mat3.m[1][0] + m[1][2]*mat3.m[2][0],
				m[1][0]*mat3.m[0][1] + m[1][1]*mat3.m[1][1] + m[1][2]*mat3.m[2][1],
				m[1][0]*mat3.m[0][2] + m[1][1]*mat3.m[1][2] + m[1][2]*mat3.m[2][2],

				m[2][0]*mat3.m[0][0] + m[2][1]*mat3.m[1][0] + m[2][2]*mat3.m[2][0],
				m[2][0]*mat3.m[0][1] + m[2][1]*mat3.m[1][1] + m[2][2]*mat3.m[2][1],
				m[2][0]*mat3.m[0][2] + m[2][1]*mat3.m[1][2] + m[2][2]*mat3.m[2][2] );
		}

		// matrix identity
		void Identity(){
			m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f;
			m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f;
			m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f;
		}

		// matrix transpose
		const Matrix3 Transpose() const{
			return Matrix3(
				m[0][0], m[1][0], m[2][0],
				m[0][1], m[1][1], m[2][1],
				m[0][2], m[1][2], m[2][2] );
		}

		// matrix cofactor matrix
		const Matrix3 CofactorMatrix() const{
			return Matrix3(
				m[1][1]*m[2][2]-m[1][2]*m[2][1], m[1][2]*m[2][0]-m[1][0]*m[2][2], m[1][0]*m[2][1]-m[1][1]*m[2][0],
				m[0][2]*m[2][1]-m[0][1]*m[2][2], m[0][0]*m[2][2]-m[0][2]*m[2][0], m[0][1]*m[2][0]-m[0][0]*m[2][1],
				m[0][1]*m[1][2]-m[0][2]*m[1][1], m[0][2]*m[1][0]-m[0][0]*m[1][2], m[0][0]*m[1][1]-m[0][1]*m[1][0] );
		}

		// matrix adjugate(== C^t, but this method is coded hardly)
		const Matrix3 Adjugate() const{
			// return CofactorMatrix().Transpose();
			return Matrix3(
				m[1][1]*m[2][2]-m[1][2]*m[2][1], m[0][2]*m[2][1]-m[0][1]*m[2][2], m[0][1]*m[1][2]-m[0][2]*m[1][1],
				m[1][2]*m[2][0]-m[1][0]*m[2][2], m[0][0]*m[2][2]-m[0][2]*m[2][0], m[0][2]*m[1][0]-m[0][0]*m[1][2],
				m[1][0]*m[2][1]-m[1][1]*m[2][0], m[0][1]*m[2][0]-m[0][0]*m[2][1], m[0][0]*m[1][1]-m[0][1]*m[1][0] );
		}

		// matrix inverse
		const Matrix3 Inverse() const{
			const Matrix3 adjM = this->Adjugate();
			float det = m[0][0]*adjM.m[0][0] + m[0][1]*adjM.m[1][0] + m[0][2]*adjM.m[2][0];

			if( det == 0.0f )
				return Matrix3();

			return adjM / det;
		}

		// matrix determination
		float Determin() const{
			return det3x3(
				m[0][0], m[0][1], m[0][2],
				m[1][0], m[1][1], m[1][2],
				m[2][0], m[2][1], m[2][2] );
		}
	};

	const Matrix3 operator*(float fConstant, const Matrix3& mat3){
		return Matrix3(
				mat3.m[0][0]*fConstant, mat3.m[0][1]*fConstant, mat3.m[0][2]*fConstant,
				mat3.m[1][0]*fConstant, mat3.m[1][1]*fConstant, mat3.m[1][2]*fConstant,
				mat3.m[2][0]*fConstant, mat3.m[2][1]*fConstant, mat3.m[2][2]*fConstant );
	}

	const Matrix3 operator/(float fConstant, const Matrix3& mat3){
		return Matrix3(
				mat3.m[0][0]/fConstant, mat3.m[0][1]/fConstant, mat3.m[0][2]/fConstant,
				mat3.m[1][0]/fConstant, mat3.m[1][1]/fConstant, mat3.m[1][2]/fConstant,
				mat3.m[2][0]/fConstant, mat3.m[2][1]/fConstant, mat3.m[2][2]/fConstant );
	}



	// 4x4 square matrix
	// defined row X col
	class Matrix4{
	public:
		float m[4][4]; //row x col

		Matrix4(){
			memset(m, 0, sizeof(float)*16);
		}
		Matrix4(float _00, float _01, float _02, float _03,
				float _10, float _11, float _12, float _13,
				float _20, float _21, float _22, float _23,
				float _30, float _31, float _32, float _33)
		{
			m[0][0]=_00; m[0][1]=_01; m[0][2]=_02; m[0][3]=_03;
			m[1][0]=_10; m[1][1]=_11; m[1][2]=_12; m[1][3]=_13;
			m[2][0]=_20; m[2][1]=_21; m[2][2]=_22; m[2][3]=_23;
			m[3][0]=_30; m[3][1]=_31; m[3][2]=_32; m[3][3]=_33;
		}
		Matrix4(float** mat4){
			memcpy(m, mat4, sizeof(float)*16);
		}
		Matrix4(const Matrix4& mat4){
			memcpy(m, mat4.m, sizeof(float)*16);
		}
		void operator=(const Matrix4& mat4){
			memcpy(m, mat4.m, sizeof(float)*16);
		}


		// matrix plus operator
		const Matrix4 operator+(const Matrix4& mat4) const{
			return Matrix4(
				m[0][0]+mat4.m[0][0], m[0][1]+mat4.m[0][1], m[0][2]+mat4.m[0][2], m[0][3]+mat4.m[0][3],
				m[1][0]+mat4.m[1][0], m[1][1]+mat4.m[1][1], m[1][2]+mat4.m[1][2], m[1][3]+mat4.m[1][3],
				m[2][0]+mat4.m[2][0], m[2][1]+mat4.m[2][1], m[2][2]+mat4.m[2][2], m[2][3]+mat4.m[2][3],
				m[3][0]+mat4.m[3][0], m[3][1]+mat4.m[3][1], m[3][2]+mat4.m[3][2], m[3][3]+mat4.m[3][3] );
		}

		// matrix minus operator
		const Matrix4 operator-(const Matrix4& mat4) const{
			return Matrix4(
				m[0][0]-mat4.m[0][0], m[0][1]-mat4.m[0][1], m[0][2]-mat4.m[0][2], m[0][3]-mat4.m[0][3],
				m[1][0]-mat4.m[1][0], m[1][1]-mat4.m[1][1], m[1][2]-mat4.m[1][2], m[1][3]-mat4.m[1][3],
				m[2][0]-mat4.m[2][0], m[2][1]-mat4.m[2][1], m[2][2]-mat4.m[2][2], m[2][3]-mat4.m[2][3],
				m[3][0]-mat4.m[3][0], m[3][1]-mat4.m[3][1], m[3][2]-mat4.m[3][2], m[3][3]-mat4.m[3][3] );
		}

		// matrix constant multiply operator
		const Matrix4 operator*(float fConstant) const{
			return Matrix4(
				m[0][0]*fConstant, m[0][1]*fConstant, m[0][2]*fConstant, m[0][3]*fConstant,
				m[1][0]*fConstant, m[1][1]*fConstant, m[1][2]*fConstant, m[1][3]*fConstant,
				m[2][0]*fConstant, m[2][1]*fConstant, m[2][2]*fConstant, m[2][3]*fConstant, 
				m[3][0]*fConstant, m[3][1]*fConstant, m[3][2]*fConstant, m[3][3]*fConstant );
		}

		friend const Matrix4 operator*(float fConstant, const Matrix4& mat4);

		// matrix constant division operator
		const Matrix4 operator/(float fConstant) const{
			return Matrix4(
				m[0][0]/fConstant, m[0][1]/fConstant, m[0][2]/fConstant, m[0][3]/fConstant,
				m[1][0]/fConstant, m[1][1]/fConstant, m[1][2]/fConstant, m[1][3]/fConstant,
				m[2][0]/fConstant, m[2][1]/fConstant, m[2][2]/fConstant, m[2][3]/fConstant, 
				m[3][0]/fConstant, m[3][1]/fConstant, m[3][2]/fConstant, m[3][3]/fConstant );
		}

		friend const Matrix4 operator/(float fConstant, const Matrix4& mat4);

		// matrix multiply operator
		const Matrix4 operator*(const Matrix4& mat4) const{
			return Matrix4(
				m[0][0]*mat4.m[0][0] + m[0][1]*mat4.m[1][0] + m[0][2]*mat4.m[2][0] + m[0][3]*mat4.m[3][0],
				m[0][0]*mat4.m[0][1] + m[0][1]*mat4.m[1][1] + m[0][2]*mat4.m[2][1] + m[0][3]*mat4.m[3][1],
				m[0][0]*mat4.m[0][2] + m[0][1]*mat4.m[1][2] + m[0][2]*mat4.m[2][2] + m[0][3]*mat4.m[3][2],
				m[0][0]*mat4.m[0][3] + m[0][1]*mat4.m[1][3] + m[0][2]*mat4.m[2][3] + m[0][3]*mat4.m[3][3],

				m[1][0]*mat4.m[0][0] + m[1][1]*mat4.m[1][0] + m[1][2]*mat4.m[2][0] + m[1][3]*mat4.m[3][0],
				m[1][0]*mat4.m[0][1] + m[1][1]*mat4.m[1][1] + m[1][2]*mat4.m[2][1] + m[1][3]*mat4.m[3][1],
				m[1][0]*mat4.m[0][2] + m[1][1]*mat4.m[1][2] + m[1][2]*mat4.m[2][2] + m[1][3]*mat4.m[3][2],
				m[1][0]*mat4.m[0][3] + m[1][1]*mat4.m[1][3] + m[1][2]*mat4.m[2][3] + m[1][3]*mat4.m[3][3],

				m[2][0]*mat4.m[0][0] + m[2][1]*mat4.m[1][0] + m[2][2]*mat4.m[2][0] + m[2][3]*mat4.m[3][0],
				m[2][0]*mat4.m[0][1] + m[2][1]*mat4.m[1][1] + m[2][2]*mat4.m[2][1] + m[2][3]*mat4.m[3][1],
				m[2][0]*mat4.m[0][2] + m[2][1]*mat4.m[1][2] + m[2][2]*mat4.m[2][2] + m[2][3]*mat4.m[3][2],
				m[2][0]*mat4.m[0][3] + m[2][1]*mat4.m[1][3] + m[2][2]*mat4.m[2][3] + m[2][3]*mat4.m[3][3],

				m[3][0]*mat4.m[0][0] + m[3][1]*mat4.m[1][0] + m[3][2]*mat4.m[2][0] + m[3][3]*mat4.m[3][0],
				m[3][0]*mat4.m[0][1] + m[3][1]*mat4.m[1][1] + m[3][2]*mat4.m[2][1] + m[3][3]*mat4.m[3][1],
				m[3][0]*mat4.m[0][2] + m[3][1]*mat4.m[1][2] + m[3][2]*mat4.m[2][2] + m[3][3]*mat4.m[3][2],
				m[3][0]*mat4.m[0][3] + m[3][1]*mat4.m[1][3] + m[3][2]*mat4.m[2][3] + m[3][3]*mat4.m[3][3] );
		}

		// matrix identity
		void Identity(){
			m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f; m[0][3]=0.0f;
			m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f; m[1][3]=0.0f;
			m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f; m[2][3]=0.0f;
			m[3][0]=0.0f; m[3][1]=0.0f; m[3][2]=0.0f; m[3][3]=1.0f;
		}

		// matrix transpose
		const Matrix4 Transpose() const{
			return Matrix4(
				m[0][0], m[1][0], m[2][0], m[3][0],
				m[0][1], m[1][1], m[2][1], m[3][1],
				m[0][2], m[1][2], m[2][2], m[3][2],
				m[0][3], m[1][3], m[2][3], m[3][3] );
		}

		// matrix cofactor matrix
		const Matrix4 CofactorMatrix() const{
			return Matrix4(
				det3x3(m[1][1],m[1][2],m[1][3],m[2][1],m[2][2],m[2][3],m[3][1],m[3][2],m[3][3]),
				-det3x3(m[1][0],m[1][2],m[1][3],m[2][0],m[2][2],m[2][3],m[3][0],m[3][2],m[3][3]),
				det3x3(m[1][0],m[1][1],m[1][3],m[2][0],m[2][1],m[2][3],m[3][0],m[3][1],m[3][3]),
				-det3x3(m[1][0],m[1][1],m[1][2],m[2][0],m[2][1],m[2][2],m[3][0],m[3][1],m[3][2]),

				-det3x3(m[0][1],m[0][2],m[0][3],m[2][1],m[2][2],m[2][3],m[3][1],m[3][2],m[3][3]),
				det3x3(m[0][0],m[0][2],m[0][3],m[2][0],m[2][2],m[2][3],m[3][0],m[3][2],m[3][3]),
				-det3x3(m[0][0],m[0][1],m[0][3],m[2][0],m[2][1],m[2][3],m[3][0],m[3][1],m[3][3]),
				det3x3(m[0][0],m[0][1],m[0][2],m[2][0],m[2][1],m[2][2],m[3][0],m[3][1],m[3][2]),

				det3x3(m[0][1],m[0][2],m[0][3],m[1][1],m[1][2],m[1][3],m[3][1],m[3][2],m[3][3]),
				-det3x3(m[0][0],m[0][2],m[0][3],m[1][0],m[1][2],m[1][3],m[3][0],m[3][2],m[3][3]),
				det3x3(m[0][0],m[0][1],m[0][3],m[1][0],m[1][1],m[1][3],m[3][0],m[3][1],m[3][3]),
				-det3x3(m[0][0],m[0][1],m[0][2],m[1][0],m[1][1],m[1][2],m[3][0],m[3][1],m[3][2]),

				-det3x3(m[0][1],m[0][2],m[0][3],m[1][1],m[1][2],m[1][3],m[2][1],m[2][2],m[2][3]),
				det3x3(m[0][0],m[0][2],m[0][3],m[1][0],m[1][2],m[1][3],m[2][0],m[2][2],m[2][3]),
				-det3x3(m[0][0],m[0][1],m[0][3],m[1][0],m[1][1],m[1][3],m[2][0],m[2][1],m[2][3]),
				det3x3(m[0][0],m[0][1],m[0][2],m[1][0],m[1][1],m[1][2],m[2][0],m[2][1],m[2][2]) );
		}

		// matrix adjugate(== C^t, but this method is coded hardly)
		const Matrix4 Adjugate() const{
			// return CofactorMatrix().Transpose();
			return Matrix4(
				det3x3(m[1][1],m[1][2],m[1][3],m[2][1],m[2][2],m[2][3],m[3][1],m[3][2],m[3][3]),
				-det3x3(m[0][1],m[0][2],m[0][3],m[2][1],m[2][2],m[2][3],m[3][1],m[3][2],m[3][3]),
				det3x3(m[0][1],m[0][2],m[0][3],m[1][1],m[1][2],m[1][3],m[3][1],m[3][2],m[3][3]),
				-det3x3(m[0][1],m[0][2],m[0][3],m[1][1],m[1][2],m[1][3],m[2][1],m[2][2],m[2][3]),

				-det3x3(m[1][0],m[1][2],m[1][3],m[2][0],m[2][2],m[2][3],m[3][0],m[3][2],m[3][3]),
				det3x3(m[0][0],m[0][2],m[0][3],m[2][0],m[2][2],m[2][3],m[3][0],m[3][2],m[3][3]),
				-det3x3(m[0][0],m[0][2],m[0][3],m[1][0],m[1][2],m[1][3],m[3][0],m[3][2],m[3][3]),
				det3x3(m[0][0],m[0][2],m[0][3],m[1][0],m[1][2],m[1][3],m[2][0],m[2][2],m[2][3]),

				det3x3(m[1][0],m[1][1],m[1][3],m[2][0],m[2][1],m[2][3],m[3][0],m[3][1],m[3][3]),
				-det3x3(m[0][0],m[0][1],m[0][3],m[2][0],m[2][1],m[2][3],m[3][0],m[3][1],m[3][3]),
				det3x3(m[0][0],m[0][1],m[0][3],m[1][0],m[1][1],m[1][3],m[3][0],m[3][1],m[3][3]),
				-det3x3(m[0][0],m[0][1],m[0][3],m[1][0],m[1][1],m[1][3],m[2][0],m[2][1],m[2][3]),

				-det3x3(m[1][0],m[1][1],m[1][2],m[2][0],m[2][1],m[2][2],m[3][0],m[3][1],m[3][2]),
				det3x3(m[0][0],m[0][1],m[0][2],m[2][0],m[2][1],m[2][2],m[3][0],m[3][1],m[3][2]),
				-det3x3(m[0][0],m[0][1],m[0][2],m[1][0],m[1][1],m[1][2],m[3][0],m[3][1],m[3][2]),
				det3x3(m[0][0],m[0][1],m[0][2],m[1][0],m[1][1],m[1][2],m[2][0],m[2][1],m[2][2]) );
		}

		// matrix inverse
		const Matrix4 Inverse() const{
			const Matrix4 adjM = this->Adjugate();
			float det = m[0][0]*adjM.m[0][0] + m[0][1]*adjM.m[1][0] + m[0][2]*adjM.m[2][0] + m[0][3]*adjM.m[3][0];

			if( det == 0.0f )
				return Matrix4();

			return adjM / det;
		}

		// matrix determination
		float Determin() const{
			return det4x4(
				m[0][0], m[0][1], m[0][2], m[0][3],
				m[1][0], m[1][1], m[1][2], m[1][3],
				m[2][0], m[2][1], m[2][2], m[2][3],
				m[3][0], m[3][1], m[3][2], m[3][3] );
		}
	};

	const Matrix4 operator*(float fConstant, const Matrix4& mat4){
		return Matrix4(
				mat4.m[0][0]*fConstant, mat4.m[0][1]*fConstant, mat4.m[0][2]*fConstant, mat4.m[0][3]*fConstant,
				mat4.m[1][0]*fConstant, mat4.m[1][1]*fConstant, mat4.m[1][2]*fConstant, mat4.m[1][3]*fConstant,
				mat4.m[2][0]*fConstant, mat4.m[2][1]*fConstant, mat4.m[2][2]*fConstant, mat4.m[2][3]*fConstant, 
				mat4.m[3][0]*fConstant, mat4.m[3][1]*fConstant, mat4.m[3][2]*fConstant, mat4.m[3][3]*fConstant );
	}

	const Matrix4 operator/(float fConstant, const Matrix4& mat4){
		return Matrix4(
				mat4.m[0][0]/fConstant, mat4.m[0][1]/fConstant, mat4.m[0][2]/fConstant, mat4.m[0][3]/fConstant,
				mat4.m[1][0]/fConstant, mat4.m[1][1]/fConstant, mat4.m[1][2]/fConstant, mat4.m[1][3]/fConstant,
				mat4.m[2][0]/fConstant, mat4.m[2][1]/fConstant, mat4.m[2][2]/fConstant, mat4.m[2][3]/fConstant, 
				mat4.m[3][0]/fConstant, mat4.m[3][1]/fConstant, mat4.m[3][2]/fConstant, mat4.m[3][3]/fConstant );
	}


}
#endif
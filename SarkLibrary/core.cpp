#include <stdlib.h>
#include <memory.h>

#include "core.h"
#include "Debug.h"


namespace sark{
	//=============== Vector Types =================

	/**
	Type of vector 2D
	*/

	Vector2::Vector2() :x(0.0f), y(0.0f)
	{ }
	Vector2::Vector2(real val) : x(val), y(val)
	{ }
	Vector2::Vector2(real _x, real _y)
		: x(_x), y(_y)
	{ }
	Vector2::Vector2(const Vector2& v){
		x = v.x; y = v.y;
	}
	Vector2& Vector2::operator=(const Vector2& v){
		x = v.x; y = v.y;
		return *this;
	}

	void Vector2::Set(real _x, real _y){
		x = _x; y = _y;
	}

	// vector magnitude
	real Vector2::Magnitude() const{
		return math::sqrt(math::sqre(x) + math::sqre(y));
	}

	real Vector2::MagnitudeSq() const{
		return math::sqre(x) + math::sqre(y);
	}

	// unary - operator
	const Vector2 Vector2::operator-() const{
		return Vector2(-x, -y);
	}


	// vector plus operator
	const Vector2 Vector2::operator+(const Vector2& v) const{
		return Vector2(x + v.x, y + v.y);
	}

	Vector2& Vector2::operator+=(const Vector2& v){
		x += v.x; y += v.y;
		return *this;
	}

	const Vector2 Vector2::operator+(real fConstant) const{
		return Vector2(x + fConstant, y + fConstant);
	}

	Vector2& Vector2::operator+=(real fConstant){
		x += fConstant; y += fConstant;
		return *this;
	}


	// vector minus operator
	const Vector2 Vector2::operator-(const Vector2& v) const{
		return Vector2(x - v.x, y - v.y);
	}

	Vector2& Vector2::operator-=(const Vector2& v){
		x -= v.x; y -= v.y;
		return *this;
	}

	const Vector2 Vector2::operator-(real fConstant) const{
		return Vector2(x - fConstant, y - fConstant);
	}

	Vector2& Vector2::operator-=(real fConstant){
		x -= fConstant; y -= fConstant;
		return *this;
	}


	// vector constant multiplication operator (it is not cross operation)
	const Vector2 Vector2::operator*(const Vector2& v) const{
		return Vector2(x*v.x, y*v.y);
	}

	Vector2& Vector2::operator*=(const Vector2& v){
		x *= v.x; y *= v.y;
		return *this;
	}

	const Vector2 Vector2::operator*(real fConstant) const{
		return Vector2(x*fConstant, y*fConstant);
	}

	Vector2& Vector2::operator*=(real fConstant){
		x *= fConstant; y *= fConstant;
		return *this;
	}


	// vector constnat division operator
	const Vector2 Vector2::operator/(real fConstant) const{
		ONLYDBG_CODEBLOCK(
		if (fConstant == 0.f)
			LogFatal("division by zero");
		);
		return Vector2(x / fConstant, y / fConstant);
	}

	Vector2& Vector2::operator/=(real fConstant){
		ONLYDBG_CODEBLOCK(
		if (fConstant == 0.f)
			LogFatal("division by zero");
		);
		x /= fConstant; y /= fConstant;
		return *this;
	}


	// boolean operators
	bool Vector2::operator==(const Vector2& v) const{
		return (x == v.x && y == v.y);
	}

	bool Vector2::operator!=(const Vector2& v) const{
		return (x != v.x || y != v.y);
	}

	// it consider real type equal
	bool Vector2::Equal(const Vector2& v) const{
		return (math::real_equal(x, v.x) && math::real_equal(y, v.y));
	}


	// check if zero vector
	bool Vector2::IsZero() const{
		return (x == 0.0f && y == 0.0f);
	}

	// dot product
	real Vector2::Dot(const Vector2& v) const{
		return x*v.x + y*v.y;
	}


	// get normal and normalize this
	const Vector2 Vector2::Normal() const{
		real mag = Magnitude();
		ONLYDBG_CODEBLOCK(
		if (math::real_equal(mag, 0.f))
			LogFatal("division by zero");
		);
		return Vector2(x / mag, y / mag);
	}
	void Vector2::Normalize(){
		real mag = Magnitude();
		ONLYDBG_CODEBLOCK(
		if (math::real_equal(mag, 0.f))
			LogFatal("division by zero");
		);
		x /= mag;
		y /= mag;
	}


	// calculate Euclidean distance of given two vectors
	real Vector2::Distance(const Vector2& v1, const Vector2& v2){
		return math::sqrt(math::sqre(v1.x - v2.x) + math::sqre(v1.y - v2.y));
	}

	// calculate radians of given two vectors
	real Vector2::Angle(const Vector2& v1, const Vector2& v2){
		return math::acos(v1.Normal().Dot(v2.Normal()));
	}


	// Vector2 friend operators
	const Vector2 operator+(real fConstant, const Vector2& v){
		return Vector2(fConstant + v.x, fConstant + v.y);
	}

	const Vector2 operator-(real fConstant, const Vector2& v){
		return Vector2(fConstant - v.x, fConstant - v.y);
	}

	const Vector2 operator*(real fConstant, const Vector2& v){
		return Vector2(fConstant * v.x, fConstant * v.y);
	}

	// x-axis right
	const Vector2 Vector2::Right(1, 0);
	// y-axis up
	const Vector2 Vector2::Up(0, 1);

	// general gravity force. 9.8 m/s^2 of negative y-axis
	const Vector2 Vector2::Gravity(0, -9.80665f);



	/**
	Type of vector 3D
	*/

	Vector3::Vector3() :x(0.0f), y(0.0f), z(0.0f)
	{ }
	Vector3::Vector3(real val) : x(val), y(val), z(val)
	{ }
	Vector3::Vector3(real _x, real _y, real _z)
		: x(_x), y(_y), z(_z)
	{ }
	Vector3::Vector3(const Vector3& v){
		x = v.x; y = v.y; z = v.z;
	}
	Vector3& Vector3::operator=(const Vector3& v){
		x = v.x; y = v.y; z = v.z;
		return *this;
	}

	void Vector3::Set(real _x, real _y, real _z){
		x = _x; y = _y; z = _z;
	}

	// explicitly type cast
	Vector3::Vector3(const Vector2& v){
		x = v.x; y = v.y; z = 0.0f;
	}


	// vector magnitude
	real Vector3::Magnitude() const{
		return math::sqrt(math::sqre(x) + math::sqre(y) + math::sqre(z));
	}

	real Vector3::MagnitudeSq() const{
		return math::sqre(x) + math::sqre(y) + math::sqre(z);
	}

	// unary - operator
	const Vector3 Vector3::operator-() const{
		return Vector3(-x, -y, -z);
	}


	// vector plus operator
	const Vector3 Vector3::operator+(const Vector3& v) const{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3& Vector3::operator+=(const Vector3& v){
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	const Vector3 Vector3::operator+(real fConstant) const{
		return Vector3(x + fConstant, y + fConstant, z + fConstant);
	}

	Vector3& Vector3::operator+=(real fConstant){
		x += fConstant; y += fConstant; z += fConstant;
		return *this;
	}


	// vector minus operator
	const Vector3 Vector3::operator-(const Vector3& v) const{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3& Vector3::operator-=(const Vector3& v){
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	const Vector3 Vector3::operator-(real fConstant) const{
		return Vector3(x - fConstant, y - fConstant, z - fConstant);
	}

	Vector3& Vector3::operator-=(real fConstant){
		x -= fConstant; y -= fConstant; z -= fConstant;
		return *this;
	}


	// vector constant multiplication operator (it is not cross operation)
	const Vector3 Vector3::operator*(const Vector3& v) const{
		return Vector3(x*v.x, y*v.y, z*v.z);
	}

	Vector3& Vector3::operator*=(const Vector3& v){
		x *= v.x; y *= v.y; z *= v.z;
		return *this;
	}

	const Vector3 Vector3::operator*(real fConstant) const{
		return Vector3(x*fConstant, y*fConstant, z*fConstant);
	}

	Vector3& Vector3::operator*=(real fConstant){
		x *= fConstant; y *= fConstant; z *= fConstant;
		return *this;
	}


	// vector x matrix
	const Vector3 Vector3::operator*(const Matrix3& mat3) const{
		return Vector3(
			x*mat3.m[0][0] + y*mat3.m[1][0] + z*mat3.m[2][0],
			x*mat3.m[0][1] + y*mat3.m[1][1] + z*mat3.m[2][1],
			x*mat3.m[0][2] + y*mat3.m[1][2] + z*mat3.m[2][2]);
	}

	// vector x matrix for this
	Vector3& Vector3::operator*=(const Matrix3& mat3){
		Set(x*mat3.m[0][0] + y*mat3.m[1][0] + z*mat3.m[2][0],
			x*mat3.m[0][1] + y*mat3.m[1][1] + z*mat3.m[2][1],
			x*mat3.m[0][2] + y*mat3.m[1][2] + z*mat3.m[2][2]);
		return *this;
	}


	// vector constnat division operator
	const Vector3 Vector3::operator/(real fConstant) const{
		ONLYDBG_CODEBLOCK(
		if (fConstant == 0.f)
			LogFatal("division by zero");
		);
		return Vector3(x / fConstant, y / fConstant, z / fConstant);
	}

	Vector3& Vector3::operator/=(real fConstant){
		ONLYDBG_CODEBLOCK(
		if (fConstant == 0.f)
			LogFatal("division by zero");
		);
		x /= fConstant; y /= fConstant; z /= fConstant;
		return *this;
	}


	// boolean operators
	bool Vector3::operator==(const Vector3& v) const{
		return (x == v.x && y == v.y && z == v.z);
	}

	bool Vector3::operator!=(const Vector3& v) const{
		return (x != v.x || y != v.y || z != v.z);
	}

	// it consider real type equal
	bool Vector3::Equal(const Vector3& v) const{
		return (math::real_equal(x, v.x) && math::real_equal(y, v.y) && math::real_equal(z, v.z));
	}

	// check if zero vector
	bool Vector3::IsZero() const{
		return (x == 0.0f && y == 0.0f && z == 0.0f);
	}

	// dot product
	real Vector3::Dot(const Vector3& v) const{
		return x*v.x + y*v.y + z*v.z;
	}

	// cross product
	const Vector3 Vector3::Cross(const Vector3& v) const{
		return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}


	// get normal and normalize this
	const Vector3 Vector3::Normal() const{
		real mag = Magnitude();
		ONLYDBG_CODEBLOCK(
		if (math::real_equal(mag, 0.f))
			LogFatal("division by zero");
		);
		return Vector3(x / mag, y / mag, z / mag);
	}
	void Vector3::Normalize(){
		real mag = Magnitude();
		ONLYDBG_CODEBLOCK(
		if (math::real_equal(mag, 0.f))
			LogFatal("division by zero");
		);
		x /= mag;
		y /= mag;
		z /= mag;
	}


	// calculate Euclidean distance of given two vectors
	real Vector3::Distance(const Vector3& v1, const Vector3& v2){
		return math::sqrt(math::sqre(v1.x - v2.x) + math::sqre(v1.y - v2.y) + math::sqre(v1.z - v2.z));
	}

	// calculate radians of given two vectors
	real Vector3::Angle(const Vector3& v1, const Vector3& v2){
		return math::acos(v1.Normal().Dot(v2.Normal()));
	}

	// Vector3 friend operators
	const Vector3 operator+(real fConstant, const Vector3& v){
		return Vector3(fConstant + v.x, fConstant + v.y, fConstant + v.z);
	}

	const Vector3 operator-(real fConstant, const Vector3& v){
		return Vector3(fConstant - v.x, fConstant - v.y, fConstant - v.z);
	}

	const Vector3 operator*(real fConstant, const Vector3& v){
		return Vector3(fConstant * v.x, fConstant * v.y, fConstant * v.z);
	}

	// x-axis right
	const Vector3 Vector3::Right(1, 0, 0);
	// y-axis up
	const Vector3 Vector3::Up(0, 1, 0);
	// negative z-axis forward
	const Vector3 Vector3::Forward(0, 0, -1);

	// general gravity force. 9.8 m/s^2 of negative y-axis
	const Vector3 Vector3::Gravity(0, -9.80665f, 0);



	/**
	Type of vector 4D
	*/

	Vector4::Vector4(){
		x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f;
	}
	Vector4::Vector4(real val){
		x = y = z = w = val;
	}
	Vector4::Vector4(real _x, real _y, real _z, real _w){
		x = _x; y = _y; z = _z; w = _w;
	}
	Vector4::Vector4(const Vector3& vec3, real _w){
		xyz = vec3; w = _w;
	}
	Vector4::Vector4(const Vector4& v){
		x = v.x; y = v.y; z = v.z; w = v.w;
	}
	Vector4& Vector4::operator=(const Vector4& v){
		x = v.x; y = v.y; z = v.z; w = v.w;
		return *this;
	}

	void Vector4::Set(real _x, real _y, real _z, real _w){
		x = _x; y = _y; z = _z; w = _w;
	}

	// explicitly type cast
	Vector4::Vector4(const Vector2& v){
		x = v.x; y = v.y; z = 0.0f; w = 0.0f;
	}
	Vector4::Vector4(const Vector3& v){
		x = v.x; y = v.y; z = v.z; w = 0.0f;
	}


	// vector magnitude
	real Vector4::Magnitude() const{
		return math::sqrt(math::sqre(x) + math::sqre(y) + math::sqre(z) + math::sqre(w));
	}

	real Vector4::MagnitudeSq() const{
		return math::sqre(x) + math::sqre(y) + math::sqre(z) + math::sqre(w);
	}

	// unary - operator
	const Vector4 Vector4::operator-() const{
		return Vector4(-x, -y, -z, -w);
	}


	// vector plus operator
	const Vector4 Vector4::operator+(const Vector4& v) const{
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vector4& Vector4::operator+=(const Vector4& v){
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}

	const Vector4 Vector4::operator+(real fConstant) const{
		return Vector4(x + fConstant, y + fConstant, z + fConstant, w + fConstant);
	}

	Vector4& Vector4::operator+=(real fConstant){
		x += fConstant; y += fConstant; z += fConstant; w += fConstant;
		return *this;
	}


	// vector minus operator
	const Vector4 Vector4::operator-(const Vector4& v) const{
		return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Vector4& Vector4::operator-=(const Vector4& v){
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
		return *this;
	}

	const Vector4 Vector4::operator-(real fConstant) const{
		return Vector4(x - fConstant, y - fConstant, z - fConstant, w - fConstant);
	}

	Vector4& Vector4::operator-=(real fConstant){
		x -= fConstant; y -= fConstant; z -= fConstant; w -= fConstant;
		return *this;
	}


	// vector constant multiplication operator (it is not cross operation)
	const Vector4 Vector4::operator*(const Vector4& v) const{
		return Vector4(x*v.x, y*v.y, z*v.z, w*v.w);
	}

	Vector4& Vector4::operator*=(const Vector4& v){
		x *= v.x; y *= v.y; z *= v.z; w *= v.w;
		return *this;
	}

	const Vector4 Vector4::operator*(real fConstant) const{
		return Vector4(x*fConstant, y*fConstant, z*fConstant, w*fConstant);
	}

	Vector4& Vector4::operator*=(real fConstant){
		x *= fConstant; y *= fConstant; z *= fConstant; w *= fConstant;
		return *this;
	}


	// vector x matrix
	const Vector4 Vector4::operator*(const Matrix4& mat4) const{
		return Vector4(
			x*mat4.m[0][0] + y*mat4.m[1][0] + z*mat4.m[2][0] + w*mat4.m[3][0],
			x*mat4.m[0][1] + y*mat4.m[1][1] + z*mat4.m[2][1] + w*mat4.m[3][1],
			x*mat4.m[0][2] + y*mat4.m[1][2] + z*mat4.m[2][2] + w*mat4.m[3][2],
			x*mat4.m[0][3] + y*mat4.m[1][3] + z*mat4.m[2][3] + w*mat4.m[3][3]);
	}

	// vector x matrix for this
	Vector4& Vector4::operator*=(const Matrix4& mat4){
		Set(x*mat4.m[0][0] + y*mat4.m[1][0] + z*mat4.m[2][0] + w*mat4.m[3][0],
			x*mat4.m[0][1] + y*mat4.m[1][1] + z*mat4.m[2][1] + w*mat4.m[3][1],
			x*mat4.m[0][2] + y*mat4.m[1][2] + z*mat4.m[2][2] + w*mat4.m[3][2],
			x*mat4.m[0][3] + y*mat4.m[1][3] + z*mat4.m[2][3] + w*mat4.m[3][3]);
		return *this;
	}


	// vector constnat division operator
	const Vector4 Vector4::operator/(real fConstant) const{
		ONLYDBG_CODEBLOCK(
		if (fConstant == 0.f)
			LogFatal("division by zero");
		);
		return Vector4(x / fConstant, y / fConstant, z / fConstant, w / fConstant);
	}

	Vector4& Vector4::operator/=(real fConstant){
		ONLYDBG_CODEBLOCK(
		if (fConstant == 0.f)
			LogFatal("division by zero");
		);
		x /= fConstant; y /= fConstant; z /= fConstant; w /= fConstant;
		return *this;
	}


	// boolean operators
	bool Vector4::operator==(const Vector4& v) const{
		return (x == v.x && y == v.y && z == v.z && w == v.w);
	}

	bool Vector4::operator!=(const Vector4& v) const{
		return (x != v.x || y != v.y || z != v.z || w != v.w);
	}

	// it consider real type equal
	bool Vector4::Equal(const Vector4& v) const{
		return (math::real_equal(x, v.x) && math::real_equal(y, v.y)
			&& math::real_equal(z, v.z) && math::real_equal(w, v.w));
	}

	// check if zero vector
	bool Vector4::IsZero() const{
		return (x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f);
	}

	// dot product
	real Vector4::Dot(const Vector4& v) const{
		return x*v.x + y*v.y + z*v.z + w*v.w;
	}

	// get normal and normalize this
	const Vector4 Vector4::Normal() const{
		real mag = Magnitude();
		ONLYDBG_CODEBLOCK(
		if (math::real_equal(mag, 0.f))
			LogFatal("division by zero");
		);
		return Vector4(x / mag, y / mag, z / mag, w / mag);
	}
	void Vector4::Normalize(){
		real mag = Magnitude();
		ONLYDBG_CODEBLOCK(
		if (math::real_equal(mag, 0.f))
			LogFatal("division by zero");
		);
		x /= mag;
		y /= mag;
		z /= mag;
		w /= mag;
	}

	// calculate Euclidean distance of given two vectors
	real Vector4::Distance(const Vector4& v1, const Vector4& v2){
		return math::sqrt(math::sqre(v1.x - v2.x) + math::sqre(v1.y - v2.y) + math::sqre(v1.z - v2.z) + math::sqre(v1.w - v2.w));
	}

	// Vector4 friend operators
	const Vector4 operator+(real fConstant, const Vector4& v){
		return Vector4(fConstant + v.x, fConstant + v.y, fConstant + v.z, fConstant + v.w);
	}

	const Vector4 operator-(real fConstant, const Vector4& v){
		return Vector4(fConstant - v.x, fConstant - v.y, fConstant - v.z, fConstant - v.w);
	}

	const Vector4 operator*(real fConstant, const Vector4& v){
		return Vector4(fConstant * v.x, fConstant * v.y, fConstant * v.z, fConstant * v.w);
	}


	//=============== Matrix Types =================

	/**
	Type of 3x3 square matrix defined as row X col order
	*/

	Matrix3::Matrix3(real diagonal){
		memset(m, 0, sizeof(real)* 9);
		m[0][0] = m[1][1] = m[2][2] = diagonal;
	}
	Matrix3::Matrix3(real _00, real _01, real _02,
		real _10, real _11, real _12,
		real _20, real _21, real _22)
	{
		m[0][0] = _00; m[0][1] = _01; m[0][2] = _02;
		m[1][0] = _10; m[1][1] = _11; m[1][2] = _12;
		m[2][0] = _20; m[2][1] = _21; m[2][2] = _22;
	}
	Matrix3::Matrix3(real** mat3){
		memcpy(m, mat3, sizeof(real)* 9);
	}
	Matrix3::Matrix3(const Vector3& row0, const Vector3& row1, const Vector3& row2){
		row[0] = row0; row[1] = row1; row[2] = row2;
	}
	Matrix3::Matrix3(const Matrix3& mat3){
		memcpy(m, mat3.m, sizeof(real)* 9);
	}
	Matrix3& Matrix3::operator=(const Matrix3& mat3){
		memcpy(m, mat3.m, sizeof(real)* 9);
		return *this;
	}

	// set matrix elements
	void Matrix3::Set(real _00, real _01, real _02,
		real _10, real _11, real _12,
		real _20, real _21, real _22)
	{
		m[0][0] = _00; m[0][1] = _01; m[0][2] = _02;
		m[1][0] = _10; m[1][1] = _11; m[1][2] = _12;
		m[2][0] = _20; m[2][1] = _21; m[2][2] = _22;
	}
	// set matrix elements
	void Matrix3::Set(const Vector3& row0, const Vector3& row1, const Vector3& row2){
		row[0] = row0; row[1] = row1; row[2] = row2;
	}

	// getting matrix as GL format pointer
	const real* Matrix3::GetRawMatrix() const{
		return reinterpret_cast<const real*>(m);
	}


	// matrix plus operator
	const Matrix3 Matrix3::operator+(const Matrix3& mat3) const{
		return Matrix3(
			m[0][0] + mat3.m[0][0], m[0][1] + mat3.m[0][1], m[0][2] + mat3.m[0][2],
			m[1][0] + mat3.m[1][0], m[1][1] + mat3.m[1][1], m[1][2] + mat3.m[1][2],
			m[2][0] + mat3.m[2][0], m[2][1] + mat3.m[2][1], m[2][2] + mat3.m[2][2]);
	}

	Matrix3& Matrix3::operator+=(const Matrix3& mat3){
		row[0] += mat3.row[0];
		row[1] += mat3.row[1];
		row[2] += mat3.row[2];
		return *this;
	}


	// matrix minus operator
	const Matrix3 Matrix3::operator-(const Matrix3& mat3) const{
		return Matrix3(
			m[0][0] - mat3.m[0][0], m[0][1] - mat3.m[0][1], m[0][2] - mat3.m[0][2],
			m[1][0] - mat3.m[1][0], m[1][1] - mat3.m[1][1], m[1][2] - mat3.m[1][2],
			m[2][0] - mat3.m[2][0], m[2][1] - mat3.m[2][1], m[2][2] - mat3.m[2][2]);
	}

	Matrix3& Matrix3::operator-=(const Matrix3& mat3){
		row[0] -= mat3.row[0];
		row[1] -= mat3.row[1];
		row[2] -= mat3.row[2];
		return *this;
	}

	// matrix multiply operator
	const Matrix3 Matrix3::operator*(const Matrix3& mat3) const{
		return Matrix3(
			m[0][0] * mat3.m[0][0] + m[0][1] * mat3.m[1][0] + m[0][2] * mat3.m[2][0],
			m[0][0] * mat3.m[0][1] + m[0][1] * mat3.m[1][1] + m[0][2] * mat3.m[2][1],
			m[0][0] * mat3.m[0][2] + m[0][1] * mat3.m[1][2] + m[0][2] * mat3.m[2][2],

			m[1][0] * mat3.m[0][0] + m[1][1] * mat3.m[1][0] + m[1][2] * mat3.m[2][0],
			m[1][0] * mat3.m[0][1] + m[1][1] * mat3.m[1][1] + m[1][2] * mat3.m[2][1],
			m[1][0] * mat3.m[0][2] + m[1][1] * mat3.m[1][2] + m[1][2] * mat3.m[2][2],

			m[2][0] * mat3.m[0][0] + m[2][1] * mat3.m[1][0] + m[2][2] * mat3.m[2][0],
			m[2][0] * mat3.m[0][1] + m[2][1] * mat3.m[1][1] + m[2][2] * mat3.m[2][1],
			m[2][0] * mat3.m[0][2] + m[2][1] * mat3.m[1][2] + m[2][2] * mat3.m[2][2]);
	}

	Matrix3& Matrix3::operator*=(const Matrix3& mat3){
		Set(m[0][0] * mat3.m[0][0] + m[0][1] * mat3.m[1][0] + m[0][2] * mat3.m[2][0],
			m[0][0] * mat3.m[0][1] + m[0][1] * mat3.m[1][1] + m[0][2] * mat3.m[2][1],
			m[0][0] * mat3.m[0][2] + m[0][1] * mat3.m[1][2] + m[0][2] * mat3.m[2][2],

			m[1][0] * mat3.m[0][0] + m[1][1] * mat3.m[1][0] + m[1][2] * mat3.m[2][0],
			m[1][0] * mat3.m[0][1] + m[1][1] * mat3.m[1][1] + m[1][2] * mat3.m[2][1],
			m[1][0] * mat3.m[0][2] + m[1][1] * mat3.m[1][2] + m[1][2] * mat3.m[2][2],

			m[2][0] * mat3.m[0][0] + m[2][1] * mat3.m[1][0] + m[2][2] * mat3.m[2][0],
			m[2][0] * mat3.m[0][1] + m[2][1] * mat3.m[1][1] + m[2][2] * mat3.m[2][1],
			m[2][0] * mat3.m[0][2] + m[2][1] * mat3.m[1][2] + m[2][2] * mat3.m[2][2]);
		return *this;
	}

	const Vector3 Matrix3::operator*(const Vector3& vec3) const{
		return Vector3(
			m[0][0] * vec3.x + m[0][1] * vec3.y + m[0][2] * vec3.z,
			m[1][0] * vec3.x + m[1][1] * vec3.y + m[1][2] * vec3.z,
			m[2][0] * vec3.x + m[2][1] * vec3.y + m[2][2] * vec3.z
			);
	}

	const Matrix3 Matrix3::operator*(real fConstant) const{
		return Matrix3(
			m[0][0] * fConstant, m[0][1] * fConstant, m[0][2] * fConstant,
			m[1][0] * fConstant, m[1][1] * fConstant, m[1][2] * fConstant,
			m[2][0] * fConstant, m[2][1] * fConstant, m[2][2] * fConstant);
	}

	Matrix3& Matrix3::operator*=(real fConstant){
		row[0] *= fConstant;
		row[1] *= fConstant;
		row[2] *= fConstant;
		return *this;
	}


	// matrix constant division operator
	const Matrix3 Matrix3::operator / (real fConstant) const{
		ONLYDBG_CODEBLOCK(
		if (fConstant == 0.f)
			LogFatal("division by zero");
		);
		return Matrix3(
			m[0][0] / fConstant, m[0][1] / fConstant, m[0][2] / fConstant,
			m[1][0] / fConstant, m[1][1] / fConstant, m[1][2] / fConstant,
			m[2][0] / fConstant, m[2][1] / fConstant, m[2][2] / fConstant);
	}

	Matrix3& Matrix3::operator/=(real fConstant){
		row[0] /= fConstant;
		row[1] /= fConstant;
		row[2] /= fConstant;
		return *this;
	}


	// check if zero matrix
	bool Matrix3::IsZero() const{
		return (m[0][0] == 0.0f && m[0][1] == 0.0f && m[0][2] == 0.0f
			&& m[1][0] == 0.0f && m[1][1] == 0.0f && m[1][2] == 0.0f
			&& m[2][0] == 0.0f && m[2][1] == 0.0f && m[2][2] == 0.0f);
	}

	// make this to zero matrix
	void Matrix3::MakeZero(){
		memset(m, 0, sizeof(real)* 9);
	}

	// check if identity matrix
	bool Matrix3::IsIdentity() const{
		return (m[0][0] == 1.0f && m[1][1] == 1.0f && m[2][2] == 1.0f);
	}

	// make this to identity matrix
	void Matrix3::MakeIdentity(){
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
	}

	// get transposition of this matrix.
	const Matrix3 Matrix3::Transposition() const{
		return Matrix3(
			m[0][0], m[1][0], m[2][0],
			m[0][1], m[1][1], m[2][1],
			m[0][2], m[1][2], m[2][2]);
	}

	// transpose this matrix
	void Matrix3::Transpose(){
		Set(m[0][0], m[1][0], m[2][0],
			m[0][1], m[1][1], m[2][1],
			m[0][2], m[1][2], m[2][2]);
	}

	// matrix cofactor matrix
	const Matrix3 Matrix3::CofactorMatrix() const{
		return Matrix3(
			m[1][1] * m[2][2] - m[1][2] * m[2][1], m[1][2] * m[2][0] - m[1][0] * m[2][2], m[1][0] * m[2][1] - m[1][1] * m[2][0],
			m[0][2] * m[2][1] - m[0][1] * m[2][2], m[0][0] * m[2][2] - m[0][2] * m[2][0], m[0][1] * m[2][0] - m[0][0] * m[2][1],
			m[0][1] * m[1][2] - m[0][2] * m[1][1], m[0][2] * m[1][0] - m[0][0] * m[1][2], m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	}

	// matrix adjugate(== C^t, but this method is coded hardly)
	const Matrix3 Matrix3::Adjugate() const{
		// return CofactorMatrix().Transpose();
		return Matrix3(
			m[1][1] * m[2][2] - m[1][2] * m[2][1], m[0][2] * m[2][1] - m[0][1] * m[2][2], m[0][1] * m[1][2] - m[0][2] * m[1][1],
			m[1][2] * m[2][0] - m[1][0] * m[2][2], m[0][0] * m[2][2] - m[0][2] * m[2][0], m[0][2] * m[1][0] - m[0][0] * m[1][2],
			m[1][0] * m[2][1] - m[1][1] * m[2][0], m[0][1] * m[2][0] - m[0][0] * m[2][1], m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	}

	// matrix inverse
	const Matrix3 Matrix3::Inverse() const{
		const Matrix3 adjM = this->Adjugate();
		real det = m[0][0] * adjM.m[0][0] + m[0][1] * adjM.m[1][0] + m[0][2] * adjM.m[2][0];

		ONLYDBG_CODEBLOCK(
		if (det == 0.f)
			LogFatal("uninvertible matrix");
		);

		return adjM / det;
	}

	// matrix determination
	real Matrix3::Determin() const{
		return det3x3(
			m[0][0], m[0][1], m[0][2],
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2]);
	}

	// to rotation quaternion
	const Quaternion Matrix3::ToQuaternion() const{
		// implementation from:
		// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
		real tr = m[0][0] + m[1][1] + m[2][2];

		if (tr > 0) {
			real S = math::sqrt(tr + 1.f) * 2.f; // S=4*qw 
			return Quaternion(
				(m[2][1] - m[1][2]) / S,
				(m[0][2] - m[2][0]) / S,
				(m[1][0] - m[0][1]) / S,
				0.25f * S);
		}
		else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2])) {
			real S = math::sqrt(1.f + m[0][0] - m[1][1] - m[2][2]) * 2.f; // S=4*qx 
			return Quaternion(
				0.25f * S,
				(m[0][1] + m[1][0]) / S,
				(m[0][2] + m[2][0]) / S,
				(m[2][1] - m[1][2]) / S);
		}
		else if (m[1][1] > m[2][2]) {
			real S = math::sqrt(1.f + m[1][1] - m[0][0] - m[2][2]) * 2.f; // S=4*qy
			return Quaternion(
				(m[0][1] + m[1][0]) / S,
				0.25f * S,
				(m[1][2] + m[2][1]) / S,
				(m[0][2] - m[2][0]) / S);
		}
		else {
			real S = math::sqrt(1.f + m[2][2] - m[0][0] - m[1][1]) * 2.f; // S=4*qz
			return Quaternion(
				(m[0][2] + m[2][0]) / S,
				(m[1][2] + m[2][1]) / S,
				0.25f * S,
				(m[1][0] - m[0][1]) / S);
		}
	}

	const Matrix3 operator*(real fConstant, const Matrix3& mat3){
		return Matrix3(
			mat3.m[0][0] * fConstant, mat3.m[0][1] * fConstant, mat3.m[0][2] * fConstant,
			mat3.m[1][0] * fConstant, mat3.m[1][1] * fConstant, mat3.m[1][2] * fConstant,
			mat3.m[2][0] * fConstant, mat3.m[2][1] * fConstant, mat3.m[2][2] * fConstant);
	}

	const Matrix3 operator/(real fConstant, const Matrix3& mat3){
		ONLYDBG_CODEBLOCK(
		if (fConstant == 0.f)
			LogFatal("division by zero");
		);
		return Matrix3(
			mat3.m[0][0] / fConstant, mat3.m[0][1] / fConstant, mat3.m[0][2] / fConstant,
			mat3.m[1][0] / fConstant, mat3.m[1][1] / fConstant, mat3.m[1][2] / fConstant,
			mat3.m[2][0] / fConstant, mat3.m[2][1] / fConstant, mat3.m[2][2] / fConstant);
	}



	/**
	Type of 4x4 square matrix defined as row X col order
	*/

	Matrix4::Matrix4(real diagonal){
		memset(m, 0, sizeof(real)* 16);
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = diagonal;
		
	}
	Matrix4::Matrix4(real _00, real _01, real _02, real _03,
		real _10, real _11, real _12, real _13,
		real _20, real _21, real _22, real _23,
		real _30, real _31, real _32, real _33)
	{
		m[0][0] = _00; m[0][1] = _01; m[0][2] = _02; m[0][3] = _03;
		m[1][0] = _10; m[1][1] = _11; m[1][2] = _12; m[1][3] = _13;
		m[2][0] = _20; m[2][1] = _21; m[2][2] = _22; m[2][3] = _23;
		m[3][0] = _30; m[3][1] = _31; m[3][2] = _32; m[3][3] = _33;
	}
	Matrix4::Matrix4(real** mat4){
		memcpy(m, mat4, sizeof(real)* 16);
	}
	Matrix4::Matrix4(const Vector4& row0,
		const Vector4& row1,
		const Vector4& row2,
		const Vector4& row3)
	{
		row[0] = row0; row[1] = row1; row[2] = row2; row[3] = row3;
	}
	Matrix4::Matrix4(const Matrix4& mat4){
		memcpy(m, mat4.m, sizeof(real)* 16);
	}
	Matrix4& Matrix4::operator=(const Matrix4& mat4){
		memcpy(m, mat4.m, sizeof(real)* 16);
		return *this;
	}

	Matrix4::Matrix4(const Matrix3& mat3){
		m[0][0] = mat3.m[0][0]; m[0][1] = mat3.m[0][1]; m[0][2] = mat3.m[0][2]; m[0][3] = 0.0f;
		m[1][0] = mat3.m[1][0]; m[1][1] = mat3.m[1][1]; m[1][2] = mat3.m[1][2]; m[1][3] = 0.0f;
		m[2][0] = mat3.m[2][0]; m[2][1] = mat3.m[2][1]; m[2][2] = mat3.m[2][2]; m[2][3] = 0.0f;
		m[3][0] = mat3.m[3][0]; m[3][1] = mat3.m[3][1]; m[3][2] = mat3.m[3][2]; m[3][3] = 0.0f;
	}

	// set matrix elements
	void Matrix4::Set(real _00, real _01, real _02, real _03,
		real _10, real _11, real _12, real _13,
		real _20, real _21, real _22, real _23,
		real _30, real _31, real _32, real _33)
	{
		m[0][0] = _00; m[0][1] = _01; m[0][2] = _02; m[0][3] = _03;
		m[1][0] = _10; m[1][1] = _11; m[1][2] = _12; m[1][3] = _13;
		m[2][0] = _20; m[2][1] = _21; m[2][2] = _22; m[2][3] = _23;
		m[3][0] = _30; m[3][1] = _31; m[3][2] = _32; m[3][3] = _33;
	}
	// set matrix elements
	void Matrix4::Set(const Vector4& row0,
		const Vector4& row1,
		const Vector4& row2,
		const Vector4& row3)
	{
		row[0] = row0; row[1] = row1; row[2] = row2; row[3] = row3;
	}

	// getting matrix as GL format pointer
	const real* Matrix4::GetRawMatrix() const{
		return reinterpret_cast<const real*>(m);
	}


	// matrix plus operator
	const Matrix4 Matrix4::operator+(const Matrix4& mat4) const{
		return Matrix4(
			m[0][0] + mat4.m[0][0], m[0][1] + mat4.m[0][1], m[0][2] + mat4.m[0][2], m[0][3] + mat4.m[0][3],
			m[1][0] + mat4.m[1][0], m[1][1] + mat4.m[1][1], m[1][2] + mat4.m[1][2], m[1][3] + mat4.m[1][3],
			m[2][0] + mat4.m[2][0], m[2][1] + mat4.m[2][1], m[2][2] + mat4.m[2][2], m[2][3] + mat4.m[2][3],
			m[3][0] + mat4.m[3][0], m[3][1] + mat4.m[3][1], m[3][2] + mat4.m[3][2], m[3][3] + mat4.m[3][3]);
	}

	Matrix4& Matrix4::operator+=(const Matrix4& mat4){
		row[0] += mat4.row[0];
		row[1] += mat4.row[1];
		row[2] += mat4.row[2];
		row[3] += mat4.row[3];
		return *this;
	}


	// matrix minus operator
	const Matrix4 Matrix4::operator-(const Matrix4& mat4) const{
		return Matrix4(
			m[0][0] - mat4.m[0][0], m[0][1] - mat4.m[0][1], m[0][2] - mat4.m[0][2], m[0][3] - mat4.m[0][3],
			m[1][0] - mat4.m[1][0], m[1][1] - mat4.m[1][1], m[1][2] - mat4.m[1][2], m[1][3] - mat4.m[1][3],
			m[2][0] - mat4.m[2][0], m[2][1] - mat4.m[2][1], m[2][2] - mat4.m[2][2], m[2][3] - mat4.m[2][3],
			m[3][0] - mat4.m[3][0], m[3][1] - mat4.m[3][1], m[3][2] - mat4.m[3][2], m[3][3] - mat4.m[3][3]);
	}

	Matrix4& Matrix4::operator-=(const Matrix4& mat4){
		row[0] -= mat4.row[0];
		row[1] -= mat4.row[1];
		row[2] -= mat4.row[2];
		row[3] -= mat4.row[3];
		return *this;
	}


	// matrix multiply operator
	const Matrix4 Matrix4::operator*(const Matrix4& mat4) const{
		return Matrix4(
			m[0][0] * mat4.m[0][0] + m[0][1] * mat4.m[1][0] + m[0][2] * mat4.m[2][0] + m[0][3] * mat4.m[3][0],
			m[0][0] * mat4.m[0][1] + m[0][1] * mat4.m[1][1] + m[0][2] * mat4.m[2][1] + m[0][3] * mat4.m[3][1],
			m[0][0] * mat4.m[0][2] + m[0][1] * mat4.m[1][2] + m[0][2] * mat4.m[2][2] + m[0][3] * mat4.m[3][2],
			m[0][0] * mat4.m[0][3] + m[0][1] * mat4.m[1][3] + m[0][2] * mat4.m[2][3] + m[0][3] * mat4.m[3][3],

			m[1][0] * mat4.m[0][0] + m[1][1] * mat4.m[1][0] + m[1][2] * mat4.m[2][0] + m[1][3] * mat4.m[3][0],
			m[1][0] * mat4.m[0][1] + m[1][1] * mat4.m[1][1] + m[1][2] * mat4.m[2][1] + m[1][3] * mat4.m[3][1],
			m[1][0] * mat4.m[0][2] + m[1][1] * mat4.m[1][2] + m[1][2] * mat4.m[2][2] + m[1][3] * mat4.m[3][2],
			m[1][0] * mat4.m[0][3] + m[1][1] * mat4.m[1][3] + m[1][2] * mat4.m[2][3] + m[1][3] * mat4.m[3][3],

			m[2][0] * mat4.m[0][0] + m[2][1] * mat4.m[1][0] + m[2][2] * mat4.m[2][0] + m[2][3] * mat4.m[3][0],
			m[2][0] * mat4.m[0][1] + m[2][1] * mat4.m[1][1] + m[2][2] * mat4.m[2][1] + m[2][3] * mat4.m[3][1],
			m[2][0] * mat4.m[0][2] + m[2][1] * mat4.m[1][2] + m[2][2] * mat4.m[2][2] + m[2][3] * mat4.m[3][2],
			m[2][0] * mat4.m[0][3] + m[2][1] * mat4.m[1][3] + m[2][2] * mat4.m[2][3] + m[2][3] * mat4.m[3][3],

			m[3][0] * mat4.m[0][0] + m[3][1] * mat4.m[1][0] + m[3][2] * mat4.m[2][0] + m[3][3] * mat4.m[3][0],
			m[3][0] * mat4.m[0][1] + m[3][1] * mat4.m[1][1] + m[3][2] * mat4.m[2][1] + m[3][3] * mat4.m[3][1],
			m[3][0] * mat4.m[0][2] + m[3][1] * mat4.m[1][2] + m[3][2] * mat4.m[2][2] + m[3][3] * mat4.m[3][2],
			m[3][0] * mat4.m[0][3] + m[3][1] * mat4.m[1][3] + m[3][2] * mat4.m[2][3] + m[3][3] * mat4.m[3][3]);
	}

	Matrix4& Matrix4::operator*=(const Matrix4& mat4){
		Set(m[0][0] * mat4.m[0][0] + m[0][1] * mat4.m[1][0] + m[0][2] * mat4.m[2][0] + m[0][3] * mat4.m[3][0],
			m[0][0] * mat4.m[0][1] + m[0][1] * mat4.m[1][1] + m[0][2] * mat4.m[2][1] + m[0][3] * mat4.m[3][1],
			m[0][0] * mat4.m[0][2] + m[0][1] * mat4.m[1][2] + m[0][2] * mat4.m[2][2] + m[0][3] * mat4.m[3][2],
			m[0][0] * mat4.m[0][3] + m[0][1] * mat4.m[1][3] + m[0][2] * mat4.m[2][3] + m[0][3] * mat4.m[3][3],

			m[1][0] * mat4.m[0][0] + m[1][1] * mat4.m[1][0] + m[1][2] * mat4.m[2][0] + m[1][3] * mat4.m[3][0],
			m[1][0] * mat4.m[0][1] + m[1][1] * mat4.m[1][1] + m[1][2] * mat4.m[2][1] + m[1][3] * mat4.m[3][1],
			m[1][0] * mat4.m[0][2] + m[1][1] * mat4.m[1][2] + m[1][2] * mat4.m[2][2] + m[1][3] * mat4.m[3][2],
			m[1][0] * mat4.m[0][3] + m[1][1] * mat4.m[1][3] + m[1][2] * mat4.m[2][3] + m[1][3] * mat4.m[3][3],

			m[2][0] * mat4.m[0][0] + m[2][1] * mat4.m[1][0] + m[2][2] * mat4.m[2][0] + m[2][3] * mat4.m[3][0],
			m[2][0] * mat4.m[0][1] + m[2][1] * mat4.m[1][1] + m[2][2] * mat4.m[2][1] + m[2][3] * mat4.m[3][1],
			m[2][0] * mat4.m[0][2] + m[2][1] * mat4.m[1][2] + m[2][2] * mat4.m[2][2] + m[2][3] * mat4.m[3][2],
			m[2][0] * mat4.m[0][3] + m[2][1] * mat4.m[1][3] + m[2][2] * mat4.m[2][3] + m[2][3] * mat4.m[3][3],

			m[3][0] * mat4.m[0][0] + m[3][1] * mat4.m[1][0] + m[3][2] * mat4.m[2][0] + m[3][3] * mat4.m[3][0],
			m[3][0] * mat4.m[0][1] + m[3][1] * mat4.m[1][1] + m[3][2] * mat4.m[2][1] + m[3][3] * mat4.m[3][1],
			m[3][0] * mat4.m[0][2] + m[3][1] * mat4.m[1][2] + m[3][2] * mat4.m[2][2] + m[3][3] * mat4.m[3][2],
			m[3][0] * mat4.m[0][3] + m[3][1] * mat4.m[1][3] + m[3][2] * mat4.m[2][3] + m[3][3] * mat4.m[3][3]);
		return *this;
	}

	const Vector4 Matrix4::operator*(const Vector4& vec4) const{
		return Vector4(
			m[0][0] * vec4.x + m[0][1] * vec4.y + m[0][2] * vec4.z + m[0][3] * vec4.w,
			m[1][0] * vec4.x + m[1][1] * vec4.y + m[1][2] * vec4.z + m[1][3] * vec4.w,
			m[2][0] * vec4.x + m[2][1] * vec4.y + m[2][2] * vec4.z + m[2][3] * vec4.w,
			m[3][0] * vec4.x + m[3][1] * vec4.y + m[3][2] * vec4.z + m[3][3] * vec4.w
			);
	}

	const Matrix4 Matrix4::operator*(real fConstant) const{
		return Matrix4(
			m[0][0] * fConstant, m[0][1] * fConstant, m[0][2] * fConstant, m[0][3] * fConstant,
			m[1][0] * fConstant, m[1][1] * fConstant, m[1][2] * fConstant, m[1][3] * fConstant,
			m[2][0] * fConstant, m[2][1] * fConstant, m[2][2] * fConstant, m[2][3] * fConstant,
			m[3][0] * fConstant, m[3][1] * fConstant, m[3][2] * fConstant, m[3][3] * fConstant);
	}

	Matrix4& Matrix4::operator*=(real fConstant){
		row[0] *= fConstant;
		row[1] *= fConstant;
		row[2] *= fConstant;
		row[3] *= fConstant;
		return *this;
	}


	// matrix constant division operator
	const Matrix4 Matrix4::operator/(real fConstant) const{
		ONLYDBG_CODEBLOCK(
		if (fConstant == 0.f)
			LogFatal("division by zero");
		);
		return Matrix4(
			m[0][0] / fConstant, m[0][1] / fConstant, m[0][2] / fConstant, m[0][3] / fConstant,
			m[1][0] / fConstant, m[1][1] / fConstant, m[1][2] / fConstant, m[1][3] / fConstant,
			m[2][0] / fConstant, m[2][1] / fConstant, m[2][2] / fConstant, m[2][3] / fConstant,
			m[3][0] / fConstant, m[3][1] / fConstant, m[3][2] / fConstant, m[3][3] / fConstant);
	}

	Matrix4& Matrix4::operator/=(real fConstant){
		row[0] /= fConstant;
		row[1] /= fConstant;
		row[2] /= fConstant;
		row[3] /= fConstant;
		return *this;
	}


	// check if zero matrix
	bool Matrix4::IsZero() const{
		return (m[0][0] == 0.0f && m[0][1] == 0.0f && m[0][2] == 0.0f && m[0][3] == 0.0f
			&& m[1][0] == 0.0f && m[1][1] == 0.0f && m[1][2] == 0.0f && m[1][3] == 0.0f
			&& m[2][0] == 0.0f && m[2][1] == 0.0f && m[2][2] == 0.0f && m[2][3] == 0.0f
			&& m[3][0] == 0.0f && m[3][1] == 0.0f && m[3][2] == 0.0f && m[3][3] == 0.0f);
	}

	// make this to zero matrix
	void Matrix4::MakeZero(){
		memset(m, 0, sizeof(real)* 16);
	}

	// check if identity matrix
	bool Matrix4::IsIdentity() const{
		return (m[0][0] == 1.0f && m[1][1] == 1.0f && m[2][2] == 1.0f && m[3][3] == 1.0f);
	}

	// make this to identity matrix
	void Matrix4::MakeIdentity(){
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}

	// get transposition of this matrix
	const Matrix4 Matrix4::Transposition() const{
		return Matrix4(
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	// transpose this matrix
	void Matrix4::Transpose(){
		Set(m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	// matrix cofactor matrix
	const Matrix4 Matrix4::CofactorMatrix() const{
		return Matrix4(
			det3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]),
			-det3x3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]),
			det3x3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]),
			-det3x3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]),

			-det3x3(m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]),
			det3x3(m[0][0], m[0][2], m[0][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]),
			-det3x3(m[0][0], m[0][1], m[0][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]),
			det3x3(m[0][0], m[0][1], m[0][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]),

			det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3]),
			-det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[3][0], m[3][2], m[3][3]),
			det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[3][0], m[3][1], m[3][3]),
			-det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[3][0], m[3][1], m[3][2]),

			-det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3]),
			det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3]),
			-det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3]),
			det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]));
	}

	// matrix adjugate(== C^t, but this method is coded hardly)
	const Matrix4 Matrix4::Adjugate() const{
		// return CofactorMatrix().Transpose();
		return Matrix4(
			det3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]),
			-det3x3(m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]),
			det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3]),
			-det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3]),

			-det3x3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]),
			det3x3(m[0][0], m[0][2], m[0][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]),
			-det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[3][0], m[3][2], m[3][3]),
			det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3]),

			det3x3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]),
			-det3x3(m[0][0], m[0][1], m[0][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]),
			det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[3][0], m[3][1], m[3][3]),
			-det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3]),

			-det3x3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]),
			det3x3(m[0][0], m[0][1], m[0][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]),
			-det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[3][0], m[3][1], m[3][2]),
			det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]));
	}

	// matrix inverse
	const Matrix4 Matrix4::Inverse() const{
		const Matrix4 adjM = this->Adjugate();
		real det = m[0][0] * adjM.m[0][0] + m[0][1] * adjM.m[1][0] + m[0][2] * adjM.m[2][0] + m[0][3] * adjM.m[3][0];

		ONLYDBG_CODEBLOCK(
		if (det == 0.f)
			LogFatal("uninvertible matrix");
		);

		return adjM / det;
	}

	// matrix determination
	real Matrix4::Determin() const{
		return det4x4(
			m[0][0], m[0][1], m[0][2], m[0][3],
			m[1][0], m[1][1], m[1][2], m[1][3],
			m[2][0], m[2][1], m[2][2], m[2][3],
			m[3][0], m[3][1], m[3][2], m[3][3]);
	}

	const Quaternion Matrix4::ToQuaternion() const{
		// implementation from:
		// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
		real tr = m[0][0] + m[1][1] + m[2][2];

		if (tr > 0) {
			real S = math::sqrt(tr + 1.f) * 2.f; // S=4*qw 
			return Quaternion(
				(m[2][1] - m[1][2]) / S,
				(m[0][2] - m[2][0]) / S,
				(m[1][0] - m[0][1]) / S,
				0.25f * S);
		}
		else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2])) {
			real S = math::sqrt(1.f + m[0][0] - m[1][1] - m[2][2]) * 2.f; // S=4*qx 
			return Quaternion(
				0.25f * S,
				(m[0][1] + m[1][0]) / S,
				(m[0][2] + m[2][0]) / S,
				(m[2][1] - m[1][2]) / S);
		}
		else if (m[1][1] > m[2][2]) {
			real S = math::sqrt(1.f + m[1][1] - m[0][0] - m[2][2]) * 2.f; // S=4*qy
			return Quaternion(
				(m[0][1] + m[1][0]) / S,
				0.25f * S,
				(m[1][2] + m[2][1]) / S,
				(m[0][2] - m[2][0]) / S);
		}
		else {
			real S = math::sqrt(1.f + m[2][2] - m[0][0] - m[1][1]) * 2.f; // S=4*qz
			return Quaternion(
				(m[0][2] + m[2][0]) / S,
				(m[1][2] + m[2][1]) / S,
				0.25f * S,
				(m[1][0] - m[0][1]) / S);
		}
	}

	const Matrix4 operator*(real fConstant, const Matrix4& mat4){
		return Matrix4(
			mat4.m[0][0] * fConstant, mat4.m[0][1] * fConstant, mat4.m[0][2] * fConstant, mat4.m[0][3] * fConstant,
			mat4.m[1][0] * fConstant, mat4.m[1][1] * fConstant, mat4.m[1][2] * fConstant, mat4.m[1][3] * fConstant,
			mat4.m[2][0] * fConstant, mat4.m[2][1] * fConstant, mat4.m[2][2] * fConstant, mat4.m[2][3] * fConstant,
			mat4.m[3][0] * fConstant, mat4.m[3][1] * fConstant, mat4.m[3][2] * fConstant, mat4.m[3][3] * fConstant);
	}

	const Matrix4 operator/(real fConstant, const Matrix4& mat4){
		ONLYDBG_CODEBLOCK(
		if (fConstant == 0.f)
			LogFatal("division by zero");
		);
		return Matrix4(
			mat4.m[0][0] / fConstant, mat4.m[0][1] / fConstant, mat4.m[0][2] / fConstant, mat4.m[0][3] / fConstant,
			mat4.m[1][0] / fConstant, mat4.m[1][1] / fConstant, mat4.m[1][2] / fConstant, mat4.m[1][3] / fConstant,
			mat4.m[2][0] / fConstant, mat4.m[2][1] / fConstant, mat4.m[2][2] / fConstant, mat4.m[2][3] / fConstant,
			mat4.m[3][0] / fConstant, mat4.m[3][1] / fConstant, mat4.m[3][2] / fConstant, mat4.m[3][3] / fConstant);
	}



	/**
	Type of quaternion
	*/

	Quaternion::Quaternion(){
		s = 0.0f;
		x = y = z = 0.0f;
	}
	Quaternion::Quaternion(real _x, real _y, real _z, real _s){
		s = _s;
		x = _x; y = _y; z = _z;
	}

	Quaternion::Quaternion(const Vector3& v, real _s){
		s = _s;
		x = v.x; y = v.y; z = v.z;
	}
	Quaternion::Quaternion(const Vector2& v, real _z, real _s){
		s = _s;
		x = v.x; y = v.y; z = _z;
	}

	Quaternion::Quaternion(const Vector3& rotationAxis, real theta, bool axis_normalized){
		MakeRotatingQuat(rotationAxis, theta, axis_normalized);
	}
	Quaternion::Quaternion(const real roll, real pitch, real yaw){
		MakeRotatingQuat(roll, pitch, yaw);
	}

	Quaternion::Quaternion(const Matrix3& rotMat){
		*this = rotMat.ToQuaternion();
	}
	Quaternion::Quaternion(const Matrix4& rotMat){
		*this = rotMat.ToQuaternion();
	}


	Quaternion::Quaternion(const Quaternion& q){
		s = q.s; x = q.x; y = q.y; z = q.z;
	}
	Quaternion& Quaternion::operator=(const Quaternion& q){
		s = q.s; x = q.x; y = q.y; z = q.z;
		return *this;
	}

	void Quaternion::Set(real _x, real _y, real _z, real _s){
		s = _s;
		x = _x; y = _y; z = _z;
	}


	// is scalar factor zero?
	bool Quaternion::IsPure() const{
		return (s == 0.0f);
	}

	// make this into pure quaternion
	void Quaternion::MakePure(){
		s = 0.0f;
	}

	// quaternion plus operation
	const Quaternion Quaternion::operator+(const Quaternion& q) const{
		return Quaternion(x + q.x, y + q.y, z + q.z, s + q.s);
	}
	Quaternion& Quaternion::operator+=(const Quaternion& q){
		s += q.s;
		x += q.x; y += q.y; z += q.z;
		return *this;
	}

	// quaternion minus operation
	const Quaternion Quaternion::operator-(const Quaternion& q) const{
		return Quaternion(x - q.x, y - q.y, z - q.z, s - q.s);
	}
	Quaternion& Quaternion::operator-=(const Quaternion& q){
		s -= q.s;
		x -= q.x; y -= q.y; z -= q.z;
		return *this;
	}

	// Hamilton product: [s1, v1]*[s2, v2] = [s1*s2 - dot(v1,v2), s1*v2 + s2*v1 + cross(v1,v2)]
	const Quaternion Quaternion::operator*(const Quaternion& q) const{
		return Quaternion(
			s*q.x + x*q.s + y*q.z - z*q.y,
			s*q.y - x*q.z + y*q.s + z*q.x,
			s*q.z + x*q.y - y*q.x + z*q.s,
			s*q.s - x*q.x - y*q.y - z*q.z);
	}
	Quaternion& Quaternion::operator*=(const Quaternion& q){
		Set(s*q.x + x*q.s + y*q.z - z*q.y,
			s*q.y - x*q.z + y*q.s + z*q.x,
			s*q.z + x*q.y - y*q.x + z*q.s,
			s*q.s - x*q.x - y*q.y - z*q.z);
		return *this;
	}

	// get conjugation
	const Quaternion Quaternion::Conjugation() const{
		return Quaternion(-x, -y, -z, s);
	}
	// conjugate this
	void Quaternion::Conjugate(){
		Set(-x, -y, -z, s);
	}

	// magnitude which called 'norm' or 'the tensor of quaternion'
	real Quaternion::Magnitude() const{
		return math::sqrt(math::sqre(s) + math::sqre(x) + math::sqre(y) + math::sqre(z));
	}
	real Quaternion::MagnitudeSq() const{
		return math::sqre(s) + math::sqre(x) + math::sqre(y) + math::sqre(z);
	}

	// get normalized quaternion of this
	const Quaternion Quaternion::Normal() const{
		real mag = Magnitude();
		ONLYDBG_CODEBLOCK(
		if (mag == 0.f)
			LogFatal("division by zero");
		);
		return Quaternion(x / mag, y / mag, z / mag, s / mag);
	}
	// normalize this
	void Quaternion::Normalize(){
		real mag = Magnitude();
		ONLYDBG_CODEBLOCK(
		if (mag == 0.f)
			LogFatal("division by zero");
		);
		s /= mag;
		x /= mag;
		y /= mag;
		z /= mag;
	}

	// inverse, q^{-1} = conj(q)/(norm(q)^2)
	const Quaternion Quaternion::Inverse() const{
		real factor = MagnitudeSq();
		ONLYDBG_CODEBLOCK(
		if (factor == 0.f)
			LogFatal("division by zero");
		);
		return Quaternion(-x / factor, -y / factor, -z / factor, s / factor);
	}

	// make this as the rotating quaternion from given axis vector and theta
	void Quaternion::MakeRotatingQuat(const Vector3& axis, real theta, bool axis_normalized){
		if (axis_normalized)
			v = math::sin(theta / 2.0f) * axis;
		else
			v = math::sin(theta / 2.0f) * axis.Normal();
		s = math::cos(theta / 2.0f);
	}
	// make this as the rotating quaternion from given each axis rotating factor
	// (roll: z, pitch: x, yaw: y axis rotating factor)
	void Quaternion::MakeRotatingQuat(const real roll, real pitch, real yaw){
		real cos_yaw = (yaw == 0.0f ? 1.0f : math::cos(yaw / 2.0f));
		real sin_yaw = (yaw == 0.0f ? 0.0f : math::sin(yaw / 2.0f));
		real cos_pitch = (pitch == 0.0f ? 1.0f : math::cos(pitch / 2.0f));
		real sin_pitch = (pitch == 0.0f ? 0.0f : math::sin(pitch / 2.0f));
		real cos_roll = (roll == 0.0f ? 1.0f : math::cos(roll / 2.0f));
		real sin_roll = (roll == 0.0f ? 0.0f : math::sin(roll / 2.0f));

		Set(cos_yaw*sin_pitch*cos_roll + sin_yaw*cos_pitch*sin_roll,
			sin_yaw*cos_pitch*cos_roll - cos_yaw*sin_pitch*sin_roll,
			cos_yaw*cos_pitch*sin_roll - sin_yaw*sin_pitch*cos_roll,
			cos_yaw*cos_pitch*cos_roll + sin_yaw*sin_pitch*sin_roll);
	}


	// convert quaternion to matrix 3D (only for rotation quaternion)
	const Matrix3 Quaternion::ToMatrix3(bool isNormalized){
		real xx, xy, xz, xs;
		real yy, yz, ys;
		real zz, zs;

		if (isNormalized){
			xx = x*x;
			xy = x*y;
			xz = x*z;
			xs = x*s;

			yy = y*y;
			yz = y*z;
			ys = y*s;

			zz = z*z;
			zs = z*s;
		}
		else{
			Quaternion normq = Normal();
			xx = normq.x*normq.x;
			xy = normq.x*normq.y;
			xz = normq.x*normq.z;
			xs = normq.x*normq.s;

			yy = normq.y*normq.y;
			yz = normq.y*normq.z;
			ys = normq.y*normq.s;

			zz = normq.z*normq.z;
			zs = normq.z*normq.s;
		}

		return Matrix3(
			1.0f - 2.0f*(yy + zz), 2.0f*(xy - zs), 2.0f*(xz + ys),
			2.0f*(xy + zs), 1.0f - 2.0f*(xx + zz), 2.0f*(yz - xs),
			2.0f*(xz - ys), 2.0f*(yz + xs), 1.0f - 2.0f*(xx + yy));
	}

	// convert quaternion to matrix 4D (only for rotation quaternion)
	const Matrix4 Quaternion::ToMatrix4(bool isNormalized){
		real xx, xy, xz, xs;
		real yy, yz, ys;
		real zz, zs;

		if (isNormalized){
			xx = x*x;
			xy = x*y;
			xz = x*z;
			xs = x*s;

			yy = y*y;
			yz = y*z;
			ys = y*s;

			zz = z*z;
			zs = z*s;
		}
		else{
			Quaternion normq = Normal();
			xx = normq.x*normq.x;
			xy = normq.x*normq.y;
			xz = normq.x*normq.z;
			xs = normq.x*normq.s;

			yy = normq.y*normq.y;
			yz = normq.y*normq.z;
			ys = normq.y*normq.s;

			zz = normq.z*normq.z;
			zs = normq.z*normq.s;
		}

		return Matrix4(
			1.0f - 2.0f*(yy + zz), 2.0f*(xy - zs), 2.0f*(xz + ys), 0.0f,
			2.0f*(xy + zs), 1.0f - 2.0f*(xx + zz), 2.0f*(yz - xs), 0.0f,
			2.0f*(xz - ys), 2.0f*(yz + xs), 1.0f - 2.0f*(xx + yy), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	// rotate input vector from given axis vector and theta
	void Quaternion::Rotate(Vector3& v, const Vector3& axis, real theta, bool axis_normalized){
		Quaternion P(v, 0.f);
		Quaternion q;
		q.MakeRotatingQuat(axis, theta, axis_normalized);

		P = (q * P) * q.Conjugation();
		v.Set(P.x, P.y, P.z);
	}

	// rotate input vector from given each axis rotating factor
	// (roll: z, pitch: x, yaw: y axis rotating factor)
	void Quaternion::Rotate(Vector3& v, real roll, real pitch, real yaw){
		Quaternion P(v, 0.f);
		Quaternion q;
		q.MakeRotatingQuat(roll, pitch, yaw);

		P = (q * P) * q.Conjugation();
		v.Set(P.x, P.y, P.z);
	}

}
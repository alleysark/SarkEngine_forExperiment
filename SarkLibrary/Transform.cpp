#include "Transform.h"

namespace sark{

	Transform::Transform(){
		mmatWorld.MakeIdentity();
	}
	Transform::~Transform(){}

	// get transformation matrix
	const Matrix4& Transform::GetMatrix(){
		if (mmatWorld.m[3][3] != 0)
			return mmatWorld;

		mmatWorld = mRotator.ToMatrix4(true);
		mmatWorld.m[0][3] = mPosition.x;
		mmatWorld.m[1][3] = mPosition.y;
		mmatWorld.m[2][3] = mPosition.z;
		mmatWorld.m[3][3] = 1.0f;
		return mmatWorld;
	}

	// get position vector
	const Position3& Transform::GetPosition() const{
		return mPosition;
	}

	// translate into the other position from given vector
	void Transform::Translate(const Position3& position){
		mPosition = position;
		mmatWorld.m[3][3] = 0.0f;
	}
	// translate into the other position from given vector
	void Transform::Translate(real x, real y, real z){
		mPosition.Set(x, y, z);
		mmatWorld.m[3][3] = 0.0f;
	}

	// translate this position additionally
	void Transform::TranslateMore(const Position3& add){
		mPosition += add;
		mmatWorld.m[3][3] = 0.0f;
	}
	// translate this position additionally
	void Transform::TranslateMore(real add_x, real add_y, real add_z){
		mPosition += Position3(add_x, add_y, add_z);
		mmatWorld.m[3][3] = 0.0f;
	}

	// rotate it from given axis and theta
	void Transform::Rotate(const Vector3& axis, real theta){
		mRotator.MakeRotatingQuat(axis, theta);
		mmatWorld.m[3][3] = 0.0f;
	}
	// rotate it from given rotating factor roll(z-axis), pitch(x-axis) and yaw(y-axis)
	void Transform::Rotate(real roll, real pitch, real yaw){
		mRotator.MakeRotatingQuat(roll, pitch, yaw);
		mmatWorld.m[3][3] = 0.0f;
	}

}
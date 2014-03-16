#include "Transform.h"
#include "scenes.h"

namespace sark{

	Transform::Transform(ASceneComponent* reference)
		: mReference(reference)
	{
		mTransformMat.MakeIdentity();
	}
	Transform::~Transform(){}

	// get transformation matrix
	const Matrix4& Transform::GetMatrix(){
		if (mTransformMat.m[3][3] != 0)
			return mTransformMat;

		// recalculate transform matrix only when
		// its transformation has been changed
		mTransformMat = mRotator.ToMatrix4(true);
		mTransformMat.m[0][3] = mPosition.x;
		mTransformMat.m[1][3] = mPosition.y;
		mTransformMat.m[2][3] = mPosition.z;
		return mTransformMat;
	}

	// get position vector
	const Position3& Transform::GetPosition() const{
		return mPosition;
	}

	// translate into the other position from given vector
	void Transform::Translate(const Position3& position){
		mPosition = position;
		TransformStained();
	}
	// translate into the other position from given vector
	void Transform::Translate(real x, real y, real z){
		mPosition.Set(x, y, z);
		TransformStained();
	}

	// translate this position additionally
	void Transform::TranslateMore(const Position3& add){
		mPosition += add;
		TransformStained();
	}
	// translate this position additionally
	void Transform::TranslateMore(real add_x, real add_y, real add_z){
		mPosition += Position3(add_x, add_y, add_z);
		TransformStained();
	}

	// rotate it from given axis and theta
	void Transform::Rotate(const Vector3& axis, real theta){
		mRotator.MakeRotatingQuat(axis, theta);
		TransformStained();
	}
	// rotate it from given rotating factor roll(z-axis), pitch(x-axis) and yaw(y-axis)
	void Transform::Rotate(real roll, real pitch, real yaw){
		mRotator.MakeRotatingQuat(roll, pitch, yaw);
		TransformStained();
	}


	// all the methods of Transform class have to call this function
	// when the properties (translation and rotator quaternion) are changed.
	// if there is reference scene component, it sets the changed flag of absolute 
	// transform of reference scene component.
	void Transform::TransformStained(){
		mTransformMat.m[3][3] = 0;
		if (mReference != NULL){
			mReference->TransformStained();
		}
	}
}
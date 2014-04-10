#include "Transform.h"
#include "ASceneComponent.h"

namespace sark{

	Transform::Transform(ASceneComponent* reference)
		: mReference(reference), mTranslation(0), mRotator(0, 0, 0, 1), mScale(1.f),
		mLocalTM(true), mAbsoluteTM(true)
	{}
	Transform::~Transform(){}

	// get absolute transformation matrix
	const Matrix4& Transform::GetMatrix(){
		if (IS_STAINED(mAbsoluteTM)){
			// recalculate absolute transform matrix only when
			// its or ancestor's transformation has been changed
			if (mReference->mParent == NULL){
				mAbsoluteTM = GetLocalMatrix();
			}
			else{
				mAbsoluteTM
					= mReference->mParent->mTransform.GetMatrix() * GetLocalMatrix();
			}
		}
		return mAbsoluteTM;
	}

	// get local transformation matrix
	const Matrix4& Transform::GetLocalMatrix(){
		if (IS_STAINED(mLocalTM)){
			// recalculate transform matrix only when
			// its transformation has been changed

			// transformed v = TRS * v
			// (T*R) is same as MatR with setting translation factors
			mLocalTM = mRotator.ToMatrix4(true);
			mLocalTM.m[0][3] = mTranslation.x;
			mLocalTM.m[1][3] = mTranslation.y;
			mLocalTM.m[2][3] = mTranslation.z;

			if (mScale.x!=1 || mScale.y!=1 || mScale.z!=1){
				// mLocalTM = (T*R) * sMat
				for (integer i = 0; i < 3; i++){
					mLocalTM.row[i].x *= mScale.x;
					mLocalTM.row[i].y *= mScale.y;
					mLocalTM.row[i].z *= mScale.z;
				}
			}
		}
		return mLocalTM;
	}

	// get world space position
	const Position3 Transform::GetPosition(){
		const Matrix4& myAbsMat = GetMatrix();
		return Position3(myAbsMat.m[0][3], myAbsMat.m[1][3], myAbsMat.m[2][3]);
	}

	// get world space direction.
	// formal direction vector is Vector3::Forward
	const Vector3 Transform::GetDirection(){
		const Matrix4& myAbsMat = GetMatrix();

		if (myAbsMat.IsIdentity()){
			return Vector3::Forward;
		}
		else{
			// getting rotation factor
			Matrix3 rotMat(myAbsMat.row[0].xyz, myAbsMat.row[1].xyz, myAbsMat.row[2].xyz);
			return rotMat * Vector3::Forward;
		}
	}

	// get local position (translation) 
	const Position3& Transform::GetLocalPosition() const{
		return mTranslation;
	}

	// get local rotator
	const Quaternion& Transform::GetLocalRotation() const{
		return mRotator;
	}

	// get local scaling
	const Vector3& Transform::GetLocalScale() const{
		return mScale;
	}


	// translate into the other position from given vector
	void Transform::Translate(const Position3& position){
		mTranslation = position;
		TransformStained();
	}
	// translate into the other position from given vector
	void Transform::Translate(real x, real y, real z){
		mTranslation.Set(x, y, z);
		TransformStained();
	}

	// translate this position additionally
	void Transform::TranslateMore(const Position3& add){
		mTranslation += add;
		TransformStained();
	}
	// translate this position additionally
	void Transform::TranslateMore(real add_x, real add_y, real add_z){
		mTranslation += Position3(add_x, add_y, add_z);
		TransformStained();
	}

	// rotate it from given axis and theta
	void Transform::Rotate(const Vector3& axis, real theta, bool axis_normalized){
		mRotator.MakeRotatingQuat(axis, theta, axis_normalized);
		TransformStained();
	}
	// rotate it from given rotating factor roll(z-axis), pitch(x-axis) and yaw(y-axis)
	void Transform::Rotate(real roll, real pitch, real yaw){
		mRotator.MakeRotatingQuat(roll, pitch, yaw);
		TransformStained();
	}

	// rotate it additionally from given axis and theta
	void Transform::RotateMore(const Vector3& axis, real theta, bool axis_normalized){
		Quaternion q;
		q.MakeRotatingQuat(axis, theta, axis_normalized);
		mRotator *= q;
		TransformStained();
	}
	// rotate it additionally from given rotating factor roll(z-axis), pitch(x-axis) and yaw(y-axis)
	void Transform::RotateMore(real roll, real pitch, real yaw){
		Quaternion q;
		q.MakeRotatingQuat(roll, pitch, yaw);
		mRotator *= q;
		TransformStained();
	}

	// scale it by given scaling factor
	void Transform::Scale(const Vector3& scaleAs){
		mScale = scaleAs;
		TransformStained();
	}
	// scale it by given scaling factor
	void Transform::Scale(real sx, real sy, real sz){
		mScale.Set(sx, sy, sz);
		TransformStained();
	}


	// all the methods of Transform class have to call this function
	// when the properties (translation and rotator quaternion) are changed.
	// if there is reference scene component, it sets the changed flag of absolute 
	// transform of reference scene component.
	// it also sets the 'is transformed' factors of reference scene component
	// and its offspring's.
	void Transform::TransformStained(bool callOnLocal){
		if (callOnLocal){
			STAIN_TRANSMATRIX(mLocalTM);
			mReference->mIsTransformed = true;
		}
		STAIN_TRANSMATRIX(mAbsoluteTM);

		if (mReference != NULL){
			ASceneComponent::ChildComponentContainer::iterator itr
				= mReference->GetChildren().begin();
			ASceneComponent::ChildComponentContainer::iterator end
				= mReference->GetChildren().end();

			for (; itr != end; itr++){
				(*itr)->mIsTransformed = true;
				(*itr)->mTransform.TransformStained(false);
			}
		}
	}
}
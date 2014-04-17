#include "Transform.h"
#include "ASceneComponent.h"

namespace sark{

	Transform::Transform(ASceneComponent* reference)
		: mReference(reference), mTranslation(0), mRotator(0, 0, 0, 1), mScale(1.f),
		mLocalTM(true), mAbsoluteTM(true), mAbsolutePosition(0), mAbsoluteDirection(Vector3::Forward)
	{}
	Transform::~Transform(){}

	// get absolute transformation matrix
	const Matrix4& Transform::GetMatrix(){
		UpdateTransform();
		return mAbsoluteTM;
	}

	// get local transformation matrix
	const Matrix4& Transform::GetLocalMatrix(){
		UpdateTransform();
		return mLocalTM;
	}

	// get world space position
	const Position3& Transform::GetPosition(){
		UpdateTransform();
		return mAbsolutePosition;
	}

	// get world space direction.
	// formal direction vector is Vector3::Forward
	const Vector3& Transform::GetDirection(){
		UpdateTransform();
		return mAbsoluteDirection;
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
		}
		STAIN_TRANSMATRIX(mAbsoluteTM);

		if (mReference != NULL){
			ASceneComponent::ChildComponentContainer::iterator itr
				= mReference->GetChildren().begin();
			ASceneComponent::ChildComponentContainer::iterator end
				= mReference->GetChildren().end();

			for (; itr != end; itr++){
				(*itr)->mTransform.TransformStained(false);
			}
		}
	}

	// apply whole transformation factors onto its matrices.
	void Transform::UpdateTransform(){
		bool isUpdated = false;

		if (IS_STAINED(mLocalTM)){
			isUpdated = true;
			// recalculate transform matrix only when
			// its transformation has been changed

			// transformed v = TRS * v
			// (T*R) is same as MatR with setting translation factors
			mLocalTM = mRotator.ToMatrix4(true);
			mLocalTM.m[0][3] = mTranslation.x;
			mLocalTM.m[1][3] = mTranslation.y;
			mLocalTM.m[2][3] = mTranslation.z;

			if (mScale.x != 1 || mScale.y != 1 || mScale.z != 1){
				// mLocalTM = (T*R) * sMat
				for (integer i = 0; i < 3; i++){
					mLocalTM.row[i].x *= mScale.x;
					mLocalTM.row[i].y *= mScale.y;
					mLocalTM.row[i].z *= mScale.z;
				}
			}
		}

		if (IS_STAINED(mAbsoluteTM)){
			isUpdated = true;
			// recalculate absolute transform matrix only when
			// its or ancestor's transformation has been changed
			if (mReference == NULL || mReference->mParent == NULL){
				mAbsoluteTM = mLocalTM;
			}
			else{
				mAbsoluteTM
					= mReference->mParent->mTransform.GetMatrix() * mLocalTM;
			}
		}

		if (isUpdated){
			// update absolute position
			mAbsolutePosition.Set(mAbsoluteTM.m[0][3], mAbsoluteTM.m[1][3], mAbsoluteTM.m[2][3]);

			// updated absolute direction = [rot matrix of absMat]xVector3::Forward
			//mAbsoluteDirection.Set(
			//	mAbsoluteTM.row[0].xyz.Dot(Vector3::Forward),
			//	mAbsoluteTM.row[1].xyz.Dot(Vector3::Forward),
			//	mAbsoluteTM.row[2].xyz.Dot(Vector3::Forward));

			// but Vector3::Forward can be assumed that value is (0,0,-1),
			// updated absolute direction should be like below.
			mAbsoluteDirection.Set(-mAbsoluteTM.m[0][2], -mAbsoluteTM.m[1][2], -mAbsoluteTM.m[2][2]);
		}
	}
}
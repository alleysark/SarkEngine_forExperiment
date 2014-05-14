#include "Transform.h"
#include "ASceneComponent.h"

namespace sark{

	Transform::Transform(ASceneComponent* reference)
		: mReference(reference), mRotator(0, 0, 0, 1), mScale(1.f),
		mLocalTM(true), mAbsoluteTM(true)
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
	const Position3 Transform::GetPosition(){
		UpdateTransform();
		return Position3(mAbsoluteTM.m[0][3], mAbsoluteTM.m[1][3], mAbsoluteTM.m[2][3]);
	}

	// get world space direction.
	// formal direction vector is Vector3::Forward
	// *note: if this and its ancestors scale themselves,
	// the returned direction cannot guarantee the correctness.
	const Vector3 Transform::GetDirection(){
		UpdateTransform();
		// updated absolute direction = [rot matrix of absMat]xVector3::Forward
		//mAbsoluteDirection.Set(
		//	mAbsoluteTM.row[0].xyz.Dot(Vector3::Forward),
		//	mAbsoluteTM.row[1].xyz.Dot(Vector3::Forward),
		//	mAbsoluteTM.row[2].xyz.Dot(Vector3::Forward));
		// but Vector3::Forward can be assumed that value is (0,0,-1),
		// so absolute direction should be like below.
		return Vector3(-mAbsoluteTM.m[0][2], -mAbsoluteTM.m[1][2], -mAbsoluteTM.m[2][2]);
	}

	// get world space rotation.
	// *note: if this and its ancestors scale themselves,
	// the returned rotation quaternion cannot guarantee
	// the correctness.
	const Quaternion Transform::GetRotation(){
		UpdateTransform();
		return mAbsoluteTM.ToQuaternion();
	}

	// get local position (translation) 
	const Position3 Transform::GetLocalPosition() const{
		return Position3(mLocalTM.m[0][3], mLocalTM.m[1][3], mLocalTM.m[2][3]);
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
		mLocalTM.m[0][3] = position.x;
		mLocalTM.m[1][3] = position.y;
		mLocalTM.m[2][3] = position.z;
		TransformStained(false);
	}
	// translate into the other position from given vector
	void Transform::Translate(real x, real y, real z){
		mLocalTM.m[0][3] = x;
		mLocalTM.m[1][3] = y;
		mLocalTM.m[2][3] = z;
		TransformStained(false);
	}

	// translate this position additionally
	void Transform::TranslateMore(const Position3& add){
		mLocalTM.m[0][3] += add.x;
		mLocalTM.m[1][3] += add.y;
		mLocalTM.m[2][3] += add.z;
		TransformStained(false);
	}
	// translate this position additionally
	void Transform::TranslateMore(real add_x, real add_y, real add_z){
		mLocalTM.m[0][3] += add_x;
		mLocalTM.m[1][3] += add_y;
		mLocalTM.m[2][3] += add_z;
		TransformStained(false);
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
	void Transform::TransformStained(bool stainLocal){
		if (stainLocal){
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
		if (IS_STAINED(mLocalTM)){
			// recalculate transform matrix only when
			// its transformation has been changed

			// transformed v = TRS * v
			Matrix3 matRot = mRotator.ToMatrix3(true);
			mLocalTM.row[0].xyz = matRot.row[0];
			mLocalTM.row[1].xyz = matRot.row[1];
			mLocalTM.row[2].xyz = matRot.row[2];

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
	}
}
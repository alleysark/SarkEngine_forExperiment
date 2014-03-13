#include "Camera.h"

namespace sarklib{

	Camera::Camera() :mEye(0), mLookat(0.f, 0.f, 1.f), mUp(0.f, 1.f, 0.f){
		MakeMatrix();
	}

	Camera::Camera(const Vector3& eye, const Vector3& lookat, const Vector3& up)
		: mEye(eye), mLookat(lookat), mUp(up){
		MakeMatrix();
	}

	Camera::~Camera(){}


	// make view matrix from camera properties(eye,at,up)
	void Camera::MakeMatrix(){
		Vector3 direction = mEye - mLookat;

		Vector3& u = mmatView.row[0].xyz;
		u = mUp.Cross(direction);
		u.Normalize();

		Vector3& v = mmatView.row[1].xyz;
		v = direction.Cross(u);
		v.Normalize();

		Vector3& n = mmatView.row[2].xyz;
		n = u.Cross(v);
	}

	// get view transformation matrix
	const Matrix4& Camera::GetMatrix(){
		return mmatView;
	}

	// get u-axis basis of view space (it'll be the x-axis). u is the right-direction
	const Vector3& Camera::GetBasisU(){
		return mmatView.row[0].xyz;
	}
	// get v-axis basis of view space (it'll be the y-axis). v is the up-direction
	const Vector3& Camera::GetBasisV(){
		return mmatView.row[1].xyz;
	}
	// get n-axis basis of view space (it'll be the z-axis). n is the opposite view-direction
	const Vector3& Camera::GetBasisN(){
		return mmatView.row[2].xyz;
	}

	// get eye position
	const Position3& Camera::GetEye() const{
		return mEye;
	}
	// get lookat position
	const Position3& Camera::GetLookat() const{
		return mLookat;
	}
	// get up vector
	const Vector3& Camera::GetUp() const{
		return mUp;
	}

	// set camera
	void Camera::Set(const Position3& eye, const Position3& lookat, const Vector3& up){
		mEye = eye;
		mLookat = lookat;
		mUp = up;
		MakeMatrix();
	}
	// set eye position
	void Camera::SetEye(const Position3& eye){
		mEye = eye;
		MakeMatrix();
	}
	// set lookat position
	void Camera::SetLookat(const Position3& lookat){
		mLookat = lookat;
		MakeMatrix();
	}
	// set up vector
	void Camera::SetUp(const Vector3& up){
		mUp = up;
		MakeMatrix();
	}


	// basic functions

	// move camera into forward direction (positive distance goes to forward)
	void Camera::MoveForward(real distance){
		mEye = mEye - distance*mmatView.row[2].xyz;
		mLookat = mLookat - distance*mmatView.row[2].xyz;

		MakeMatrix();
	}

	// move camera into sideward direction (positive distance goes to right)
	void Camera::MoveSideward(real distance){
		mEye = mEye + distance*mmatView.row[0].xyz;
		mLookat = mLookat + distance*mmatView.row[0].xyz;

		MakeMatrix();
	}

	// move camera into upward direction (positive distance goes to up)
	void Camera::MoveUpward(real distance){
		mEye = mEye + distance*mmatView.row[1].xyz;
		mLookat = mLookat + distance*mmatView.row[1].xyz;

		MakeMatrix();
	}

	// turn camera on an axis of u (likes x)
	// it nod its sight (positive radian makes it to see up)
	void Camera::Pitch(real rad){
		Vector3 lookDirection = mLookat - mEye;
		Quaternion::Rotate(lookDirection, mmatView.row[0].xyz, rad);
		mLookat = mEye + lookDirection;

		MakeMatrix();
	}

	// turn camera on an axis of v (likes y)
	// it shakes its sight (positive radian makes it to see left)
	void Camera::Yaw(real rad){
		Vector3 lookDirection = mLookat - mEye;
		Quaternion::Rotate(lookDirection, mmatView.row[1].xyz, rad);
		mLookat = mEye + lookDirection;

		MakeMatrix();
	}

	// turn camera on an axis of n (likes z)
	// it tilt its sight (positive radian makes it to see left-tilting)
	void Camera::Roll(real rad){
		Quaternion::Rotate(mmatView.row[0].xyz, mmatView.row[2].xyz, rad);
		Quaternion::Rotate(mmatView.row[1].xyz, mmatView.row[2].xyz, rad);
	}

}
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core.h"

namespace sark{

	// camera defines the most basic functionality and properties
	// it can be extended by inheritance
	// camera space is defined by u,v,n orthonormal non-standard basis
	// they are extracted from given eye, at, up vectors
	class Camera{
	protected:
		Position3 mEye;	//origin of camera space
		Position3 mLookat;	//focusing target
		Vector3 mUp;	//camera's standing axis

		Matrix4 mViewMatrix;

	public:
		Camera();
		Camera(const Vector3& eye, const Vector3& lookat, const Vector3& up = Vector3(0.f, 1.f, 0.f));
		virtual ~Camera();

	private:
		// make view matrix from camera properties(eye,at,up)
		void MakeMatrix();

	public:
		// get view transformation matrix
		const Matrix4& GetViewMatrix();

		// get u-axis basis of view space (it'll be the x-axis). u is the right-direction
		const Vector3& GetBasisU();
		// get v-axis basis of view space (it'll be the y-axis). v is the up-direction
		const Vector3& GetBasisV();
		// get n-axis basis of view space (it'll be the z-axis). n is the opposite view-direction
		const Vector3& GetBasisN();

		// get eye position
		const Position3& GetEye() const;
		// get lookat position
		const Position3& GetLookat() const;
		// get up vector
		const Vector3& GetUp() const;

		// set camera
		void Set(const Position3& eye, const Position3& lookat, const Vector3& up = Vector3(0.f, 1.f, 0.f));
		// set eye position
		void SetEye(const Position3& eye);
		// set lookat position
		void SetLookat(const Position3& lookat);
		// set up vector
		void SetUp(const Vector3& up = Vector3(0.f, 1.f, 0.f));


		// basic functions

		// move camera into forward direction (positive distance goes to forward)
		void MoveForward(real distance);

		// move camera into sideward direction (positive distance goes to right)
		void MoveSideward(real distance);

		// move camera into upward direction (positive distance goes to up)
		void MoveUpward(real distance);

		// turn camera on an axis of u (likes x)
		// it nod its sight (positive radian makes it to see up)
		void Pitch(real rad);

		// turn camera on an axis of v (likes y)
		// it shakes its sight (positive radian makes it to see left)
		void Yaw(real rad);

		// turn camera on an axis of n (likes z)
		// it tilt its sight (positive radian makes it to see left-tilting)
		void Roll(real rad);
		
	};

}
#endif
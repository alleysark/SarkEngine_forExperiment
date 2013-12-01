#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "types.h"

namespace sarklib{

	class Camera{
	private:
		Vector3 mEye;
		Vector3 mLookat;
		Vector3 mUp;

	public:
		Camera(){ }
		Camera(const Vector3& eye, const Vector3& lookat, const Vector3& up)
			: mEye(eye), mLookat(lookat), mUp(up)
		{ }
		Camera(float eyeX, float eyeY, float eyeZ,
			float lookatX, float lookatY, float lookatZ,
			float upX, float upY, float upZ)
			: mEye(eyeX, eyeY, eyeZ), mLookat(lookatX, lookatY, lookatZ), mUp(upX, upY, upZ)
		{ }
		Camera(const Camera& cam){
			mEye = cam.mEye;
			mLookat = cam.mLookat;
			mUp = cam.mUp;
		}
		void operator=(const Camera& cam){
			mEye = cam.mEye;
			mLookat = cam.mLookat;
			mUp = cam.mUp;
		}

		// accessing methods
		const Vector3& GetEye(){ return mEye; }
		void SetEye(float eyeX, float eyeY, float eyeZ){
			mEye.x=eyeX; mEye.y=eyeY; mEye.z=eyeZ;
		}
		void SetEye(const Vector3& eye){
			mEye = eye;
		}

		const Vector3& GetLookat(){ return mLookat; }
		void SetLookat(float lookatX, float lookatY, float lookatZ){
			mLookat.x=lookatX; mLookat.y=lookatY; mLookat.z=lookatZ;
		}
		void SetLookat(const Vector3& lookat){
			mLookat = lookat;
		}

		const Vector3& GetUp(){ return mUp; }
		void SetUp(float upX, float upY, float upZ){
			mUp.x=upX; mUp.y=upY; mUp.z=upZ;
		}
		void SetUp(const Vector3& up){
			mUp = up;
		}

	};

}
#endif
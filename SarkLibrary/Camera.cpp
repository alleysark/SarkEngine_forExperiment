#include "Camera.h"
#include <GL/glew.h>

namespace sark{

	//=============================================
	//		Camera::Viewport class implementation
	//=============================================

	Camera::Viewport::Viewport()
		: x(0.f), y(0.f), width(0.f), height(0.f), minZ(0.f), maxZ(1.f)
	{}


	//=========================================================
	//			Camera::ViewVolume class implementation
	// 
	// * implementation reference page:
	// * http://www.songho.ca/opengl/gl_projectionmatrix.html
	//=========================================================

	Camera::ViewVolume::ViewVolume(real _width, real _height, real _depth){
		SetOrthographic(_width, _height, _depth);
	}
	Camera::ViewVolume::ViewVolume(real _fovy, real _aspect, real _znear, real _zfar){
		SetPerspective(_fovy, _aspect, _znear, _zfar);
	}

	Camera::ViewVolume::~ViewVolume(){}


	// set view as orthographically
	void Camera::ViewVolume::SetOrthographic(real _width, real _height, real _depth){
		ortho = true;

		height = _height / 2.f;
		width = _width / 2.f;
		zNear = 0.f;
		zFar = _depth;

		// calculate projection matrix
		projMatrix.MakeZero();
		projMatrix.m[0][0] = 1.f / width;
		projMatrix.m[1][1] = 1.f / height;
		projMatrix.m[2][2] = -2.f / (zFar - zNear);
		projMatrix.m[2][3] = -(zFar + zNear) / (zFar - zNear);
		projMatrix.m[3][3] = 1.f;
	}

	// set view as perspectively
	void Camera::ViewVolume::SetPerspective(real _fovy, real _aspect, real _znear, real _zfar){
		ortho = false;

		height = math::tan(math::deg2rad(_fovy / 2.f)) * _znear;
		width = height * _aspect;
		zNear = _znear;
		zFar = _zfar;


		// calculate projection matrix
		projMatrix.MakeZero();
		projMatrix.m[0][0] = zNear / width;
		projMatrix.m[1][1] = zNear / height;
		projMatrix.m[2][2] = -(zNear + zFar) / (zFar - zNear);
		projMatrix.m[2][3] = -(2.f * zFar * zNear) / (zFar - zNear);
		projMatrix.m[3][2] = -1.f;
	}



	//=============================================
	//		Camera class implementation
	//=============================================

	Camera::Camera()
		: mVolume(90.f, 1.f, 0.1f, 100.f),
		mEye(0.f), mLookat(0.f, 0.f, -1.f), mUp(0.f, 1.f, 0.f)
	{
		mViewMatrix.MakeIdentity();
	}

	Camera::Camera(const Vector3& eye, const Vector3& lookat, const Vector3& up)
		: mVolume(90.f, 1.f, 0.1f, 100.f),
		mEye(eye), mLookat(lookat), mUp(up)
	{
		UpdateViewMatrix();
	}

	Camera::~Camera(){}


	// make view matrix from camera properties(eye,at,up)
	void Camera::UpdateViewMatrix(){
		Vector3& n = mViewMatrix.row[2].xyz;
		n = mEye - mLookat;
		n.Normalize();

		Vector3& u = mViewMatrix.row[0].xyz;
		u = mUp.Cross(n);
		u.Normalize();

		Vector3& v = mViewMatrix.row[1].xyz;
		v = n.Cross(u);
		v.Normalize();

		mViewMatrix.m[0][3] = -mEye.Dot(u);
		mViewMatrix.m[1][3] = -mEye.Dot(v);
		mViewMatrix.m[2][3] = -mEye.Dot(n);
		mViewMatrix.m[3][3] = 1.f;
	}

	// get view transformation matrix
	const Matrix4& Camera::GetViewMatrix(){
		return mViewMatrix;
	}

	// get projection transformation matrix
	const Matrix4& Camera::GetProjMatrix(){
		return mVolume.projMatrix;
	}


	// is this view orthographic view?
	bool Camera::IsOrthoView() const{
		return mVolume.ortho;
	}

	// make camera view volume as orthographic-view.
	// it defines cube-like view volume
	// which range of [-width/2, width/2] for x, [-height/2, height/2] for y
	// and [0, depth] for z.
	void Camera::Orthographic(real width, real height, real depth){
		mVolume.SetOrthographic(width, height, depth);
	}

	// make camera view volume as perspective-view.
	// it defines truncated quadrangular pyramid volume called 'frustum'.
	// 
	// fovy is Field Of View as degree,
	// aspect is the ratio of width of height (width/height),
	// znear is positive distance from cop to nearest plane,
	// zfar is positive distance from cop to farthest plane.
	void Camera::Perspective(real fovy, real aspect, real znear, real zfar){
		mVolume.SetPerspective(fovy, aspect, znear, zfar);
	}


	// get viewport
	const Camera::Viewport& Camera::GetViewport() const{
		return mViewport;
	}
	// set viewport
	void Camera::SetViewport(integer x, integer y, integer width, integer height){
		glViewport(x, y, width, height);

		mViewport.x = (real)x;
		mViewport.y = (real)y;
		mViewport.width = (real)width;
		mViewport.height = (real)height;
	}
	// set viewport with depth range.
	void Camera::SetViewport(integer x, integer y, integer width, integer height, real minz, real maxz){
		glViewport(x, y, width, height);
		glDepthRange(minz, maxz);

		mViewport.x = (real)x;
		mViewport.y = (real)y;
		mViewport.width = (real)width;
		mViewport.height = (real)height;
		mViewport.minZ = minz;
		mViewport.maxZ = maxz;
	}

	// get view volume
	const Camera::ViewVolume& Camera::GetViewVolume() const{
		return mVolume;
	}


	// get u-axis basis of view space (it'll be the x-axis). u is the right-direction
	const Vector3& Camera::GetBasisU(){
		return mViewMatrix.row[0].xyz;
	}
	// get v-axis basis of view space (it'll be the y-axis). v is the up-direction
	const Vector3& Camera::GetBasisV(){
		return mViewMatrix.row[1].xyz;
	}
	// get n-axis basis of view space (it'll be the z-axis). n is the opposite view-direction
	const Vector3& Camera::GetBasisN(){
		return mViewMatrix.row[2].xyz;
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
		UpdateViewMatrix();
	}
	// set eye position
	void Camera::SetEye(const Position3& eye){
		mEye = eye;
		UpdateViewMatrix();
	}
	// set lookat position
	void Camera::SetLookat(const Position3& lookat){
		mLookat = lookat;
		UpdateViewMatrix();
	}
	// set up vector
	void Camera::SetUp(const Vector3& up){
		mUp = up;
		UpdateViewMatrix();
	}


	// generate ray from screen coordinates
	const Ray Camera::ScreenToWorldRay(const Position2& screenCoord){
		// from viewport transform,
		// projection coord(px, py) can be calculated.
		real px = (2.f / mViewport.width) * (screenCoord.x - mViewport.x) - 1.f;
		real py = -(2.f / mViewport.height) * (screenCoord.y - mViewport.y) + 1.f;

		real cx, cy; // coord in camera near plane
		if (mVolume.ortho){
		}
		else{
			// cx = px * (zn*aspectr/cot(fovy/2))
			// cy = py * (zn/cot(fovy/2))
			cx = px * mVolume.zNear / mVolume.projMatrix.m[0][0];
			cy = py * mVolume.zNear / mVolume.projMatrix.m[1][1];
		}

		// W = inv(VT) x C
		//   = inv(R x T) x C
		//   = inv(T) x inv(R) x C
		//   = inv(T) x transp(R) x C.
		// and inv(T) is translation into Eye
		const Vector3& u = GetBasisU();
		const Vector3& v = GetBasisV();
		const Vector3& n = GetBasisN();
		Matrix4 invVT(
			u.x, v.x, n.x, mEye.x,
			u.y, v.y, n.y, mEye.y,
			u.z, v.z, n.z, mEye.z,
			0.f, 0.f, 0.f, 1.f);
		Vector4 worldCoord = invVT * Vector4(cx, cy, -mVolume.zNear, 1);
		return Ray(worldCoord.xyz, (worldCoord.xyz - mEye));
	}


	// basic functions

	// move camera into forward direction (positive distance goes to forward)
	void Camera::MoveForward(real distance){
		mEye = mEye - distance*mViewMatrix.row[2].xyz;
		mLookat = mLookat - distance*mViewMatrix.row[2].xyz;

		UpdateViewMatrix();
	}

	// move camera into sideward direction (positive distance goes to right)
	void Camera::MoveSideward(real distance){
		mEye = mEye + distance*mViewMatrix.row[0].xyz;
		mLookat = mLookat + distance*mViewMatrix.row[0].xyz;

		UpdateViewMatrix();
	}

	// move camera into upward direction (positive distance goes to up)
	void Camera::MoveUpward(real distance){
		mEye = mEye + distance*mViewMatrix.row[1].xyz;
		mLookat = mLookat + distance*mViewMatrix.row[1].xyz;

		UpdateViewMatrix();
	}

	// turn camera on an axis of u (likes x)
	// it nod its sight (positive radian makes it to see up)
	void Camera::Pitch(real rad){
		Vector3 lookDirection = mLookat - mEye;
		Quaternion::Rotate(lookDirection, mViewMatrix.row[0].xyz, rad);
		Quaternion::Rotate(mUp, mViewMatrix.row[0].xyz, rad);
		mLookat = mEye + lookDirection;

		UpdateViewMatrix();
	}

	// turn camera on an axis of v (likes y)
	// it shakes its sight (positive radian makes it to see left)
	void Camera::Yaw(real rad){
		Vector3 lookDirection = mLookat - mEye;
		Quaternion::Rotate(lookDirection, mViewMatrix.row[1].xyz, rad);
		mLookat = mEye + lookDirection;

		UpdateViewMatrix();
	}

	// turn camera on an axis of n (likes z)
	// it tilt its sight (positive radian makes it to see left-tilting)
	void Camera::Roll(real rad){
		Quaternion::Rotate(mUp, mViewMatrix.row[2].xyz, rad);
		UpdateViewMatrix();
	}

}
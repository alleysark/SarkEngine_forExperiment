#include "Camera.h"
#include <GL/glew.h>

namespace sark{

	//=============================================
	//		Camera::Viewport class implementation
	//=============================================

	Camera::Viewport::Viewport(real _x, real _y,
		real _width, real _height, real _minz, real _maxz)
		: x(_x), y(_y), width(_width), height(_height), minZ(_minz), maxZ(_maxz)
	{}


	//=========================================================
	//			Camera::ViewVolume class implementation
	// 
	// * implementation reference page:
	// * http://www.songho.ca/opengl/gl_projectionmatrix.html
	//=========================================================

	Camera::ViewVolume::ViewVolume(real _left, real _right,
		real _bottom, real _top, real _znear, real _zfar)
	{
		SetOrthographic(_left, _right, _bottom, _top, _znear, _zfar);
	}
	Camera::ViewVolume::ViewVolume(real _fovy, real _aspect, real _znear, real _zfar){
		SetPerspective(_fovy, _aspect, _znear, _zfar);
	}

	Camera::ViewVolume::~ViewVolume(){}


	// set view as orthographically
	void Camera::ViewVolume::SetOrthographic(real _left, real _right,
		real _bottom, real _top, real _znear, real _zfar)
	{
		ortho = true;

		left = _left;
		right = _right;
		bottom = _bottom;
		top = _top;
		zNear = _znear;
		zFar = _zfar;

		// half of width and height
		real hoW = (right - left) / 2.f;
		real hoH = (top - bottom) / 2.f;

		// calculate projection matrix
		projMatrix.MakeZero();
		projMatrix.m[0][0] = 1.f / hoW;
		projMatrix.m[1][1] = 1.f / hoH;
		projMatrix.m[2][2] = -2.f / (zFar - zNear);
		projMatrix.m[2][3] = -(zFar + zNear) / (zFar - zNear);
		projMatrix.m[3][3] = 1.f;
	}

	// set view as perspectively
	void Camera::ViewVolume::SetPerspective(real _fovy, real _aspect, real _znear, real _zfar){
		ortho = false;

		real hoH = math::tan(math::deg2rad(_fovy / 2.f)) * _znear;
		real hoW = hoH * _aspect;

		left = -hoW;
		right = hoW;
		bottom = -hoH;
		top = hoH;
		zNear = _znear;
		zFar = _zfar;

		// calculate projection matrix
		projMatrix.MakeZero();
		projMatrix.m[0][0] = zNear / hoW;
		projMatrix.m[1][1] = zNear / hoH;
		projMatrix.m[2][2] = -(zNear + zFar) / (zFar - zNear);
		projMatrix.m[2][3] = -(2.f * zFar * zNear) / (zFar - zNear);
		projMatrix.m[3][2] = -1.f;
	}



	//=============================================
	//		Camera class implementation
	//=============================================

	Camera::Camera(const Vector3& eye, const Vector3& lookat, const Vector3& up)
		: mViewport(0, 0, 600, 480), mVolume(90.f, 1.f, 0.1f, 100.f)
	{
		Set(eye, lookat, up);
	}

	Camera::~Camera(){}

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
	// it defines cube-like view volume from given properties.
	void Camera::Orthographic(real left, real right, real bottom, real top, real znear, real zfar){
		mVolume.SetOrthographic(left, right, bottom, top, znear, zfar);
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
	// set viewport again. it just calls graphic api with its properties
	void Camera::SetViewport(){
		glViewport(mViewport.x, mViewport.y, mViewport.width, mViewport.height);
		glDepthRange(mViewport.minZ, mViewport.maxZ);
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

	// set camera
	void Camera::Set(const Position3& eye, const Position3& lookat, const Vector3& up){
		mEye = eye;
		
		Vector3& n = mViewMatrix.row[2].xyz;
		n = eye - lookat;
		n.Normalize();

		Vector3& u = mViewMatrix.row[0].xyz;
		u = up.Cross(n);
		u.Normalize();

		Vector3& v = mViewMatrix.row[1].xyz;
		v = n.Cross(u);

		mViewMatrix.m[0][3] = -eye.Dot(u);
		mViewMatrix.m[1][3] = -eye.Dot(v);
		mViewMatrix.m[2][3] = -eye.Dot(n);
		mViewMatrix.m[3][3] = 1.f;
	}
	// set eye position
	void Camera::SetEye(const Position3& eye){
		mEye = eye;

		mViewMatrix.m[0][3] = -mEye.Dot(mViewMatrix.row[0].xyz);
		mViewMatrix.m[1][3] = -mEye.Dot(mViewMatrix.row[1].xyz);
		mViewMatrix.m[2][3] = -mEye.Dot(mViewMatrix.row[2].xyz);
	}


	// generate ray from screen coordinates
	const Ray Camera::ScreenToWorldRay(const Position2& screenCoord){
		// from viewport transform,
		// projection coord(px, py) can be calculated.
		// px = (2/vp.W)(sx - vp.x) - 1
		// py = -(2/vp.H)(sy - vp.y) + 1
		real px = (2.f / mViewport.width) * (screenCoord.x - mViewport.x) - 1.f;
		real py = -(2.f / mViewport.height) * (screenCoord.y - mViewport.y) + 1.f;

		Vector4 CS_dir; // camera space ray direction
		if (mVolume.ortho){
		}
		else{
			// camera space to clip space
			// C = (cx, cy, -n, 1)
			//      [cx]   [m00*cx]   [(m00/n)cx]   [px]
			// PT x [cy] = [m11*cy] > [(m11/n)cy] = [py]
			//      [-n]   [   ?  ]   [    0    ]   [ 0]
			//      [ 1]   [   n  ]   [    1    ]   [ 1]
			// 
			// cx = px * (n/m00)   px/m00
			// cy = py * (n/m11) > py/m11
			// cz = -n             -1
			//
			// CS_dir = C - EYE
			// *note: CS_dir is not normalized, it just divided by 'n'
			//        for convenience of computing.
			CS_dir.x = px / mVolume.projMatrix.m[0][0];
			CS_dir.y = py / mVolume.projMatrix.m[1][1];
			CS_dir.z = -1.0f;
			CS_dir.w = 0.0f;
		}

		// W = inv(VT) x C
		//   = inv(R x T) x C
		//   = inv(T) x inv(R) x C
		//   = inv(T) x transp(R) x C.
		// and inv(T) is translation into Eye
		// but, CS_dir's w factor is zero. so,
		const Vector3& u = GetBasisU();
		const Vector3& v = GetBasisV();
		const Vector3& n = GetBasisN();
		Vector3 WS_dir( // world space ray direction
			u.x*CS_dir.x + v.x*CS_dir.y + n.x*CS_dir.z,
			u.y*CS_dir.x + v.y*CS_dir.y + n.y*CS_dir.z,
			u.z*CS_dir.x + v.z*CS_dir.y + n.z*CS_dir.z);

		return Ray(
			mEye + mVolume.zNear*WS_dir, // ray starts from zn plane
			WS_dir, true, // actually, WS_dir doesn't normalized.
			mVolume.zFar - mVolume.zNear // to zf plane
			);
	}


	// basic functions

	// move camera into direction, fix lookat or not
	void Camera::MoveTo(const Vector3& direction, real distance, bool dir_normalized){
		if (dir_normalized)
			SetEye(mEye + distance * direction);
		else
			SetEye(mEye + distance * direction.Normal());
	}

	// move camera into forward direction (positive distance goes to forward)
	void Camera::MoveForward(real distance){
		SetEye(mEye - distance*GetBasisN());
	}

	// move camera into sideward direction (positive distance goes to right)
	void Camera::MoveSideward(real distance){
		SetEye(mEye + distance*GetBasisU());
	}

	// move camera into upward direction (positive distance goes to up)
	void Camera::MoveUpward(real distance){
		SetEye(mEye + distance*GetBasisV());
	}

	// turn camera on an axis of u (likes x)
	// it nod its sight (positive radian makes it to see up)
	void Camera::Pitch(real rad){
		Vector3& u = mViewMatrix.row[0].xyz;
		Vector3& v = mViewMatrix.row[1].xyz;
		Vector3& n = mViewMatrix.row[2].xyz;

		Quaternion q(u, rad, true);
		Quaternion q_ = q.Conjugation();

		v = (q * Quaternion(v) * q_).v;
		n = (q * Quaternion(n) * q_).v;
		
		mViewMatrix.m[1][3] = -mEye.Dot(v);
		mViewMatrix.m[2][3] = -mEye.Dot(n);
	}

	// turn camera on an axis of v (likes y)
	// it shakes its sight (positive radian makes it to see left)
	void Camera::Yaw(real rad){
		Vector3& u = mViewMatrix.row[0].xyz;
		Vector3& v = mViewMatrix.row[1].xyz;
		Vector3& n = mViewMatrix.row[2].xyz;

		Quaternion q(v, rad, true);
		Quaternion q_ = q.Conjugation();

		u = (q * Quaternion(u) * q_).v;
		n = (q * Quaternion(n) * q_).v;

		mViewMatrix.m[0][3] = -mEye.Dot(u);
		mViewMatrix.m[2][3] = -mEye.Dot(n);
	}

	// turn camera on an axis of n (likes z)
	// it tilt its sight (positive radian makes it to see left-tilting)
	void Camera::Roll(real rad){
		Vector3& u = mViewMatrix.row[0].xyz;
		Vector3& v = mViewMatrix.row[1].xyz;
		Vector3& n = mViewMatrix.row[2].xyz;

		Quaternion q(n, rad, true);
		Quaternion q_ = q.Conjugation();

		u = (q * Quaternion(u) * q_).v;
		v = (q * Quaternion(v) * q_).v;

		mViewMatrix.m[0][3] = -mEye.Dot(u);
		mViewMatrix.m[1][3] = -mEye.Dot(v);
	}

}
#include "View.h"
#include "Transform.h"

namespace sark{

	//=============================================
	//		Viewport class implementation
	//=============================================

	Viewport::Viewport()
		: mX(0.f), mY(0.f), mWidth(0.f), mHeight(0.f)
	{}

	// get x of viewport
	const real& Viewport::GetX() const{
		return mX;
	}
	// get y of viewport
	const real& Viewport::GetY() const{
		return mY;
	}

	// get width of viewport
	const real& Viewport::GetWidth() const{
		return mWidth;
	}
	// get height of viewport
	const real& Viewport::GetHeight() const{
		return mHeight;
	}

	// set viewport.
	void Viewport::Set(real x, real y, real width, real height){
		mX = x; mY = y;
		mWidth = width; mHeight = height;
	}



	//=========================================================
	//				View class implementation
	// 
	// * implementation reference page:
	// * http://www.songho.ca/opengl/gl_projectionmatrix.html
	//=========================================================

	View::View(){
		Perspective(90, 1, 0.01f, 10.f);
	}

	View::~View(){}


	// make orthographic view. it defines cube-like view volume
	// which range of [-width/2, width/2] for x, [-height/2, height/2] for y
	// and [0, depth] for z.
	void View::Orthographic(real width, real height, real depth){
		mOrtho = true;

		mH = height / 2.f;
		mW = width / 2.f;
		mzNear = 0.f;
		mzFar = depth;

		// calculate projection matrix
		mProjMatrix.MakeZero();
		mProjMatrix.m[0][0] = 1.f / mW;
		mProjMatrix.m[1][1] = 1.f / mH;
		mProjMatrix.m[2][2] = -2.f / (mzFar - mzNear);
		mProjMatrix.m[2][3] = -(mzFar + mzNear) / (mzFar - mzNear);
		mProjMatrix.m[3][3] = 1.f;
	}

	// make perspective view
	// it defines truncated quadrangular pyramid volume called 'frustum'.
	// 
	// fovy is Field Of View as degree,
	// aspect is the ratio of width of height (width/height),
	// znear is positive distance from cop to nearest plane,
	// zfar is positive distance from cop to farthest plane.
	void View::Perspective(real fovy, real aspect, real znear, real zfar){
		mOrtho = false;

		mH = math::tan(math::deg2rad(fovy / 2.f)) * znear;
		mW = mH * aspect;
		mzNear = znear;
		mzFar = zfar;
		

		// calculate projection matrix
		mProjMatrix.MakeZero();
		mProjMatrix.m[0][0] = mzNear / mW;
		mProjMatrix.m[1][1] = mzNear / mH;
		mProjMatrix.m[2][2] = -(mzNear + mzFar) / (mzFar - mzNear);
		mProjMatrix.m[2][3] = -(2.f * mzFar * mzNear) / (mzFar - mzNear);
		mProjMatrix.m[3][2] = -1.f;
	}


	// is this view orthographic view?
	bool View::IsOrthoView() const{
		return mOrtho;
	}

	// get projection matrix
	const Matrix4& View::GetProjMatrix() const{
		return mProjMatrix;
	}

	// get viewport
	const Viewport& View::GetViewport() const{
		return mViewport;
	}
	// set viewport
	void View::SetViewport(real x, real y, real width, real height){
		mViewport.Set(x, y, width, height);
	}

}
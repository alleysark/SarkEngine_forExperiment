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



	//=============================================
	//		IView class implementation
	//=============================================

	IView::IView()
		: mProjMatrix(true)
	{ }

	IView::~IView(){}

	// get projection matrix
	const Matrix4& IView::GetProjMatrix() const{
		return mProjMatrix;
	}

	// get viewport
	const Viewport& IView::GetViewport() const{
		return mViewport;
	}
	// set viewport
	void IView::SetViewport(real x, real y, real width, real height){
		mViewport.Set(x, y, width, height);
	}



	//=============================================
	//		OrthogonalView class implementation
	//=============================================

	// default properties are 20,20,10
	OrthogonalView::OrthogonalView()
		: mProperty(20.f, 20.f, 10.f)
	{
		UpdateProjMatrix();
	}
	OrthogonalView::OrthogonalView(real width, real height, real depth)
		: mProperty(width, height, depth)
	{
		UpdateProjMatrix();
	}

	// set view properties
	void OrthogonalView::Set(const OrthogonalProperty& orthoProperty){
		mProperty = orthoProperty;
		UpdateProjMatrix();
	}

	// update projection matrix
	void OrthogonalView::UpdateProjMatrix(){
		// TODO:
	}


	//=============================================
	//		PerspectiveView class implementation
	//=============================================

	// default properties are 90, 1, 0.01, 10
	PerspectiveView::PerspectiveView()
		: mProperty(90.f, 1.f, 0.01f, 10.f)
	{
		UpdateProjMatrix();
	}
	PerspectiveView::PerspectiveView(real fovy, real aspect, real z_near, real z_far)
		: mProperty(fovy, aspect, z_near, z_far)
	{
		UpdateProjMatrix();
	}

	// set view properties
	void PerspectiveView::Set(const PerspectiveProperty& persProperty){
		mProperty = persProperty;
		UpdateProjMatrix();
	}

	// update projection matrix
	void PerspectiveView::UpdateProjMatrix(){
		// TODO:
	}
}
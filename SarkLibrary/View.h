#ifndef __VIEW_H__
#define __VIEW_H__

#include "core.h"

namespace sark{

	// view port of window
	class Viewport{
	private:
		real mX, mY;
		real mWidth, mHeight;

	public:
		Viewport();
		
		// get x of viewport
		const real& GetX() const;
		// get y of viewport
		const real& GetY() const;
		
		// get width of viewport
		const real& GetWidth() const;
		// get height of viewport
		const real& GetHeight() const;

		// set viewport.
		void Set(real x, real y, real width, real height);
	};



	// interface of view.
	// it defines the features of view, shape of view-volume,
	// viewport and projection matrix.
	// there is two viewing style; <OrthogonalView> and <PerspectiveView>.
	class IView{
	public:
		struct OrthogonalProperty{
			// width of cube volume.
			// range will be [-width/2, width/2]
			real mWidth;

			// height of cube volume.
			// range will be [-height/2, height/2]
			real mHeight;

			// depth of cube volume.
			// range will be [0, depth]
			real mDepth;

			OrthogonalProperty(real width, real height, real depth){
				mWidth = width;
				mHeight = height;
				mDepth = depth;
			}
		};

		struct PerspectiveProperty{
			// field of view about vertical (y property)
			real mFovy;
			// aspect ratio (width / height)
			real mAspect;
			// near plane of view frustum
			real mNear;
			// far plane of view frustum
			real mFar;

			PerspectiveProperty(real fovy, real aspect, real z_near, real z_far){
				mFovy = fovy;
				mAspect = aspect;
				mNear = z_near;
				mFar = z_far;
			}
		};

	protected:
		// projection matrix
		Matrix4 mProjMatrix;

		// viewport definition
		Viewport mViewport;

	public:
		IView();

		virtual ~IView();

		// set property for orthogonal view
		virtual void Set(const OrthogonalProperty& orthoProperty) = 0;

		// set property for perspective view
		virtual void Set(const PerspectiveProperty& persProperty) = 0;

		// get projection matrix
		const Matrix4& GetProjMatrix() const;

	protected:
		// update projection matrix
		virtual void UpdateProjMatrix() = 0;

	public:

		// get viewport
		const Viewport& GetViewport() const;
		// set viewport
		void SetViewport(real x, real y, real width, real height);


		// // is the input position culled by this view volume?
		// // (input position have to be defined in world space)
		// virtual bool IsCulled(const Position3& position) const = 0;

		// virtual bool IsCulled(const Sphere& sphere) const = 0;
	};



	// orthogonal view which is one of the viewing style.
	// it defines cube-like view volume.
	class OrthogonalView : public IView{
	private:
		OrthogonalProperty mProperty;

	public:
		// default properties are 20,20,10
		OrthogonalView();
		OrthogonalView(real width, real height, real depth);

		// set view properties
		void Set(const OrthogonalProperty& orthoProperty);

	private:
		// update projection matrix
		void UpdateProjMatrix();

		// not used
		void Set(const PerspectiveProperty&){}
	};



	// perspective view which is one of the viewing style.
	// it defines truncated quadrangular pyramid volume called 'frustum'.
	class PerspectiveView : public IView{
	private:
		PerspectiveProperty mProperty;

	public:
		// default properties are 90, 1, 0.01, 10
		PerspectiveView();
		PerspectiveView(real fovy, real aspect, real z_near, real z_far);
		
		// set view properties
		void Set(const PerspectiveProperty& persProperty);

	private:
		// update projection matrix
		void UpdateProjMatrix();

		// not used
		void Set(const OrthogonalProperty&){}
	};
}
#endif
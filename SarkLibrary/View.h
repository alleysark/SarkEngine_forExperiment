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



	// it defines the features of view, shape of view-volume,
	// viewport and projection matrix.
	// there is two viewing style; 'Orthographic view' and 'Perspective view'.
	class View{
	private:
		// it is true when if it is orthographic view
		bool mOrtho;

		// half factor of height. it defines the range as [-mH, mH]
		real mH;

		// half factor of width. it defines the range as [-mW, mW]
		real mW;

		// positive value of z-distance from cop to nearest plane.
		real mzNear;

		// positive value of z-distance from cop to farthest plane.
		real mzFar;

		// projection matrix
		Matrix4 mProjMatrix;

		// viewport definition
		Viewport mViewport;

	public:
		View();

		~View();

		// make orthographic view. it defines cube-like view volume
		// which range of [-width/2, width/2] for x, [-height/2, height/2] for y
		// and [0, depth] for z.
		void Orthographic(real width, real height, real depth);

		// make perspective view
		// it defines truncated quadrangular pyramid volume called 'frustum'.
		// 
		// fovy is Field Of View as degree,
		// aspect is the ratio of width of height (width/height),
		// znear is positive distance from cop to nearest plane,
		// zfar is positive distance from cop to farthest plane.
		void Perspective(real fovy, real aspect, real znear, real zfar);


		// is this view orthographic view?
		bool IsOrthoView() const;

		// get projection matrix
		const Matrix4& GetProjMatrix() const;

		// get viewport
		const Viewport& GetViewport() const;
		// set viewport
		void SetViewport(real x, real y, real width, real height);


		// // is the input position culled by this view volume?
		// // (input position have to be defined in world space)
		// virtual bool IsCulled(const Position3& position) const = 0;

		// virtual bool IsCulled(const Sphere& sphere) const = 0;
	};

}
#endif
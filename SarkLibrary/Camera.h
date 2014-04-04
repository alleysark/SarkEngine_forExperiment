#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core.h"
#include "shapes.h"

namespace sark{

	// camera defines the most basic functionality and properties of view
	// (viewport, view volume style, view transforming).
	// it also can be extended by inheritance.
	// 
	// camera space is defined by u,v,n orthonormal non-standard basis.
	// they are extracted from given eye, at, up vectors.
	// 
	// viewport decides where is the rendered image presented on application window.
	// it is used for sort of screen to world ray conversion.
	// 
	// view volume can be one of the orthographic or perspective, and projection
	// transformation matrix is decided by its viewing style.
	// view volume is also used to culling world objects for rendering performance.
	class Camera{
	public:
		// view port definition of window
		class Viewport{
		public:
			real x, y;
			real width, height;

			real minZ, maxZ;

			Viewport();
		};


		// it defines the features of view, shape of view-volume,
		// viewport and projection matrix.
		// there is two viewing style; 'Orthographic view' and 'Perspective view'.
		class ViewVolume{
		public:
			// it is true when if it is orthographic view
			bool ortho;

			// half factor of height. it defines the range as [-H, H]
			real height;

			// half factor of width. it defines the range as [-W, W]
			real width;

			// positive value of z-distance from cop to nearest plane.
			real zNear;

			// positive value of z-distance from cop to farthest plane.
			real zFar;

			// projection matrix
			Matrix4 projMatrix;

		public:
			ViewVolume(real _width, real _height, real _depth);
			ViewVolume(real _fovy, real _aspect, real _znear, real _zfar);
			~ViewVolume();

			// set view as orthographically
			void SetOrthographic(real _width, real _height, real _depth);

			// set view as perspectively
			void SetPerspective(real _fovy, real _aspect, real _znear, real _zfar);
		};

	protected:
		//origin of camera space
		Position3 mEye;

		//focusing target
		Position3 mLookat;

		//camera's standing axis
		Vector3 mUp;


		// view transform matrix for transforming into camera space.
		Matrix4 mViewMatrix;


		// viewport definition
		Viewport mViewport;

		// what viewing volume is (orthographic or perspective).
		// it has the projection matrix and viewport definition
		ViewVolume mVolume;

	public:
		Camera();
		Camera(const Vector3& eye, const Vector3& lookat, const Vector3& up = Vector3(0.f, 1.f, 0.f));
		virtual ~Camera();

	private:
		// update view matrix from camera properties(eye,at,up)
		void UpdateViewMatrix();

	public:
		// get view transformation matrix
		const Matrix4& GetViewMatrix();

		// get projection transformation matrix
		const Matrix4& GetProjMatrix();


		// is this view orthographic view?
		bool IsOrthoView() const;

		// make camera view volume as orthographic-view.
		// it defines cube-like view volume
		// which range of [-width/2, width/2] for x, [-height/2, height/2] for y
		// and [0, depth] for z.
		void Orthographic(real width, real height, real depth);

		// make camera view volume as perspective-view.
		// it defines truncated quadrangular pyramid volume called 'frustum'.
		// 
		// fovy is Field Of View as degree,
		// aspect is the ratio of width of height (width/height),
		// znear is positive distance from cop to nearest plane,
		// zfar is positive distance from cop to farthest plane.
		void Perspective(real fovy, real aspect, real znear, real zfar);


		// get viewport
		const Viewport& GetViewport() const;
		// set viewport
		void SetViewport(integer x, integer y, integer width, integer height);
		// set viewport with depth range.
		void SetViewport(integer x, integer y, integer width, integer height, real minz, real maxz);

		// get view volume
		const ViewVolume& GetViewVolume() const;


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


		// generate ray from screen coordinates
		const Ray ScreenToWorldRay(const Position2& screenCoord);


		// basic camera functions

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
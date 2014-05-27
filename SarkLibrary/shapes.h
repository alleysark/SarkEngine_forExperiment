#ifndef __SHAPES_H__
#define __SHAPES_H__

#include <vector>
#include "core.h"

namespace sark{

	class IShape;
	
	class Ray;
	class Plane;
	class Triangle;
	class Sphere;
	class AxisAlignedBox;
	class OrientedBox;
	class ConvexHull; // see 'ConvexHull.h'

	
	// interface of shape things.
	class IShape{
	public:
		enum Type{ RAY, PLANE, SPHERE, AABOX, OBOX, CONVEXHULL };
	
		IShape(){}
		virtual ~IShape(){}

		// get type of shape
		virtual Type GetType() const = 0;

		// intersection test.
		virtual bool IntersectWith(const IShape* shapeB) const = 0;
	};



	// ray is represented as start position and
	// direction vector with limitation.
	class Ray : public IShape{
	public:
		// start position. the origin of ray
		Vector3 pos;

		// normalized direction vector
		Vector3 dir;

		// limitation of ray distance. positive value.
		real limit;

		Ray();
		Ray(const Vector3& position,
			const Vector3& direction,
			real limitation);
		// ray as line segment
		Ray(const Vector3& A, const Vector3& B);

		Type GetType() const override;

		bool IntersectWith(const IShape* shapeB) const override;

		bool IntersectPointWith(const IShape* shapeB, Vector3* out_P) const;
	};



	// plane is represented as normal vector and
	// an arbitrary point on the plane.
	class Plane : public IShape{
	public:
		Vector3 norm;
		Vector3 p;

		Plane();
		Plane(const Vector3& normal, const Vector3& point);
		// Ax + By + Cz + D = 0
		Plane(const Vector3& normal, real D);

		Type GetType() const override;

		bool IntersectWith(const IShape* shapeB) const override;
	};



	// center and radius form of sphere
	class Sphere : public IShape{
	public:
		// origin position of sphere
		Vector3 pos;

		// radius
		real r;

		Sphere();
		Sphere(const Vector3& position, real radius);

		Type GetType() const override;

		bool IntersectWith(const IShape* shapeB) const override;
	};


	// box that its faces are aligned with
	// standard coordinates axis orientation.
	// it is represented as min, max position.
	class AxisAlignedBox : public IShape{
	public:
		// minimum position
		Vector3 min;

		// maximum position
		Vector3 max;

		AxisAlignedBox();
		AxisAlignedBox(const Vector3& posMin, const Vector3& posMax);
		AxisAlignedBox(const Vector3& center, real half_ext[3]);
		

		Type GetType() const override;

		bool IntersectWith(const IShape* shapeB) const override;
	};


	// box which has the own orientation.
	// it is represented as center position
	// of box and its own basis with extensions.
	class OrientedBox : public IShape{
	public:
		// center position of box.
		Vector3 pos;

		// half-extention of each axis.
		Vector3 ext;

		// orthonormal axis
		Vector3 axis[3];

		OrientedBox();
		OrientedBox(const Vector3& position, const Vector3& extention);
		OrientedBox(const Vector3& position, const Vector3& extention,
			const Vector3 _axis[3]);

		Type GetType() const override;

		void SetAxis(const Matrix4& TM);

		bool IntersectWith(const IShape* shapeB) const override;
	};

}
#endif
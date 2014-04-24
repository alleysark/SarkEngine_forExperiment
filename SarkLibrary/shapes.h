#ifndef __SHAPES_H__
#define __SHAPES_H__

#include "core.h"

namespace sark{

	class IShape;
	
	class Ray;

	class Sphere;
	class AxisAlignedBox;
	class OrientedBox;
	class Polyhedron;

	
	// interface of shape things.
	// shape can be intersected with each others.
	class IShape{
	public:
		enum Type{ RAY, SPHERE, AABOX, OBOX, POLYHEDRON, EXTENSION };
		typedef bool(*ExtShapeIntersectionChecker)(const IShape* shape1, const IShape* shape2);

	protected:
		Type mType;
		static ExtShapeIntersectionChecker extChecker;

	public:
		IShape(Type type);
		virtual ~IShape();

		// get type of shape
		Type GetType() const;

		virtual bool IsIntersectedWith(const IShape* shape) const = 0;

		static void SetExtensionShapeIntersectionChecker(ExtShapeIntersectionChecker checker);
	};



	// ray the half line.
	// it is represented by start position and normalized direction vector.
	// and it also has the limitation of its distance.
	class Ray : public IShape{
	public:
		// start position. the origin of ray
		Position3 pos;

		// normalized direction vector
		Vector3 dir;

		// limitation of ray distance. positive value.
		real limit;

		Ray();
		Ray(const Position3& position, const Vector3& direction,
			bool dir_normalized = false, real limitation = REAL_MAX);

		void Set(const Position3& position, const Vector3& direction,
			bool dir_normalized = false, real limitation = REAL_MAX);

		bool IsIntersectedWith(const IShape* shape) const override;
	};


	// center and radius form of sphere
	class Sphere : public IShape{
	public:
		// origin position of sphere
		Position3 pos;

		// radius
		real r;

		Sphere();
		Sphere(const Position3& position, real radius);

		void Set(const Position3& position, real radius);

		bool IsIntersectedWith(const IShape* shape) const override;
	};


	// box that its faces are aligned by coordinate axis orientation.
	// it is represented by two cater-cornered positions.
	class AxisAlignedBox : public IShape{
	public:
		// cater-cornered minimum position.
		Position3 min;

		// cater-cornered maximum position.
		Position3 max;

		AxisAlignedBox();
		AxisAlignedBox(const Position3& posMin, const Position3& posMax);

		void Set(const Position3& posMin, const Position3& posMax);

		bool IsIntersectedWith(const IShape* shape) const override;
	};


	// box which has the own orientation.
	// it is represented by center position of box and its own basis with extensions.
	class OrientedBox : public IShape{
	public:
		// center position of box.
		Position3 pos;

		//vec3 as axis, w factor as extension
		Vector4 axis[3]; 

		OrientedBox();
		OrientedBox(const Position3& position,
			const Vector4& basisXext, const Vector4& basisYext, const Vector4& basisZext,
			bool basis_normalized = false);

		void Set(const Position3& position,
			const Vector4& basisXext, const Vector4& basisYext, const Vector4& basisZext,
			bool basis_normalized = false);

		bool IsIntersectedWith(const IShape* shape) const override;
	};


	class Mesh;
	class Transform;

	// polyhedron of triangle set.
	// it has the reference of mesh and relation transform
	class Polyhedron : public IShape{
	public:
		// reference of mesh.
		Mesh* refMesh;

		// reference of transform.
		// it can't be const for calling GetMatrix()
		Transform* refTransform;

		Polyhedron();
		Polyhedron(Mesh* referenceMesh, Transform* referenceTransform);

		void Set(Mesh* referenceMesh, Transform* referenceTransform);

		bool IsIntersectedWith(const IShape* shape) const override;
	};



	// intersection check functions
	
	// ray - sphere intersection
	bool IsIntersected(const Ray* ray, const Sphere* sphere);
	// ray - axis aligned box intersection
	bool IsIntersected(const Ray* ray, const AxisAlignedBox* aabox);
	// ray - oriented box intersection
	bool IsIntersected(const Ray* ray, const OrientedBox* obox);
	// ray - polyhedron intersection
	bool IsIntersected(const Ray* ray, const Polyhedron* poly);

	// sphere - sphere intersection
	bool IsIntersected(const Sphere* sphere1, const Sphere* sphere2);
	// sphere - axis aligned box intersection
	bool IsIntersected(const Sphere* sphere, const AxisAlignedBox* aabox);
	// sphere - oriented box intersection
	bool IsIntersected(const Sphere* sphere, const OrientedBox* obox);
	// sphere - polyhedron intersection
	bool IsIntersected(const Sphere* sphere, const Polyhedron* poly);

	// axis aligned box - axis aligned box intersection
	bool IsIntersected(const AxisAlignedBox* aabox1, const AxisAlignedBox* aabox2);
	// axis aligned box - oriented box intersection
	bool IsIntersected(const AxisAlignedBox* aabox, const OrientedBox* obox);
	// axis aligned box - polyhedron intersection
	bool IsIntersected(const AxisAlignedBox* aabox, const Polyhedron* poly);

	// oriented box - oriented box intersection
	bool IsIntersected(const OrientedBox* obox1, const OrientedBox* obox2);
	// oriented box - polyhedron intersection
	bool IsIntersected(const OrientedBox* obox, const Polyhedron* poly);

	// polyhedron - polyhedron intersection
	bool IsIntersected(const Polyhedron* poly1, const Polyhedron* poly2);



	// assistance functions

	// is the given value 't' in between arbitrary order 'a' and 'b'?
	inline bool IsInBetween(real a, real b, real t){
		return((a < b)
			? (t < a || b < t) ? false : true
			: (t < b || a < t) ? false : true);
	}

	// is aabb contains the given position?
	inline bool ContainsIn(const AxisAlignedBox* aabox, const Position3& pos){
		if ((pos.x < aabox->min.x || aabox->max.x < pos.x) ||
			(pos.y < aabox->min.y || aabox->max.y < pos.y) ||
			(pos.z < aabox->min.z || aabox->max.z < pos.z)){
			return false;
		}
		return true;
	}

}
#endif
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
		typedef bool(*ExtShapeIntersectionChecker)(IShape* shape1, IShape* shape2);

	protected:
		Type mType;
		static ExtShapeIntersectionChecker extChecker;

	public:
		IShape(Type type);
		virtual ~IShape();

		// get type of shape
		Type GetType() const;

		virtual bool IsIntersectedWith(IShape* shape) = 0;

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

		Ray(const Position3& position, const Vector3& direction,
			bool dir_normalized = false, real limitation = REAL_MAX);

		bool IsIntersectedWith(IShape* shape) override;
	};


	// center and radius form of sphere
	class Sphere : public IShape{
	public:
		// origin position of sphere
		Position3 pos;

		// radius
		real r;

		Sphere(const Position3& position, real radius);

		bool IsIntersectedWith(IShape* shape) override;
	};


	// box that its faces are aligned by coordinate axis orientation.
	// it is represented by two cater-cornered positions.
	class AxisAlignedBox : public IShape{
	public:
		// cater-cornered minimum position.
		Position3 min;

		// cater-cornered maximum position.
		Position3 max;

		AxisAlignedBox(const Position3& posMin, const Position3& posMax);

		bool IsIntersectedWith(IShape* shape) override;
	};


	// box which has the own orientation.
	// it is represented by center position of box and its own basis with extensions.
	class OrientedBox : public IShape{
		// center position of box.
		Position3 pos;

		//vec3 as axis, w factor as extension
		Vector4 axis[3]; 

		OrientedBox(const Position3& position,
			const Vector3& basisX, const Vector3& basisY, const Vector3& basisZ, const Vector3& halfOfExt,
			bool basis_normalized = false);
		OrientedBox(const Position3& position,
			const Vector4& basisXext, const Vector4& basisYext, const Vector4& basisZext,
			bool basis_normalized = false);

		bool IsIntersectedWith(IShape* shape) override;
	};


	class Mesh;

	// polyhedron of triangle set.
	// it has the reference of mesh
	class Polyhedron : public IShape{
	public:
		// const reference of mesh.
		const Mesh* refMesh;

		Polyhedron(const Mesh* mesh);

		bool IsIntersectedWith(IShape* shape) override;
	};



	// intersection check functions
	
	// ray - sphere intersection
	bool IsIntersected(Ray* ray, Sphere* sphere);
	// ray - axis aligned box intersection
	bool IsIntersected(Ray* ray, AxisAlignedBox* aabox);
	// ray - oriented box intersection
	bool IsIntersected(Ray* ray, OrientedBox* obox);
	// ray - polyhedron intersection
	bool IsIntersected(Ray* ray, Polyhedron* poly);

	// sphere - sphere intersection
	bool IsIntersected(Sphere* sphere1, Sphere* sphere2);
	// sphere - axis aligned box intersection
	bool IsIntersected(Sphere* sphere, AxisAlignedBox* aabox);
	// sphere - oriented box intersection
	bool IsIntersected(Sphere* sphere, OrientedBox* obox);
	// sphere - polyhedron intersection
	bool IsIntersected(Sphere* sphere, Polyhedron* poly);

	// axis aligned box - axis aligned box intersection
	bool IsIntersected(AxisAlignedBox* aabox1, AxisAlignedBox* aabox2);
	// axis aligned box - oriented box intersection
	bool IsIntersected(AxisAlignedBox* aabox, OrientedBox* obox);
	// axis aligned box - polyhedron intersection
	bool IsIntersected(AxisAlignedBox* aabox, Polyhedron* poly);

	// oriented box - oriented box intersection
	bool IsIntersected(OrientedBox* obox1, OrientedBox* obox2);
	// oriented box - polyhedron intersection
	bool IsIntersected(OrientedBox* obox, Polyhedron* poly);

	// polyhedron - polyhedron intersection
	bool IsIntersected(Polyhedron* poly1, Polyhedron* poly2);

}
#endif
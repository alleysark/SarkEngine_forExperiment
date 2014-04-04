#include "shapes.h"
#include "Mesh.h"
#include <algorithm>

namespace sark{

	IShape::ExtShapeIntersectionChecker IShape::extChecker = NULL;

	IShape::IShape(Type type)
		: mType(type)
	{}

	IShape::~IShape(){}

	IShape::Type IShape::GetType() const{
		return mType;
	}

	void IShape::SetExtensionShapeIntersectionChecker(ExtShapeIntersectionChecker checker){
		extChecker = checker;
	}


	// ======================================================
	//			implemented shapes below..
	// ======================================================

	Ray::Ray(const Position3& position, const Vector3& direction, bool dir_normalized, real limitation)
		: IShape(IShape::RAY), pos(position), dir(direction), limit(limitation)
	{
		if (!dir_normalized)
			dir.Normalize();
	}

	bool Ray::IsIntersectedWith(const IShape* shape) const{
		switch (shape->GetType()){
		case IShape::RAY:
			return false;
		case IShape::SPHERE:
			return IsIntersected(this, dynamic_cast<const Sphere*>(shape));
		case IShape::AABOX:
			return IsIntersected(this, dynamic_cast<const AxisAlignedBox*>(shape));
		case IShape::OBOX:
			return IsIntersected(this, dynamic_cast<const OrientedBox*>(shape));
		case IShape::POLYHEDRON:
			return IsIntersected(this, dynamic_cast<const Polyhedron*>(shape));
		default:
			if (extChecker != NULL)
				return extChecker(this, shape);
			return false;
		}
	}


	// sphere
	Sphere::Sphere(const Point3& position, real radius)
		: IShape(IShape::SPHERE), pos(position), r(radius)
	{}

	bool Sphere::IsIntersectedWith(const IShape* shape) const{
		switch (shape->GetType()){
		case IShape::RAY:
			return IsIntersected(dynamic_cast<const Ray*>(shape), this);
		case IShape::SPHERE:
			return IsIntersected(this, dynamic_cast<const Sphere*>(shape));
		case IShape::AABOX:
			return IsIntersected(this, dynamic_cast<const AxisAlignedBox*>(shape));
		case IShape::OBOX:
			return IsIntersected(this, dynamic_cast<const OrientedBox*>(shape));
		case IShape::POLYHEDRON:
			return IsIntersected(this, dynamic_cast<const Polyhedron*>(shape));
		default:
			if (extChecker != NULL)
				return extChecker(this, shape);
			return false;
		}
	}


	// axis aligned box
	AxisAlignedBox::AxisAlignedBox(const Point3& posMin, const Point3& posMax)
		: IShape(IShape::AABOX), min(posMin), max(posMax)
	{}

	bool AxisAlignedBox::IsIntersectedWith(const IShape* shape) const{
		switch (shape->GetType()){
		case IShape::RAY:
			return IsIntersected(dynamic_cast<const Ray*>(shape), this);
		case IShape::SPHERE:
			return IsIntersected(dynamic_cast<const Sphere*>(shape), this);
		case IShape::AABOX:
			return IsIntersected(this, dynamic_cast<const AxisAlignedBox*>(shape));
		case IShape::OBOX:
			return IsIntersected(this, dynamic_cast<const OrientedBox*>(shape));
		case IShape::POLYHEDRON:
			return IsIntersected(this, dynamic_cast<const Polyhedron*>(shape));
		default:
			if (extChecker != NULL)
				return extChecker(this, shape);
			return false;
		}
	}


	// oriented box
	OrientedBox::OrientedBox(const Position3& position,
		const Vector3& basisX, const Vector3& basisY, const Vector3& basisZ, const Vector3& halfOfExt,
		bool basis_normalized)
		: IShape(IShape::OBOX), pos(position)
	{
		axis[0] = Vector4(basisX, halfOfExt.x);
		axis[1] = Vector4(basisY, halfOfExt.y);
		axis[2] = Vector4(basisZ, halfOfExt.z);
		if (!basis_normalized){
			axis[0].xyz.Normalize();
			axis[1].xyz.Normalize();
			axis[2].xyz.Normalize();
		}
	}
	OrientedBox::OrientedBox(const Position3& position,
		const Vector4& basisXext, const Vector4& basisYext, const Vector4& basisZext,
		bool basis_normalized)
		: IShape(IShape::OBOX), pos(position)
	{
		axis[0] = basisXext;
		axis[1] = basisYext;
		axis[2] = basisZext;
		if (!basis_normalized){
			axis[0].xyz.Normalize();
			axis[1].xyz.Normalize();
			axis[2].xyz.Normalize();
		}
	}

	bool OrientedBox::IsIntersectedWith(const IShape* shape) const{
		switch (shape->GetType()){
		case IShape::RAY:
			return IsIntersected(dynamic_cast<const Ray*>(shape), this);
		case IShape::SPHERE:
			return IsIntersected(dynamic_cast<const Sphere*>(shape), this);
		case IShape::AABOX:
			return IsIntersected(dynamic_cast<const AxisAlignedBox*>(shape), this);
		case IShape::OBOX:
			return IsIntersected(this, dynamic_cast<const OrientedBox*>(shape));
		case IShape::POLYHEDRON:
			return IsIntersected(this, dynamic_cast<const Polyhedron*>(shape));
		default:
			if (extChecker != NULL)
				return extChecker(this, shape);
			return false;
		}
	}


	// polyhedron
	Polyhedron::Polyhedron(const Mesh* mesh)
		: IShape(IShape::POLYHEDRON), refMesh(mesh)
	{}

	bool Polyhedron::IsIntersectedWith(const IShape* shape) const{
		switch (shape->GetType()){
		case IShape::RAY:
			return IsIntersected(dynamic_cast<const Ray*>(shape), this);
		case IShape::SPHERE:
			return IsIntersected(dynamic_cast<const Sphere*>(shape), this);
		case IShape::AABOX:
			return IsIntersected(dynamic_cast<const AxisAlignedBox*>(shape), this);
		case IShape::OBOX:
			return IsIntersected(dynamic_cast<const OrientedBox*>(shape), this);
		case IShape::POLYHEDRON:
			return IsIntersected(this, dynamic_cast<const Polyhedron*>(shape));
		default:
			if (extChecker != NULL)
				return extChecker(this, shape);
			return false;
		}
	}


	// ======================================================
	//		intersection check functions of basic shapes
	//
	//				referenced documents
	//	* http://paulbourke.net/geometry/pointlinetriSlab/
	// ======================================================

	// ray - sphere intersection
	bool IsIntersected(const Ray* ray, const Sphere* sphere){
		Vector3 p_o = ray->pos - sphere->pos;
		real a = ray->dir.MagnitudeSq();
		real b = ray->dir.Dot(p_o); //half of b
		real c = p_o.MagnitudeSq() - math::sqre(sphere->r);

		real det = math::sqre(b) - a*c;
		if (det < 0){
			return false;
		}

		if (det == 0){
			real t = -b / a;
			if (0 <= t && t <= ray->limit)
				return true;
		}
		else{//det>0
			det = math::sqrt(det);
			real t = (-b + det) / a;
			if (0 <= t && t <= ray->limit)
				return true;

			t = (-b - det) / a;
			if (0 <= t && t <= ray->limit)
				return true;
		}
		return false;
	}

	// ray - axis aligned box intersection
	bool IsIntersected(const Ray* ray, const AxisAlignedBox* aabox){ return false; }
	
	// ray - oriented box intersection
	bool IsIntersected(const Ray* ray, const OrientedBox* obox){ return false; }
	
	// ray - polyhedron intersection
	bool IsIntersected(const Ray* ray, const Polyhedron* poly){ return false; }


	// sphere - sphere intersection
	bool IsIntersected(const Sphere* sphere1, const Sphere* sphere2){
		real dist = (sphere1->pos - sphere2->pos).Magnitude();
		if (dist <= sphere1->r + sphere2->r)
			return true;
		return false;
	}

	// sphere - axis aligned box intersection
	bool IsIntersected(const Sphere* sphere, const AxisAlignedBox* aabox){ return false; }
	
	// sphere - oriented box intersection
	bool IsIntersected(const Sphere* sphere, const OrientedBox* obox){ return false; }
	
	// sphere - polyhedron intersection
	bool IsIntersected(const Sphere* sphere, const Polyhedron* poly){ return false; }

	// axis aligned box - axis aligned box intersection
	bool IsIntersected(const AxisAlignedBox* aabox1, const AxisAlignedBox* aabox2){ return false; }
	
	// axis aligned box - oriented box intersection
	bool IsIntersected(const AxisAlignedBox* aabox, const OrientedBox* obox){ return false; }
	
	// axis aligned box - polyhedron intersection
	bool IsIntersected(const AxisAlignedBox* aabox, const Polyhedron* poly){ return false; }

	// oriented box - oriented box intersection
	bool IsIntersected(const OrientedBox* obox1, const OrientedBox* obox2){ return false; }
	
	// oriented box - polyhedron intersection
	bool IsIntersected(const OrientedBox* obox, const Polyhedron* poly){ return false; }

	// polyhedron - polyhedron intersection
	bool IsIntersected(const Polyhedron* poly1, const Polyhedron* poly2){ return false; }


}
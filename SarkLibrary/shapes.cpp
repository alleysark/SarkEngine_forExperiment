#include "shapes.h"
#include "tools.h"
#include "Debug.h"
#include <algorithm>

namespace sark{

	// ======================================================
	//			implemented shapes below..
	// ======================================================

	// ray
	Ray::Ray()
		: pos(0), dir(Vector3::Forward), limit(0){}

	Ray::Ray(const Vector3& position, const Vector3& direction, real limitation)
		: pos(position), dir(direction), limit(limitation){}

	Ray::Ray(const Vector3& A, const Vector3& B)
		: pos(A), dir(B - A), limit(1.f){}

	IShape::Type Ray::GetType() const{
		return IShape::RAY;
	}

	bool Ray::IntersectWith(const IShape* shapeB) const{
		return IntersectPointWith(shapeB, NULL);
	}

	bool Ray::IntersectPointWith(const IShape* shapeB, Vector3* out_P) const{
		switch (shapeB->GetType()){
		case IShape::RAY:
			break;
		case IShape::PLANE:{
				const Plane& plane = reinterpret_cast<const Plane&>(*shapeB);
				return tool::Ray_PlaneIntersection(pos, dir, limit, plane.norm, plane.p, out_P);
			}
			break;
		case IShape::SPHERE:{
				const Sphere& sphere = reinterpret_cast<const Sphere&>(*shapeB);
				return tool::Ray_SphereIntersection(pos, dir, limit, sphere.pos, sphere.r, out_P);
			}
			break;
		case IShape::AABOX:{
				const AxisAlignedBox& aab = reinterpret_cast<const AxisAlignedBox&>(*shapeB);
				return tool::Ray_AxisAlignedBoxIntersection(pos, dir, limit, aab.min, aab.max, out_P);
			}
			break;
		case IShape::OBOX:{
				const OrientedBox& ob = reinterpret_cast<const OrientedBox&>(*shapeB);
				return tool::Ray_OrientedBoxIntersection(pos, dir, limit, ob.pos, ob.ext, ob.axis, out_P);
			}
			break;
		case IShape::CONVEXHULL:
			break;
		}
		return false;
	}


	// plane
	Plane::Plane()
		: norm(Vector3::Up), p(0){}

	Plane::Plane(const Vector3& normal, const Vector3& point)
		: norm(normal), p(point){}

	// Ax + By + Cz + D = 0
	Plane::Plane(const Vector3& normal, real D)
		: norm(normal)
	{
		if (norm.x != 0)
			p = Vector3(-D / norm.x, 0, 0);
		else if (norm.y != 0)
			p = Vector3(0, -D / norm.y, 0);
		else if (norm.z != 0)
			p = Vector3(0, 0, -D / norm.z);
		else
			LogWarn("invalid plane normal vector");
	}

	IShape::Type Plane::GetType() const{
		return IShape::PLANE;
	}

	bool Plane::IntersectWith(const IShape* shapeB) const{
		switch (shapeB->GetType()){
		case IShape::RAY:{
				const Ray& ray = reinterpret_cast<const Ray&>(*shapeB);
				return tool::Ray_PlaneIntersection(ray.pos, ray.dir, ray.limit, norm, p);
			}
			break;
		case IShape::PLANE:
			break;
		case IShape::SPHERE:
			break;
		case IShape::AABOX:
			break;
		case IShape::OBOX:
			break;
		case IShape::CONVEXHULL:
			break;
		}
		return false;
	}


	// sphere
	Sphere::Sphere()
		: pos(0), r(1.f){}

	Sphere::Sphere(const Vector3& position, real radius)
		: pos(position), r(radius)
	{}

	IShape::Type Sphere::GetType() const{
		return IShape::SPHERE;
	}

	bool Sphere::IntersectWith(const IShape* shapeB) const{
		switch (shapeB->GetType()){
		case IShape::RAY:{
				const Ray& ray = reinterpret_cast<const Ray&>(*shapeB);
				return tool::Ray_SphereIntersection(ray.pos, ray.dir, ray.limit, pos, r);
			}
			break;
		case IShape::PLANE:
			break;
		case IShape::SPHERE:{
				const Sphere& sphereB = reinterpret_cast<const Sphere&>(*shapeB);
				return tool::Sphere_SphereIntersection(pos, r, sphereB.pos, sphereB.r);
			}
			break;
		case IShape::AABOX:{
				const AxisAlignedBox& aab = reinterpret_cast<const AxisAlignedBox&>(*shapeB);
				return tool::Sphere_AxisAlignedBoxIntersection(pos, r, aab.min, aab.max);
			}
			break;
		case IShape::OBOX:{
				const OrientedBox& ob = reinterpret_cast<const OrientedBox&>(*shapeB);
				return tool::Sphere_OrientedBoxIntersection(pos, r, ob.pos, ob.ext, ob.axis);
			}
			break;
		case IShape::CONVEXHULL:
			break;
		}
		return false;
	}


	// axis aligned box
	AxisAlignedBox::AxisAlignedBox()
		: min(-0.5f), max(0.5f){}

	AxisAlignedBox::AxisAlignedBox(const Vector3& posMin, const Vector3& posMax)
		: min(posMin), max(posMax){}

	AxisAlignedBox::AxisAlignedBox(const Vector3& center, real half_ext[3])
		: min(center.x - half_ext[0], center.y - half_ext[1], center.z - half_ext[2]),
		max(center.x + half_ext[0], center.y + half_ext[1], center.z + half_ext[2]){}

	IShape::Type AxisAlignedBox::GetType() const{
		return IShape::AABOX;
	}

	bool AxisAlignedBox::IntersectWith(const IShape* shapeB) const{
		switch (shapeB->GetType()){
		case IShape::RAY:{
				const Ray& ray = reinterpret_cast<const Ray&>(*shapeB);
				return tool::Ray_AxisAlignedBoxIntersection(ray.pos, ray.dir, ray.limit, min, max);
			}
			break;
		case IShape::PLANE:
			break;
		case IShape::SPHERE:{
				const Sphere& sphere = reinterpret_cast<const Sphere&>(*shapeB);
				return tool::Sphere_AxisAlignedBoxIntersection(sphere.pos, sphere.r, min, max);
			}
			break;
		case IShape::AABOX:{
				const AxisAlignedBox& aabB = reinterpret_cast<const AxisAlignedBox&>(*shapeB);
				return tool::AxisAlignedBox_AxisAlignedBoxIntersection(min, max, aabB.min, aabB.max);
			}
			break;
		case IShape::OBOX:{
				const OrientedBox& ob = reinterpret_cast<const OrientedBox&>(*shapeB);
				return tool::AxisAlignedBox_OrientedBoxIntersection(min, max, ob.pos, ob.ext, ob.axis);
			}
			break;
		case IShape::CONVEXHULL:
			break;
		}
		return false;
	}


	// oriented box
	OrientedBox::OrientedBox()
		: pos(0), ext(0.5f)
	{
		axis[0].Set(1, 0, 0); axis[1].Set(0, 1, 0); axis[2].Set(0, 0, 1);
	}

	OrientedBox::OrientedBox(const Vector3& position, const Vector3& extention)
		: pos(position), ext(extention)
	{
		axis[0].Set(1, 0, 0); axis[1].Set(0, 1, 0); axis[2].Set(0, 0, 1);
	}
	
	OrientedBox::OrientedBox(const Vector3& position, const Vector3& extention,
		const Vector3 _axis[3])
		: pos(position), ext(extention)
	{
		axis[0] = _axis[0]; axis[1] = _axis[1]; axis[2] = _axis[2];
	}

	IShape::Type OrientedBox::GetType() const{
		return IShape::OBOX;
	}

	bool OrientedBox::IntersectWith(const IShape* shapeB) const{
		switch (shapeB->GetType()){
		case IShape::RAY:{
				const Ray& ray = reinterpret_cast<const Ray&>(*shapeB);
				return tool::Ray_OrientedBoxIntersection(ray.pos, ray.dir, ray.limit, pos, ext, axis);
			}
			break;
		case IShape::PLANE:
			break;
		case IShape::SPHERE:{
				const Sphere& sphere = reinterpret_cast<const Sphere&>(*shapeB);
				return tool::Sphere_OrientedBoxIntersection(sphere.pos, sphere.r, pos, ext, axis);
			}
			break;
		case IShape::AABOX:{
				const AxisAlignedBox& aab = reinterpret_cast<const AxisAlignedBox&>(*shapeB);
				return tool::AxisAlignedBox_OrientedBoxIntersection(aab.min, aab.max, pos, ext, axis);
			}
			break;
		case IShape::OBOX:{
				const OrientedBox& obB = reinterpret_cast<const OrientedBox&>(*shapeB);
				return tool::OrientedBox_OrientedBoxIntersection(pos, ext, axis, obB.pos, obB.ext, obB.axis);
			}
			break;
		case IShape::CONVEXHULL:
			break;
		}
		return false;
	}

	void OrientedBox::SetAxis(const Matrix4& TM){
		// to orient the object, it needs the rotation matrix R.
		// therefore, transposition of R can transform the object
		// in the world-space into the origin of the object-space.
		// it means that each rows of transposition of R becomes
		// the orthonormal basis of the object-space.
		axis[0].Set(TM.m[0][0], TM.m[1][0], TM.m[2][0]);
		axis[1].Set(TM.m[0][1], TM.m[1][1], TM.m[2][1]);
		axis[2].Set(TM.m[0][2], TM.m[1][2], TM.m[2][2]);
	}
	
}
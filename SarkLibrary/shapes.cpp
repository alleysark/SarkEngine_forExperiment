#include "shapes.h"
#include "Mesh.h"
#include "Transform.h"
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

	Ray::Ray() : IShape(IShape::RAY){}

	Ray::Ray(const Position3& position, const Vector3& direction, bool dir_normalized, real limitation)
		: IShape(IShape::RAY), pos(position), dir(direction), limit(limitation)
	{
		if (!dir_normalized)
			dir.Normalize();
	}

	void Ray::Set(const Position3& position, const Vector3& direction, bool dir_normalized, real limitation){
		pos = position;
		dir = direction;
		limit = limitation;

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


	Sphere::Sphere() : IShape(IShape::SPHERE){}

	Sphere::Sphere(const Point3& position, real radius)
		: IShape(IShape::SPHERE), pos(position), r(radius)
	{}

	void Sphere::Set(const Point3& position, real radius){
		pos = position;
		r = radius;
	}

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


	AxisAlignedBox::AxisAlignedBox() : IShape(IShape::AABOX){}

	AxisAlignedBox::AxisAlignedBox(const Position3& position, real extention[3])
		: IShape(IShape::AABOX), pos(position), ext(extention[0], extention[1], extention[2])
	{}

	AxisAlignedBox::AxisAlignedBox(const Point3& posMin, const Point3& posMax)
		: IShape(IShape::AABOX)
	{
		pos = (posMin + posMax) / 2.f;
		ext = (posMax - posMin) / 2.f;
	}

	void AxisAlignedBox::Set(const Position3& position, real extention[3]){
		pos = position;
		ext = { extention[0], extention[1], extention[2] };
	}
	void AxisAlignedBox::Set(const Point3& posMin, const Point3& posMax){
		pos = (posMin + posMax) / 2.f;
		ext = (posMax - posMin) / 2.f;
	}

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


	OrientedBox::OrientedBox() : IShape(IShape::OBOX){}

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

	void OrientedBox::Set(const Position3& position,
		const Vector4& basisXext, const Vector4& basisYext, const Vector4& basisZext,
		bool basis_normalized){
		pos = position;
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


	Polyhedron::Polyhedron() : IShape(IShape::POLYHEDRON){}

	Polyhedron::Polyhedron(Mesh* referenceMesh, Transform* referenceTransform)
		: IShape(IShape::POLYHEDRON), refMesh(referenceMesh), refTransform(referenceTransform)
	{}

	void Polyhedron::Set(Mesh* referenceMesh, Transform* referenceTransform){
		refMesh = referenceMesh;
		refTransform = referenceTransform;
	}

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
	//	* http://paulbourke.net/geometry/
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
	bool IsIntersected(const Ray* ray, const AxisAlignedBox* aabox){
		if (ContainsIn(aabox, ray->pos)){
			return true;
		}
		if (ray->limit < REAL_MAX && ContainsIn(aabox, ray->pos + ray->limit*ray->dir)){
			return true;
		}

		real t_min = 0;
		real t_max = REAL_MAX;;

		for (int i = 0; i<3; i++){
			if (math::real_equal(ray->dir.v[i], 0.f)){
				if (ray->pos.v[i] < (aabox->pos.v[i] - aabox->ext.v[i]) ||
					ray->pos.v[i] > (aabox->pos.v[i] + aabox->ext.v[i]))
					return false;
			}
			else{
				float t1 = (-ray->pos.v[i] - (aabox->pos.v[i] - aabox->ext.v[i])) / ray->dir.v[i];
				float t2 = (-ray->pos.v[i] - (aabox->pos.v[i] + aabox->ext.v[i])) / ray->dir.v[i];
				if (t1 > t2){
					std::swap(t1, t2);
				}
				t_min = math::max(t_min, t1);
				t_max = math::min(t_max, t2);
				if (t_min > t_max)
					return false;
			}
		}

		if (t_min > ray->limit)
			return false;
		return true;
	}
	
	// ray - oriented box intersection
	bool IsIntersected(const Ray* ray, const OrientedBox* obox){
		Matrix4 obMat;
		for (integer i = 0; i < 3; i++){
			obMat.row[i].xyz = obox->axis[i].xyz;
			obMat.row[i].w = -obox->pos.Dot(obox->axis[i].xyz);
		}
		obMat.row[3].Set(0, 0, 0, 1);

		Ray transRay(
			(obMat*Vector4(ray->pos, 1)).xyz, 
			Vector3(obox->axis[0].xyz.Dot(ray->dir), obox->axis[1].xyz.Dot(ray->dir), obox->axis[2].xyz.Dot(ray->dir)),
			true,ray->limit);
		AxisAlignedBox aabox(Position3(-obox->axis[0].w, -obox->axis[1].w, -obox->axis[2].w),
			Position3(obox->axis[0].w, obox->axis[1].w, obox->axis[2].w));
		return IsIntersected(&transRay, &aabox);
	}
	
	// ray - polyhedron intersection
	bool IsIntersected(const Ray* ray, const Polyhedron* poly){
		const std::vector<Position3>& vertices = poly->refMesh->GetPositions();
		const std::vector<Mesh::Face>& faces = poly->refMesh->GetFaces();

		// transform ray into object space.
		const Matrix4& TM = poly->refTransform->GetMatrix();
		Matrix4 invTRS = TM.Inverse();
		Matrix3 invRS = Matrix3(
			TM.row[0].xyz,
			TM.row[1].xyz,
			TM.row[2].xyz).Inverse();
		Ray os_ray(
			(invTRS * Vector4(ray->pos, 1)).xyz,
			invRS * ray->dir, true,
			ray->limit);

		std::vector<Mesh::Face>::const_iterator itr = faces.begin();
		std::vector<Mesh::Face>::const_iterator end = faces.end();
		for (; itr != end; itr++){
			const Vector3& A = vertices[itr->a];
			const Vector3& B = vertices[itr->b];
			const Vector3& C = vertices[itr->c];
			
			// barycentric form of triangle plane =>
			// P(v,w) = (1 - v - w)A + vB + wC; {0<=v<=1, 0<=w<=1, 0<=v+w<=1}

			// parametric equation of ray =>
			// r(t) = o + t*d;
			// 'o' is start position vector, 'd' is normalized direction vector

			// if the ray intersects with triangle, it'll satisfy the equations below.
			// P(v,w)=r(t)
			// => (1 - v - w)A + vB + wC = o + t*d
			// => (-d)t + (B-A)v + (C-A)w = o - A;
			// let these to be represented as follows,
			Vector3 e1 = B - A;
			Vector3 e2 = C - A;
			Vector3 s = os_ray.pos - A;

			// and it is linear equation with three variables.
			//                [t]
			// [(-d) e1 e2] x [v] = s;
			//                [w]
			
			// from Cramer's rule -
			// "Ax = b  =>  x_i = det(A_i)/det(A),
			//  where A_i  is the matrix formed by replacing
			//  the ith column of A  by the column vector b" -
			// ,we can get the results.
			// (well,, it's little hard to express whole equations)

			// result can be summarized by some cross-product rules.
			// [t]    1     [s ， (e1 x e2)]    1     [(s x e1)，e2]
			// [v] = ---  x [(-d)，(s X e2)] = ---  x [(d x e2)，s ]
			// [w]  det(A)  [(-d)，(e1 X s)]  det(A)  [(s x e1)，d ]
			// , where det(A) is (e2 x e1)，d

			Vector3 p = os_ray.dir.Cross(e2);
			Vector3 q = s.Cross(e1);

			real det = p.Dot(e1);
			if (math::real_equal(det, 0))
				continue;
			det = 1.f / det;

			real t = det * q.Dot(e2);
			real v = det * p.Dot(s);
			real w = det * q.Dot(os_ray.dir);

			// from t,v and w, it can be tested whether ray intersects with triangle or not
			if (0 <= t && t <= os_ray.limit){
				if ((v >= 0) && (w >= 0) && (v + w <= 1))
					return true;
			}
		}
		return false;
	}


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



	// assistance functions

}
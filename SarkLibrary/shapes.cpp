#include "shapes.h"
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


	// sphere
	Sphere::Sphere()
		: pos(0), r(1.f){}

	Sphere::Sphere(const Vector3& position, real radius)
		: pos(position), r(radius)
	{}

	IShape::Type Sphere::GetType() const{
		return IShape::SPHERE;
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


	// oriented box
	OrientedBox::OrientedBox()
		: pos(0), ext(0.5f), rot(true){}

	OrientedBox::OrientedBox(const Vector3& position, const Vector3& extention)
		: pos(position), ext(extention), rot(true){}
	
	OrientedBox::OrientedBox(const Vector3& position, const Vector3& extention,
		const Matrix3& rotation)
		: pos(position), ext(extention), rot(rotation){}

	IShape::Type OrientedBox::GetType() const{
		return IShape::OBOX;
	}


	// convex hull
	ConvexHull::ConvexHull(){}

	IShape::Type ConvexHull::GetType() const{
		return IShape::CONVEXHULL;
	}



	// ======================================================
	//		intersection check functions of basic shapes
	//
	//				referenced documents
	//  * Mathematics for computer graphics | John Vince | Springer | 2010.03.01
	//  * 3D graphics for game programming | JungHyun Han | CRC Press | 2011.01.01
	//	* http://paulbourke.net/geometry/
	//  * http://geomalgorithms.com/a06-_intersect-2.html
	// ======================================================

	
	// ray-plane intersection check.
	bool Ray_PlaneIntersection(
		const Vector3& ray_p, const Vector3& ray_v,
		const Vector3& plane_n, const Vector3& plane_p,
		real* out_t)
	{
		// whether the line vector and plane normal are perpendicular each other.
		if (ray_v.Dot(plane_n) == 0)
			return false;

		// Plane(x,y,z) = n_x*x + n_y*y + n_z*z + D = 0
		real minus_D = plane_n.Dot(plane_p);

		if (out_t != NULL){
			// (1): P(t) = ray_p + ray_v*t
			// (2): Plane(x,y,z) = n_x*x + n_y*y + n_z*z + D = 0
			*out_t = (minus_D - plane_n.Dot(ray_p)) / plane_n.Dot(ray_v);
		}
		return true;
	}

	// ray-sphere intersection test.
	int8 Ray_SphereIntersection(
		const Vector3& ray_p, const Vector3& ray_v,
		const Vector3& sphere_p, const real& sphere_r,
		real* out_t1, real* out_t2)
	{
		Vector3 p_o = ray_p - sphere_p;
		real a = ray_v.MagnitudeSq();
		real b = ray_v.Dot(p_o); //half of b
		real c = p_o.MagnitudeSq() - math::sqre(sphere_r);

		real det = math::sqre(b) - a*c;
		if (det < 0){
			return 0;
		}
		else if (det == 0){
			if (out_t1!=NULL)
				*out_t1 = -b / a;
			return 1;
		}
		else{//det>0
			det = math::sqrt(det);

			if (out_t1!=NULL)
				*out_t1 = (-b - det) / a;
			if (out_t2!=NULL)
				*out_t2 = (-b + det) / a;
			return 2;
		}
	}

	// ray-axis aligned box intersection test.
	bool Ray_AxisAlignedBoxIntersection(
		const Vector3& ray_p, const Vector3& ray_v,
		const Vector3& aab_min, const Vector3& aab_max,
		real* out_t1, real* out_t2)
	{
		real t_min = -REAL_MAX;
		real t_max = REAL_MAX;;

		for (integer i = 0; i<3; i++){
			if (math::real_equal(ray_v.v[i], 0.f)){
				if (ray_p.v[i] < aab_min.v[i] || aab_max.v[i] < ray_p.v[i]){
					return false;
				}
			}
			else{
				real t1 = (aab_min.v[i] - ray_p.v[i]) / ray_v.v[i];
				real t2 = (aab_max.v[i] - ray_p.v[i]) / ray_v.v[i];
				if (t1 > t2){
					std::swap(t1, t2);
				}

				if (t1 > t_min)
					t_min = t1;
				if (t2 < t_max)
					t_max = t2;

				if (t_min > t_max)
					return false;
			}
		}

		if (out_t1 != NULL)
			*out_t1 = t_min;
		if (out_t2 != NULL)
			*out_t2 = t_max;
		return true;
	}

	// ray - oriented box intersection test.
	bool Ray_OrientedBoxIntersection(
		const Vector3& ray_p, const Vector3& ray_v,
		const Vector3& ob_p, const Vector3& ob_ext, const Matrix3& ob_rot,
		real* out_t1, real* out_t2)
	{
		// transform ray into oriented box space.
		// it seems like the camera transformation
		// with Eye as ob_p, and basis as ob_rot matrix.
		Vector3 transray_p
			= Vector3(
			ob_rot.row[0].Dot(ray_p) - ob_rot.row[0].Dot(ob_p),
			ob_rot.row[1].Dot(ray_p) - ob_rot.row[1].Dot(ob_p),
			ob_rot.row[2].Dot(ray_p) - ob_rot.row[2].Dot(ob_p));
		Vector3 transray_v
			= Vector3(
			ob_rot.row[0].Dot(ray_v),
			ob_rot.row[1].Dot(ray_v),
			ob_rot.row[2].Dot(ray_v));

		// from now, oriented box is regarded as axis aligned box.
		return Ray_AxisAlignedBoxIntersection(
			transray_p, transray_v,
			-ob_ext, ob_ext,
			out_t1, out_t2);
	}

	// ray intersection test as regards the barycentric coordinates equation.
	bool Ray_BarycentricCoordIntersection(
		const Vector3& ray_p, const Vector3& ray_v,
		const Vector3& bc_o, const Vector3& bc_e1, const Vector3& bc_e2,
		real* out_t, real* out_u, real* out_v)
	{
		// parametric equation of line =>
		// L(t) = ray_p + ray_v*t

		// if the line intersects with barycentric plane,
		// it'll satisfy the equations below.
		// P(u,v) = L(t)
		// => -(ray_v)t + (e1)u + (e2)v = ray_p - origin

		// for your accommodation, let us represent ray_p - origin as 's'.
		Vector3 s = ray_p - bc_o;

		// and it is linear equation with three variables.
		//                [t]
		// [(-d) e1 e2] x [u] = s;
		//                [v]

		// from Cramer's rule -
		// "Ax = b  =>  x_i = det(A_i)/det(A),
		//  where A_i  is the matrix formed by replacing
		//  the ith column of A  by the column vector b" -
		// ,we can get the results.
		// (well,, it's little hard to express whole equations)

		// result can be summarized by some cross-product rules.
		// [t]    1     [s ， (e1 x e2)]    1     [(s x e1)，e2]
		// [u] = ---  x [(-d)，(s X e2)] = ---  x [(d x e2)，s ]
		// [v]  det(A)  [(-d)，(e1 X s)]  det(A)  [(s x e1)，d ]
		// , where det(A) is (e2 x e1)，d

		Vector3 p = ray_v.Cross(bc_e2);
		Vector3 q = s.Cross(bc_e1);

		real det = p.Dot(bc_e1);
		if (math::real_equal(det, 0))
			return false;
		det = 1.f / det;

		if (out_t != NULL)
			*out_t = det * q.Dot(bc_e2);
		if (out_u != NULL)
			*out_u = det * p.Dot(s);
		if (out_v != NULL)
			*out_v = det * q.Dot(ray_v);

		return true;
	}

	// triangle intersection test as regards the barycentric coordinates equation.
	bool Triangle_BarycentricCoordIntersection(
		const Vector3& A, const Vector3& B, const Vector3& C,
		const Vector3& bc_o, const Vector3& bc_e1, const Vector3& bc_e2,
		Vector3* out_P, Vector3* out_Q,
		real* out_Pu, real* out_Pv,
		real* out_Qu, real* out_Qv)
	{
		// normal vector of barycentric coordinates plane.
		Vector3 n = bc_e1.Cross(bc_e2);

		// point locations.
		int8 A_loc = PointLocationByPlane(A, n, bc_o);
		int8 B_loc = PointLocationByPlane(B, n, bc_o);
		int8 C_loc = PointLocationByPlane(C, n, bc_o);

		// zero location (on the plane) handling.
		if (A_loc == 0 || B_loc == 0 || C_loc == 0){
			// it is too hard to handle this situations..
			return false;
		}

		// determin which edge do we take.
		Vector3 v1, v2;
		if (A_loc == B_loc){
			if (A_loc == C_loc)
				return false;
			v1 = C - A;// line A-C
			v2 = B - C;// line C-B
		}
		else if (A_loc == C_loc){
			v1 = B - A;// line A-B
			v2 = B - C;// line C-B	
		}
		else{
			v1 = B - A;// line A-B
			v2 = A - C;// line C-A
		}

		// parameters for P and Q.
		real pt = 0, qt = 0;

		if (!Ray_BarycentricCoordIntersection(A, v1, bc_o, bc_e1, bc_e2,
			&pt, out_Pu, out_Pv))
			return false;
		if (pt < 0 || 1 < pt)
			return false;

		if (!Ray_BarycentricCoordIntersection(C, v2, bc_o, bc_e1, bc_e2,
			&qt, out_Qu, out_Qv))
			return false;
		if (qt < 0 || 1 < qt)
			return false;

		if (out_P != NULL)
			*out_P = A + v1*pt;
		if (out_Q != NULL)
			*out_Q = C + v2*qt;
		return true;
	}


	// triangle - triangle intersection test.
	bool Triangle_TriangleIntersection(
		const Vector3& A1, const Vector3& B1, const Vector3& C1,
		const Vector3& A2, const Vector3& B2, const Vector3& C2,
		Vector3* out_P, Vector3* out_Q)
	{
		Vector3 e1 = B2 - A2;
		Vector3 e2 = C2 - A2;
		Vector3 P1, Q1;
		if (!Triangle_BarycentricCoordIntersection(A1, B1, C1,
			A2, e1, e2, &P1, &Q1))
			return false;

		e1 = B1 - A1;
		e2 = C1 - A1;
		Vector3 P2, Q2;
		if (!Triangle_BarycentricCoordIntersection(A2, B2, C2,
			A1, e1, e2, &P2, &Q2))
			return false;

		// two segments P1-Q1 and P2-Q2 are collinear.
		// take one axis and test overlap.
		uinteger i = 0;
		for (i = 0; i<3; i++){
			if (P1.v[i] != Q1.v[i])
				break;
		}
		if (i == 3){
			for (i = 0; i<3; i++){
				if (P2.v[i] != Q2.v[i])
					break;
			}
			if (i == 3){
				if (P1 == P2){
					// two triangles intersect at a point.
					// it is extremely special case.
					*out_P = P1;
					*out_Q = P1;
					return true;
				}
				return false;
			}
		}

		// sort P1-Q1 and P2-Q2
		if (Q1.v[i] < P1.v[i])
			std::swap(P1, Q1);
		if (Q2.v[i] < P2.v[i])
			std::swap(P2, Q2);


		// possible segment overlapping cases.
		// (1)P2----Q2  (4)P2----Q2       (6)P2----Q2
		//	    (2)P2----Q2     (5)P2----Q2
		//	    (3)P2--------------------Q2
		//	           P1------------Q1
		if (P2.v[i] < P1.v[i]){
			if (Q2.v[i] < P1.v[i]){
				return false; //(1). not overlap
			}
			else if (Q2.v[i] <= Q1.v[i]){
				*out_P = P1; //(2)
				*out_Q = Q2;
			}
			else{
				*out_P = P1; //(3)
				*out_Q = Q1;
			}
		}
		else if (P2.v[i] <= Q1.v[i]){
			if (Q2.v[i] <= Q1.v[i]){
				*out_P = P2; //(4)
				*out_Q = Q2;
			}
			else{
				*out_P = P2; //(5)
				*out_Q = Q1;
			}
		}
		else{
			return false; //(6)
		}
		return true;
	}

	// sphere - sphere intersection test.
	bool Sphere_SphereIntersection(
		const Vector3& sphere1_p, const real& sphere1_r,
		const Vector3& sphere2_p, const real& sphere2_r,
		Vector3* out_P)
	{
		real distSq = (sphere1_p - sphere2_p).MagnitudeSq();
		if (distSq > math::sqre(sphere1_r + sphere2_r)){
			return false;
		}

		if (out_P != NULL){
			*out_P = (sphere1_p + sphere2_p) / 2.f;
		}
		return true;
	}

	// axis aligned box - axis aligned box intersection test.
	bool AxisAlignedBox_AxisAlignedBoxIntersection(
		const Vector3& aab1_min, const Vector3& aab1_max,
		const Vector3& aab2_min, const Vector3& aab2_max)
	{
		for (integer i = 0; i < 3; i++){
			if (aab1_min.v[i] > aab2_max.v[i]
				|| aab1_max.v[i] < aab2_min.v[i]){
				return false;
			}
		}
		return true;
	}

}
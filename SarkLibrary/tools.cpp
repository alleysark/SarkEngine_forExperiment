#include "tools.h"

namespace sark{
	namespace tool{

		// get farthest point in given specific direction.
		const Vector3 FarthestPointInDirection(
			const std::vector<Vector3>& pointSet, const Vector3& direction)
		{
			integer idx = 0;
			real maxd = -REAL_MAX;
			uinteger size = pointSet.size();
			for (uinteger i = 0; i < size; i++){
				real d = direction.Dot(pointSet[i]);
				if (maxd < d){
					maxd = d;
					idx = i;
				}
			}
			return pointSet[idx];
		}

		// ======================================================
		//		intersection check functions of basic shapes
		//
		//				referenced documents
		//  * Mathematics for computer graphics | John Vince | Springer | 2010.03.01
		//  * 3D graphics for game programming | JungHyun Han | CRC Press | 2011.01.01
		//  * Real-Time Collision Detection | Christer Ericson | Morgan Kaufmann Pub. | 2005.02.07
		//	* http://paulbourke.net/geometry/
		//  * http://geomalgorithms.com/a06-_intersect-2.html
		// ======================================================

		// ray-triangle intersection test.
		bool Ray_TriangleIntersection(
			const Vector3& ray_p, const Vector3& ray_v, const real& ray_l,
			const Vector3& A, const Vector3& B, const Vector3& C,
			Vector3* out_P)
		{
			Vector3 e1 = B - A;
			Vector3 e2 = C - A;
			real t, u, v;
			if (!Ray_BarycentricCoordIntersection(ray_p, ray_v, A, e1, e2, &t, &u, &v)){
				return false;
			}

			if (t < 0 || ray_l < t)
				return false;
			if (u < 0 || v<0 || (u + v)>1)
				return false;
			
			if (out_P != NULL)
				*out_P = ray_p + t*ray_v;
			return true;
		}

		// ray-plane intersection check.
		bool Ray_PlaneIntersection(
			const Vector3& ray_p, const Vector3& ray_v, const real& ray_l,
			const Vector3& plane_n, const Vector3& plane_p,
			Vector3* out_P)
		{
			// whether the line vector and plane normal are perpendicular each other.
			real n_dot_v = plane_n.Dot(ray_v);
			if (n_dot_v == 0){
				if (PointLocationByPlane(ray_p, plane_n, plane_p) != 0)
					return false;
				// ray lying on the plane.
				if (out_P != NULL)
					*out_P = ray_p + ray_v*(ray_l / 2.f);
				return true;
			}

			// Plane(x,y,z) = n_x*x + n_y*y + n_z*z + D = 0
			real minus_D = plane_n.Dot(plane_p);

			// (1): P(t) = ray_p + ray_v*t
			// (2): Plane(x,y,z) = n_x*x + n_y*y + n_z*z + D = 0
			real t = (minus_D - plane_n.Dot(ray_p)) / n_dot_v;

			if (t < 0 || ray_l < t)
				return false;

			if (out_P != NULL)
				*out_P = ray_p + ray_v*t;
			return true;
		}

		// ray-plane intersection test.
		bool Ray_PlaneIntersection(
			const Vector3& ray_p, const Vector3& ray_v, const real& ray_l,
			const Vector3& plane_n, const real& plane_d,
			Vector3* out_P)
		{
			// whether the line vector and plane normal are perpendicular each other.
			real n_dot_v = plane_n.Dot(ray_v);
			if (n_dot_v == 0){
				// compute a point on the plane.
				Vector3 p(0.f);
				for (uinteger i = 0; i < 3; i++){
					if (plane_n.v[i] != 0){
						p.v[i] = -plane_d / plane_n.v[i];
						break;
					}
				}
				if (PointLocationByPlane(ray_p, plane_n, p) != 0)
					return false;
				// ray lying on the plane.
				if (out_P != NULL)
					*out_P = ray_p + ray_v*(ray_l / 2.f);
				return true;
			}

			real t = (-plane_d - plane_n.Dot(ray_p)) / n_dot_v;

			if (t < 0 || ray_l < t)
				return false;

			if (out_P != NULL)
				*out_P = ray_p + ray_v*t;
			return true;
		}

		// ray-sphere intersection test.
		bool Ray_SphereIntersection(
			const Vector3& ray_p, const Vector3& ray_v, const real& ray_l,
			const Vector3& sphere_p, const real& sphere_r,
			Vector3* out_P)
		{
			Vector3 p_o = ray_p - sphere_p;
			real a = ray_v.MagnitudeSq();
			real b = ray_v.Dot(p_o); //half of b
			real c = p_o.MagnitudeSq() - math::sqre(sphere_r);

			real det = math::sqre(b) - a*c;
			if (det < 0){
				return false;
			}

			real t = 0;
			if (det == 0){
				t = -b / a;
				if (t < 0 || ray_l < t)
					return false;
			}
			else{//det>0
				det = math::sqrt(det);

				t = (-b - det) / a;
				if (t > ray_l)
					return false;

				if (t < 0){
					t = (-b + det) / a;
					if (t < 0 || ray_l < t)
						return false;
				}
			}

			if (out_P != NULL)
				*out_P = ray_p + ray_v*t;
			return true;
		}

		// ray-axis aligned box intersection test.
		bool Ray_AABoxIntersection(
			const Vector3& ray_p, const Vector3& ray_v, const real& ray_l,
			const Vector3& aab_min, const Vector3& aab_max,
			Vector3* out_P)
		{
			real t_min = -REAL_MAX;
			real t_max = REAL_MAX;
			real t1, t2;

			for (integer i = 0; i<3; i++){
				if (math::real_equal(ray_v.v[i], 0.f)){
					if (ray_p.v[i] < aab_min.v[i] || aab_max.v[i] < ray_p.v[i]){
						return false;
					}
				}
				else{
					t1 = (aab_min.v[i] - ray_p.v[i]) / ray_v.v[i];
					t2 = (aab_max.v[i] - ray_p.v[i]) / ray_v.v[i];
					if (t1 > t2){
						std::swap(t1, t2);
					}

					if (t1 > t_min)
						t_min = t1;
					if (t2 < t_max)
						t_max = t2;

					if ((t_min > t_max) || (t_min > ray_l))
						return false;
				}
			}

			if (t_min < 0){
				if (t_max > ray_l)
					return false;
				t_min = t_max;
			}
			if (out_P != NULL)
				*out_P = ray_p + ray_v*t_min;
			return true;
		}

		// ray - oriented box intersection test.
		bool Ray_OBoxIntersection(
			const Vector3& ray_p, const Vector3& ray_v, const real& ray_l,
			const Vector3& ob_p, const Vector3& ob_ext, const Vector3 ob_axis[3],
			Vector3* out_P)
		{
			// transform ray into oriented box space.
			// it seems like the camera transformation
			// with Eye as ob_p, and basis as ob_rot matrix.
			Vector3 transray_p
				= Vector3(
				ob_axis[0].Dot(ray_p) - ob_axis[0].Dot(ob_p),
				ob_axis[1].Dot(ray_p) - ob_axis[1].Dot(ob_p),
				ob_axis[2].Dot(ray_p) - ob_axis[2].Dot(ob_p));
			Vector3 transray_v
				= Vector3(
				ob_axis[0].Dot(ray_v),
				ob_axis[1].Dot(ray_v),
				ob_axis[2].Dot(ray_v));

			real t_min = -REAL_MAX;
			real t_max = REAL_MAX;

			Vector3 aab_min = -ob_ext;
			const Vector3& aab_max = ob_ext;

			for (integer i = 0; i<3; i++){
				if (math::real_equal(transray_v.v[i], 0.f)){
					if (transray_p.v[i] < aab_min.v[i] || aab_max.v[i] < transray_p.v[i]){
						return false;
					}
				}
				else{
					real t1 = (aab_min.v[i] - transray_p.v[i]) / transray_v.v[i];
					real t2 = (aab_max.v[i] - transray_p.v[i]) / transray_v.v[i];
					if (t1 > t2){
						std::swap(t1, t2);
					}

					if (t1 > t_min){
						t_min = t1;
						if (t_min > ray_l)
							return false;
					}
					if (t2 < t_max){
						t_max = t2;
						if (t_max < 0)
							return false;
					}

					if ((t_min > t_max))
						return false;
				}
			}

			if (t_min < 0){
				if (t_max > ray_l)
					return false;
				t_min = t_max;
			}
			if (out_P != NULL)
				*out_P = ray_p + ray_v*t_min;
			return true;
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
		bool Triangle_PlaneIntersection(
			const Vector3& A, const Vector3& B, const Vector3& C,
			const Vector3& plane_n, const Vector3& plane_p,
			Vector3* out_P, Vector3* out_Q)
		{
			// point locations.
			int8 A_loc = PointLocationByPlane(A, plane_n, plane_p);
			int8 B_loc = PointLocationByPlane(B, plane_n, plane_p);
			int8 C_loc = PointLocationByPlane(C, plane_n, plane_p);

			// possible position of A,B and C.
			// +  o   o   o   ∴ n   
			// 0 -o---o---o-- Κ plane
			// -  o   o   o
			// , there are 27 cases.
			if (A_loc == 0){
				if (B_loc == 0){
					if (C_loc == 0){
						// [1]. ABC is lying on the plane
						Vector3 CM = (A + B + C) / 3.f;
						if (out_P != NULL) *out_P = CM;
						if (out_Q != NULL) *out_Q = CM;
					}
					else{
						// [2]. edge A-B is lying ~.
						if (out_P != NULL) *out_P = A;
						if (out_Q != NULL) *out_Q = B;
					}
				}
				else{
					if (C_loc == 0){
						// [2]. edge A-C is lying ~.
						if (out_P != NULL) *out_P = A;
						if (out_Q != NULL) *out_Q = C;
					}
					else{
						if (B_loc == C_loc){
							// [2]. point A is lying ~.
							if (out_P != NULL) *out_P = A;
							if (out_Q != NULL) *out_Q = A;
						}
						else{
							// [2]. point A is lying ~,
							// edge B-C is intersected with plane.
							if (out_P != NULL) *out_P = A;
							Ray_PlaneIntersection(B, C - B, 1, plane_n, plane_p, out_Q);
						}
					}
				}
			}
			else{
				if (B_loc == 0){
					if (C_loc == 0){
						// [2]. edge B-C is lying ~.
						if (out_P != NULL) *out_P = B;
						if (out_Q != NULL) *out_Q = C;
					}
					else{
						if (A_loc == C_loc){
							// [2]. point B is lying ~.
							if (out_P != NULL) *out_P = B;
							if (out_Q != NULL) *out_Q = B;
						}
						else{
							// [2]. point B is lying ~,
							// edge A-C is intersected ~.
							if (out_P != NULL) *out_P = B;
							Ray_PlaneIntersection(A, C - A, 1, plane_n, plane_p, out_Q);
						}
					}
				}
				else{
					if (C_loc == 0){
						if (A_loc == B_loc){
							// [2]. point C is lying ~.
							if (out_P != NULL) *out_P = C;
							if (out_Q != NULL) *out_Q = C;
						}
						else{
							// [2]. point C is lying ~,
							// edge A-B is intersected ~.
							if (out_P != NULL) *out_P = C;
							Ray_PlaneIntersection(A, B - A, 1, plane_n, plane_p, out_Q);
						}
					}
					else{
						if (A_loc == B_loc){
							if (A_loc == C_loc){
								// [2]. ABC is in the sky.
								// there are no intersections.
								return false;
							}
							else{
								// [2]. edge A-C and B-C is intersected ~.
								Ray_PlaneIntersection(A, C - A, 1, plane_n, plane_p, out_P);
								Ray_PlaneIntersection(B, C - B, 1, plane_n, plane_p, out_Q);
							}
						}
						else{
							if (A_loc == C_loc){
								// [2]. edge A-B and B-C is intersected ~.
								Ray_PlaneIntersection(A, B - A, 1, plane_n, plane_p, out_P);
								Ray_PlaneIntersection(B, C - B, 1, plane_n, plane_p, out_Q);
							}
							else{
								// [2]. edge A-B and A-C is intersected ~.
								Ray_PlaneIntersection(A, B - A, 1, plane_n, plane_p, out_P);
								Ray_PlaneIntersection(A, C - A, 1, plane_n, plane_p, out_Q);
							}
						}
					}
				}
			}
			return true;
		}


		// triangle - triangle intersection test.
		bool Triangle_TriangleIntersection(
			const Vector3& A1, const Vector3& B1, const Vector3& C1,
			const Vector3& A2, const Vector3& B2, const Vector3& C2,
			Vector3* out_P, Vector3* out_Q,
			Vector3* out_n)
		{
			Vector3 n = (B2 - A2).Cross(C2 - A2);
			Vector3 P1, Q1;
			if (!Triangle_PlaneIntersection(A1, B1, C1, n, A2, &P1, &Q1))
				return false;

			if (out_n != NULL)
				*out_n = n.Normal();

			n = (B1 - A1).Cross(C1 - A1);
			Vector3 P2, Q2;
			if (!Triangle_PlaneIntersection(A2, B2, C2, n, A1, &P2, &Q2))
				return false;

			// two segments P1-Q1 and P2-Q2 are collinear.
			// take one axis and test overlap.
			uinteger i = 0;
			for (i = 0; i<3; i++){
				if (!math::almost_equal(P1.v[i], Q1.v[i]))
					break;
			}
			if (i == 3){
				for (i = 0; i<3; i++){
					if (!math::almost_equal(P2.v[i], Q2.v[i]))
						break;
				}
				if (i == 3){
					// P1==Q1 , P2==Q2
					if (P1 == P2){
						// two triangles intersect at a point.
						// it is super extremely special case.
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

		// sphere - axis aligned box intersection test.
		bool Sphere_AABoxIntersection(
			const Vector3& sphere_p, const real& sphere_r,
			const Vector3& aab_min, const Vector3& aab_max)
		{
			real distSq = math::sqre(sphere_r);
			for (uinteger i = 0; i < 3; i++){
				if (sphere_p.v[i] < aab_min.v[i])
					distSq -= math::sqre(sphere_p.v[i] - aab_min.v[i]);
				else if (sphere_p.v[i] > aab_max.v[i])
					distSq -= math::sqre(sphere_p.v[i] - aab_max.v[i]);
			}
			return (distSq >= 0);
		}

		// sphere - oriented box intersection test.
		bool Sphere_OBoxIntersection(
			const Vector3& sphere_p, const real& sphere_r,
			const Vector3& ob_p, const Vector3& ob_ext, const Vector3 ob_axis[3])
		{
			// transform sphere into oriented box space.
			// it seems like the camera transformation
			// with Eye as ob_p, and basis as ob_rot matrix.
			Vector3 transSphere_p
				= Vector3(
				ob_axis[0].Dot(sphere_p) - ob_axis[0].Dot(ob_p),
				ob_axis[1].Dot(sphere_p) - ob_axis[1].Dot(ob_p),
				ob_axis[2].Dot(sphere_p) - ob_axis[2].Dot(ob_p));

			// from now, oriented box is regarded as axis aligned box.
			return Sphere_AABoxIntersection(
				transSphere_p, sphere_r,
				-ob_ext, ob_ext);
		}

		// axis aligned box - axis aligned box intersection test.
		bool AABox_AABoxIntersection(
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

		// axis aligned box - oriented box intersection test.
		// *note: code from ch.4.4 Oriented Bounding Boxes(OBBs)
		// of <Real-Time Collision Detection>.
		bool AABox_OBoxIntersection(
			const Vector3& aab_min, const Vector3& aab_max,
			const Vector3& ob_p, const Vector3& ob_ext, const Vector3 ob_axis[3])
		{
			real ra = 0.f, rb = 0.f;
			Vector3 aab_ext = (aab_max - aab_min) / 2.f;
			Vector3 t = ob_p - ((aab_min + aab_max) / 2.f);

			Matrix3 absR;
			for (uinteger i = 0; i < 3; i++){
				for (uinteger j = 0; j < 3; j++){
					absR.m[i][j] = math::abs(ob_axis[j].v[i]) + math::EPSILON;
				}
			}

			// test axes L: A0, A1, A2
			for (uinteger i = 0; i < 3; i++){
				ra = aab_ext.v[i];
				rb = ob_ext.Dot(absR.row[i]); //projection ob's ext into aab space.
				if (math::abs(t.v[i]) > ra + rb)
					return false;
			}

			// test axes L: B0, B1, B2
			// combine aab with transposition of rotation matrix
			// to transform aab into ob's coordinate.
			for (uinteger i = 0; i < 3; i++){
				ra = absR.m[0][i] * aab_ext.x + absR.m[1][i] * aab_ext.y
					+ absR.m[2][i] * aab_ext.z;
				rb = ob_ext.v[i];
				if (math::abs(absR.m[0][i] * t.x
					+ absR.m[1][i] * t.y + absR.m[2][i] * t.z) > ra + rb)
					return false;
			}

			// Test axis L = (1,0,0) x B0
			ra = aab_ext.y * absR.m[2][0] + aab_ext.z * absR.m[1][0];
			rb = ob_ext.y * absR.m[0][2] + ob_ext.z * absR.m[0][1];
			if (math::abs(t.z * ob_axis[0].y - t.y * ob_axis[0].z) > ra + rb)
				return false;

			// Test axis L = (1,0,0) x B1
			ra = aab_ext.y * absR.m[2][1] + aab_ext.z * absR.m[1][1];
			rb = ob_ext.x * absR.m[0][2] + ob_ext.z * absR.m[0][0];
			if (math::abs(t.z * ob_axis[1].y - t.y * ob_axis[1].z) > ra + rb)
				return false;

			// Test axis L = (1,0,0) x B2
			ra = aab_ext.y * absR.m[2][2] + aab_ext.z * absR.m[1][2];
			rb = ob_ext.x * absR.m[0][1] + ob_ext.y * absR.m[0][0];
			if (math::abs(t.z * ob_axis[2].y - t.y * ob_axis[2].z) > ra + rb)
				return false;

			// Test axis L = (0,1,0) x B0
			ra = aab_ext.x * absR.m[2][0] + aab_ext.z * absR.m[0][0];
			rb = ob_ext.y * absR.m[1][2] + ob_ext.z * absR.m[1][1];
			if (math::abs(t.x * ob_axis[0].z - t.z * ob_axis[0].x) > ra + rb)
				return false;

			// Test axis L = (0,1,0) x B1
			ra = aab_ext.x * absR.m[2][1] + aab_ext.z * absR.m[0][1];
			rb = ob_ext.x * absR.m[1][2] + ob_ext.z * absR.m[1][0];
			if (math::abs(t.x * ob_axis[1].z - t.z * ob_axis[1].x) > ra + rb)
				return false;

			// Test axis L = (0,1,0) x B2
			ra = aab_ext.x * absR.m[2][2] + aab_ext.z * absR.m[0][2];
			rb = ob_ext.x * absR.m[1][1] + ob_ext.y * absR.m[1][0];
			if (math::abs(t.x * ob_axis[2].z - t.z * ob_axis[2].x) > ra + rb)
				return false;

			// Test axis L = (0,0,1) x B0
			ra = aab_ext.x * absR.m[1][0] + aab_ext.y * absR.m[0][0];
			rb = ob_ext.y * absR.m[2][2] + ob_ext.z * absR.m[2][1];
			if (math::abs(t.y * ob_axis[0].x - t.x * ob_axis[0].y) > ra + rb)
				return false;

			// Test axis L = (0,0,1) x B1
			ra = aab_ext.x * absR.m[1][1] + aab_ext.y * absR.m[0][1];
			rb = ob_ext.x * absR.m[2][2] + ob_ext.z * absR.m[2][0];
			if (math::abs(t.y * ob_axis[1].x - t.x * ob_axis[1].y) > ra + rb)
				return false;

			// Test axis L = (0,0,1) x B2
			ra = aab_ext.x * absR.m[1][2] + aab_ext.y * absR.m[0][2];
			rb = ob_ext.x * absR.m[2][1] + ob_ext.y * absR.m[2][0];
			if (math::abs(t.y * ob_axis[2].x - t.x * ob_axis[2].y) > ra + rb)
				return false;

			return true;
		}

		// oriented box - oriented box intersection test.
		// *note: code from ch.4.4 Oriented Bounding Boxes(OBBs)
		// of <Real-Time Collision Detection>.
		// *note: some reasons(i don't know what they are),
		// it decides some slight gaps between the two boxes
		// as overlapped case very often.
		bool OBox_OBoxIntersection(
			const Vector3& ob1_p, const Vector3& ob1_ext, const Vector3 ob1_axis[3],
			const Vector3& ob2_p, const Vector3& ob2_ext, const Vector3 ob2_axis[3])
		{
			real ra = 0, rb = 0;

			// transformation matrix which is transforming
			// the ob2's world-pos into ob1's coordinates frame.
			Matrix3 R;
			Matrix3 absR;
			for (uinteger i = 0; i < 3; i++){
				for (uinteger j = 0; j < 3; j++){
					R.m[i][j] = ob1_axis[i].Dot(ob2_axis[j]);
					absR.m[i][j] = math::abs(R.m[i][j]) + math::EPSILON;
				}
			}

			Vector3 t = ob2_p - ob1_p;
			t = Vector3(t.Dot(ob1_axis[0]), t.Dot(ob1_axis[1]), t.Dot(ob1_axis[2]));

			// test
			for (uinteger i = 0; i < 3; i++){
				ra = ob1_ext.v[i];
				rb = ob2_ext.x * absR.m[i][0] + ob2_ext.y*absR.m[i][1] + ob2_ext.z*absR.m[i][2];
				if (math::abs(t.v[i]) > ra + rb)
					return false;
			}

			for (uinteger i = 0; i < 3; i++){
				ra = ob1_ext.x*absR.m[0][i] + ob1_ext.y*absR.m[1][i] + ob1_ext.z*absR.m[2][i];
				rb = ob2_ext.v[i];
				if (math::abs(t.x*absR.m[0][i] + t.y*absR.m[1][i] + t.z*absR.m[2][i]) > ra + rb)
					return false;
			}

			// Test axis L = A0 x B0
			ra = ob1_ext.y * absR.m[2][0] + ob1_ext.z * absR.m[1][0];
			rb = ob2_ext.y * absR.m[0][2] + ob2_ext.z * absR.m[0][1];
			if (math::abs(t.z * R.m[1][0] - t.y * R.m[2][0]) > ra + rb)
				return false;

			// Test axis L = A0 x B1
			ra = ob1_ext.y * absR.m[2][1] + ob1_ext.z * absR.m[1][1];
			rb = ob2_ext.x * absR.m[0][2] + ob2_ext.z * absR.m[0][0];
			if (math::abs(t.z * R.m[1][1] - t.y * R.m[2][1]) > ra + rb)
				return false;

			// Test axis L = A0 x B2
			ra = ob1_ext.y * absR.m[2][2] + ob1_ext.z * absR.m[1][2];
			rb = ob2_ext.x * absR.m[0][1] + ob2_ext.y * absR.m[0][0];
			if (math::abs(t.z * R.m[1][2] - t.y * R.m[2][2]) > ra + rb)
				return false;

			// Test axis L = A1 x B0
			ra = ob1_ext.x * absR.m[2][0] + ob1_ext.z * absR.m[0][0];
			rb = ob2_ext.y * absR.m[1][2] + ob2_ext.z * absR.m[1][1];
			if (math::abs(t.x * R.m[2][0] - t.z * R.m[0][0]) > ra + rb)
				return false;

			// Test axis L = A1 x B1
			ra = ob1_ext.x * absR.m[2][1] + ob1_ext.z * absR.m[0][1];
			rb = ob2_ext.x * absR.m[1][2] + ob2_ext.z * absR.m[1][0];
			if (math::abs(t.x * R.m[2][1] - t.z * R.m[0][1]) > ra + rb)
				return false;

			// Test axis L = A1 x B2
			ra = ob1_ext.x * absR.m[2][2] + ob1_ext.z * absR.m[0][2];
			rb = ob2_ext.x * absR.m[1][1] + ob2_ext.y * absR.m[1][0];
			if (math::abs(t.x * R.m[2][2] - t.z * R.m[0][2]) > ra + rb)
				return false;

			// Test axis L = A2 x B0
			ra = ob1_ext.x * absR.m[1][0] + ob1_ext.y * absR.m[0][0];
			rb = ob2_ext.y * absR.m[2][2] + ob2_ext.z * absR.m[2][1];
			if (math::abs(t.y * R.m[0][0] - t.x * R.m[1][0]) > ra + rb)
				return false;

			// Test axis L = A2 x B1
			ra = ob1_ext.x * absR.m[1][1] + ob1_ext.y * absR.m[0][1];
			rb = ob2_ext.x * absR.m[2][2] + ob2_ext.z * absR.m[2][0];
			if (math::abs(t.y * R.m[0][1] - t.x * R.m[1][1]) > ra + rb)
				return false;

			// Test axis L = A2 x B2
			ra = ob1_ext.x * absR.m[1][2] + ob1_ext.y * absR.m[0][2];
			rb = ob2_ext.x * absR.m[2][1] + ob2_ext.y * absR.m[2][0];
			if (math::abs(t.y * R.m[0][2] - t.x * R.m[1][2]) > ra + rb)
				return false;

			return true;
		}

	}
}
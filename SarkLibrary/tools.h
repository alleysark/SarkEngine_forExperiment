#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <vector>
#include "core.h"

namespace sark{
	namespace tool{

		// point location about plane.
		// *param:
		//     P       - point.
		//     plane_n - plane normal vector.
		//     plane_p - a point on the plane.
		// *return: position is above(1), on(0) or below(-1) the plane.
		inline int8 PointLocationByPlane(const Vector3& P, const Vector3& plane_n, const Vector3& plane_p){
			return math::sign((P - plane_p).Dot(plane_n));
		}

		// shortest distance from plane to point.
		// *param:
		//     P       - point.
		//     plane_n - plane normal vector.
		//     plane_p - a point on the plane.
		inline real DistancePointFromPlane(const Vector3& P, const Vector3& plane_n, const Vector3& plane_p){
			// D = -dot(plane_p, plane_n)
			return (P.Dot(plane_n) - plane_p.Dot(plane_n)) / plane_n.Magnitude();
		}

		// get farthest point in given specific direction.
		const Vector3 FarthestPointInDirection(
			const std::vector<Vector3>& pointSet, const Vector3& direction);

		// separate axis theorem
		bool SeparateAxisTest(
			const std::vector<Vector3>& pointsA, const std::vector<Vector3>& pointsB,
			const Vector3& axis);

		// compute center of mass
		const Vector3 ComputeCenterOfMass(const std::vector<Vector3>& points);

		// ======================================================
		//		intersection check functions of basic shapes
		// ======================================================

		// ray-triangle intersection test.
		// *param:
		//     ray_p - a point on the line.
		//     ray_v - direction vector of line.
		//     ray_l - limitation of line.
		//     A,B,C - three points of triangle.
		//     out_P - output intersected point.
		bool Ray_TriangleIntersection(
			const Vector3& ray_p, const Vector3& ray_v, const real& ray_l,
			const Vector3& A, const Vector3& B, const Vector3& C,
			Vector3* out_P = NULL);

		// ray-plane intersection test.
		// *param:
		//     ray_p   - a point on the line.
		//     ray_v   - direction vector of line.
		//     ray_l   - limitation of line.
		//     plane_n - normal vector of plane.
		//     plane_p - a point on the plane.
		//     out_P   - output intersected point.
		bool Ray_PlaneIntersection(
			const Vector3& ray_p, const Vector3& ray_v, const real& ray_l,
			const Vector3& plane_n, const Vector3& plane_p,
			Vector3* out_P = NULL);

		// ray-plane intersection test.
		// *param:
		//     ray_p   - a point on the line.
		//     ray_v   - direction vector of line.
		//     ray_l   - limitation of line.
		//     plane_n - normal vector of plane.
		//     plane_d - D factor of plane.
		//     out_P   - output intersected point.
		bool Ray_PlaneIntersection(
			const Vector3& ray_p, const Vector3& ray_v, const real& ray_l,
			const Vector3& plane_n, const real& plane_d,
			Vector3* out_P = NULL);

		// ray - sphere intersection test.
		// *param:
		//     ray_p    - a point on the line.
		//     ray_v    - direction vector of line.
		//     ray_l    - limitation of line.
		//     sphere_p - origin of sphere.
		//     sphere_r - radius of sphere.
		//     out_P    - nearest intersected point.
		bool Ray_SphereIntersection(
			const Vector3& ray_p, const Vector3& ray_v, const real& ray_l,
			const Vector3& sphere_p, const real& sphere_r,
			Vector3* out_P = NULL);

		// ray - axis aligned box intersection test.
		// *param:
		//     ray_p   - a point on the line.
		//     ray_v   - direction vector of line.
		//     ray_l   - limitation of line.
		//     aab_min - min position of axis aligned box.
		//     aab_max - max position of axis aligned box.
		//     out_P    - nearest intersected point.
		bool Ray_AABoxIntersection(
			const Vector3& ray_p, const Vector3& ray_v, const real& ray_l,
			const Vector3& aab_min, const Vector3& aab_max,
			Vector3* out_P = NULL);

		// ray - oriented box intersection test.
		// *param:
		//     ray_p  - a point on the line.
		//     ray_v  - direction vector of line.
		//     ray_l   - limitation of line.
		//     ob_p   - center position of oriented box.
		//     ob_ext - extention of oriented box.
		//     ob_axis - orthonormal axis of oriented box.
		//     out_P    - nearest intersected point.
		bool Ray_OBoxIntersection(
			const Vector3& ray_p, const Vector3& ray_v, const real& ray_l,
			const Vector3& ob_p, const Vector3& ob_ext, const Vector3 ob_axis[3],
			Vector3* out_P = NULL);

		// ray intersection test as regards the barycentric coordinates equation.
		// *param:
		//     ray_p  - a point on the line.
		//     ray_v  - direction vector of line.
		//     bc_o   - origin of barycentric coordinates.
		//     bc_e1  - an axis of barycentric coordinates.
		//     bc_e2  - the other axis of barycentric coordinates.
		//     out_t  - output parameter t for equation.
		//     out_u, out_v - output parameters for barycentric equation.
		// *point on the barycentric coordinates: P(u,v) = bc_o + bc_e1*u + bc_e2*v
		bool Ray_BarycentricCoordIntersection(
			const Vector3& ray_p, const Vector3& ray_v,
			const Vector3& bc_o, const Vector3& bc_e1, const Vector3& bc_e2,
			real* out_t = NULL, real* out_u = NULL, real* out_v = NULL);

		// triangle - plane intersection test.
		// *param:
		//     A,B,C   - vertices of triangle.
		//     plane_n - normal vector of plane.
		//     plane_p - a point on the plane.
		//     out_P, out_Q - output positions where they intersected.
		bool Triangle_PlaneIntersection(
			const Vector3& A, const Vector3& B, const Vector3& C,
			const Vector3& plane_n, const Vector3& plane_p,
			Vector3* out_P = NULL, Vector3* out_Q = NULL);

		// triangle - triangle intersection test.
		// *param:
		//     A1, B1, C1   - three vertices of triangle 1.
		//     A2, B2, C2   - three vertices of triangle 2.
		//     out_P, out_Q - intersected line P-Q.
		//     out_n        - computed unit normal vector(of triangle 2).
		bool Triangle_TriangleIntersection(
			const Vector3& A1, const Vector3& B1, const Vector3& C1,
			const Vector3& A2, const Vector3& B2, const Vector3& C2,
			Vector3* out_P = NULL, Vector3* out_Q = NULL,
			Vector3* out_n = NULL);

		// sphere - sphere intersection test.
		// *param:
		//     sphere1_p - origin position of sphere 1.
		//     sphere1_r - radius of sphere 1.
		//     sphere2_p - origin position of sphere 2.
		//     sphere2_r - radius of sphere 2.
		//     out_P     - output position where they intersected.
		bool Sphere_SphereIntersection(
			const Vector3& sphere1_p, const real& sphere1_r,
			const Vector3& sphere2_p, const real& sphere2_r,
			Vector3* out_P = NULL);

		// sphere - axis aligned box intersection test.
		// *param:
		//     sphere_p - origin position of sphere.
		//     sphere_r - radius of sphere.
		//     aab_min  - min position of box.
		//     aab_max  - max position of box.
		bool Sphere_AABoxIntersection(
			const Vector3& sphere_p, const real& sphere_r,
			const Vector3& aab_min, const Vector3& aab_max);

		// sphere - oriented box intersection test.
		// *param:
		//     sphere_p - origin position of sphere.
		//     sphere_r - radius of sphere.
		//     ob_p     - center position of oriented box.
		//     ob_ext   - extention of oriented box.
		//     ob_axis  - orthonormal axis of oriented box.
		bool Sphere_OBoxIntersection(
			const Vector3& sphere_p, const real& sphere_r,
			const Vector3& ob_p, const Vector3& ob_ext, const Vector3 ob_axis[3]);

		// axis aligned box - axis aligned box intersection test.
		// *note: it does not compute contact point.
		// *param:
		//     aab1_min, aab1_max - min, max of axis aligned box 1.
		//     aab2_min, aab2_max - min, max of axis aligned box 2.
		bool AABox_AABoxIntersection(
			const Vector3& aab1_min, const Vector3& aab1_max,
			const Vector3& aab2_min, const Vector3& aab2_max);

		// axis aligned box - oriented box intersection test.
		// *param:
		//     aab_min - min position of box.
		//     aab_max - max position of box.
		//     ob_p    - center position of oriented box.
		//     ob_ext  - extention of oriented box.
		//     ob_axis - orthonormal axis of oriented box.
		bool AABox_OBoxIntersection(
			const Vector3& aab_min, const Vector3& aab_max,
			const Vector3& ob_p, const Vector3& ob_ext, const Vector3 ob_axis[3]);

		// oriented box - oriented box intersection test.
		// *param:
		//     ob1_p    - center position of oriented box 1.
		//     ob1_ext  - extention of oriented box 1.
		//     ob1_axis - orthonormal axis of oriented box 1.
		//     ob2_p    - center position of oriented box 2.
		//     ob2_ext  - extention of oriented box 2.
		//     ob2_axis - orthonormal axis of oriented box 2.
		bool OBox_OBoxIntersection(
			const Vector3& ob1_p, const Vector3& ob1_ext, const Vector3 ob1_axis[3],
			const Vector3& ob2_p, const Vector3& ob2_ext, const Vector3 ob2_axis[3]);

	}
}
#endif
#ifndef __GJK_EPA_H__
#define __GJK_EPA_H__

#include <vector>
#include <list>
#include "core.h"

namespace sark{

	class ConvexHull;

	// Gilbert Johnsom Keerthi and
	// Expanding Polytope Algorithm package class.
	class GJK_EPA{
	public:
		// type of simplex for GJK and EPA process.
		typedef std::vector<Vector3> Simplex;

		// do GJK process.
		// it detects the collision of two convex hulls.
		// if they intersect then the simplex data of minkowski set
		// will be stored in the 'out_simplex'.
		static bool DoGJK(const ConvexHull* convexA, const ConvexHull* convexB,
			Simplex* out_simplex = NULL);

		// do EPA process.
		// it compute the contact normal and penetration depth.
		// simplex should be the termination data of GJK and
		// it can be expanded when EPA run.
		// if EPA does successfully, it'll store the collision
		// informations into 'out_*' buffer.
		static bool DoEPA(const ConvexHull* convexA, const ConvexHull* convexB,
			Simplex& simplex,
			Vector3* out_normal = NULL, real* out_depth = NULL);


	private:
		// temporal face for EPA process.
		class Face{
		public:
			union{
				struct{
					uinteger a, b, c;
				};
				struct{
					uinteger idx[3];
				};
			};
			Vector3 normal;
			real distance;
			
			// it initialize normal and distance as ccw order of given a,b,c.
			Face(uinteger _a, uinteger _b, uinteger _c,
				const Vector3& _normal, const real& _distance);

			Face(uinteger _a, uinteger _b, uinteger _c, const Simplex& simplex);
		};
		typedef std::list<Face> FaceList;


		// return the farthest point in direction at
		// the set of minkowski sum of two convex point sets.
		static const Vector3 SupportPoint(
			const ConvexHull* convexA, const ConvexHull* convexB,
			const Vector3& direction);

		// check whether the simplex contains the origin
		// and then update simplex if it doesn't.
		static bool GJK_CheckAndUpdate(Simplex& simplex, Vector3& dir);

		// expand simplex to be containing the origin
		// as far as possible.
		static void EPA_Expand(const Simplex& simplex, FaceList& faces,
			const std::vector<FaceList::iterator>& targets);
	};

}
#endif
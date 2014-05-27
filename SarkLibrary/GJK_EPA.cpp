#include "GJK_EPA.h"
#include "shapes.h"
#include "tools.h"
#include "ConvexHull.h"
#include "Debug.h"

namespace sark{

	// ============ GJK_EPA::Face implementation ================

	GJK_EPA::Face::Face(uinteger _a, uinteger _b, uinteger _c,
		const Vector3& _normal, const real& _distance)
		: a(_a), b(_b), c(_c), normal(_normal), distance(_distance){}

	GJK_EPA::Face::Face(uinteger _a, uinteger _b, uinteger _c,
		const Simplex& simplex)
		: a(_a), b(_b), c(_c)
	{
		normal = (simplex[b] - simplex[a]).Cross(simplex[c] - simplex[a]);
		normal.Normalize();
		
		// compute distance
		distance = normal.Dot(simplex[a]);
	}


	// ================= GJK_EPA implementation ==================

	// it returns true if two convex shape(in world space) A and B intersect each other.
	// and the last simplex will be stored in 'out_simplex' buffer on true cases.
	bool GJK_EPA::DoGJK(const ConvexHull* convexA, const ConvexHull* convexB,
		Simplex* out_simplex)
	{
		Simplex simplex;

		// initialize simplex
		// point D
		Vector3 dir = Vector3::Right;
		simplex.push_back(SupportPoint(convexA, convexB, dir));
		if (dir.Dot(simplex.back()) < 0)
			return false;

		// point C
		dir = -dir;
		simplex.push_back(SupportPoint(convexA, convexB, dir));
		if (dir.Dot(simplex.back()) < 0)
			return false;

		// point B
		//dir = (D - C) x (O - C)
		dir = (simplex[0] - simplex[1]).Cross(-simplex[1]);
		if (dir == Vector3(0, 0, 0)){
			if (out_simplex != NULL){
				// origin is lying on the line CD. so two shapes are intersecting obviously.
				// for the purpose of EPA, simplex should be completed as tetrahedron(in 3D).
				// *caution: same points in the current simplex can be added as a new point very occasionally.
				// but i just ignore this case.. please remember this problematic implemantation.
				simplex.push_back(SupportPoint(convexA, convexB, Vector3::Up));
				dir = (simplex[1] - simplex[2]).Cross(simplex[0] - simplex[2]);
				simplex.push_back(SupportPoint(convexA, convexB, dir));
				*out_simplex = simplex;
			}
			return true;
		}
		simplex.push_back(SupportPoint(convexA, convexB, dir));

		// point A (compute direction only)
		do{
			// dir = (C - B) x (D - B)
			dir = (simplex[1] - simplex[2]).Cross(simplex[0] - simplex[2]);

			// check location of origin
			int8 loc = tool::PointLocationByPlane(Vector3(0), dir, simplex[2]);
			if (loc == 0){
				// adjust point B
				simplex[2] = SupportPoint(convexA, convexB, dir);
				continue;
			}
			if (loc < 0){
				dir = -dir;
			}
			break;
		} while (true);


		// GJK iteration. find a simplex which contains the origin.
		while (true){
			simplex.push_back(SupportPoint(convexA, convexB, dir));
			if (dir.Dot(simplex.back()) < 0){
				return false;
			}

			if (GJK_CheckAndUpdate(simplex, dir)){
				if (out_simplex != NULL)
					*out_simplex = simplex;
				return true;
			}
		}
	}

	// it returns contact normal and penetration depth.
	bool GJK_EPA::DoEPA(const ConvexHull* convexA, const ConvexHull* convexB,
		Simplex& simplex,
		Vector3* out_normal, real* out_depth)
	{
		if (simplex.size() != 4){
			LogWarn("given simplex is not completed");
			return false;
		}

		// init face list.
		FaceList faces;
		{
			const Vector3& A = simplex[3];
			const Vector3& B = simplex[2];
			const Vector3& C = simplex[1];
			const Vector3& D = simplex[0];

			// is A above BCD?
			Vector3 BCD = (C - B).Cross(D - B);
			int8 loc = tool::PointLocationByPlane(A, BCD, B);
			if (loc > 0){
				// orient BCD toward outside
				BCD = -BCD;
				BCD.Normalize();
				real dist_BCD = BCD.Dot(B);

				// ABC, ACD, ADB, BDC
				faces.push_back(Face(3, 2, 1, simplex));
				faces.push_back(Face(3, 1, 0, simplex));
				faces.push_back(Face(3, 0, 2, simplex));
				faces.push_back(Face(2, 0, 1, BCD, dist_BCD));
			}
			else if (loc < 0){
				BCD.Normalize();
				real dist_BCD = BCD.Dot(B);

				// ACB, ADC, ABD, BCD
				faces.push_back(Face(3, 1, 2, simplex));
				faces.push_back(Face(3, 0, 1, simplex));
				faces.push_back(Face(3, 2, 0, simplex));
				faces.push_back(Face(2, 1, 0, BCD, dist_BCD));
			}
			else{
				LogWarn("A is on the same plane of BCD");
				return false;
			}
		}

		FaceList::iterator itr;
		FaceList::iterator closest;
		FaceList::iterator end = faces.end();

		do{
			itr = faces.begin();
			closest = itr;
			for (; itr != end; itr++){
				if (itr->distance < closest->distance){
					closest = itr;
				}
			}

			Vector3 P = SupportPoint(convexA, convexB, closest->normal);

			real d = P.Dot(closest->normal);
			if (d - closest->distance < 0.00001f){
				// closest face is really closest and we're done.
				if (out_normal != NULL)
					*out_normal = closest->normal;
				if (out_depth != NULL)
					*out_depth = closest->distance;
				return true;
			}
			else{
				// add point and make sure that face list to be convex.
				// note that added point p can't be placed on the any tip of voronoi regions
				// because the points of simplex are already farthest points of
				// minkowski set.
				simplex.push_back(P);

				// find visible faces
				std::vector<FaceList::iterator> targets;
				for (itr = faces.begin(); itr != end; itr++){
					if (tool::PointLocationByPlane(P, itr->normal, simplex[itr->a]) > 0){
						targets.push_back(itr);
					}
				}
				
				// expand polytope
				EPA_Expand(simplex, faces, targets);
			}
		} while (true);
	}


	// return the farthest point in direction at
	// the set of minkowski sum of two convex point sets.
	const Vector3 GJK_EPA::SupportPoint(const ConvexHull* convexA, const ConvexHull* convexB,
		const Vector3& direction)
	{
		const Vector3 farA = tool::FarthestPointInDirection(convexA->GetTransPointSet(), direction);
		const Vector3 farB = tool::FarthestPointInDirection(convexB->GetTransPointSet(), -direction);
		return farA - farB;
	}

	// check whether the simplex contains the origin
	// and then update simplex if it doesn't.
	bool GJK_EPA::GJK_CheckAndUpdate(Simplex& simplex, Vector3& dir){
		const Vector3& A = simplex[3];
		const Vector3& B = simplex[2];
		const Vector3& C = simplex[1];
		const Vector3& D = simplex[0];
		Vector3 AO = -A;
		Vector3 AB = B - A;
		Vector3 AC = C - A;
		Vector3 AD = D - A;

		if (dir.Dot((C - B).Cross(D - B)) > 0){
			// A is above BCD. normal of ABC, ADB, ACD are candidates
			dir = AB.Cross(AC);
			if (AO.Dot(dir) > 0){
				simplex.erase(simplex.begin()+0);
			}
			else{
				dir = AD.Cross(AB);
				if (AO.Dot(dir) > 0){
					simplex.erase(simplex.begin() + 1);
				}
				else{
					dir = AC.Cross(AD);
					if (AO.Dot(dir) > 0){
						simplex.erase(simplex.begin() + 2);
					}
					else{
						// origin is in the tetrahedron ABCD
						return true;
					}
				}
			}
		}
		else{
			// A is under BCD. normal of ACB, ABD, ADC are candidates
			dir = AC.Cross(AB);
			if (AO.Dot(dir) > 0){
				simplex.erase(simplex.begin() + 0);
			}
			else{
				dir = AB.Cross(AD);
				if (AO.Dot(dir) > 0){
					simplex.erase(simplex.begin() + 1);
				}
				else{
					dir = AD.Cross(AC);
					if (AO.Dot(dir) > 0){
						simplex.erase(simplex.begin() + 2);
					}
					else{
						// origin is in the tetrahedron ABCD
						return true;
					}
				}
			}
		}
		return false;
	}

	// expand simplex to be containing the origin
	// as far as possible.
	void GJK_EPA::EPA_Expand(const Simplex& simplex, FaceList& faces,
		const std::vector<FaceList::iterator>& targets)
	{
		uinteger pidx = simplex.size() - 1;
		uinteger szTarget = targets.size();
		if (szTarget == 0){

		}
		else if (szTarget == 1){
			// point is in a certain face side of voronoi regions.
			// there are 3 faces should be added and a face should be removed.
			faces.push_back(Face(pidx, targets[0]->a, targets[0]->b, simplex));
			faces.push_back(Face(pidx, targets[0]->b, targets[0]->c, simplex));
			faces.push_back(Face(pidx, targets[0]->c, targets[0]->a, simplex));
			faces.erase(targets[0]);
		}
		else if (szTarget == 2){
			// point is in a certain edge side of voronoi regions.
			// there are 4 faces should be added and two faces should be removed.
			// find unique and one way ordered indices of two target faces.
			uinteger uniqIdxs[4];
			for (uinteger i = 0; i < 3; i++){
				uinteger j = 0;
				for (; j < 3; j++){
					if (targets[0]->idx[i] == targets[1]->idx[j])
						break;
				}
				if (j != 3){
					// it can be guaranteed that the target faces are orderd by ccw.
					// (they were sorted when compute those face normals)
					// so, (i+1)---(j+2) or (i+2)---(j+1) is the common edge.
					if (targets[0]->idx[(i + 1) % 3] == targets[1]->idx[(j + 2) % 3]){
						// (i)--(j+1)--(i+1)--(i+2) order
						uniqIdxs[0] = targets[0]->idx[i];
						uniqIdxs[1] = targets[1]->idx[(j + 1) % 3];
						uniqIdxs[2] = targets[0]->idx[(i + 1) % 3];
						uniqIdxs[3] = targets[0]->idx[(i + 2) % 3];
						break;
					}
					else{ //if (i+2)---(j+1)
						// (i)--(i+1)--(i+2)--(j+2)
						uniqIdxs[0] = targets[0]->idx[i];
						uniqIdxs[1] = targets[0]->idx[(i + 1) % 3];
						uniqIdxs[2] = targets[0]->idx[(i + 2) % 3];
						uniqIdxs[3] = targets[1]->idx[(j + 2) % 3];
						break;
					}
				}
			}

			faces.push_back(Face(pidx, uniqIdxs[0], uniqIdxs[1], simplex));
			faces.push_back(Face(pidx, uniqIdxs[1], uniqIdxs[2], simplex));
			faces.push_back(Face(pidx, uniqIdxs[2], uniqIdxs[3], simplex));
			faces.push_back(Face(pidx, uniqIdxs[3], uniqIdxs[0], simplex));
			faces.erase(targets[0]);
			faces.erase(targets[1]);
		}
		else{
			// unique index list
			std::vector<uinteger> uniqIdxs;

			// 1. push indices of targets[0] into idxs.
			uniqIdxs.push_back(targets[0]->a);
			uniqIdxs.push_back(targets[0]->b);
			uniqIdxs.push_back(targets[0]->c);
			uinteger szIdxs = uniqIdxs.size();

			// 2. gather the indices from targets uniquely.
			uinteger i, j, k;
			for (i = 1; i < szTarget; i++){
				// push indices from target[i] into idxs uniquely.
				for (j = 0; j < 3; j++){
					bool unique = true;
					for (k = 0; k < szIdxs; k++){
						if (targets[i]->idx[j] == uniqIdxs[k]){
							unique = false;
							break;
						}
					}
					if (unique){
						uniqIdxs.push_back(targets[i]->idx[j]);
						szIdxs++;
					}
				}
			}

			// 3. compute the center point of unique vertices.
			Vector3 CP(0);
			for (i = 0; i < szIdxs; i++)
				CP += simplex[uniqIdxs[i]];
			CP /= (real)szIdxs;

			// 4. project unique vertices into a certain plane.
			// at this point, i choose the plane which has CP-P as plane normal
			// and is passing the origin.
			Vector3 norm = simplex[pidx] - CP;
			norm.Normalize();

			Vector3 projCP = CP - CP.Dot(norm)*norm;
			std::vector<Vector3> projPoints(szIdxs);
			for (i = 0; i < szIdxs; i++){
				const Vector3& Q = simplex[uniqIdxs[i]];
				projPoints[i] = Q - Q.Dot(norm)*norm;
			}

			// 5. compute angles of CP-projPoints axis as plane normal.
			std::vector<real> angles(szIdxs);
			Vector3 standAxis = projPoints[0] - projCP;
			standAxis.Normalize();

			angles[0] = 0;
			for (i = 1; i < szIdxs; i++){
				Vector3 line = (projPoints[i] - projCP).Normal();
				Vector3 perp = standAxis.Cross(line);
				angles[i] = math::acos(standAxis.Dot(line));
				// if sin(еш) <0 then angle is over PI
				if (perp.Dot(norm) < 0){
					angles[i] = math::PI*2.f - angles[i];
				}
			}

			// 6. sort angles with indices ordered by angles ascent.
			// algorithm from wikipedia.
			real tmp;
			for (i = 1; i<szIdxs; i++){
				j = i;
				tmp = angles[j];
				k = uniqIdxs[j];

				while (--j >= 0 && tmp < angles[j]){
					angles[j + 1] = angles[j];
					uniqIdxs[j + 1] = uniqIdxs[j];
				}
				angles[j + 1] = tmp;
				uniqIdxs[j + 1] = k;
			}

			// 7. add and remove faces
			szIdxs -= 1;
			for (i = 0; i < szIdxs;){
				faces.push_back(Face(pidx, uniqIdxs[i++], uniqIdxs[i], simplex));
			}
			faces.push_back(Face(pidx, uniqIdxs[szIdxs], uniqIdxs[0], simplex));

			for (i = 0; i < szTarget; i++){
				faces.erase(targets[i]);
			}
		}
	}

}
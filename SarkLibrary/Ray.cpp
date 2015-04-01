#include "Ray.h"
#include "ACollider.h"
#include "tools.h"
#include "SphereCollider.h"
#include "AABoxCollider.h"
#include "OBoxCollider.h"
#include "ConvexHull.h"

namespace sark {

	Ray::Ray()
		: pos(0), dir(Vector3::Forward), limit(0) {}

	Ray::Ray(const Vector3& position, const Vector3& direction, real limitation)
		: pos(position), dir(direction), limit(limitation) {}

	Ray::Ray(const Vector3& A, const Vector3& B)
		: pos(A), dir(B - A), limit(1.f) {}

	bool Ray::IntersectWith(const ACollider* coll, Vector3* out_P) const {
		switch (coll->GetType()) {
		case ACollider::SPHERE:{
				const SphereCollider& sphere = reinterpret_cast<const SphereCollider&>(*coll);
				return tool::Ray_SphereIntersection(pos, dir, limit, sphere.pos, sphere.r, out_P);
			}
			break;
		case ACollider::AABOX:{
				const AABoxCollider& aab = reinterpret_cast<const AABoxCollider&>(*coll);
				return tool::Ray_AABoxIntersection(pos, dir, limit, aab.min, aab.max, out_P);
			}
			break;
		case ACollider::OBOX:{
				const OBoxCollider& ob = reinterpret_cast<const OBoxCollider&>(*coll);
				return tool::Ray_OBoxIntersection(pos, dir, limit, ob.pos, ob.ext, ob.axis, out_P);
			}
			break;
		case ACollider::CONVEXHULL:{
				const ConvexHull& cvx = reinterpret_cast<const ConvexHull&>(*coll);
				const ConvexHull::FaceSet& faces = cvx.GetFaceSet();
				if (faces.size() == 0) {
					// if convex hull is just point cloud,
					// it cannot be tested.
					return false;
				}
				ConvexHull::FaceIterator fitr = faces.cbegin();
				ConvexHull::FaceIterator fend = faces.cend();
				const ConvexHull::PointSet points = cvx.GetTransPointSet();

				for (; fitr != fend; fitr++) {
					const Vector3& A = points[fitr->a];
					const Vector3& B = points[fitr->b];
					const Vector3& C = points[fitr->c];
					if (tool::Ray_TriangleIntersection(pos, dir, limit, A, B, C, out_P)) {
						return true;
					}
				}
			}
			break;
		}
		return false;
	}

}
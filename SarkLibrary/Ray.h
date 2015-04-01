#ifndef __RAY_H__
#define __RAY_H__

#include "core.h"

namespace sark {

	class ACollider;

	// the ray toward direction vector.
	class Ray {
	public:
		// start position. the origin of ray.
		Vector3 pos;

		// normalized direction vector.
		Vector3 dir;

		// limitation of ray distance. positive value.
		real limit;

		Ray();
		Ray(const Vector3& position, const Vector3& direction,
			real limitation);
		// ray as line segment
		Ray(const Vector3& A, const Vector3& B);

		// test intersection with collider
		// and return intersected position if it does.
		bool IntersectWith(const ACollider* coll,
			Vector3* out_P = NULL) const;
	};

}
#endif
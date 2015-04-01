#include "SphereCollider.h"
#include "tools.h"
#include "ASceneComponent.h"
#include "AABoxCollider.h"
#include "OBoxCollider.h"

namespace sark {

	SphereCollider::SphereCollider(ASceneComponent* reference)
		: ACollider(reference), pos(0), r(1.f)
	{}

	SphereCollider::SphereCollider(ASceneComponent* reference, const Vector3& position, real radius)
		: ACollider(reference), pos(position), r(radius)
	{}

	SphereCollider::~SphereCollider() {}

	// get type of collider
	const ACollider::Type SphereCollider::GetType() const {
		return ACollider::SPHERE;
	}

	// intersection test.
	bool SphereCollider::IntersectWith(const ACollider* coll) const {
		switch (coll->GetType()) {
		case ACollider::SPHERE:{
				const SphereCollider& sphereB = reinterpret_cast<const SphereCollider&>(*coll);
				return tool::Sphere_SphereIntersection(pos, r, sphereB.pos, sphereB.r);
			}
			break;
		case ACollider::AABOX:{
				const AABoxCollider& aab = reinterpret_cast<const AABoxCollider&>(*coll);
				return tool::Sphere_AABoxIntersection(pos, r, aab.min, aab.max);
			}
			break;
		case ACollider::OBOX:{
				const OBoxCollider& ob = reinterpret_cast<const OBoxCollider&>(*coll);
				return tool::Sphere_OBoxIntersection(pos, r, ob.pos, ob.ext, ob.axis);
			}
			break;
		case ACollider::CONVEXHULL:
			break;
		}
		return false;
	}

	// update sphere
	void SphereCollider::Update() {
		pos = mReference->GetTransform().GetPosition();
	}

}
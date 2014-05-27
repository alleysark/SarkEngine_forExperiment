#include "AABoxCollider.h"
#include "tools.h"
#include "SphereCollider.h"
#include "OBoxCollider.h"
#include "ASceneComponent.h"

namespace sark{

	// axis aligned box
	AABoxCollider::AABoxCollider(ASceneComponent* reference)
		: ACollider(reference), min(-0.5f), max(0.5f){}

	AABoxCollider::AABoxCollider(ASceneComponent* reference,
		const Vector3& posMin, const Vector3& posMax)
		: ACollider(reference), min(posMin), max(posMax){}

	AABoxCollider::AABoxCollider(ASceneComponent* reference,
		const Vector3& center, real half_ext[3])
		: ACollider(reference), 
		min(center.x - half_ext[0], center.y - half_ext[1], center.z - half_ext[2]),
		max(center.x + half_ext[0], center.y + half_ext[1], center.z + half_ext[2]){}

	AABoxCollider::~AABoxCollider(){}

	const ACollider::Type AABoxCollider::GetType() const{
		return ACollider::AABOX;
	}

	bool AABoxCollider::IntersectWith(const ACollider* coll) const{
		switch (coll->GetType()){
		case ACollider::SPHERE:{
				const SphereCollider& sphere = reinterpret_cast<const SphereCollider&>(*coll);
				return tool::Sphere_AABoxIntersection(sphere.pos, sphere.r, min, max);
			}
			break;
		case ACollider::AABOX:{
				const AABoxCollider& aabB = reinterpret_cast<const AABoxCollider&>(*coll);
				return tool::AABox_AABoxIntersection(min, max, aabB.min, aabB.max);
			}
			break;
		case ACollider::OBOX:{
				const OBoxCollider& ob = reinterpret_cast<const OBoxCollider&>(*coll);
				return tool::AABox_OBoxIntersection(min, max, ob.pos, ob.ext, ob.axis);
			}
			break;
		case ACollider::CONVEXHULL:
			break;
		}
		return false;
	}

	// update aabox
	void AABoxCollider::Update(){
	}

}
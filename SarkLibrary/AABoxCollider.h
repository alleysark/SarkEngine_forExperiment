#ifndef __AABOX_COLLIDER_H__
#define __AABOX_COLLIDER_H__

#include "core.h"
#include "ACollider.h"

namespace sark {

	// axis aligned box.
	// box that its faces are aligned with
	// standard coordinates axis orientation.
	// it is represented as min, max position.
	class AABoxCollider : public ACollider {
	public:
		// minimum position
		Vector3 min;

		// maximum position
		Vector3 max;

		AABoxCollider(ASceneComponent* reference);
		AABoxCollider(ASceneComponent* reference,
			const Vector3& posMin, const Vector3& posMax);
		AABoxCollider(ASceneComponent* reference,
			const Vector3& center, real half_ext[3]);
		~AABoxCollider();

		// get type
		const Type GetType() const override;

		// intersection test
		bool IntersectWith(const ACollider* coll) const override;

		// update aabox
		void Update() override;
	};

}
#endif
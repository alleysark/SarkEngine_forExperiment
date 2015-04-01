#ifndef __OBOX_COLLIDER_H__
#define __OBOX_COLLIDER_H__

#include "core.h"
#include "ACollider.h"

namespace sark {

	// oriented box.
	// box which has the own orientation.
	// it is represented as center position
	// of box and its own basis with extensions.
	class OBoxCollider : public ACollider {
	public:
		// center position of box.
		Vector3 pos;

		// half-extention of each axis.
		Vector3 ext;

		// orthonormal axis
		Vector3 axis[3];

		OBoxCollider(ASceneComponent* reference);
		OBoxCollider(ASceneComponent* reference,
			const Vector3& position, const Vector3& extention);
		OBoxCollider(ASceneComponent* reference,
			const Vector3& position, const Vector3& extention,
			const Vector3 _axis[3]);

		// get type.
		const Type GetType() const override;

		// intersection test.
		bool IntersectWith(const ACollider* coll) const override;

		// update obox
		void Update() override;
	};

}
#endif
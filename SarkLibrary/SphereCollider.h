#ifndef __SPHERE_COLLIDER_H__
#define __SPHERE_COLLIDER_H__

#include "core.h"
#include "ACollider.h"

namespace sark{

	// center and radius form of sphere
	class SphereCollider : public ACollider{
	public:
		// origin position of sphere
		Vector3 pos;

		// radius
		real r;

		SphereCollider(ASceneComponent* reference);
		SphereCollider(ASceneComponent* reference, const Vector3& position, real radius);
		~SphereCollider();

		// get type of collider
		const Type GetType() const override;

		// intersection test.
		bool IntersectWith(const ACollider* coll) const override;

		// update sphere
		void Update() override;
	};

}
#endif
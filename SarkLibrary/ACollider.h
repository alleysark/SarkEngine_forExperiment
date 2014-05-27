#ifndef __A_COLLIDER_H__
#define __A_COLLIDER_H__

#include "core.h"

namespace sark{

	class ASceneComponent;

	// interface of collider.
	class ACollider{
	public:
		enum Type{ SPHERE, AABOX, OBOX, CONVEXHULL };

	protected:
		// refernece pointer.
		ASceneComponent* mReference;
		
	public:
		ACollider(ASceneComponent* reference);

		virtual ~ACollider();

		// get type of collider
		virtual const Type GetType() const = 0;

		// intersection test.
		virtual bool IntersectWith(const ACollider* coll) const = 0;

		// update collider.
		virtual void Update() = 0;
	};

}
#endif
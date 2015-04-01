#include "OBoxCollider.h"
#include "tools.h"
#include "ASceneComponent.h"
#include "SphereCollider.h"
#include "AABoxCollider.h"

namespace sark {

	OBoxCollider::OBoxCollider(ASceneComponent* reference)
		: ACollider(reference), pos(0), ext(0.5f)
	{
		axis[0].Set(1, 0, 0); axis[1].Set(0, 1, 0); axis[2].Set(0, 0, 1);
	}

	OBoxCollider::OBoxCollider(ASceneComponent* reference,
		const Vector3& position, const Vector3& extention)
		: ACollider(reference), pos(position), ext(extention)
	{
		axis[0].Set(1, 0, 0); axis[1].Set(0, 1, 0); axis[2].Set(0, 0, 1);
	}

	OBoxCollider::OBoxCollider(ASceneComponent* reference,
		const Vector3& position, const Vector3& extention,
		const Vector3 _axis[3])
		: ACollider(reference), pos(position), ext(extention)
	{
		axis[0] = _axis[0]; axis[1] = _axis[1]; axis[2] = _axis[2];
	}

	const ACollider::Type OBoxCollider::GetType() const {
		return ACollider::OBOX;
	}

	bool OBoxCollider::IntersectWith(const ACollider* coll) const {
		switch (coll->GetType()) {
		case ACollider::SPHERE:{
				const SphereCollider& sphere = reinterpret_cast<const SphereCollider&>(*coll);
				return tool::Sphere_OBoxIntersection(sphere.pos, sphere.r, pos, ext, axis);
			}
			break;
		case ACollider::AABOX:{
				const AABoxCollider& aab = reinterpret_cast<const AABoxCollider&>(*coll);
				return tool::AABox_OBoxIntersection(aab.min, aab.max, pos, ext, axis);
			}
			break;
		case ACollider::OBOX:{
				const OBoxCollider& obB = reinterpret_cast<const OBoxCollider&>(*coll);
				return tool::OBox_OBoxIntersection(pos, ext, axis, obB.pos, obB.ext, obB.axis);
			}
			break;
		case ACollider::CONVEXHULL:
			break;
		}
		return false;
	}

	void OBoxCollider::Update() {
		// to orient the object, it needs the rotation matrix R.
		// therefore, transposition of R can transform the object
		// in the world-space into the origin of the object-space.
		// it means that each rows of transposition of R becomes
		// the orthonormal basis of the object-space.
		const Matrix4& TM = mReference->GetTransform().GetMatrix();
		axis[0].Set(TM.m[0][0], TM.m[1][0], TM.m[2][0]);
		axis[1].Set(TM.m[0][1], TM.m[1][1], TM.m[2][1]);
		axis[2].Set(TM.m[0][2], TM.m[1][2], TM.m[2][2]);

		pos = mReference->GetTransform().GetPosition();
	}

}
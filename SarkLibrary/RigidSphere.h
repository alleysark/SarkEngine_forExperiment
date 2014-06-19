#ifndef __RIGID_SPHERE_H__
#define __RIGID_SPHERE_H__

#include "StaticModel.h"

namespace sark{

	class Mesh;
	class RigidBody;
	class ACollider;

	// simple rigid sphere scene component.
	class RigidSphere : public StaticModel{
	private:
		// radius
		real mRadius;
		uinteger mSlice, mStack;

	public:
		// create sphere from given properties.
		// inverse of initial inertia tensor(invI0) is computed automatically.
		RigidSphere(real radius, uinteger slice, uinteger stack,
			real invMass = 0.f,
			const Vector3& velocity = 0.f, const Vector3& angularVelocity = 0.f,
			bool gravityOn = true);

		// create sphere from given properties.
		// inverse of initial inertia tensor(invI0) is computed automatically.
		RigidSphere(const std::string& name, ASceneComponent* parent, bool activate,
			real radius, uinteger slice, uinteger stack,
			real invMass = 0.f,
			const Vector3& velocity = 0.f, const Vector3& angularVelocity = 0.f,
			bool gravityOn = true);

		~RigidSphere();

		// get radius of sphere.
		const real& GetRadius() const;

		// get slice count.
		const uinteger& GetSliceCount() const;

		// get stack count.
		const uinteger& GetStackCount() const;

	private:
		void CreateSphere();
	};

}
#endif
#ifndef __RIGID_SPHERE_H__
#define __RIGID_SPHERE_H__

#include "ASceneComponent.h"
#include "Mesh.h"
#include "RigidBody.h"
#include "shapes.h"

namespace sark{

	// simple rigid sphere scene component.
	class RigidSphere : public ASceneComponent{
	private:
		// mesh object
		Mesh* mMesh;

		// rigid body object
		RigidBody* mRigidBody;

		// bounding sphere
		Sphere* mSphere;

		// radius
		real mRadius;
		uinteger mSlice, mStack;

	public:
		// create sphere from given properties.
		// inverse of initial inertia tensor(invI0) is computed automatically.
		RigidSphere(real radius, uinteger slice, uinteger stack,
			real invMass = 1.f,
			const Vector3& velocity = 0.f, const Vector3& angularVelocity = 0.f,
			bool gravityOn = true, bool fixed = false);

		// create sphere from given properties.
		// inverse of initial inertia tensor(invI0) is computed automatically.
		RigidSphere(const std::string& name, ASceneComponent* parent, bool activate,
			real radius, uinteger slice, uinteger stack,
			real invMass = 1.f,
			const Vector3& velocity = 0.f, const Vector3& angularVelocity = 0.f,
			bool gravityOn = true, bool fixed = false);

		~RigidSphere();

		// get radius of sphere.
		const real& GetRadius() const;

		const IShape* GetBoundingShape() const override;

		Mesh* GetMesh() override;

		RigidBody* GetRigidBody() override;

		void Update() override;

		void Render() override;

	private:
		void CreateSphere();
	};

}
#endif
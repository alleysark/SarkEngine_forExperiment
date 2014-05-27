#ifndef __RIGID_BODY_H__
#define __RIGID_BODY_H__

#include "core.h"

namespace sark{

	class ASceneComponent;

	// rigid body descriptor.
	// "In physics, a rigid body is an idealization of
	// a solid body in which deformation is neglected."
	// -wikipedia
	class RigidBody{
	private:
		// it consider the scene component position
		// as center of mass by default.
		Vector3 mCM;

		// inverse of object mass. 1/M
		// if it is zero, then this body is fixed object.
		real mInvMass;

		// inverse of initial inertia tensor (I_0)^{-1}
		Matrix3 mInvI0;

		// current linear velocity.
		Vector3 mVelocity;

		// current angular velocity.
		Vector3 mAngularVelocity;

		// if true, it is affected by gravity.
		bool mGravityOn;

		// temporal summation result
		// of affected linear forces.
		Vector3 mForce;

		// temporal summation result
		// of affected torque.
		Vector3 mTorque;

		// refernece pointer.
		ASceneComponent* mReference;

	public:
		RigidBody(ASceneComponent* reference,
			const real invMass, const Matrix3& invI0,
			const Vector3& velocity, const Vector3& angularVelocity,
			bool gravityOn);

		~RigidBody();

		// get center of mass.
		const Position3 GetCM();

		// get inverse of mass. 1/M
		const real& GetInvMass() const;
		// set mass. if zero, then body is fixed.
		void SetInvMass(real invMass);

		// get inverse of initial inertia tensor.
		const Matrix3& GetInvInertiaTensor0() const;
		// set inverse initial inertia tensor
		void SetInvInertiaTensor0(const Matrix3& invI0);

		// get inverse inertia tensor of time t. inv(I(t))
		const Matrix3 GetInvInertiaTensor() const;

		// get current linear velocity.
		const Vector3& GetVelocity() const;
		// set current linear velocity.
		void SetVelocity(const Vector3& velocity);

		// get current angular velocity.
		const Vector3& GetAngularVelocity() const;
		// set current angular velocity.
		void SetAngularVelocity(const Vector3& angularVelocity);

		// add linear force to this rigid body.
		void AddForce(const Vector3& force);

		// add force which is affected
		// onto the given position.
		// *note: it assumes that the
		// position is on the surface of body.
		void AddForceOn(const Position3& position,
			const Vector3& force);

		// is this body affected by gravity.
		bool IsGravityOn() const;
		// set this body to be affected by gravity or not.
		void GravityOn(bool on);

		// is this body fixed.
		bool IsFixed() const;

		// update translational terms and rotational terms.
		void Update();
	};

}
#endif
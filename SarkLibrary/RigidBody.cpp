#include "RigidBody.h"
#include "ASceneComponent.h"
#include "Transform.h"
#include "Engine.h"
#include "Debug.h"

namespace sark {

	RigidBody::RigidBody(ASceneComponent* reference,
		const real invMass, const Matrix3& invI0,
		const Vector3& velocity, const Vector3& angularVelocity,
		bool gravityOn)
		: mReference(reference), mInvMass(invMass), mInvI0(invI0),
		mVelocity(velocity), mAngularVelocity(angularVelocity),
		mGravityOn(gravityOn),
		mForce(0.f), mTorque(0.f)
	{
		if (mReference == NULL) {
			LogError("component reference can't be NULL for rigid body");
		}
	}

	RigidBody::~RigidBody() {}

	// get center of mass.
	const Position3 RigidBody::GetCM() {
		return mReference->GetTransform().GetPosition();
	}

	// get mass.
	const real& RigidBody::GetInvMass() const {
		return mInvMass;
	}

	// set mass. if zero, then body is fixed.
	void RigidBody::SetInvMass(real invMass) {
		mInvMass = invMass;
	}

	// get inverse of initial inertia tensor.
	const Matrix3& RigidBody::GetInvInertiaTensor0() const {
		return mInvI0;
	}

	// set inverse initial inertia tensor
	void RigidBody::SetInvInertiaTensor0(const Matrix3& invI0) {
		mInvI0 = invI0;
	}

	// get inverse inertia tensor of time t. inv(I(t))
	const Matrix3 RigidBody::GetInvInertiaTensor() const {
		const Matrix4& matRef = mReference->GetTransform().GetMatrix();
		Matrix3 rotMat(
			matRef.row[0].xyz,
			matRef.row[1].xyz,
			matRef.row[2].xyz);
		// inv(I) = R(t) * inverse(I0) * transpos(R(t))
		return rotMat * mInvI0 * rotMat.Transposition();
	}

	// get current linear velocity.
	const Vector3& RigidBody::GetVelocity() const {
		return mVelocity;
	}

	// set current linear velocity.
	void RigidBody::SetVelocity(const Vector3& velocity) {
		mVelocity = velocity;
	}

	// get current angular velocity.
	const Vector3& RigidBody::GetAngularVelocity() const {
		return mAngularVelocity;
	}
	// set current angular velocity.
	void RigidBody::SetAngularVelocity(const Vector3& angularVelocity) {
		mAngularVelocity = angularVelocity;
	}

	// add linear force to this rigid body.
	void RigidBody::AddForce(const Vector3& force) {
		mForce += force;
	}

	// add force which is affected onto the given position.
	void RigidBody::AddForceOn(const Position3& position,
		const Vector3& force)
	{
		// r: CM to position vector
		Vector3 r = position - mReference->GetTransform().GetPosition();
		// ur: -normalize(r)
		Vector3 ur = -r.Normal();

		// force decomposition.
		Vector3 linearF = ur * force.Dot(ur);
		//tangentialF = force - linearF;

		mForce += linearF;
		mTorque += r.Cross(force - linearF);
	}

	// is this body affected by gravity.
	bool RigidBody::IsGravityOn() const {
		return mGravityOn;
	}

	// set this body to be affected by gravity or not.
	void RigidBody::GravityOn(bool on) {
		mGravityOn = on;
	}

	// is this body fixed.
	bool RigidBody::IsFixed() const {
		return (mInvMass == 0);
	}

	// update translational terms and rotational terms.
	void RigidBody::Update() {
		if (mInvMass == 0) {
			mVelocity = 0.f;
			mAngularVelocity = 0.f;
			return;
		}

		const real& dt = Engine::GetInstance()->GetTimer().GetDeltaTime();
		Transform& transRef = mReference->GetTransform();

		if (mGravityOn)
			mForce += Vector3::Gravity;


		// translational terms.
		// x(t+ツt) = x(t) + v(t)*ツt
		transRef.TranslateMore(mVelocity*dt);

		// from F(t) = M*a(t), a(t) = F(t)/M
		Vector3 acc = mForce * mInvMass;

		// v(t+ツt) = v(t) + a(t)*ツt
		mVelocity = mVelocity + acc*dt;

		// initialize force
		mForce = 0.f;


		// rotational terms.
		// R(t+ツt) <= q(t+ツt) = q_w(t)q(t)
		Quaternion q = transRef.GetRotation();
		real magw = mAngularVelocity.Magnitude();
		if (math::real_notequal(magw, 0.f)) {
			// ヨ = |��|*ツt
			real half_theta = magw*dt / 2.f;
			Vector3 unitw = mAngularVelocity / magw;
			Quaternion q_w(math::sin(half_theta)*unitw, math::cos(half_theta));

			// q(t+ツt) = q_w(t)q(t)
			transRef.RotateMore(q_w);
		}

		// メ(t) = inv(I(t))*ン(t)
		Vector3 angularAcc = GetInvInertiaTensor() * mTorque;

		// ��(t+ツt) = ��(t) + メ(t)*ツt
		mAngularVelocity = mAngularVelocity + angularAcc*dt;

		// initialize torque
		mTorque = 0.f;
	}

}
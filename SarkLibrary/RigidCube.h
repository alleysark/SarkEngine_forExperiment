#ifndef __RIGID_CUBE_H__
#define __RIGID_CUBE_H__

#include "ASceneComponent.h"

namespace sark{

	class Mesh;
	class RigidBody;
	class ACollider;

	// simple rigid cube scene component.
	class RigidCube : public ASceneComponent{
	private:
		// mesh object
		Mesh* mMesh;

		// rigid body object
		RigidBody* mRigidBody;

		// collider
		ACollider* mCollider;

		// basic properties of cube
		real mWidth, mHeight, mDepth;

	public:
		// create cube from given properties
		RigidCube(real width, real height, real depth,
			real invMass = 0.f,
			const Vector3& velocity = 0.f, const Vector3& angularVelocity = 0.f,
			bool gravityOn = true);

		// create cube from given properties
		RigidCube(const std::string& name, ASceneComponent* parent, bool activate,
			real width, real height, real depth,
			real invMass = 0.f,
			const Vector3& velocity = 0.f, const Vector3& angularVelocity = 0.f,
			bool gravityOn = true);
		
		~RigidCube();

		// get width of cube.
		const real& GetWidth() const;
		// get height of cube.
		const real& GetHeight() const;
		// get depth of cube.
		const real& GetDepth() const;

		// get collider.
		const ACollider* GetCollider() const override;
		
		// set new collider. or set NULL.
		// you may pass new collider through memory allocation.
		// *note: do not pass the address of plane variable.
		void SetCollider(ACollider* newColl) override;

		Mesh* GetMesh() override;

		RigidBody* GetRigidBody() override;

		void Update() override;

		void Render() override;

	private:
		void CreateCube();
	};

}
#endif
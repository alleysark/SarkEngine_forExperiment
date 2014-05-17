#ifndef __RIGID_CUBE_H__
#define __RIGID_CUBE_H__

#include "ASceneComponent.h"
#include "Mesh.h"
#include "RigidBody.h"
#include "shapes.h"

namespace sark{

	// simple rigid cube scene component.
	class RigidCube : public ASceneComponent{
	private:
		// mesh object
		Mesh* mMesh;

		// rigid body object
		RigidBody* mRigidBody;

		// bounding box
		OrientedBox* mOBox;

		// basic properties of cube
		real mWidth, mHeight, mDepth;

	public:
		// create cube from given properties
		RigidCube(real width, real height, real depth,
			real invMass = 1.f,
			const Vector3& velocity = 0.f, const Vector3& angularVelocity = 0.f,
			bool gravityOn = true, bool fixed = false);

		// create cube from given properties
		RigidCube(const std::string& name, ASceneComponent* parent, bool activate,
			real width, real height, real depth,
			real invMass = 1.f,
			const Vector3& velocity = 0.f, const Vector3& angularVelocity = 0.f,
			bool gravityOn = true, bool fixed = false);
		
		~RigidCube();

		// get width of cube.
		const real& GetWidth() const;
		// get height of cube.
		const real& GetHeight() const;
		// get depth of cube.
		const real& GetDepth() const;

		const IShape* GetBoundingShape() const override;

		Mesh* GetMesh() override;

		RigidBody* GetRigidBody() override;

		void Update() override;

		void Render() override;

	private:
		void CreateCube();
	};

}
#endif
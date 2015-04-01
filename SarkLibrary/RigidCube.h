#ifndef __RIGID_CUBE_H__
#define __RIGID_CUBE_H__

#include "StaticModel.h"

namespace sark {

	// simple rigid cube scene component.
	class RigidCube : public StaticModel {
	private:
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

	private:
		void CreateCube();
	};

}
#endif
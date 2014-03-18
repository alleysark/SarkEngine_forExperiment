#ifndef __RIGID_CUBE_H__
#define __RIGID_CUBE_H__

#include "ASceneComponent.h"
#include "Mesh.h"

namespace sark{

	// simple rigid cube scene component.
	class RigidCube : public ASceneComponent{
	private:
		Mesh mMesh;

		// basic properties of cube
		real mWidth, mHeight, mDepth;

	public:
		// create cube from given properties
		RigidCube(real width, real height, real depth);

		~RigidCube();

		void Update();

		void Render();
	};

}
#endif
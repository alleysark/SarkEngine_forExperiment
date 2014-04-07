#ifndef __RIGID_CUBE_H__
#define __RIGID_CUBE_H__

#include "ASceneComponent.h"
#include "Mesh.h"
#include "shapes.h"

namespace sark{

	// simple rigid cube scene component.
	class RigidCube : public ASceneComponent{
	private:
		Mesh mMesh;

		// bounding box
		OrientedBox mOBox, mUpdatedObox;

		// basic properties of cube
		real mWidth, mHeight, mDepth;

	public:
		// create cube from given properties
		RigidCube(real width, real height, real depth);

		~RigidCube();

		const IShape* GetBoundingShape() const override;

		void Update() override;

		void Render() override;
	};

}
#endif
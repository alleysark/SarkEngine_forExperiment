#ifndef __RIGID_CUBE_H__
#define __RIGID_CUBE_H__

#include "ASceneComponent.h"
#include "Mesh.h"
#include "shapes.h"

namespace sark{

	// simple rigid cube scene component.
	class RigidCube : public ASceneComponent{
	private:
		// mesh object
		Mesh* mMesh;

		// bounding box
		OrientedBox mOBox, mUpdatedObox;

		// basic properties of cube
		real mWidth, mHeight, mDepth;

	public:
		// create cube from given properties
		RigidCube(real width, real height, real depth);

		// create cube from given properties
		RigidCube(const std::string& name, ASceneComponent* parent, bool activate,
			real width, real height, real depth);
		
		~RigidCube();

		// get width of cube.
		const real& GetWidth() const;
		// get height of cube.
		const real& GetHeight() const;
		// get depth of cube.
		const real& GetDepth() const;

		const IShape* GetBoundingShape() const override;

		void Update() override;

		void Render() override;

	private:
		void CreateCube();
	};

}
#endif
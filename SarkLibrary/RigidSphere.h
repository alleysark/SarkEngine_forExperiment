#ifndef __RIGID_SPHERE_H__
#define __RIGID_SPHERE_H__

#include "ASceneComponent.h"
#include "Mesh.h"
#include "shapes.h"

namespace sark{

	// simple rigid sphere scene component.
	class RigidSphere : public ASceneComponent{
	private:
		// static mesh object
		Mesh* mMesh;

		// bounding sphere
		Sphere mSphere;

		// radius
		real mRadius;
		uinteger mSlice, mStack;

	public:
		// create sphere from given properties
		RigidSphere(real radius, uinteger slice, uinteger stack);

		// create sphere from given properties
		RigidSphere(const std::string& name, ASceneComponent* parent, bool activate,
			real radius, uinteger slice, uinteger stack);

		~RigidSphere();

		const IShape* GetBoundingShape() const override;

		void Update() override;

		void Render() override;

	private:
		void CreateSphere();
	};

}
#endif
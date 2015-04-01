#ifndef __A_MODEL_H__
#define __A_MODEL_H__

#include <memory>
#include "core.h"
#include "ASceneComponent.h"
#include "IUncopiable.hpp"

namespace sark {

	class Mesh;
	class Material;
	class RigidBody;
	class ACollider;

	// renderable model component.
	// for simplification, it uses single mesh model.
	class AModel : public ASceneComponent, IUncopiable {
	protected:
		// single mesh.
		Mesh* mMesh;
		
		// material reference
		Material* mMaterial;

		// model can be rigid body.
		RigidBody* mRigidBody;

		// model can have its own collider.
		ACollider* mCollider;

	public:
		AModel(const std::string& name, ASceneComponent* parent, bool activate);

		virtual ~AModel();

	public:
		// get collider. it is Nullable pointer.
		ACollider* GetCollider() override;

		// set new collider or empty by null pointer
		void SetCollider(ACollider* newColl);

		// get mesh object of scene component.
		Mesh* GetMesh() override;

		// get material reference of model
		Material* GetMaterial();

		// set material
		void SetMaterial(Material* material);

		// get rigid body. it can be NULL for the non-rigid body.
		RigidBody* GetRigidBody() override;

		// set new rigid body or empty by null pointer
		// 
		// *note: if you already have rigid body object, you can modify
		// its properties through the instance pointer from GetRigidBody().
		// it's better then reallocation with entire properties.
		void SetRigidBody(RigidBody* newBody);
	};

}
#endif
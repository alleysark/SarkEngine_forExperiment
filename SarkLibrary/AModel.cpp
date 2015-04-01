#include "AModel.h"
#include "Mesh.h"
#include "Material.h"
#include "RigidBody.h"
#include "ACollider.h"

namespace sark {

	AModel::AModel(const std::string& name, ASceneComponent* parent, bool activate)
		: ASceneComponent(name, parent, activate),
		mMesh(NULL), mRigidBody(NULL), mCollider(NULL)
	{
		mMesh = new Mesh();
	}

	AModel::~AModel() {
		if (mMesh != NULL)
			delete mMesh;
		if (mRigidBody != NULL)
			delete mRigidBody;
		if (mCollider != NULL)
			delete mCollider;
	}

	// get collider. it is Nullable pointer.
	ACollider* AModel::GetCollider() {
		return mCollider;
	}

	// set new collider or empty by null pointer
	void AModel::SetCollider(ACollider* newColl) {
		mCollider = newColl;
	}

	// get mesh object of scene component.
	Mesh* AModel::GetMesh() {
		return mMesh;
	}

	Material* AModel::GetMaterial() {
		return mMaterial;
	}

	// set material
	void AModel::SetMaterial(Material* material) {
		mMaterial = material;
	}

	// get rigid body. it can be NULL for the non-rigid body.
	RigidBody* AModel::GetRigidBody() {
		return mRigidBody;
	}

	// set new rigid body or empty unique_ptr.
	void AModel::SetRigidBody(RigidBody* newBody) {
		mRigidBody = newBody;
	}

}
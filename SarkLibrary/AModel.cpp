#include "AModel.h"
#include "Mesh.h"
#include "Material.h"
#include "RigidBody.h"
#include "ACollider.h"

namespace sark{

	AModel::AModel(const std::string& name, ASceneComponent* parent, bool activate)
		: ASceneComponent(name, parent, activate),
		mMesh(NULL), mRigidBody(NULL), mCollider(NULL)
	{
		mMesh = new Mesh();
	}

	AModel::~AModel(){
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

	// set new collider or empty unique_ptr.
	void AModel::SetCollider(std::unique_ptr<ACollider>& newColl){
		if (mCollider != NULL){
			delete mCollider;
			mCollider = NULL;
		}

		if (newColl){
			mCollider = newColl.get();
			newColl.release();
		}
	}

	// get mesh object of scene component.
	Mesh* AModel::GetMesh(){
		return mMesh;
	}

	std::shared_ptr<Material> AModel::GetMaterial() {
		return mMaterialRef;
	}

	// set material
	void AModel::SetMaterial(std::shared_ptr<Material> material) {
		mMaterialRef = material;
	}

	// get rigid body. it can be NULL for the non-rigid body.
	RigidBody* AModel::GetRigidBody(){
		return mRigidBody;
	}

	// set new rigid body or empty unique_ptr.
	void AModel::SetRigidBody(std::unique_ptr<RigidBody>& newBody){
		if (mRigidBody != NULL){
			delete mRigidBody;
			mRigidBody = NULL;
		}

		if (newBody){
			mRigidBody = newBody.get();
			newBody.release();
		}
	}

}
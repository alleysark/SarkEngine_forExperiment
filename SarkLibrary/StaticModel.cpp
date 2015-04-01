#include "StaticModel.h"
#include "Mesh.h"
#include "ACollider.h"
#include "RigidBody.h"

namespace sark {

	StaticModel::StaticModel(const std::string& name, 
		ASceneComponent* parent, bool activate)
		: AModel(name, parent, activate)
	{}

	StaticModel::~StaticModel() {}

	void StaticModel::Update() {
		if (mCollider != NULL)
			mCollider->Update();
		if (mRigidBody != NULL)
			mRigidBody->Update();
	}

	void StaticModel::Render() {
		mMesh->Draw();
	}

}
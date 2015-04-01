#include "ASceneComponent.h"

namespace sark {

	ASceneComponent::ComponentID ASceneComponent::_nextComponentID = 0;
	ASceneComponent::ComponentID ASceneComponent::_getUniqueComponentID() {
		return _nextComponentID++;
	}

	// component id is automatically generated when constructor is called.
	// 'name' can be empty to make default name.
	// 'parent' can be NULL for the root scene component.
	// 'activate' avtivates this scene component or not.
	ASceneComponent::ASceneComponent(const std::string& name, ASceneComponent* parent, bool activate)
		: mParent(parent), mTransform(this), mActivated(activate)
	{
		mComponentId = _getUniqueComponentID();
		if (name.empty())
			mComponentName = std::to_string(mComponentId);
		else
			mComponentName = name;

		if (parent != NULL)
			parent->AddChild(this);
	}

	// every derived class have to ensure release of your resources.
	ASceneComponent::~ASceneComponent() {}

	// get scene component ID
	const ASceneComponent::ComponentID& ASceneComponent::GetComponentID() const {
		return mComponentId;
	}
	// set scene component name
	void ASceneComponent::SetComponentName(const std::string& name) {
		mComponentName = name;
	}

	// get scene component name
	const std::string& ASceneComponent::GetComponentName() const {
		return mComponentName;
	}


	// get parent component pointer
	ASceneComponent* ASceneComponent::GetParent() const {
		return mParent;
	}
	// set new parent. parent can be NULL to make this global component.
	void ASceneComponent::SetParent(ASceneComponent* newParent) {
		if (mParent != NULL) {
			mParent->DeleteChild(mComponentId);
		}

		mParent = newParent;
		if (newParent != NULL) {
			newParent->AddChild(this);
		}
	}


	// get a child of id
	ASceneComponent* ASceneComponent::GetChild(const ComponentID& id) {
		if (mChildren.empty())
			return NULL;

		ChildContainer::iterator itr = mChildren.begin();
		ChildContainer::iterator end = mChildren.end();
		for (; itr != end; itr++) {
			if ((*itr)->GetComponentID() == id)
				return *itr;
		}
		return NULL;
	}
	// get a child who is firstly matched with queried name
	ASceneComponent* ASceneComponent::GetChild(const std::string& name) {
		if (mChildren.empty())
			return NULL;

		ChildContainer::iterator itr = mChildren.begin();
		ChildContainer::iterator end = mChildren.end();
		for (; itr != end; itr++) {
			if ((*itr)->GetComponentName() == name)
				return *itr;
		}
		return NULL;
	}

	// add a child into its children container as uniquely
	bool ASceneComponent::AddChild(ASceneComponent* child) {
		if (GetChild(child->GetComponentID()) != NULL)
			return false;
		mChildren.push_back(child);
		return true;
	}

	// delete a child from its children container. it doesn't delete pulled child from memory
	void ASceneComponent::DeleteChild(const ComponentID& id) {
		if (mChildren.size() == 0)
			return;

		ASceneComponent* pulled = NULL;
		ChildContainer::iterator itr = mChildren.begin();
		ChildContainer::iterator end = mChildren.end();
		for (; itr != end; itr++) {
			if ((*itr)->GetComponentID() == id) {
				mChildren.erase(itr);
				break;
			}
		}
	}


	// get all the children who are matched with queried name
	std::list<ASceneComponent*> ASceneComponent::GetChildren(const std::string& name) {
		std::list<ASceneComponent*> results;

		ChildContainer::iterator itr = mChildren.begin();
		ChildContainer::iterator end = mChildren.end();
		for (; itr != end; itr++) {
			if ((*itr)->GetComponentName() == name) {
				results.push_back(*itr);
			}
		}
		return results;
	}
	// save all the children who are matched with queried name 
	// into given list reference. it returns the size of result.
	uint32 ASceneComponent::GetChildren(const std::string& name, 
		std::list<ASceneComponent*>& refContainer)
	{
		if (mChildren.size() == 0)
			return 0;

		ChildContainer::iterator itr = mChildren.begin();
		ChildContainer::iterator end = mChildren.end();
		for (; itr != end; itr++) {
			if ((*itr)->GetComponentName() == name) {
				refContainer.push_back(*itr);
			}
		}
		return refContainer.size();
	}


	// get the children container
	ASceneComponent::ChildContainer& ASceneComponent::GetChildren() {
		return mChildren;
	}

	// is this scene component child of given component?
	bool ASceneComponent::IsChildOf(const ComponentID& id) {
		ASceneComponent* parent = mParent;
		while (parent != NULL) {
			if (parent->GetComponentID() == id) {
				return true;
			}
			else {
				parent = parent->GetParent();
			}
		}
		return false;
	}


	// get transform object of this component. it is local transform object.
	Transform& ASceneComponent::GetTransform() {
		return mTransform;
	}

	// is this component activated?
	bool ASceneComponent::IsActive() const {
		return mActivated;
	}
	// set component activation property
	void ASceneComponent::Activate(bool act) {
		mActivated = act;
	}

}
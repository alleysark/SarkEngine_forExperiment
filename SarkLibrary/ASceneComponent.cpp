#include "ASceneComponent.h"

namespace sark{

	ASceneComponent::ComponentID ASceneComponent::_nextComponentID = 0;
	ASceneComponent::ComponentID ASceneComponent::_getUniqueComponentID(){
		return _nextComponentID++;
	}

	// component id is automatically generated when constructor is called
	ASceneComponent::ASceneComponent(ASceneComponent* parent)
		: mParent(parent), mTransform(this), mActivated(true)
	{
		mComponentId = _getUniqueComponentID();
		mComponentName = std::to_string(mComponentId);
	}

	// component id is automatically generated when constructor is called.
	ASceneComponent::ASceneComponent(const std::string& name, ASceneComponent* parent)
		: mParent(parent), mTransform(this), mActivated(true)
	{
		mComponentId = _getUniqueComponentID();
		mComponentName = name;
	}

	// every derived class have to ensure release of your resources.
	ASceneComponent::~ASceneComponent(){}

	// get scene component ID
	const ASceneComponent::ComponentID& ASceneComponent::GetComponentID() const{
		return mComponentId;
	}
	// set scene component name
	void ASceneComponent::SetComponentName(const std::string& name){
		mComponentName = name;
	}

	// get scene component name
	const std::string& ASceneComponent::GetComponentName() const{
		return mComponentName;
	}


	// get parent component pointer
	ASceneComponent* ASceneComponent::GetParent() const{
		return mParent;
	}
	// set new parent. parent can be NULL to make this global component.
	void ASceneComponent::SetParent(ASceneComponent* newParent){
		if (mParent != NULL){
			mParent->PullChild(mComponentId);
		}

		mParent = newParent;
		if (newParent != NULL){
			newParent->PushChild(this);
		}
	}


	// get a child of id
	ASceneComponent* ASceneComponent::GetChild(const ComponentID& id){
		if (mChildren.size() == 0)
			return NULL;

		ChildComponentContainer::iterator itr = mChildren.begin();
		ChildComponentContainer::iterator end = mChildren.end();
		for (; itr != end; itr++){
			if ((*itr)->GetComponentID() == id)
				return (*itr);
		}
		return NULL;
	}
	// get a child who is firstly matched with queried name
	ASceneComponent* ASceneComponent::GetChild(const std::string& name){
		if (mChildren.size() == 0)
			return NULL;

		ChildComponentContainer::iterator itr = mChildren.begin();
		ChildComponentContainer::iterator end = mChildren.end();
		for (; itr != end; itr++){
			if ((*itr)->GetComponentName() == name)
				return (*itr);
		}
		return NULL;
	}

	// push a child into its children container as uniquely
	bool ASceneComponent::PushChild(ASceneComponent* child){
		if (GetChild(child->GetComponentID()) != NULL)
			return false;
		mChildren.push_back(child);
		return true;
	}

	// pull a child from its children container. it doesn't delete pulled child from memory
	ASceneComponent* ASceneComponent::PullChild(const ComponentID& id){
		if (mChildren.size() == 0)
			return NULL;

		ASceneComponent* pulled = NULL;
		ChildComponentContainer::iterator itr = mChildren.begin();
		ChildComponentContainer::iterator end = mChildren.end();
		for (; itr != end; itr++){
			if ((*itr)->GetComponentID() == id){
				pulled = (*itr);
				mChildren.erase(itr);
				break;
			}
		}
		return pulled;
	}


	// get all the children who are matched with queried name
	const ASceneComponent::ChildComponentContainer ASceneComponent::GetChildren(const std::string& name){
		ChildComponentContainer results;

		ChildComponentContainer::iterator itr = mChildren.begin();
		ChildComponentContainer::iterator end = mChildren.end();
		for (; itr != end; itr++){
			if ((*itr)->GetComponentName() == name)
				results.push_back((*itr));
		}
		return results;
	}
	// save all the children who are matched with queried name 
	// into given list reference. it returns the size of result.
	uint32 ASceneComponent::GetChildren(const std::string& name, ChildComponentContainer& refContainer){
		if (mChildren.size() == 0)
			return 0;

		ChildComponentContainer::iterator itr = mChildren.begin();
		ChildComponentContainer::iterator end = mChildren.end();
		for (; itr != end; itr++){
			if ((*itr)->GetComponentName() == name)
				refContainer.push_back((*itr));
		}
		return refContainer.size();
	}


	// get the children container
	ASceneComponent::ChildComponentContainer& ASceneComponent::GetChildren(){
		return mChildren;
	}


	// get transform object of this component. it is local transform object.
	Transform& ASceneComponent::GetTransform(){
		return mTransform;
	}


	// is this component activated?
	bool ASceneComponent::IsActive() const{
		return mActivated;
	}
	// set component activation property
	void ASceneComponent::Activate(bool act){
		mActivated = act;
	}

}
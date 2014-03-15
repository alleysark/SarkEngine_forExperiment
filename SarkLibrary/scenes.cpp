#include "scenes.h"
#include <algorithm>

namespace sark{

	//=============================================
	//		ASceneComponent class implementation
	//=============================================

	ASceneComponent::ComponentID ASceneComponent::_nextComponentID = 0;
	ASceneComponent::ComponentID ASceneComponent::_getUniqueComponentID(){
		return _nextComponentID++;
	}

	// component id is automatically generated when constructor is called
	ASceneComponent::ASceneComponent(ASceneComponent* parent)
		: mParent(parent), mActivated(true)
	{
		mComponentId = _getUniqueComponentID();
		mComponentName = std::to_string(mComponentId);
	}
	
	// component id is automatically generated when constructor is called.
	ASceneComponent::ASceneComponent(const std::string& name, ASceneComponent* parent)
		: mParent(parent), mActivated(true)
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
		mParent = newParent;
	}


	// get a child of id
	ASceneComponent* ASceneComponent::GetChild(const ComponentID& id){
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
		ChildComponentContainer::iterator itr = mChildren.begin();
		ChildComponentContainer::iterator end = mChildren.end();
		for (; itr != end; itr++){
			if ((*itr)->GetComponentName() == name)
				return (*itr);
		}
		return NULL;
	}
	// get all the children who are matched with queried name
	std::list<ASceneComponent*> ASceneComponent::GetChildren(const std::string& name){
		std::list<ASceneComponent*> results;

		ChildComponentContainer::iterator itr = mChildren.begin();
		ChildComponentContainer::iterator end = mChildren.end();
		for (; itr != end; itr++){
			if ((*itr)->GetComponentName() == name)
				results.push_back((*itr));
		}
		return results;
	}

	// get the children container
	const ASceneComponent::ChildComponentContainer& ASceneComponent::GetChildren() const{
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



	//=============================================
	//		AScene::Layer class implementation
	//=============================================

	// if layer needs to be sorted, scene will sort it when camera is updated
	AScene::Layer::Layer(bool needSorted){
		mNeedSorted = needSorted;
	}

	// layer is deleted with all the contained scene components
	AScene::Layer::~Layer(){
		ComponentReplicaArray::iterator itr = componentReplicas.begin();
		ComponentReplicaArray::iterator end = componentReplicas.end();
		for (; itr != end; itr++){
			delete (*itr);
		}
		components.clear();
		componentReplicas.clear();
	}

	// does it need to be sorted?
	bool AScene::Layer::NeedSorted() const{
		return mNeedSorted;
	}

	// sort all scene components in this layer by relative distance of input position
	void AScene::Layer::Sort(const Position3& position){
		// update relative distance
		ComponentReplicaArray::iterator itr = componentReplicas.begin();
		ComponentReplicaArray::iterator end = componentReplicas.end();
		for (; itr != end; itr++){
			(*itr)->rel_distance 
				= ((*itr)->GetTransform().GetPosition() - position).MagnitudeSq();
		}

		// sort
		std::sort(
			componentReplicas.begin(), 
			componentReplicas.end(), 
			[](const ASceneComponent* lhs, const ASceneComponent* rhs)->bool{
			return (lhs->rel_distance <= rhs->rel_distance);
		});
	}

	// add scene component into this layer
	void AScene::Layer::AddSceneComponent(ASceneComponent* sceneComponent){
		if (sceneComponent == NULL)
			return;

		ComponentMap::const_iterator find = components.find(sceneComponent->GetComponentID());
		if (find != components.cend())
			return;

		components.insert(find, ComponentMap::value_type(sceneComponent->GetComponentID(), sceneComponent));
		componentReplicas.push_back(sceneComponent);
	}

	// delete scene component from this layer
	void AScene::Layer::DeleteSceneComponent(ASceneComponent* sceneComponent){
		if (sceneComponent == NULL)
			return;

		// erase input thing from component map
		ComponentMap::const_iterator find = components.find(sceneComponent->GetComponentID());
		if (find == components.cend())
			return;
		components.erase(find);

		// and also erase from replica array
		ComponentReplicaArray::iterator itr = componentReplicas.begin();
		ComponentReplicaArray::iterator end = componentReplicas.end();
		for (; itr != end; itr++){
			if ((*itr) == sceneComponent){
				componentReplicas.erase(itr);
				break;
			}
		}
	}
	// delete scene component from this layer
	void AScene::Layer::DeleteSceneComponent(const ASceneComponent::ComponentID& componentId){
		// erase input thing from component map
		ComponentMap::const_iterator find = components.find(componentId);
		if (find == components.cend())
			return;
		components.erase(find);

		// and also erase from replica array
		ComponentReplicaArray::iterator itr = componentReplicas.begin();
		ComponentReplicaArray::iterator end = componentReplicas.end();
		for (; itr != end; itr++){
			if ((*itr)->GetComponentID() == componentId){
				componentReplicas.erase(itr);
				break;
			}
		}
	}

	// find the scene component from given component id
	ASceneComponent* AScene::Layer::FindSceneComponent(const ASceneComponent::ComponentID& componentId){
		ComponentMap::const_iterator find = components.find(componentId);
		if (find == components.cend())
			return NULL;
		return find->second;
	}

	// update all scene components
	void AScene::Layer::UpdateAll(){
		ComponentReplicaArray::iterator itr = componentReplicas.begin();
		ComponentReplicaArray::iterator end = componentReplicas.end();
		for (; itr != end; itr++){
			if (!(*itr)->IsActive())
				continue;
			(*itr)->Update();
		}
	}

	// render all scene components
	void AScene::Layer::RenderAll(){
		ComponentReplicaArray::iterator itr = componentReplicas.begin();
		ComponentReplicaArray::iterator end = componentReplicas.end();
		for (; itr != end; itr++){
			if (!(*itr)->IsActive())
				continue;
			(*itr)->Render();
		}
	}



	//=============================================
	//			AScene class implementation
	//=============================================

	AScene::AScene(){
	}

	AScene::~AScene(){
		LayerContainer::iterator itr = mLayers.begin();
		LayerContainer::iterator end = mLayers.end();
		for (; itr != end; itr++){
			delete (*itr);
		}
		mLayers.clear();

		CameraContainer::iterator camItr = mCameras.begin();
		CameraContainer::iterator camEnd = mCameras.end();
		for (; camItr != camEnd; camItr++){
			delete (*itr);
		}
		mCameras.clear();
		mMainCam = NULL;
	}

}
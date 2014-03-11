#include "scenes.h"

namespace sarklib{

	//=============================================
	//		ASceneComponent class implementation
	//=============================================

	ASceneComponent::ComponentID ASceneComponent::_nextComponentID = 0;
	ASceneComponent::ComponentID ASceneComponent::_getUniqueComponentID(){
		return _nextComponentID++;
	}

	ASceneComponent::ASceneComponent(){
		mComponentId = _getUniqueComponentID();
		mbVisible = true;
	}

	ASceneComponent::~ASceneComponent(){}

	const ASceneComponent::ComponentID& ASceneComponent::GetComponentID() const{
		return mComponentId;
	}

	Transform& ASceneComponent::GetTransform(){
		return mTransform;
	}

	bool ASceneComponent::IsVisible() const{
		return mbVisible;
	}

	void ASceneComponent::SetVisibility(bool visibility){
		mbVisible = visibility;
	}



	//=============================================
	//		AScene::Layer class implementation
	//=============================================

	AScene::Layer::Layer(bool needSorted){
		mbNeedSorted = needSorted;
	}

	void AScene::Layer::Sort(Vector3 pivotPosition){
		real* distanceArr = new real[sceneComponents.size()];

		for (ComponentContainer::iterator itr = sceneComponents.begin();
			itr != sceneComponents.end(); itr++){

		}
	}

	ASceneComponent* AScene::Layer::FindSceneComponent(const ASceneComponent::ComponentID& componentId){
		ComponentContainer::iterator itr = sceneComponents.begin();
		ComponentContainer::iterator itrEnd = sceneComponents.end();
		for (; itr != itrEnd; itr++){
			if ((*itr)->GetComponentID() == componentId){
				return (*itr);
			}
		}
		return NULL;
	}



	//=============================================
	//			AScene class implementation
	//=============================================

	AScene::AScene(std::string name){
		mstrName = name;
	}

	AScene::~AScene(){}

	const std::string& AScene::GetName() const{
		return mstrName;
	}

	ASceneComponent* AScene::GetSceneComponent(const ASceneComponent::ComponentID& componentId, std::string layerName){
		LayerContainer::iterator findedLayer = mLayers.find(layerName);
		if (findedLayer != mLayers.end())
			return NULL;

		return findedLayer->second->FindSceneComponent(componentId);
	}
}
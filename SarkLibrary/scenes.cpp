#include "scenes.h"

namespace sarklib{

	//=============================================
	//		ASceneComponent class implementation
	//=============================================

	ASceneComponent::ASceneComponent(){
		mbVisible = true;
	}

	ASceneComponent::~ASceneComponent(){}

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

}
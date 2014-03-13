#include "scenes.h"
#include <algorithm>

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
		mbEnable = true;
	}

	ASceneComponent::~ASceneComponent(){}

	const ASceneComponent::ComponentID& ASceneComponent::GetComponentID() const{
		return mComponentId;
	}

	Transform& ASceneComponent::GetTransform(){
		return mTransform;
	}

	bool ASceneComponent::IsEnable() const{
		return mbEnable;
	}

	void ASceneComponent::Enable(bool enable){
		mbEnable = enable;
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
			if (!(*itr)->IsEnable())
				continue;
			(*itr)->Update();
		}
	}

	// render all scene components
	void AScene::Layer::RenderAll(){
		ComponentReplicaArray::iterator itr = componentReplicas.begin();
		ComponentReplicaArray::iterator end = componentReplicas.end();
		for (; itr != end; itr++){
			if (!(*itr)->IsEnable())
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
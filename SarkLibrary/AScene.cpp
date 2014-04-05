#include "AScene.h"
#include <algorithm>

namespace sark{

	//=============================================
	//		AScene::Layer class implementation
	//=============================================

	AScene::Layer::Layer(){
	}

	AScene::Layer::~Layer(){
		mReplicas.clear();
	}

	// push component into this layer
	void AScene::Layer::Push(ASceneComponent* component){
		mReplicas.push_back(component);
	}

	// pop component from this layer
	void AScene::Layer::Pop(ASceneComponent* component){
		ReplicaArray::iterator itr = mReplicas.begin();
		ReplicaArray::iterator end = mReplicas.end();
		for (; itr != end; itr++){
			if ((*itr) == component){
				mReplicas.erase(itr);
				break;
			}
		}
	}
	// pop component from this layer
	void AScene::Layer::Pop(const ASceneComponent::ComponentID& componentId){
		ReplicaArray::iterator itr = mReplicas.begin();
		ReplicaArray::iterator end = mReplicas.end();
		for (; itr != end; itr++){
			if ((*itr)->GetComponentID() == componentId){
				mReplicas.erase(itr);
				break;
			}
		}
	}
	// pop component from this layer
	void AScene::Layer::Pop(ReplicaArrayIterator itrator){
		mReplicas.erase(itrator);
	}

	// sort all scene components in this layer by relative distance of input position
	void AScene::Layer::Sort(const Position3& position){
		// update relative distance
		ReplicaArray::iterator itr = mReplicas.begin();
		ReplicaArray::iterator end = mReplicas.end();
		for (; itr != end; itr++){
			(*itr)->rel_distance 
				= ((*itr)->GetTransform().GetPosition() - position).MagnitudeSq();
		}

		// sort
		std::sort(
			mReplicas.begin(),
			mReplicas.end(),
			[](const ASceneComponent* lhs, const ASceneComponent* rhs)->bool{
			return (lhs->rel_distance <= rhs->rel_distance);
		});
	}

	// get array iterator of begin
	AScene::Layer::ReplicaArrayIterator AScene::Layer::Begin(){
		return mReplicas.begin();
	}

	// get array iterator of end
	AScene::Layer::ReplicaArrayIterator AScene::Layer::End(){
		return mReplicas.end();
	}

	// update only for this layer
	void AScene::Layer::UpdateAll(){
		ReplicaArray::iterator itr = mReplicas.begin();
		ReplicaArray::iterator end = mReplicas.end();
		for (; itr != end; itr++){
			if (!(*itr)->IsActive())
				continue;
			(*itr)->Update();
		}
	}

	// render only for this layer
	void AScene::Layer::RenderAll(){
		ReplicaArray::iterator itr = mReplicas.begin();
		ReplicaArray::iterator end = mReplicas.end();
		for (; itr != end; itr++){
			if (!(*itr)->IsActive())
				continue;
			(*itr)->Render();
		}
	}



	//=============================================
	//			AScene class implementation
	//=============================================

	AScene::AScene(){}

	AScene::~AScene(){}

	// clear whole scene components
	void AScene::ClearSceneComponents(){
		ComponentMap::iterator itr = mComponents.begin();
		ComponentMap::iterator end = mComponents.end();
		for (; itr != end; itr++){
			delete itr->second;
		}
		mComponents.clear();
	}

	// clear whole cameras
	void AScene::ClearCameras(){
		CameraContainer::iterator itr = mCameras.begin();
		CameraContainer::iterator end = mCameras.end();
		for (; itr != end; itr++){
			delete (*itr);
		}
		mCameras.clear();
		mMainCam = NULL;
	}


	// add scene component
	bool AScene::AddSceneComponent(ASceneComponent* sceneComponent){
		if (sceneComponent == NULL)
			return false;

		ComponentMap::const_iterator find = mComponents.find(sceneComponent->GetComponentID());
		if (find != mComponents.cend())
			return false;
	
		mComponents.insert(find, ComponentMap::value_type(sceneComponent->GetComponentID(), sceneComponent));
		return true;
	}

	// delete scene component by address
	bool AScene::DeleteSceneComponent(ASceneComponent* sceneComponent){
		if (sceneComponent == NULL)
			return false;

		// erase input thing from component map
		ComponentMap::const_iterator find = mComponents.find(sceneComponent->GetComponentID());
		if (find == mComponents.cend())
			return false;
		
		mComponents.erase(find);
		return true;
	}

	// delete scene component by component id
	bool AScene::DeleteSceneComponent(const ASceneComponent::ComponentID& componentId){
		// erase input thing from component map
		ComponentMap::const_iterator find = mComponents.find(componentId);
		if (find == mComponents.cend())
			return false;
		
		mComponents.erase(find);
		return true;
	}

	// delete all scene components by component name
	uinteger AScene::DeleteSceneComponents(const std::string& componentName){
		uinteger count = 0;
		ComponentMap::iterator itr = mComponents.begin();
		ComponentMap::iterator end = mComponents.end();
		for (; itr != end;){
			if (itr->second->GetComponentName() == componentName){
				mComponents.erase(itr++);
				count++;
			}
			else{
				itr++;
			}
		}
		return count;
	}

	// find the scene component from given component id
	ASceneComponent* AScene::FindSceneComponent(const ASceneComponent::ComponentID& componentId){
		ComponentMap::const_iterator find = mComponents.find(componentId);
		if (find == mComponents.cend())
			return NULL;
		return find->second;
	}

	// find whole scene components matched with given component name
	std::list<ASceneComponent*> AScene::FindSceneComponents(const std::string& componentName){
		std::list<ASceneComponent*> out;

		ComponentMap::iterator itr = mComponents.begin();
		ComponentMap::iterator end = mComponents.end();
		for (; itr != end;itr++){
			if (itr->second->GetComponentName() == componentName){
				out.push_back(itr->second);
			}
		}
		return out;
	}
}
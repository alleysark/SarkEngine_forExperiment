#include "resources.h"

namespace sark{

	ResourceManager::ResourceManager(){
		mstrBasePath = "./";
	}

	ResourceManager* ResourceManager::instance = NULL;
	ResourceManager* ResourceManager::GetInstance(){
		if (instance == NULL){
			instance = new ResourceManager();
		}
		return instance;
	}

	void ResourceManager::SetBasePath(const std::string& pathName){
		mstrBasePath = pathName;
	}

	template<class ResourceType>
	const ResourceType* ResourceManager::Load(std::string name){
		ResourceMap::iterator itr = mRescMap.find(name);
		if (itr != mRescMap.end()){
			return dynamic_cast<ResourceType*>(itr->second);
		}

		IResource* resc = ResourceType::Load(this->mstrBasePath + name);
		if (resc == NULL){
			return NULL;
		}

		mResourceMap[name] = resc;

		return resc;
	}

}
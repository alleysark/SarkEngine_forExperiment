#include "resources.h"

namespace sarklib{

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

	template<class ResourceLoaderType>
	const IResource* ResourceManager::Load(std::string name){
		ResourceMap::iterator itr = mRescMap.find(name);
		if (itr != mRescMap.end()){
			return itr->second;
		}

		std::ifstream fin;
		fin.open(this->mstrBasePath + name, std::ios::binary | std::ios::beg);
		if (!fin.is_open()){
			return NULL;
		}

		IResource* resc = ResourceLoaderType::Load(fin);
		if (resc == NULL){
			fin.close();
			return NULL;
		}

		mResourceMap[name] = resc;

		fin.close();
		return resc;
	}

}
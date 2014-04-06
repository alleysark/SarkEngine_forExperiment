#include "resources.h"

namespace sark{

	ResourceManager::ResourceManager()
		: mBasePath("")
	{}

	ResourceManager::~ResourceManager(){
		ResourceMap::iterator itr = mResources.begin();
		ResourceMap::iterator end = mResources.end();
		for (; itr != end; itr++)
			delete itr->second;
		mResources.clear();
	}

	// get base path
	const std::string& ResourceManager::GetBasePath() const{
		return mBasePath;
	}
	// set base path
	void ResourceManager::SetBasePath(const std::string& pathName){
		mBasePath = pathName;
	}

}
#include "resources.h"

namespace sark{

	ResourceManager::ResourceManager()
		: mBasePath("")
	{}

	ResourceManager::~ResourceManager(){
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
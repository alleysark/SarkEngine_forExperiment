#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <string>
#include <map>
#include "core.h"

namespace sark{

	class IResource;
	template<class ImpResourceType> class IResourceLoader;


	// resource management class.
	// for resource loading, caching, releasing, etc..
	// resources can be cached by its name when it is already loaded before using
	class ResourceManager{
	public:
		typedef std::map<std::string, IResource*> ResourceMap;

	private:
		// resources are stored
		ResourceMap mResources;

		// base path string
		std::string mBasePath;
		
	public:
		ResourceManager();

		~ResourceManager();

		// get base path
		const std::string& GetBasePath() const;
		// set base path
		void SetBasePath(const std::string& pathName);

		template<class ResourceType>
		ResourceType* Load(const std::string& name){
			ResourceMap::iterator itr = mResources.find(name);
			if (itr != mResources.end()){
				return dynamic_cast<ResourceType*>(itr->second);
			}

			ResourceType* resc = ResourceType::Load(mBasePath + name);
			if (resc == NULL){
				return NULL;
			}

			mResources[name] = resc;
			return resc;
		}
	};


	// resource interface.
	// user defined resource should have inherit it(or sub interfaces)
	// and IResourceLoader.
	class IResource{
	public:
		IResource(){}
		virtual ~IResource(){}
	};

	// resource loader static-interface as <crtp pattern>.
	// user defined resource should have inherit this and IResource.
	template<class ImpResourceType>
	class IResourceLoader{
	public:
		static ImpResourceType* Load(const std::string& path){
			return ImpResourceType::LoadImp(path);
		}

		// do not forget to implementing static method 'LoadImp'.
		// it'll emit error if you are not.
		// static ImpResourceType* LoadImp(const std::string& path) = 0;
	};


	// texture resource interface
	class ITextureResource : public IResource{
	protected:
		uinteger mTexId;

	public:
		ITextureResource() :mTexId(0){}
		virtual ~ITextureResource(){}

		// bind this texture
		virtual void Bind() = 0;

		// un bind this texture
		virtual void Unbind() = 0;
	};
}
#endif
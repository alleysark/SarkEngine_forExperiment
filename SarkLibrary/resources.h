#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <string>
#include <hash_map>

namespace sark{

	class IResource;
	template<class ImpResourceType> class IResourceLoader;


	// resource management singleton class
	// for resource loading, caching, releasing, etc..
	// resources can be cached by its name when it is already loaded before using
	class ResourceManager{
	private:
		static ResourceManager* instance;

		std::string mstrBasePath;
		typedef std::hash_map<std::string, IResource*> ResourceMap;
		ResourceMap mRescMap;

		ResourceManager();

	public:
		static ResourceManager* GetInstance();

		void SetBasePath(const std::string& pathName);

		template<class ResourceType>
		const ResourceType* Load(std::string name);
	};


	// resource interface.
	// user defined resource should have inherit this and IResourceLoader.
	class IResource{
	public:
		IResource(){}
		virtual ~IResource(){}

		virtual void Unload() = 0;
	};

	// resource loader static-interface as <crtp pattern>.
	// user defined resource should have inherit this and IResource.
	template<class ImpResourceType>
	class IResourceLoader{
	public:
		static ImpResourceType* Load(std::string path){
			return ImpResourceType::LoadImp(path);
		}

		// do not forget to implementing static method 'LoadImp'.
		// it'll emit error if you are not.
		// static ImpResourceType* LoadImp(std::string path) = 0;
	};
}
#endif
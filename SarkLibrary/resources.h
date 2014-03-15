#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <fstream>
#include <string>
#include <map>

namespace sark{

	class IResource;
	template<class ImpResourceLoaderType> class IResourceLoader;


	// resource management singleton class
	// for resource loading, caching, releasing, etc..
	// resources can be cached by its name when it is already loaded before using
	class ResourceManager{
	private:
		static ResourceManager* instance;

		std::string mstrBasePath;
		typedef std::map<std::string, IResource*> ResourceMap;
		ResourceMap mRescMap;

		ResourceManager();

	public:
		static ResourceManager* GetInstance();

		void SetBasePath(const std::string& pathName);

		template<class ResourceLoaderType>
		const IResource* Load(std::string name);
	};


	// derived class must have implemented functionality of 'unload' in destructor
	class IResource{
	public:
		IResource(){}
		virtual ~IResource(){}
	};

	// crtp pattern
	// derived class must have implemented static method 'LoadImp'
	template<class ImpResourceLoaderType>
	class IResourceLoader{
	public:
		static IResource* Load(std::ifstream& stream){
			return static_cast<ImpResourceLoaderType*>(this)->LoadImp(stream);
		}
	};
}
#endif
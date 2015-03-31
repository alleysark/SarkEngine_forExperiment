#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <string>
#include <map>
#include <GL/glew.h>
#include "core.h"
#include "Texture.h"

namespace sark{

	class IResource;
	template<class ImpResourceType> class IResourceLoader;

	class AModel;
	class ASceneComponent;

	// resource management class.
	// for resource loading, caching, releasing, etc..
	// resources can be cached by its name when it is already loaded before using
	class ResourceManager{
	public:
		typedef std::map<std::string, s_ptr<IResource>> ResourceMap;

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
		s_ptr<ResourceType> Load(const std::string& name){
			ResourceMap::iterator itr = mResources.find(name);
			if (itr != mResources.end()){
				return std::dynamic_pointer_cast<ResourceType>(itr->second);
			}

			s_ptr<ResourceType> resc = ResourceType::Load(mBasePath + name);
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
	// resource is uncopiable object
	class IResource{
	public:
		IResource(){}
		virtual ~IResource(){}
		
	private:
		IResource(const IResource&);
		IResource& operator=(const IResource&);
	};


	// resource loader static-interface as <crtp pattern>.
	// user defined resource should have inherit this and IResource.
	template<class ImpResourceType>
	class IResourceLoader{
	public:
		static s_ptr<ImpResourceType> Load(const std::string& path){
			return ImpResourceType::LoadImp(path);
		}

		// do not forget to implementing static method 'LoadImp'.
		// it'll emit error if you are not.
		// static s_ptr<ImpResourceType> LoadImp(const std::string& path) = 0;
	};

	// texture resource interface
	class ITextureResource : public IResource{
	public:
		ITextureResource(){}
		virtual ~ITextureResource(){}

		// get image width
		virtual const integer GetWidth() const = 0;

		// get image height
		virtual const integer GetHeight() const = 0;

		// get volumatic image depth
		virtual const integer GetDepth() const = 0;

		// get format of pixel
		virtual Texture::Format GetPixelFormat() const = 0;

		// get type of pixel
		virtual Texture::PixelType GetPixelType() const = 0;

		// get raw data of pixels
		virtual const void* GetPixels() const = 0;
	};

	// 3d model resource interface
	class IModelResource : public IResource {
	public:
		IModelResource(){}
		virtual ~IModelResource(){}

		// make this model resource to be center
		virtual void MakeItCenter() = 0;

		// create model component from this resource
		virtual AModel* CreateModel(const std::string& name = "",
			ASceneComponent* parent = NULL, bool activate = true) const = 0;
	};

}
#endif
#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <string>
#include <map>
#include <GL/glew.h>
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
	public:
		enum Format{
			// most common formats
			RGB = GL_RGB,
			RGBA = GL_RGBA,

			// other optional allowed formats
			COLOR_INDEX = GL_COLOR_INDEX,
			RED = GL_RED,
			GREEN = GL_GREEN,
			BLUE = GL_BLUE,
			ALPHA = GL_ALPHA,
			BGR_EXT = GL_BGR_EXT,
			BGRA_EXT = GL_BGRA_EXT,
			LUMINANCE = GL_LUMINANCE,
			LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA
		};

		enum PixelType{
			// most common pixel type
			UINT8 = GL_UNSIGNED_BYTE,

			// other optional allowed pixel types
			INT8 = GL_BYTE,
			BITMAP = GL_BITMAP,
			UINT16 = GL_UNSIGNED_SHORT,
			INT16 = GL_SHORT,
			UINT32 = GL_UNSIGNED_INT,
			INT32 = GL_INT,
			FLOAT32 = GL_FLOAT
		};

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
		virtual Format GetPixelFormat() const = 0;

		// get type of pixel
		virtual PixelType GetPixelType() const = 0;

		// get raw data of pixels
		virtual const void* GetPixels() const = 0;
	};
}
#endif
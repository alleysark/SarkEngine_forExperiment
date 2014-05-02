#ifndef __PNG_RESOURCE_H__
#define __PNG_RESOURCE_H__

#include "core.h"
#include "resources.h"

namespace sark{

	// PNG image format resource.
	// all about PNG handlings are under the 'libpng' library.
	// it supports the color type RGB and RGBA only.
	// (http://www.libpng.org/)
	class PNGResource : public ITextureResource, public IResourceLoader<PNGResource>{
	private:
		int32 mWidth;
		int32 mHeight;

		// PNG color type. (RGB or RGBA)
		Texture::Format mFormat;

		// pixel type
		Texture::PixelType mPixelType;

		// RGBA format
		uint8* mPixels;

	public:
		PNGResource();
		PNGResource(const PNGResource& png);

		~PNGResource();

		const integer GetWidth() const override;

		const integer GetHeight() const override;

		const integer GetDepth() const override;

		Texture::Format GetPixelFormat() const override;

		Texture::PixelType GetPixelType() const override;

		const void* GetPixels() const override;

		// load png resource.
		// implementation reference: http://zarb.org/~gc/html/libpng.html
		static PNGResource* LoadImp(const std::string& name);
	};
}
#endif
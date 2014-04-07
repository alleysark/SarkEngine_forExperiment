#ifndef __BMP_RESOURCE_H__
#define __BMP_RESOURCE_H__

#include "core.h"
#include "resources.h"

namespace sark{

#pragma pack(push, 1)
	struct BITMAPFILEHEADER {	// 14 = 2+4+2+2+4 (byte)
		uint16 bfID;			// "BM"
		uint32 bfSize;			// file size
		uint16 bfReserved1;		// reserved variable (0)
		uint16 bfReserved2;		// reserved variable (0)
		uint32 bfOffBits;		// offset of bitmap in this file
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct BITMAPINFOHEADER {	// 40 = 4+4+4+2+2+4+4+4+4+4+4 (byte)
		uint32 biSize;			// size of this structure
		int32 biWidth;			// width of bitmap image
		int32 biHeight;			// height of bitmap image
		uint16 biPlanes;		// count of plane (set as 1)
		uint16 biBitCount;		// bit per pixel
		uint32 biCompression;	// flag of compression type
		uint32 biSizeImage;		// sizeof image stream
		int32 biXPelsPerMeter;	// pixel per meter of width
		int32 biYPelsPerMeter;	// pixel per meter of height
		uint32 biClrUsed;		// used color count
		uint32 biClrImportant;	// important color of this file
	};
#pragma pack(pop)


	// BMP format resource
	// this BMP resource does not supply 256(8bit), 16bit color format
	class BMPResource : public ITextureResource, public IResourceLoader<BMPResource>{
	private:
		int32 mWidth;
		int32 mHeight;

		// RGB format
		uint8* mPixels;

	public:
		BMPResource();
		BMPResource(const BMPResource& bmp);
		~BMPResource();

		const integer GetWidth() const override;

		const integer GetHeight() const override;

		const integer GetDepth() const override;

		Format GetPixelFormat() const override;

		PixelType GetPixelType() const override;

		const void* GetPixels() const override;

		static BMPResource* LoadImp(const std::string& name);
	};
	
}
#endif
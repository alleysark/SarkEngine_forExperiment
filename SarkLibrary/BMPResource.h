#ifndef __BMP_RESOURCE_H__
#define __BMP_RESOURCE_H__

#include "core.h"
#include "resources.h"

namespace sark{

	struct BITMAPFILEHEADER {	// 14 = 2+4+2+2+4 (byte)
		uint16 bfType;			// "BM"
		uint32 bfSize;			// file size
		uint16 bfReserved1;		// reserved variable (0)
		uint16 bfReserved2;		// reserved variable (0)
		uint32 bfOffBits;		// offset of bitmap in this file
	};

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

	struct IBMPPIXELFORMAT{};

	struct BMPPIXELFORMAT24 : public IBMPPIXELFORMAT{ // B-G-R order
		uint8 b;
		uint8 g;
		uint8 r;
	};

	struct BMPPIXELFORMAT32 : public IBMPPIXELFORMAT{
		uint8 r;
		uint8 g;
		uint8 b;
		uint8 a;
	};

	// BMP format resource
	// this BMP resource does not supply 256(8bit), 16bit color format
	class BMPResource : public IResource{
	private:
		int32 mWidth;
		int32 mHeight;
		uint16 mBitCount;
		IBMPPIXELFORMAT* mpPixels;

	public:
		BMPResource(int32 width, int32 height, uint16 bitCount, IBMPPIXELFORMAT* pPixels);
		~BMPResource();
	};
	

	class BMPResourceLoader : public IResourceLoader<BMPResourceLoader>{
	public:
		static IResource* LoadImp(std::ifstream& stream);
	};
	
}
#endif
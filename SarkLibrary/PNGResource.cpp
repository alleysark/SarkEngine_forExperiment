#include "PNGResource.h"
#include "Debug.h"
#include <png.h>
#include <fcntl.h>

namespace sark{

	PNGResource::PNGResource()
		: mWidth(0), mHeight(0), mPixels(NULL)
	{}

	PNGResource::~PNGResource(){
		if (mPixels != NULL){
			delete[] mPixels;
		}
	}

	const integer PNGResource::GetWidth() const{
		return mWidth;
	}

	const integer PNGResource::GetHeight() const{
		return mHeight;
	}

	const integer PNGResource::GetDepth() const{
		return 0;
	}

	Texture::Format PNGResource::GetPixelFormat() const{
		return mFormat;
	}

	Texture::PixelType PNGResource::GetPixelType() const{
		return mPixelType;
	}

	const void* PNGResource::GetPixels() const{
		return mPixels;
	}

	s_ptr<PNGResource> PNGResource::LoadImp(const std::string& name){
		png_structp png_ptr = NULL;
		png_infop info_ptr = NULL;
		FILE *fp = NULL;

		// temporary clear macro
		#define CLEAR_MEM() do{\
				if (png_ptr != NULL && info_ptr != NULL){\
					png_destroy_read_struct(&png_ptr, &info_ptr, NULL); \
				}\
				if (fp != NULL){ fclose(fp); }\
			}while (0)

		// 8 is the maximum size that can be checked
		const uinteger HEADER_SIZE = 8;
		char header[HEADER_SIZE];    

		// open file and test for it being a png
		fopen_s(&fp, name.c_str(), "rb");
		if (!fp){
			LogError("failed to open png file");
			CLEAR_MEM();
			return NULL;
		}

		fread_s(header, HEADER_SIZE, 1, HEADER_SIZE, fp);
		if (png_sig_cmp((png_const_bytep)header, 0, HEADER_SIZE)){
			LogError("file is not recognized as a PNG format");
			CLEAR_MEM();
			return NULL;
		}

		// initialize stuff
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png_ptr){
			LogError("png_create_read_struct failed");
			CLEAR_MEM();
			return NULL;
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr){
			LogError("png_create_info_struct failed");
			CLEAR_MEM();
			return NULL;
		}

		if (setjmp(png_jmpbuf(png_ptr))){
			LogError("Error during init_io");
			CLEAR_MEM();
			return NULL;
		}

		png_init_io(png_ptr, fp);
		png_set_sig_bytes(png_ptr, HEADER_SIZE);
		png_read_info(png_ptr, info_ptr);

		png_uint_32 width, height;
		png_int_32 color_type;
		png_int_32 bit_depth; //bit per channel
		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);


		// pixel format and type check
		Texture::Format pngPixelFormat;
		Texture::PixelType pngPixelType;
		
		if (color_type == PNG_COLOR_TYPE_RGB)
			pngPixelFormat = Texture::Format::RGB;
		else if (color_type == PNG_COLOR_TYPE_RGBA)
			pngPixelFormat = Texture::Format::RGBA;
		else{
			LogError("it only supports PNG_COLOR_TYPE_RGB or PNG_COLOR_TYPE_RGBA");
			CLEAR_MEM();
			return NULL;
		}

		if (bit_depth == 8)
			pngPixelType = Texture::PixelType::UNSIGNED_BYTE;
		else if (bit_depth == 16)
			pngPixelType = Texture::PixelType::UNSIGNED_SHORT;
		else{
			LogError("invalid bit depth");
			CLEAR_MEM();
			return NULL;
		}

		// prepare to read image
		integer number_of_passes = png_set_interlace_handling(png_ptr);
		png_read_update_info(png_ptr, info_ptr);

		if (setjmp(png_jmpbuf(png_ptr))){
			LogError("Error during read_image");
			CLEAR_MEM();
			return NULL;
		}

		// read image
		s_ptr<PNGResource> png = s_ptr<PNGResource>(new PNGResource());
		png->mWidth = width;
		png->mHeight = height;
		png->mFormat = pngPixelFormat;
		png->mPixelType = pngPixelType;

		png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
		png->mPixels = new uint8[rowbytes * height];

		png_bytep* row_pointers = new png_bytep[height];
		for (integer i = 0; i < height; i++)
			row_pointers[i] = png->mPixels + i*rowbytes;

		png_read_image(png_ptr, row_pointers);
		png_read_end(png_ptr, NULL);
		
		CLEAR_MEM();
		delete[] row_pointers;

		return png;
		#undef CLEAR_MEM
	}

}
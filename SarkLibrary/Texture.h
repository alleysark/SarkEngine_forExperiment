#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GL/glew.h>
#include "core.h"

namespace sark{

	class ITextureResource;

	// texture object.
	// be careful not to confuse Texture with ITextureResource.
	class Texture{
	public:
		enum Dimension{
			TEX_1D = GL_TEXTURE_1D,
			TEX_2D = GL_TEXTURE_2D,
			TEX_3D = GL_TEXTURE_3D
		};

		struct _InternalFormatWrapper{
			enum InternalFormat{
				// most common internal formats
				ONE = 1, TWO = 2, THREE = 3, FOUR = 4,

				// other optional allowed internal formats
				ALPHA = GL_ALPHA, ALPHA4 = GL_ALPHA4, ALPHA8 = GL_ALPHA8,
				ALPHA12 = GL_ALPHA12, ALPHA16 = GL_ALPHA16,

				LUMINANCE = GL_LUMINANCE, LUMINANCE4 = GL_LUMINANCE4, LUMINANCE8 = GL_LUMINANCE8,
				LUMINANCE12 = GL_LUMINANCE12, LUMINANCE16 = GL_LUMINANCE16,
				SLUMINANCE = GL_SLUMINANCE, SLUMINANCE8 = GL_SLUMINANCE8,

				LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA, LUMINANCE4_ALPHA4 = GL_LUMINANCE4_ALPHA4,
				LUMINANCE6_ALPHA2 = GL_LUMINANCE6_ALPHA2, LUMINANCE8_ALPHA8 = GL_LUMINANCE8_ALPHA8,
				LUMINANCE12_ALPHA4 = GL_LUMINANCE12_ALPHA4, LUMINANCE12_ALPHA12 = GL_LUMINANCE12_ALPHA12,
				LUMINANCE16_ALPHA16 = GL_LUMINANCE16_ALPHA16, SLUMINANCE_ALPHA = GL_SLUMINANCE_ALPHA,
				SLUMINANCE8_ALPHA8 = GL_SLUMINANCE8_ALPHA8,

				INTENSITY = GL_INTENSITY, INTENSITY4 = GL_INTENSITY4, INTENSITY8 = GL_INTENSITY8,
				INTENSITY12 = GL_INTENSITY12, INTENSITY16 = GL_INTENSITY16,

				DEPTH_COMPONENT = GL_DEPTH_COMPONENT, DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
				DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24, DEPTH_COMPONENT32 = GL_DEPTH_COMPONENT32,

				RGB = GL_RGB,
				SRGB = GL_SRGB,

				R3_G3_B2 = GL_R3_G3_B2,
				RGB4 = GL_RGB4, RGB5 = GL_RGB5,
				RGB8 = GL_RGB8, RGB10 = GL_RGB10,
				RGB12 = GL_RGB12, RGB16 = GL_RGB16,
				SRGB8 = GL_SRGB8, SRGB_ALPHA = GL_SRGB_ALPHA,
				SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,

				RGBA = GL_RGBA,

				RGBA2 = GL_RGBA2, RGBA4 = GL_RGBA4, RGBA8 = GL_RGBA8,
				RGBA12 = GL_RGBA12, RGBA16 = GL_RGBA16,
				RGB10_A2 = GL_RGB10_A2, RGB5_A1 = GL_RGB5_A1,

				COMPRESSED_ALPHA = GL_COMPRESSED_ALPHA, COMPRESSED_LUMINANCE = GL_COMPRESSED_LUMINANCE,
				COMPRESSED_LUMINANCE_ALPHA = GL_COMPRESSED_LUMINANCE_ALPHA, COMPRESSED_INTENSITY = GL_COMPRESSED_INTENSITY,
				COMPRESSED_RGB = GL_COMPRESSED_RGB, COMPRESSED_RGBA = GL_COMPRESSED_RGBA,
			};
		};
		typedef enum _InternalFormatWrapper::InternalFormat InternalFormat;

		struct _FormatWrapper{
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
				LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,
				DEPTH_COMPONENT = GL_DEPTH_COMPONENT
			};
		};
		typedef enum _FormatWrapper::Format Format;

		struct _PixelTypeWrapper{
			enum PixelType{
				// most common pixel type
				UNSIGNED_BYTE = GL_UNSIGNED_BYTE,

				// other optional allowed pixel types
				BYTE = GL_BYTE,
				BITMAP = GL_BITMAP,
				UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
				SHORT = GL_SHORT,
				UNSIGNED_INT = GL_UNSIGNED_INT,
				INT = GL_INT,
				FLOAT = GL_FLOAT,

				UNSIGNED_BYTE_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
				UNSIGNED_BYTE_2_3_3_REV = GL_UNSIGNED_BYTE_2_3_3_REV,
				UNSIGNED_SHORT_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
				UNSIGNED_SHORT_5_6_5_REV = GL_UNSIGNED_SHORT_5_6_5_REV,
				UNSIGNED_SHORT_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
				UNSIGNED_SHORT_4_4_4_4_REV = GL_UNSIGNED_SHORT_4_4_4_4_REV,
				UNSIGNED_SHORT_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
				UNSIGNED_SHORT_1_5_5_5_REV = GL_UNSIGNED_SHORT_1_5_5_5_REV,
				UNSIGNED_INT_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
				UNSIGNED_INT_8_8_8_8_REV = GL_UNSIGNED_INT_8_8_8_8_REV,
				UNSIGNED_INT_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
				UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV
			};
		};
		typedef enum _PixelTypeWrapper::PixelType PixelType;

	private:
		// texture object id
		ObjectHandle mTexId;

		// dimension
		Dimension mDimension;

	private:
		Texture();

	public:
		// generate texture from fully given texture properties.
		// height for 2D, depth for 3D texture only.
		Texture(Dimension dimension, uint32 mipmapLevel, InternalFormat internalFormat,
			integer width, integer height, integer depth, bool border,
			Format pixelFormat, PixelType pixelType,
			const void* pixels, bool genMipMap = false);

		// generate texture from given texture resource with other properties.
		// please ensure that it should be called with proper dimensional texture resource.
		Texture(Dimension dimension, const ITextureResource* texResource,
			InternalFormat internalFormat = InternalFormat::FOUR,
			bool border = false, uint32 mipmapLevel = 0, bool genMipMap = false);

		// delete texture object
		~Texture();

		// get texture handle id
		const ObjectHandle GetHandleID() const;

		// bind texture
		void Bind() const;

		// unbind texture
		void Unbind() const;
	};

}
#endif
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GL/glew.h>
#include "core.h"
#include "resources.h"

namespace sark{

	// texture object.
	// be careful not to confuse Texture with ITextureResource.
	class Texture{
	public:
		enum Dimension{
			TEX_1D = GL_TEXTURE_1D,
			TEX_2D = GL_TEXTURE_2D,
			TEX_3D = GL_TEXTURE_3D
		};

		enum InternalFormat{
			// most common internal formats
			ONE = 1, TWO = 2, THREE = 3, FOUR = 4,

			// other optional allowed internal formats
			ALPHA = GL_ALPHA, ALPHA4 = GL_ALPHA4, ALPHA8 = GL_ALPHA8,
			ALPHA12 = GL_ALPHA12, ALPHA16 = GL_ALPHA16,

			LUMINANCE = GL_LUMINANCE, LUMINANCE4 = GL_LUMINANCE4, LUMINANCE8 = GL_LUMINANCE8,
			LUMINANCE12 = GL_LUMINANCE12, LUMINANCE16 = GL_LUMINANCE16,

			LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA, LUMINANCE4_ALPHA4 = GL_LUMINANCE4_ALPHA4,
			LUMINANCE6_ALPHA2 = GL_LUMINANCE6_ALPHA2, LUMINANCE8_ALPHA8 = GL_LUMINANCE8_ALPHA8,
			LUMINANCE12_ALPHA4 = GL_LUMINANCE12_ALPHA4, LUMINANCE12_ALPHA12 = GL_LUMINANCE12_ALPHA12,
			LUMINANCE16_ALPHA16 = GL_LUMINANCE16_ALPHA16,

			INTENSITY = GL_INTENSITY, INTENSITY4 = GL_INTENSITY4, INTENSITY8 = GL_INTENSITY8,
			INTENSITY12 = GL_INTENSITY12, INTENSITY16 = GL_INTENSITY16,

			RGB = GL_RGB,

			R3_G3_B2 = GL_R3_G3_B2,
			RGB4 = GL_RGB4, RGB5 = GL_RGB5,
			RGB8 = GL_RGB8, RGB10 = GL_RGB10,
			RGB12 = GL_RGB12, RGB16 = GL_RGB16,

			RGBA = GL_RGBA,

			RGBA2 = GL_RGBA2, RGBA4 = GL_RGBA4, RGBA8 = GL_RGBA8,
			RGBA12 = GL_RGBA12, RGBA16 = GL_RGBA16,
			RGB10_A2 = GL_RGB10_A2, RGB5_A1 = GL_RGB5_A1
		};

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
			ITextureResource::Format pixelFormat, ITextureResource::PixelType pixelType,
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
		void Bind();

		// unbind texture
		void Unbind();
	};

}
#endif
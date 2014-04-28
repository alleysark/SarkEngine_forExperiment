#include "Texture.h"
#include "resources.h"
#include "Debug.h"

namespace sark{

	// generate texture from fully given texture properties.
	// height for 2D, depth for 3D texture only.
	Texture::Texture(Dimension dimension, uint32 mipmapLevel, InternalFormat internalFormat,
		integer width, integer height, integer depth, bool border,
		Format pixelFormat, PixelType pixelType,
		const void* pixels, bool genMipMap)
	{
		mDimension = dimension;
		glGenTextures(1, &mTexId);

		glBindTexture(mDimension, mTexId);
		switch (mDimension){
		case TEX_1D:
			glTexImage1D(TEX_1D, mipmapLevel, internalFormat,
				width, (border ? 1 : 0),
				pixelFormat, pixelType, pixels);
			break;

		case TEX_2D:
			glTexImage2D(TEX_2D, mipmapLevel, internalFormat,
				width, height, (border ? 1 : 0),
				pixelFormat, pixelType, pixels);
			break;

		case TEX_3D:
			glTexImage3D(TEX_3D, mipmapLevel, internalFormat,
				width, height, depth, (border ? 1 : 0),
				pixelFormat, pixelType, pixels);
			break;
		}

		if (genMipMap){
			glGenerateMipmap(mDimension);
		}
		glBindTexture(mDimension, 0);
	}

	// generate texture from given parameter.
	// please ensure that it is called with proper dimensional texture resource.
	Texture::Texture(Dimension dimension, const ITextureResource* texResource,
		InternalFormat internalFormat, bool border, uint32 mipmapLevel, bool genMipMap)
		: Texture(dimension, mipmapLevel, internalFormat,
		texResource->GetWidth(), texResource->GetHeight(), texResource->GetDepth(),
		border, texResource->GetPixelFormat(), texResource->GetPixelType(), texResource->GetPixels(), genMipMap)
	{}

	// delete texture object
	Texture::~Texture(){
		if (mTexId != 0){
			glDeleteTextures(1, &mTexId);
			mTexId = 0;
		}
	}

	// get texture handle id
	const ObjectHandle Texture::GetHandleID() const{
		return mTexId;
	}

	// bind texture
	void Texture::Bind(){
		glBindTexture(mDimension, mTexId);
	}

	// unbind texture
	void Texture::Unbind(){
		glBindTexture(mDimension, 0);
	}

}
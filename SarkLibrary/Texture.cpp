#include "Texture.h"
#include "Debug.h"

namespace sark{

	// generate texture from given parameter.
	// please ensure that it is called with proper dimensional texture resource.
	Texture::Texture(Dimension dimension, const ITextureResource* texResource,
		InternalFormat internalFormat, bool border, uint32 mipmapLevel, bool genMipMap)
	{
		mDimension = dimension;
		glGenTextures(1, &mTexId);
		
		glBindTexture(mDimension, mTexId);
		switch (mDimension){
		case TEX_1D:
			glTexImage1D(TEX_1D, mipmapLevel, internalFormat,
				texResource->GetWidth(), (border ? 1 : 0),
				texResource->GetPixelFormat(), texResource->GetPixelType(),
				texResource->GetPixels());
			break;

		case TEX_2D:
			glTexImage2D(TEX_2D, mipmapLevel, internalFormat,
				texResource->GetWidth(), texResource->GetHeight(), (border ? 1 : 0),
				texResource->GetPixelFormat(), texResource->GetPixelType(),
				texResource->GetPixels());
			break;

		case TEX_3D:
			glTexImage3D(TEX_3D, mipmapLevel, internalFormat,
				texResource->GetWidth(), texResource->GetHeight(),
				texResource->GetDepth(), (border ? 1 : 0),
				texResource->GetPixelFormat(), texResource->GetPixelType(),
				texResource->GetPixels());
			break;
		}

		if (genMipMap){
			glGenerateMipmap(mDimension);
		}
		glBindTexture(mDimension, 0);
	}

	// delete texture object
	Texture::~Texture(){
		if (mTexId != 0){
			glDeleteTextures(1, &mTexId);
			mTexId = 0;
		}
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
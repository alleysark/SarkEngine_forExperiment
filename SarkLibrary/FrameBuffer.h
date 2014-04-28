#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

#include <vector>
#include "core.h"

namespace sark{

	class Texture;

	// frame buffer object.
	// it can define additional render target.
	// 'render to texture' or 'shadow map rendering' is common use.
	// MRT(multiple render target) is not supported yet.
	class FrameBuffer{
	public:
		enum AttachmentPoint{
			COLOR_ATTACHMENT0 = GL_COLOR_ATTACHMENT0, COLOR_ATTACHMENT1 = GL_COLOR_ATTACHMENT1,
			COLOR_ATTACHMENT2 = GL_COLOR_ATTACHMENT2, COLOR_ATTACHMENT3 = GL_COLOR_ATTACHMENT3,
			COLOR_ATTACHMENT4 = GL_COLOR_ATTACHMENT4, COLOR_ATTACHMENT5 = GL_COLOR_ATTACHMENT5,
			COLOR_ATTACHMENT6 = GL_COLOR_ATTACHMENT6, COLOR_ATTACHMENT7 = GL_COLOR_ATTACHMENT7,
			COLOR_ATTACHMENT8 = GL_COLOR_ATTACHMENT8, COLOR_ATTACHMENT9 = GL_COLOR_ATTACHMENT9,
			COLOR_ATTACHMENT10 = GL_COLOR_ATTACHMENT10, COLOR_ATTACHMENT11 = GL_COLOR_ATTACHMENT11,
			COLOR_ATTACHMENT12 = GL_COLOR_ATTACHMENT12, COLOR_ATTACHMENT13 = GL_COLOR_ATTACHMENT13,
			COLOR_ATTACHMENT14 = GL_COLOR_ATTACHMENT14, COLOR_ATTACHMENT15 = GL_COLOR_ATTACHMENT15,
			DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT, STENCIL_ATTACHEMENT = GL_STENCIL_ATTACHMENT,
			DEPTH_STENCIL_ATTACHMENT = GL_DEPTH_STENCIL_ATTACHMENT
		};

		enum RenderBufferFormat{
			RGBA4 = GL_RGBA4,
			RGB565 = GL_RGB565,
			RGB5_A1 = GL_RGB5_A1,
			DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
			STENCIL_INDEX8 = GL_STENCIL_INDEX8
		};

	private:
		// frame buffer handler.
		ObjectHandle mFbId;

		// texture images;
		std::vector<Texture*> mTextures;

		// render buffer handlers.
		std::vector<ObjectHandle> mRenderBufIds;

	public:
		// generate frame buffer object.
		FrameBuffer();

		~FrameBuffer();

		// attach texture image into current frame buffer.
		// *note: current frame buffer must have to be binded.
		// *note: given texture instance will be destroyed when this is deleted.
		// CAUTION: FrameBuffer does not support multi render target yet!
		uinteger AttachTexture(Texture* texture, AttachmentPoint attachmentPoint, integer mipMapLevel = 0);

		// generate and attach render buffer image.
		// *note: current frame buffer must have to be binded.
		// CAUTION: FrameBuffer does not support multi render target yet!
		void AttachRenderbuffer(RenderBufferFormat internalFormat,
			uinteger width, uinteger height, AttachmentPoint attachmentPoint);

		// check if the status is completed or not.
		bool CheckStatus();

		// get texture at index of 'idx'
		const Texture* GetTextureAt(uinteger idx);

		// bind this frame buffer object
		void Bind() const;

		// unbind frame buffer object
		void Unbind() const;
	};

}
#endif
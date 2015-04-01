#include "FrameBuffer.h"
#include "Texture.h"

namespace sark {

	// generate frame buffer object.
	FrameBuffer::FrameBuffer() {
		glGenFramebuffers(1, &mFbId);
	}

	FrameBuffer::~FrameBuffer() {
		if (mFbId != 0)
			glDeleteBuffers(1, &mFbId);

		for (std::vector<ObjectHandle>::iterator itr = mRenderBufIds.begin();
			itr != mRenderBufIds.end(); itr++) {
			glDeleteRenderbuffers(1, &(*itr));
		}

		mTextures.clear();
	}

	// attach texture image into current frame buffer.
	// *note: current frame buffer must have to be binded.
	// *note: given texture instance will be destroyed when this is deleted.
	// CAUTION: FrameBuffer does not support multi render target yet!
	uinteger FrameBuffer::AttachTexture(Texture* texture, AttachmentPoint attachmentPoint, integer mipMapLevel) {
		glFramebufferTexture(GL_FRAMEBUFFER, attachmentPoint, texture->GetHandleID(), mipMapLevel);
		mTextures.push_back(texture);
		return mTextures.size();
	}

	// generate and attach render buffer image.
	// *note: current frame buffer must have to be binded.
	// CAUTION: FrameBuffer does not support multi render target yet!
	void FrameBuffer::AttachRenderbuffer(RenderBufferFormat internalFormat, uinteger width, uinteger height, AttachmentPoint attachmentPoint) {
		ObjectHandle id;
		glGenRenderbuffers(1, &id);
		glBindRenderbuffer(GL_RENDERBUFFER, id);
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentPoint, GL_RENDERBUFFER, id);
		mRenderBufIds.push_back(id);
	}

	// check if the status is completed or not.
	bool FrameBuffer::CheckStatus() {
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;
		return true;
	}

	// get texture at index of 'idx'
	Texture* FrameBuffer::GetTextureAt(uinteger idx) {
		if (idx >= mTextures.size())
			return NULL;
		return mTextures[idx];
	}

	// bind this frame buffer object
	void FrameBuffer::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, mFbId);
	}

	// unbind frame buffer object
	void FrameBuffer::Unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
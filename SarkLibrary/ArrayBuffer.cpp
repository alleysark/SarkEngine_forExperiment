#include "ArrayBuffer.h"

namespace sark{

	ArrayBuffer::AttributeFeature::AttributeFeature()
		: attribTarget(AttributeSemantic::POSITION), elementCount(0),
		elementType(ElementType::NONE), bufId(0), 
		bufSize(0), bufHint(BufferHint::STATIC)
	{}



	ArrayBuffer::ArrayBuffer(const ArrayBuffer&){}
	ArrayBuffer& ArrayBuffer::operator=(const ArrayBuffer&){ return *this; }

	ArrayBuffer::ArrayBuffer()
		: mDrawMode(DrawMode::NONE)
	{
		memset(mAttribFeats, NULL, sizeof(mAttribFeats));
	}

	// delete all generated buffers.
	ArrayBuffer::~ArrayBuffer(){
		// delete all attribute buffers.
		for (integer i = 0; i < AttributeSemantic::COUNT; i++){
			if (mAttribFeats[i] != NULL){
				if (mAttribFeats[i]->bufId != 0){
					glDeleteBuffers(1, &mAttribFeats[i]->bufId);
				}
			}
			delete mAttribFeats[i];
		}
	}

	// get draw mode.
	const ArrayBuffer::DrawMode& ArrayBuffer::GetDrawMode() const{
		return mDrawMode;
	}

	// set draw mode.
	void ArrayBuffer::SetDrawMode(DrawMode drawMode){
		mDrawMode = drawMode;
	}

	// get data count of specific attribute.
	// it'll return 0 if given attribute buffer is not exists.
	const uinteger ArrayBuffer::GetDataCount(AttributeSemantic attribSemantic) const{
		if (mAttribFeats[attribSemantic] == NULL)
			return 0;
		return mAttribFeats[attribSemantic]->dataCount;
	}

	
	// bind this vertex buffer object.
	void ArrayBuffer::BindAttribBuffers() const{
		for (integer i = 0; i < AttributeSemantic::COUNT; i++){
			if (mAttribFeats[i] == NULL)
				continue;
			
			glBindBuffer(mAttribFeats[i]->bufTarget, mAttribFeats[i]->bufId);
			glEnableVertexAttribArray(mAttribFeats[i]->attribTarget);
			glVertexAttribPointer(
				mAttribFeats[i]->attribTarget, 
				mAttribFeats[i]->elementCount,
				mAttribFeats[i]->elementType, 
				0, 0, NULL);
		}
	}

	// unbind currently bound vertex buffer object 
	// and disable the enabled vertex attribute arrays.
	void ArrayBuffer::UnbindAttribBuffers() const{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		for (integer i = 0; i < AttributeSemantic::COUNT; i++){
			if (mAttribFeats[i] == NULL)
				continue;

			glDisableVertexAttribArray(mAttribFeats[i]->attribTarget);
		}
	}

	// bind specific attribute buffer object.
	bool ArrayBuffer::BindAttribBuffer(AttributeSemantic attribSemantic) const{
		if (mAttribFeats[attribSemantic] == NULL)
			return false;

		glBindBuffer(mAttribFeats[attribSemantic]->bufTarget, mAttribFeats[attribSemantic]->bufId);
		glEnableVertexAttribArray(mAttribFeats[attribSemantic]->attribTarget);
		glVertexAttribPointer(
			mAttribFeats[attribSemantic]->attribTarget,
			mAttribFeats[attribSemantic]->elementCount,
			mAttribFeats[attribSemantic]->elementType, 
			0, 0, NULL);
		return true;
	}

	// unbind currently bound vertex buffer object 
	// and disable specific vertex attribute array.
	bool ArrayBuffer::UnbindAttribBuffer(AttributeSemantic attribSemantic) const{
		if (mAttribFeats[attribSemantic] == NULL)
			return false;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(mAttribFeats[attribSemantic]->attribTarget);
		return true;
	}

	

	// draw vertex array directly.
	// you should have to bind this buffer.
	void ArrayBuffer::DrawArrays() const{
		ONLYDBG_CODEBLOCK(
		if (mAttribFeats[AttributeSemantic::POSITION] == NULL){
			LogWarn("positions are the essential attribute to draw");
			return;
		}
		if (mDrawMode == DrawMode::NONE){
			LogWarn("you should have set the draw mode to draw");
			return;
		});

		glDrawArrays(mDrawMode, 0, mAttribFeats[AttributeSemantic::POSITION]->dataCount);
	}

	// draw elements. you should have to bind relative
	// vertex buffer and this index buffer.
	void ArrayBuffer::DrawPrimitives() const{
		ONLYDBG_CODEBLOCK(
		if (mAttribFeats[AttributeSemantic::POSITION] == NULL){
			LogWarn("positions are the essential attribute to draw");
			return;
		}
		if (mAttribFeats[AttributeSemantic::INDICES] == NULL){
			LogWarn("indices are required to draw arrays as 'element'");
		}
		if (mDrawMode == DrawMode::NONE){
			LogWarn("you should have set the draw mode to draw");
			return;
		});

		glDrawElements(mDrawMode,
			mAttribFeats[AttributeSemantic::INDICES]->dataCount * mAttribFeats[AttributeSemantic::INDICES]->elementCount,
			mAttribFeats[AttributeSemantic::INDICES]->elementType, NULL);
	}
}
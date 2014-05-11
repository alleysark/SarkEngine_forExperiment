#include "ArrayBuffer.h"

namespace sark{

	ArrayBuffer::AttributeFeature::AttributeFeature()
		: attribTarget(ShaderProgram::ATTR_POSITION), elementCount(0),
		elementType(ElementType::NONE), bufId(0), 
		bufSize(0), bufHint(BufferHint::STATIC)
	{}



	ArrayBuffer::ArrayBuffer(const ArrayBuffer&){}
	ArrayBuffer& ArrayBuffer::operator=(const ArrayBuffer&){ return *this; }

	ArrayBuffer::ArrayBuffer()
		: mDrawMode(DrawMode::NONE), mDataCount(0), mPrimitiveCount(0)
	{}

	// delete all generated buffers.
	ArrayBuffer::~ArrayBuffer(){
		// delete all attribute buffers.
		AttribFeatureList::iterator itr = mAttribFeats.begin();
		AttribFeatureList::iterator end = mAttribFeats.end();
		for (; itr != end; itr++){
			if (itr->bufId != 0){
				glDeleteBuffers(1, &itr->bufId);
			}
		}

		// delete primitive buffer.
		if (mPrimitiveFeat.bufId != 0)
			glDeleteBuffers(1, &mPrimitiveFeat.bufId);
	}

	// get draw mode.
	const ArrayBuffer::DrawMode& ArrayBuffer::GetDrawMode() const{
		return mDrawMode;
	}

	// set draw mode.
	void ArrayBuffer::SetDrawMode(DrawMode drawMode){
		mDrawMode = drawMode;
	}

	// get data count.
	// as for vertex buffer, it assumes that the data counts
	// of each attribute buffers are same.
	const uinteger& ArrayBuffer::GetDataCount() const{
		return mDataCount;
	}

	// get primitive data count.
	const uinteger& ArrayBuffer::GetPrimitiveCount() const{
		return mPrimitiveCount;
	}

	// whether it has primitive buffer or not.
	bool ArrayBuffer::HasPrimitiveBuffer() const{
		return (mPrimitiveFeat.bufId != 0);
	}


	// ----- vertex attribute array buffer methods -----

	
	// bind this vertex buffer object.
	bool ArrayBuffer::BindAttribBuffers() const{
		if (mAttribFeats.empty())
			return false;

		AttribFeatureList::const_iterator itr = mAttribFeats.begin();
		AttribFeatureList::const_iterator end = mAttribFeats.end();
		for (; itr != end; itr++){
			glBindBuffer(GL_ARRAY_BUFFER, itr->bufId);
			glEnableVertexAttribArray(itr->attribTarget);
			glVertexAttribPointer(itr->attribTarget, itr->elementCount,
				itr->elementType, 0, 0, NULL);
		}

		return true;
	}

	// bind specific attribute buffer object.
	bool ArrayBuffer::BindAttribBuffer(ShaderProgram::AttributeSemantic attribSemantic) const{
		AttribFeatureList::const_iterator itr = mAttribFeats.begin();
		AttribFeatureList::const_iterator end = mAttribFeats.end();
		for (; itr != end; itr++){
			if (itr->attribTarget == attribSemantic){
				glBindBuffer(GL_ARRAY_BUFFER, itr->bufId);
				glEnableVertexAttribArray(itr->attribTarget);
				glVertexAttribPointer(itr->attribTarget, itr->elementCount,
					itr->elementType, 0, 0, NULL);
				return true;
			}
		}
		return false;
	}

	// unbind currently bound vertex buffer object.
	void ArrayBuffer::UnbindAttribBuffers() const{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		AttribFeatureList::const_iterator itr = mAttribFeats.begin();
		AttribFeatureList::const_iterator end = mAttribFeats.end();
		for (; itr != end; itr++){
			glDisableVertexAttribArray(itr->attribTarget);
		}
	}

	// draw vertex array directly.
	// you should have to bind this buffer.
	void ArrayBuffer::DrawArrays() const{
		if (mDrawMode != DrawMode::NONE)
			glDrawArrays(mDrawMode, 0, mDataCount);
	}


	// ----- primitive array buffer methods -----

	// bind primitive buffer object.
	bool ArrayBuffer::BindPrimitiveBuffer() const{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPrimitiveFeat.bufId);
		glEnableVertexAttribArray(mPrimitiveFeat.attribTarget);
		glVertexAttribPointer(mPrimitiveFeat.attribTarget,
			mPrimitiveFeat.elementCount, mPrimitiveFeat.elementType, 0, 0, NULL);

		return true;
	}

	// unbind currently bound primitive buffer object.
	void ArrayBuffer::UnbindPrimitiveBuffer() const{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(mPrimitiveFeat.attribTarget);
	}

	// draw elements. you should have to bind relative
	// vertex buffer and this index buffer.
	void ArrayBuffer::DrawPrimitives() const{
		glDrawElements(mDrawMode,
			mPrimitiveCount * mPrimitiveFeat.elementCount,
			mPrimitiveFeat.elementType, NULL);
	}
}
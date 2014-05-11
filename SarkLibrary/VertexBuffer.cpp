#include "VertexBuffer.h"

namespace sark{

	VertexBuffer::VertexBuffer(){}

	// delete whole buffer objects.
	VertexBuffer::~VertexBuffer(){
		AttribFeatureList::iterator itr = mAttribFeats.begin();
		AttribFeatureList::iterator end = mAttribFeats.end();
		for (; itr != end; itr++){
			if (itr->bufId != 0){
				glDeleteBuffers(1, &itr->bufId);
			}
		}
	}

	void VertexBuffer::SetDrawMode(DrawMode drawMode){
		mDrawMode = drawMode;
	}

	// bind this vertex buffer object.
	bool VertexBuffer::Bind() const{
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
	bool VertexBuffer::Bind(ShaderProgram::AttributeSemantic attribSemantic) const{
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
	void VertexBuffer::Unbind() const{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		AttribFeatureList::const_iterator itr = mAttribFeats.begin();
		AttribFeatureList::const_iterator end = mAttribFeats.end();
		for (; itr != end; itr++){
			glDisableVertexAttribArray(itr->attribTarget);
		}
	}

	// draw vertex array directly.
	// you should have to bind this buffer.
	void VertexBuffer::DrawArrays() const{
		if (mDrawMode != DrawMode::NONE)
			glDrawArrays(mDrawMode, 0, mDataCount);
	}

}
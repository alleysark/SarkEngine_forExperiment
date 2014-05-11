#include "IndexBuffer.h"

namespace sark{

	IndexBuffer::IndexBuffer(){}

	// delete whole buffer objects.
	IndexBuffer::~IndexBuffer(){
		if (mIndexFeat.bufId != 0)
			glDeleteBuffers(1, &mIndexFeat.bufId);
	}

	// bind this index buffer object.
	bool IndexBuffer::Bind() const{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexFeat.bufId);
		glEnableVertexAttribArray(mIndexFeat.attribTarget);
		glVertexAttribPointer(mIndexFeat.attribTarget,
			mIndexFeat.elementCount, mIndexFeat.elementType, 0, 0, NULL);

		return true;
	}

	// unbind currently bound index buffer object.
	void IndexBuffer::Unbind() const{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(mIndexFeat.attribTarget);
	}

	// draw elements. you should have to bind relative
	// vertex buffer and this index buffer.
	void IndexBuffer::DrawElements() const{
		glDrawElements(mDrawMode, mDataCount, mIndexFeat.elementType, NULL);
	}
}
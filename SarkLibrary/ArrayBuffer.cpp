#include "ArrayBuffer.h"

namespace sark{

	ArrayBuffer::AttributeFeature::AttributeFeature()
		: attribTarget(ShaderProgram::ATTR_POSITION), elementCount(0),
		elementType(ElementType::NONE), bufId(0), bufSize(0), bufHint(BufferHint::STATIC)
	{}



	ArrayBuffer::ArrayBuffer(const ArrayBuffer&){}
	ArrayBuffer& ArrayBuffer::operator=(const ArrayBuffer&){ return *this; }

	ArrayBuffer::ArrayBuffer()
		: mDrawMode(DrawMode::NONE), mDataCount(0)
	{}

	ArrayBuffer::~ArrayBuffer(){}

	// get draw mode. it can be NONE for vertex array.
	const ArrayBuffer::DrawMode& ArrayBuffer::GetDrawMode() const{
		return mDrawMode;
	}

	// get data count.
	// as for vertex buffer, it assumes that the data counts
	// of each attribute buffers are same.
	const uinteger& ArrayBuffer::GetDataCount() const{
		return mDataCount;
	}

}
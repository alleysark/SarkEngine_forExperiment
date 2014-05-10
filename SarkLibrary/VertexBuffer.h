#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include <vector>
#include <list>
#include "ArrayBuffer.h"

namespace sark{

	// vertex array buffer for mesh vertices.
	// it defines vertex buffers of user-defined attribute type
	// and it offers you the methods of accessing mapped buffer
	// as attribute accessor.
	class VertexBuffer : public ArrayBuffer{
	public:
		// type of attribute feature list.
		typedef std::list<AttributeFeature> AttribFeatureList;

	private:
		// attribute feature list
		AttribFeatureList mAttribFeats;

	public:
		VertexBuffer();

		// delete whole buffer objects.
		~VertexBuffer();

		void SetDrawMode(DrawMode drawMode);

		// generate attribute buffer with relative informations.
		//
		// *details:
		// _AttribType - type of attribute, e.g. Position3.
		// _ElemType - type of element of attribute. e.g. ElementType::REAL.
		// attribSemantic - the semantic value of attribute, e.g. ShaderProgram::ATTR_POSITION.
		// data - array data of attributes.
		// storageHint - buffer object storage hint, e.g. BufferHint::STATIC.
		template<class _AttribType, ElementType _ElemType = ElementType::NONE>
		bool GenAttributeBuffer(ShaderProgram::AttributeSemantic attribSemantic,
			const std::vector<_AttribType>& data, BufferHint storageHint = BufferHint::STATIC);

		// get mapped vertices attribute accessor.
		//
		// *note: you don't need to bind buffer before calling this.
		// it bind buffer automatically in here.
		// *note: attribute accessor will unmap the mapped buffer pointer
		// automatically when it is destructed. so please do not store
		// returned accessor at your scope(e.g. member variable).
		// *note: if your buffer storage mode was STATIC then the access
		// mode only going to be the READ_ONLY even though you were passing
		// the other access hint.
		template<class _AttribType>
		AttributeAccessor<_AttribType> GetAttributeAccessor(
			ShaderProgram::AttributeSemantic attribSemantic, AccessHint hint = AccessHint::READ_ONLY);

		// bind this vertex buffer object.
		bool Bind() const override;

		// bind specific attribute buffer object.
		bool Bind(ShaderProgram::AttributeSemantic attribSemantic) const;

		// unbind currently bound vertex buffer object.
		void Unbind() const override;

		// draw vertex array directly.
		// you should have to bind this buffer.
		void DrawArrays() const;
	};


	//----- template implementation -----//

	template<class _AttribType, ArrayBuffer::ElementType _ElemType>
	bool VertexBuffer::GenAttributeBuffer(
		ShaderProgram::AttributeSemantic attribSemantic,
		const std::vector<_AttribType>& data, BufferHint storageHint)
	{
		// check uniqueness of attribute type.
		AttribFeatureList::iterator itr = mAttribFeats.begin();
		AttribFeatureList::iterator end = mAttribFeats.end();
		for (; itr != end; itr++){
			if (itr->attribTarget == attribSemantic){
				LogWarn("there is the buffer generation request of existent attribute type. "
					"buffer object will be generated newly");

				// delete old version of buffer
				if (itr->bufId != 0)
					glDeleteBuffers(1, &itr->bufId);

				// erase feature from list
				mAttribFeats.erase(itr);
				break;
			}
		}

		// check data count appropriacy.
		if (mDataCount == 0)
			mDataCount = data.size();
		else{
			if (mDataCount != data.size()){
				LogWarn("the number of requested attribute data is not equal to previous things");
			}
		}

		AttributeFeature feat;

		// check and fill element type and element count.
		if (_ElemType == ElementType::NONE){
			feat.elementType = ElementType::REAL;

			const std::type_info& attr_type = typeid(_AttribType);
			if (attr_type == typeid(real))
				feat.elementCount = 1;
			else if (attr_type == typeid(Vector2))
				feat.elementCount = 2;
			else if (attr_type == typeid(Vector3))
				feat.elementCount = 3;
			else if (attr_type == typeid(Vector4))
				feat.elementCount = 4;
			else{
				LogWarn("auto-detection of element properties only supports for "
					"_AttribType of [real | Vector2 | Vector3 | Vector4]");
				return false;
			}
		}
		else{
			feat.elementType = _ElemType;

			switch (_ElemType){
			case ElementType::REAL:
				feat.elementCount = sizeof(_AttribType) / sizeof(real);
				break;
			case ElementType::UNSIGNED_BYTE:
				feat.elementCount = sizeof(_AttribType);
				break;
			case ElementType::UNSIGNED_SHORT:
				feat.elementCount = sizeof(_AttribType) / 2;
				break;
			case ElementType::UNSIGNED_INT:
				feat.elementCount = sizeof(_AttribType) / 4;
				break;
			}
		}

		// set other attribute features
		feat.attribTarget = attribSemantic;
		feat.bufSize = sizeof(_AttribType)*data.size();
		feat.bufHint = storageHint;

		// generate buffer
		glGenBuffers(1, &feat.bufId);
		if (feat.bufId == 0){
			LogError("failed to generate buffer");
			return false;
		}

		// set buffer data
		glBindBuffer(GL_ARRAY_BUFFER, feat.bufId);
		glBufferData(GL_ARRAY_BUFFER, feat.bufSize, &data[0], feat.bufHint);
		glEnableVertexAttribArray(feat.attribTarget);
		glVertexAttribPointer(feat.attribTarget, feat.elementCount, feat.elementType, 0, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		mAttribFeats.push_back(feat);
		return true;
	}

	template<class _AttribType>
	ArrayBuffer::AttributeAccessor<_AttribType> VertexBuffer::GetAttributeAccessor(
		ShaderProgram::AttributeSemantic attribSemantic, AccessHint hint)
	{
		// find required attribute in feature list and check.
		AttribFeatureList::iterator itr = mAttribFeats.begin();
		AttribFeatureList::iterator end = mAttribFeats.end();
		for (; itr != end; itr++){
			if (itr->attribTarget == attribSemantic)
				break;
		}
		if (itr == end){
			LogWarn("there is no matched attribute in attribute feature list");
			return AttributeAccessor<_AttribType>();
		}

		const AttributeFeature& feat = *itr;
		void* ptr = NULL;

		// bind buffer before mapping
		glBindBuffer(GL_ARRAY_BUFFER, feat.bufId);

		// access hint validation check and map buffer
		if (feat.bufHint == BufferHint::STATIC){
			ONLYDBG_CODEBLOCK(
			if (hint != AccessHint::READ_ONLY){
				LogWarn("STATIC buffer only can be accessed as READ_ONLY");
			});
			ptr = glMapBuffer(GL_ARRAY_BUFFER, AccessHint::READ_ONLY);
		}
		else{
			ptr = glMapBuffer(GL_ARRAY_BUFFER, hint);
		}

		// mapping correction check
		if (ptr == NULL){
			GLenum errcode = glGetError();
			std::string err_str = "failed to map buffer\n";
			err_str += reinterpret_cast<const char*>(gluErrorString(errcode));
			LogError(err_str);
			return AttributeAccessor<_AttribType>();
		}

		// return valid attribute accessor
		return AttributeAccessor<_AttribType>(ptr, &feat);
	}

}
#endif
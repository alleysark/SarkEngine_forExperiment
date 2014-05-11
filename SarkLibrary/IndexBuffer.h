#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

#include <vector>
#include "ArrayBuffer.h"

namespace sark{

	// index element array buffer for mesh indices.
	// it defines element buffer of user-defined primitive types
	// and it offers you the methods of accessing mapped buffer
	// as attribute accessor.
	class IndexBuffer : public ArrayBuffer{
	public:
		// provided well-known primitive type.
		// *note: you can define your own structures to represent
		// other types of draw mode. but be warnned, it's better
		// to pack the structure to prevent 4-byte alignment problem.

		// triangle primitive.
#pragma pack(push, 1)
		template<typename _T>
		class TrianglePrimitive{
		public:
			_T a, b, c;

			TrianglePrimitive()
				: a(0), b(0), c(0)
			{}
			TrianglePrimitive(_T _a, _T _b, _T _c)
				: a(_a), b(_b), c(_c)
			{}
		};
#pragma pack(pop)

		typedef TrianglePrimitive<uint16> TriangleFace16;
		typedef TrianglePrimitive<uint32> TriangleFace32;

		// quad primitive.
#pragma pack(push, 1)
		template<typename _T>
		class QuadPrimitive{
		public:
			_T a, b, c, d;

			QuadPrimitive()
				: a(0), b(0), c(0), d(0)
			{}
			QuadPrimitive(_T _a, _T _b, _T _c, _T _d)
				: a(_a), b(_b), c(_c), d(_d)
			{}
		};
#pragma pack(pop)

		typedef QuadPrimitive<uint16> QuadFace16;
		typedef QuadPrimitive<uint32> QuadFace32;

	private:
		// index attribute(?) feature.
		AttributeFeature mIndexFeat;

	public:
		IndexBuffer();

		// delete whole buffer objects.
		~IndexBuffer();

		// generate index array buffer with relative informations.
		//
		// *details:
		// _PrimitiveType - type of primitive, e.g. TriangleFace16.
		// _ElemType - type of element of attribute. e.g. ElementType::REAL.
		// drawMode - how to draw the given primitive data, e.g. DrawMode::Triangles.
		// data - array data of primitives.
		// storageHint - buffer object storage hint, e.g. BufferHint::STATIC.
		template<class _PrimitiveType, ElementType _ElemType = ElementType::NONE>
		bool GenPrimitiveBuffer(DrawMode drawMode, const std::vector<_PrimitiveType>& data,
			BufferHint storageHint = BufferHint::STATIC);

		// get mapped indices attribute accessor.
		//
		// *note: you don't need to bind buffer before calling this.
		// it bind buffer automatically in here.
		// *note: attribute accessor will unmap the mapped buffer pointer
		// automatically when it is destructed. so please do not store
		// returned accessor at your scope(e.g. member variable).
		// *note: if your buffer storage mode was STATIC then the access
		// mode only going to be the READ_ONLY even though you were passing
		// the other access hint.
		template<class _PrimitiveType>
		AttributeAccessor<_PrimitiveType> GetPrimitiveAccessor(AccessHint hint = AccessHint::READ_ONLY);

		// bind this index buffer object.
		// after using, it have to be unbinded.
		bool Bind() const override;

		// unbind currently bound index buffer object.
		void Unbind() const override;

		// draw elements. you should have to bind relative
		// vertex buffer and this index buffer.
		void DrawElements() const;
	};


	//----- template implementation -----//

	template<class _PrimitiveType, ArrayBuffer::ElementType _ElemType>
	bool IndexBuffer::GenPrimitiveBuffer(DrawMode drawMode, 
		const std::vector<_PrimitiveType>& data, BufferHint storageHint)
	{
		if (mIndexFeat.bufId != 0)
			return false;

		// check and fill element type and element count.
		if (_ElemType == ElementType::NONE){
			const std::type_info& prim_type = typeid(_PrimitiveType);
			if (prim_type == typeid(TriangleFace16)){
				mIndexFeat.elementType = ElementType::UNSIGNED_SHORT;
				mIndexFeat.elementCount = 3;
			}
			else if (prim_type == typeid(TriangleFace32)){
				mIndexFeat.elementType = ElementType::UNSIGNED_INT;
				mIndexFeat.elementCount = 3;
			}
			else if (prim_type == typeid(QuadFace16)){
				mIndexFeat.elementType = ElementType::UNSIGNED_SHORT;
				mIndexFeat.elementCount = 4;
			}
			else if (prim_type == typeid(QuadFace32)){
				mIndexFeat.elementType = ElementType::UNSIGNED_INT;
				mIndexFeat.elementCount = 4;
			}
			else{
				LogWarn("auto-detection of element properties only supports for "
					"_PrimitiveType of [TriangleFace16 | TriangleFace32 | QuadFace16 | QuadFace32]");
				return false;
			}
		}
		else{
			mIndexFeat.elementType = _ElemType;

			switch (_ElemType){
			case ElementType::REAL:
				mIndexFeat.elementCount = sizeof(_PrimitiveType) / sizeof(real);
				break;
			case ElementType::UNSIGNED_BYTE:
				mIndexFeat.elementCount = sizeof(_PrimitiveType);
				break;
			case ElementType::UNSIGNED_SHORT:
				mIndexFeat.elementCount = sizeof(_PrimitiveType) / 2;
				break;
			case ElementType::UNSIGNED_INT:
				mIndexFeat.elementCount = sizeof(_PrimitiveType) / 4;
				break;
			}
		}

		// set other attribute features
		mIndexFeat.attribTarget = ShaderProgram::ATTR_INDICES;
		mIndexFeat.bufSize = sizeof(_PrimitiveType)*data.size();
		mIndexFeat.bufHint = storageHint;

		mDrawMode = drawMode;
		mDataCount = data.size() * mIndexFeat.elementCount;

		// generate buffer
		glGenBuffers(1, &mIndexFeat.bufId);
		if (mIndexFeat.bufId == 0){
			LogError("failed to generate buffer");
			return false;
		}

		// set buffer data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexFeat.bufId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexFeat.bufSize, &data[0], mIndexFeat.bufHint);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return true;
	}

	template<class _PrimitiveType>
	ArrayBuffer::AttributeAccessor<_PrimitiveType> 
		IndexBuffer::GetPrimitiveAccessor(AccessHint hint)
	{
		void* ptr = NULL;

		// bind buffer before mapping
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexFeat.bufId);

		// access hint validation check and map buffer
		if (mIndexFeat.bufHint == BufferHint::STATIC){
			ONLYDBG_CODEBLOCK(
			if (hint != AccessHint::READ_ONLY){
				LogWarn("STATIC buffer only can be accessed as READ_ONLY");
			});
			ptr = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, AccessHint::READ_ONLY);
		}
		else{
			ptr = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, hint);
		}

		// mapping correction check
		if (ptr == NULL){
			GLenum errcode = glGetError();
			std::string err_str = "failed to map buffer\n";
			err_str += reinterpret_cast<const char*>(gluErrorString(errcode));
			LogError(err_str);
			return AttributeAccessor<_PrimitiveType>();
		}

		// return valid primitive accessor
		return AttributeAccessor<_PrimitiveType>(ptr, &mIndexFeat);
	}

}
#endif
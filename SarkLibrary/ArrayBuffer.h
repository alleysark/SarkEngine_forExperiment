#ifndef __ARRAY_BUFFER_H__
#define __ARRAY_BUFFER_H__

#include <vector>
#include <list>
#include "core.h"
#include "ShaderProgram.h"
#include "primitives.hpp"
#include "Debug.h"

namespace sark {

	// array buffer object for mesh data.
	// it offers you the function to generate vertex attribute
	// array buffer with user-defined attribute type.
	// it also offers you the accessor of mapped attribute array
	// buffer which is the random-accessible memory.
	class ArrayBuffer {
	public:
		// buffer storage hint
		struct _BufferHint_wrapper {
			enum BufferHint {
				STATIC = GL_STATIC_DRAW,
				DYNAMIC = GL_DYNAMIC_DRAW,
				STREAM = GL_STREAM_DRAW
			};
		};
		typedef _BufferHint_wrapper::BufferHint BufferHint;

		// buffer accessing hint
		struct _AccessHint_wrapper {
			enum AccessHint {
				READ_ONLY = GL_READ_ONLY,
				WRITE_ONLY = GL_WRITE_ONLY,
				READ_WRITE = GL_READ_WRITE
			};
		};
		typedef _AccessHint_wrapper::AccessHint AccessHint;

		// attribute element type
		struct _ElementType_wrapper {
			enum ElementType {
				// type real. usual type
			#ifdef SARKLIB_USING_DOUBLE
				REAL = GL_DOUBLE,
			#else
				REAL = GL_FLOAT,
			#endif

				BYTE = GL_BYTE,
				SHORT = GL_SHORT,
				INT = GL_INT,

				// types for normalized element
				UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
				UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
				UNSIGNED_INT = GL_UNSIGNED_INT,

				NONE
			};
		};
		typedef _ElementType_wrapper::ElementType ElementType;

		// draw mode definition.
		struct _DrawMode_wrapper {
			enum DrawMode {
				POINTS = GL_POINTS,
				LINES = GL_LINES,
				LINE_LOOP = GL_LINE_LOOP,
				LINE_STRIP = GL_LINE_STRIP,
				TRIANGLES = GL_TRIANGLES,
				TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
				TRIANGLE_FAN = GL_TRIANGLE_FAN,
				QUADS = GL_QUADS,
				QUAD_STRIP = GL_QUAD_STRIP,
				NONE
			};
		};
		typedef _DrawMode_wrapper::DrawMode DrawMode;


		// feature of each attribute
		class AttributeFeature {
		public:
			// attribute target, e.g. ShaderProgram::ATTR_POSITION.
			AttributeSemantic attribTarget;

			// the number of attribute element,
			// e.g. 3 for Position3 (x, y and z).
			uinteger elementCount;

			// element type of attribute.
			ElementType elementType;

			// buffer target.
			uinteger bufTarget;

			// attribute vertex buffer object.
			ObjectHandle bufId;

			// buffer size.
			uinteger bufSize;

			// buffer data storage hint
			BufferHint bufHint;

			// the number of data.
			uinteger dataCount;

		public:
			AttributeFeature();
		};

		// address of 'offset' macro
		#define addrof(ptr, offset) ((uint8*)(ptr) + (offset))

		// mapped attribute accessor helper.
		// it offers you the valid reference address.
		// *note: this class is uncopiable.
		template<class _AttribType>
		class AttributeAccessor {
		private:
			// start pointer of mapped buffer.
			void* mPtr;

			// const reference of attribute feature.
			const AttributeFeature* mFeatPtr;

		public:
			// make empty accessor
			AttributeAccessor();
			// make vertex attribute accessor of given data pointer
			AttributeAccessor(void* ptr, const AttributeFeature* featPtr);

			// accessor unmap the mapped pointer when it is destructed.
			~AttributeAccessor();

			// is this accessor empty?
			bool Empty() const;

			// get the number of attribute elements
			uinteger Count() const;

			// destroy this attribute accessor explicitly.
			// it unmap the mapped buffer and make accessor empty.
			// *note: even though you didn't destroy accessor by
			// calling this, its destructor will destroy implicitly.
			void Destroy();

			// get the reference of attribute value at 'index'.
			//
			// *note: it's better to indicate the result to
			// your reference variable when you are accessing.
			// it has tiresome operations to calculate the address
			// what you want to get.
			_AttribType& operator[](uinteger index);
			// get the read-only const reference of attribute value at 'index'.
			const _AttribType& operator[](uinteger index) const;
		};

	private:
		// primitive draw mode.
		DrawMode mDrawMode;

		// attribute buffer feature list
		AttributeFeature* mAttribFeats[AttributeSemantic::COUNT];

	private:
		ArrayBuffer(const ArrayBuffer&);
		ArrayBuffer& operator=(const ArrayBuffer&);

	public:
		ArrayBuffer();

		// delete all generated buffers.
		~ArrayBuffer();
		
		// get draw mode.
		const DrawMode& GetDrawMode() const;

		// set draw mode.
		void SetDrawMode(DrawMode drawMode);

		// get data count of specific attribute.
		// it'll return 0 if given attribute buffer is not exists.
		const uinteger GetDataCount(AttributeSemantic attribSemantic) const;


		// generate attribute buffer with relative informations.
		//
		// *details:
		// _AttribType    - type of attribute, e.g. Position3.
		// _ElemType      - type of element of attribute.
		// attribSemantic - the semantic value of attribute.
		// data           - array data of attributes.
		// storageHint    - buffer object storage hint.
		template<class _AttribType, ElementType _ElemType = ElementType::NONE>
		bool GenAttributeBuffer(
			AttributeSemantic attribSemantic,
			const std::vector<_AttribType>& data,
			BufferHint storageHint = BufferHint::STATIC);

		// get mapped attribute buffer accessor.
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
			AttributeSemantic attribSemantic,
			AccessHint hint = AccessHint::READ_ONLY);

		// bind this vertex attribute buffer object.
		// after using, it have to be unbinded.
		void BindAttribBuffers() const;

		// unbind currently bound vertex buffer object 
		// and disable the enabled vertex attribute arrays.
		void UnbindAttribBuffers() const;

		// bind specific attribute buffer object.
		bool BindAttribBuffer(AttributeSemantic attribSemantic) const;

		// unbind currently bound vertex buffer object 
		// and disable specific vertex attribute array.
		bool UnbindAttribBuffer(AttributeSemantic attribSemantic) const;
		

		// draw vertex array directly.
		// you should have to bind this buffer.
		void DrawArrays() const;

		// draw primitives. you should have to bind relative
		// vertex buffer and primitive buffer.
		void DrawPrimitives() const;
	};


	//----- template implementation of ArrayBuffer::AttributeAccessor -----//

	template<class _AttribType>
	ArrayBuffer::AttributeAccessor<_AttribType>::AttributeAccessor()
		: mPtr(NULL), mFeatPtr(NULL)
	{}

	// make vertex attribute accessor of given data pointer
	template<class _AttribType>
	ArrayBuffer::AttributeAccessor<_AttribType>::AttributeAccessor(
		void* ptr, const AttributeFeature* featPtr)
		: mPtr(ptr), mFeatPtr(featPtr)
	{}

	// accessor unmap the mapped pointer when it is destructed.
	template<class _AttribType>
	ArrayBuffer::AttributeAccessor<_AttribType>::~AttributeAccessor() {
		if (mPtr != NULL)
			Destroy();
	}

	// is this accessor empty?
	template<class _AttribType>
	bool ArrayBuffer::AttributeAccessor<_AttribType>::Empty() const {
		return (mPtr == NULL);
	}

	// get the number of attribute elements
	template<class _AttribType>
	uinteger ArrayBuffer::AttributeAccessor<_AttribType>::Count() const {
		return mFeatPtr->bufSize / sizeof(_AttribType);
	}

	// destroy this attribute accessor explicitly.
	template<class _AttribType>
	void ArrayBuffer::AttributeAccessor<_AttribType>::Destroy() {
		if (mPtr != NULL) {
			glBindBuffer(mFeatPtr->bufTarget, mFeatPtr->bufId);
			glUnmapBuffer(mFeatPtr->bufTarget);
			glBindBuffer(mFeatPtr->bufTarget, 0);
			mPtr = NULL;
		}
	}

	// get the reference of attribute value at 'index'.
	template<class _AttribType>
	_AttribType& 
		ArrayBuffer::AttributeAccessor<_AttribType>::operator[](uinteger index) {
		ONLYDBG_CODEBLOCK(
		if (index*sizeof(_AttribType) >= mFeatPtr->bufSize) {
			LogFatal("attribute index exceeds valid range");
		});
		return *reinterpret_cast<_AttribType*>(
			addrof(mPtr, index*sizeof(_AttribType)));
	}
	// get the read-only const reference of attribute value at 'index'.
	template<class _AttribType>
	const _AttribType& 
		ArrayBuffer::AttributeAccessor<_AttribType>::operator[](uinteger index) const {
		ONLYDBG_CODEBLOCK(
		if (index*sizeof(_AttribType) >= mFeatPtr->bufSize) {
			LogFatal("attribute index exceeds valid range");
		});
		return *reinterpret_cast<const _AttribType*>(
			addrof(mPtr, index*sizeof(_AttribType)));
	}


	//----- template implementation of ArrayBuffer -----//

	// generate attribute buffer with relative informations.
	template<class _AttribType, ArrayBuffer::ElementType _ElemType>
	bool ArrayBuffer::GenAttributeBuffer(AttributeSemantic attribSemantic,
		const std::vector<_AttribType>& data, BufferHint storageHint)
	{
		// check uniqueness of attribute type.
		if (mAttribFeats[attribSemantic] != NULL) {
			LogWarn("there is the buffer generation request of "
				"existent attribute type");
			return false;
		}

		// declare temporary attribute feature data.
		AttributeFeature feat;

		// check and fill element type and element count.
		if (_ElemType == ElementType::NONE) {
			const std::type_info& attr_type = typeid(_AttribType);

			if (attribSemantic != AttributeSemantic::INDICES) {
				feat.elementType = ElementType::REAL;
				if (attr_type == typeid(real))
					feat.elementCount = 1;
				else if (attr_type == typeid(Vector2))
					feat.elementCount = 2;
				else if (attr_type == typeid(Vector3))
					feat.elementCount = 3;
				else if (attr_type == typeid(Vector4))
					feat.elementCount = 4;
				else {
					LogWarn("auto-detection of element properties only supports for "
						"_AttribType of [real | Vector2 | Vector3 | Vector4]");
					return false;
				}
			}
			else {
				if (attr_type == typeid(TriangleFace16)) {
					feat.elementType = ElementType::UNSIGNED_SHORT;
					feat.elementCount = 3;
				}
				else if (attr_type == typeid(TriangleFace32)) {
					feat.elementType = ElementType::UNSIGNED_INT;
					feat.elementCount = 3;
				}
				else if (attr_type == typeid(QuadFace16)) {
					feat.elementType = ElementType::UNSIGNED_SHORT;
					feat.elementCount = 4;
				}
				else if (attr_type == typeid(QuadFace32)) {
					feat.elementType = ElementType::UNSIGNED_INT;
					feat.elementCount = 4;
				}
				else {
					LogWarn("auto-detection of element properties "
						"only supports for _PrimitiveType of "
						"[TriangleFace16 | TriangleFace32 | QuadFace16 | QuadFace32]");
					return false;
				}
			}
		}
		else {
			feat.elementType = _ElemType;

			switch (_ElemType) {
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

		// set other attribute features.
		feat.attribTarget = attribSemantic;

		// set data count
		feat.dataCount = data.size();

		// set buffer features.
		feat.bufTarget
			= (attribSemantic == AttributeSemantic::INDICES
			? GL_ELEMENT_ARRAY_BUFFER
			: GL_ARRAY_BUFFER);
		feat.bufSize = sizeof(_AttribType)*data.size();
		feat.bufHint = storageHint;

		// generate buffer
		glGenBuffers(1, &feat.bufId);
		if (feat.bufId == 0) {
			LogError("failed to generate buffer");
			return false;
		}

		// set buffer data
		glBindBuffer(feat.bufTarget, feat.bufId);
		glBufferData(feat.bufTarget, feat.bufSize, &data[0], feat.bufHint);
		glBindBuffer(feat.bufTarget, 0);

		// create attribute feature and copy data
		mAttribFeats[attribSemantic] = new AttributeFeature(feat);
		if (mAttribFeats[attribSemantic] == NULL) {
			LogError("failed to allocate attribute feature");
			return false;
		}
		return true;
	}

	// get mapped attribute buffer accessor.
	template<class _AttribType>
	ArrayBuffer::AttributeAccessor<_AttribType> ArrayBuffer::GetAttributeAccessor(
		AttributeSemantic attribSemantic, AccessHint hint)
	{
		// existence check
		if (mAttribFeats[attribSemantic] == NULL) {
			LogWarn("there is no buffer for the specified attribute");
			return AttributeAccessor<_AttribType>();
		}
		
		const AttributeFeature* feat = mAttribFeats[attribSemantic];
		void* ptr = NULL;

		// access hint validation check and warn.
		if (feat->bufHint == BufferHint::STATIC) {
			if (hint != AccessHint::READ_ONLY) {
				LogWarn("you may have created the buffer as STATIC for READ_ONLY access");
			}
		}
		else if (feat->bufHint == BufferHint::STREAM) {
			if (hint != AccessHint::WRITE_ONLY) {
				LogWarn("you may have created the buffer as STREAM for WRITE_ONLY access");
			}
		}

		// bind buffer before mapping
		glBindBuffer(feat->bufTarget, feat->bufId);

		// map the buffer
		ptr = glMapBuffer(feat->bufTarget, hint);

		// unbind buffer after mapping
		glBindBuffer(feat->bufTarget, 0);

		// mapping correction check
		if (ptr == NULL) {
			GLenum errcode = glGetError();
			std::string err_str = "failed to map buffer\n";
			err_str += reinterpret_cast<const char*>(gluErrorString(errcode));
			LogError(err_str);
			return AttributeAccessor<_AttribType>();
		}

		// return valid attribute accessor
		return AttributeAccessor<_AttribType>(ptr, feat);
	}

}
#endif
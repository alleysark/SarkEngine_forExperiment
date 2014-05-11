#ifndef __ARRAY_BUFFER_H__
#define __ARRAY_BUFFER_H__

#include "core.h"
#include "ShaderProgram.h"

namespace sark{

	// array buffer object.
	// see VertexBuffer and IndexBuffer.
	class ArrayBuffer{
	public:
		// buffer storage hint
		struct _BufferHint_wrapper{
			enum BufferHint{
				STATIC = GL_STATIC_DRAW,
				DYNAMIC = GL_DYNAMIC_DRAW,
				STREAM = GL_STREAM_DRAW
			};
		};
		typedef _BufferHint_wrapper::BufferHint BufferHint;

		// buffer accessing hint
		struct _AccessHint_wrapper{
			enum AccessHint{
				READ_ONLY = GL_READ_ONLY,
				WRITE_ONLY = GL_WRITE_ONLY,
				READ_WRITE = GL_READ_WRITE
			};
		};
		typedef _AccessHint_wrapper::AccessHint AccessHint;

		// attribute element type
		struct _ElementType_wrapper{
			enum ElementType{
				// type real. usual type
			#ifdef SARKLIB_USING_DOUBLE
				REAL = GL_DOUBLE,
			#else
				REAL = GL_FLOAT,
			#endif
				// types for normalized element
				UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
				UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
				UNSIGNED_INT = GL_UNSIGNED_INT,

				NONE
			};
		};
		typedef _ElementType_wrapper::ElementType ElementType;

		// draw mode definition.
		struct _DrawMode_wrapper{
			enum DrawMode{
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
		class AttributeFeature{
		public:
			// attribute target, e.g. ShaderProgram::ATTR_POSITION.
			ShaderProgram::AttributeSemantic attribTarget;

			// attribute element count, e.g. 3 for Position3 (x, y and z).
			uinteger elementCount;

			// element type of attribute.
			ElementType elementType;

			// attribute vertex buffer object.
			ObjectHandle bufId;

			// buffer size.
			uinteger bufSize;

			// buffer data storage hint
			BufferHint bufHint;

		public:
			AttributeFeature();
		};

		// address of 'offset' macro
		#define addrof(ptr, offset) ((uint8*)(ptr) + (offset))

		// mapped attribute accessor helper.
		// it offers you the valid reference address.
		// *note: this class is uncopiable.
		template<class _AttribType>
		class AttributeAccessor{
		private:
			// start pointer of mapped buffer.
			void* mPtr;

			// const reference of attribute feature.
			const AttributeFeature* mFeatPtr;

		private:
			AttributeAccessor<_AttribType>& operator=(const AttributeAccessor<_AttribType>&);

		public:
			// make empty accessor
			AttributeAccessor();
			// make vertex attribute accessor of given data pointer
			AttributeAccessor(void* ptr, const AttributeFeature* featPtr);

			AttributeAccessor(const AttributeAccessor<_AttribType>&);

			// accessor unmap the mapped pointer when it is destructed.
			~AttributeAccessor();

			// is this accessor empty?
			bool Empty() const;

			// get the number of attribute elements
			uinteger Count() const;

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

	protected:
		// primitive draw mode.
		DrawMode mDrawMode;

		// data count.
		uinteger mDataCount;

	private:
		ArrayBuffer(const ArrayBuffer&);
		ArrayBuffer& operator=(const ArrayBuffer&);

	public:
		ArrayBuffer();

		virtual ~ArrayBuffer();

		// bind array buffer object
		virtual bool Bind() const = 0;

		// unbind currently bound array buffer object.
		virtual void Unbind() const = 0;

		// get draw mode. it can be NONE for vertex array.
		const DrawMode& GetDrawMode() const;

		// get data count.
		// as for vertex buffer, it assumes that the data counts
		// of each attribute buffers are same.
		const uinteger& GetDataCount() const;
	};


	//----- template implementation -----//

	template<class _AttribType>
	ArrayBuffer::AttributeAccessor<_AttribType>& 
		ArrayBuffer::AttributeAccessor<_AttribType>::operator=(const AttributeAccessor<_AttribType>&){}

	template<class _AttribType>
	ArrayBuffer::AttributeAccessor<_AttribType>::AttributeAccessor()
		: mPtr(NULL), mFeatPtr(NULL)
	{}

	// make vertex attribute accessor of given data pointer
	template<class _AttribType>
	ArrayBuffer::AttributeAccessor<_AttribType>::AttributeAccessor(void* ptr, const AttributeFeature* featPtr)
		: mPtr(ptr), mFeatPtr(featPtr)
	{}

	template<class _AttribType>
	ArrayBuffer::AttributeAccessor<_AttribType>::AttributeAccessor(const AttributeAccessor<_AttribType>& acc)
		: mPtr(acc.mPtr), mFeatPtr(acc.mFeatPtr)
	{
		acc.mPtr = NULL;
	}

	// accessor unmap the mapped pointer when it is destructed.
	template<class _AttribType>
	ArrayBuffer::AttributeAccessor<_AttribType>::~AttributeAccessor(){
		if (mPtr != NULL){
			glBindBuffer(GL_VERTEX_ARRAY, mFeatPtr->bufId);
			glUnmapBuffer(GL_VERTEX_ARRAY);
			glBindBuffer(GL_VERTEX_ARRAY, 0);
			mPtr = NULL;
		}
	}

	// is this accessor empty?
	template<class _AttribType>
	bool ArrayBuffer::AttributeAccessor<_AttribType>::Empty() const{
		return (mPtr == NULL);
	}

	// get the number of attribute elements
	template<class _AttribType>
	uinteger ArrayBuffer::AttributeAccessor<_AttribType>::Count() const{
		return mFeatPtr->bufSize / sizeof(_AttribType);
	}

	// get the reference of attribute value at 'index'.
	//
	// *note: it's better to indicate the result to
	// your reference variable when you are accessing.
	// it has tiresome operations to calculate the address
	// what you want to get.
	template<class _AttribType>
	_AttribType& ArrayBuffer::AttributeAccessor<_AttribType>::operator[](uinteger index){
		ONLYDBG_CODEBLOCK(
		if (index*sizeof(_AttribType) >= mFeatPtr->bufSize){
			LogFatal("attribute index exceeds valid range");
		});
		return *reinterpret_cast<_AttribType*>(addrof(mPtr, index*sizeof(_AttribType)));
	}
	// get the read-only const reference of attribute value at 'index'.
	template<class _AttribType>
	const _AttribType& ArrayBuffer::AttributeAccessor<_AttribType>::operator[](uinteger index) const{
		ONLYDBG_CODEBLOCK(
		if (index*sizeof(_AttribType) >= mFeatPtr->bufSize){
			LogFatal("attribute index exceeds valid range");
		});
		return *reinterpret_cast<const _AttribType*>(addrof(mPtr, index*sizeof(_AttribType)));
	}

}
#endif
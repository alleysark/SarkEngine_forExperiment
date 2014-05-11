#ifndef __ARRAY_BUFFER_H__
#define __ARRAY_BUFFER_H__

#include <vector>
#include <list>
#include "core.h"
#include "ShaderProgram.h"
#include "Debug.h"

namespace sark{

	// array buffer object for mesh.
	// it defines vertex buffers of user-defined attribute
	// type and also can define primitive buffer of user-defined
	// primitive types. 
	// it offers you the methods of accessing mapped buffer
	// as attribute accessor.
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

		// type of attribute feature list.
		typedef std::list<AttributeFeature> AttribFeatureList;

	private:
		// primitive draw mode.
		DrawMode mDrawMode;

		// attribute data count.
		uinteger mDataCount;

		// primitive data count.
		uinteger mPrimitiveCount;

		// attribute buffer feature list
		AttribFeatureList mAttribFeats;

		// primitive buffer feature.
		AttributeFeature mPrimitiveFeat;

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

		// get attribute data count.
		// as for vertex buffer, it assumes that the data counts
		// of each attribute buffers are same.
		const uinteger& GetDataCount() const;

		// get primitive data count.
		const uinteger& GetPrimitiveCount() const;

		// whether it has primitive buffer or not.
		bool HasPrimitiveBuffer() const;


		// ----- vertex attribute array buffer methods -----

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
			ShaderProgram::AttributeSemantic attribSemantic,
			const std::vector<_AttribType>& data,
			BufferHint storageHint = BufferHint::STATIC);

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
			ShaderProgram::AttributeSemantic attribSemantic,
			AccessHint hint = AccessHint::READ_ONLY);

		// bind this vertex attribute buffer object.
		// after using, it have to be unbinded.
		bool BindAttribBuffers() const;

		// bind specific attribute buffer object.
		bool BindAttribBuffer(ShaderProgram::AttributeSemantic attribSemantic) const;

		// unbind currently bound vertex buffer object.
		void UnbindAttribBuffers() const;

		// draw vertex array directly.
		// you should have to bind this buffer.
		void DrawArrays() const;


		// ----- primitive array buffer methods -----

		// generate primitive array buffer with relative informations.
		//
		// *details:
		// _PrimitiveType - type of primitive, e.g. TriangleFace16.
		// _ElemType      - type of element of attribute.
		// drawMode       - how to draw the given primitive data.
		// data           - array data of primitives.
		// storageHint    - buffer object storage hint.
		template<class _PrimitiveType, ElementType _ElemType = ElementType::NONE>
		bool GenPrimitiveBuffer(
			const std::vector<_PrimitiveType>& data,
			BufferHint storageHint = BufferHint::STATIC);

		// get mapped primitive attribute accessor.
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
		AttributeAccessor<_PrimitiveType> GetPrimitiveAccessor(
			AccessHint hint = AccessHint::READ_ONLY);

		// bind primitive buffer object.
		// after using, it have to be unbinded.
		bool BindPrimitiveBuffer() const;

		// unbind currently bound primitive buffer object.
		void UnbindPrimitiveBuffer() const;

		// draw elements. you should have to bind relative
		// vertex buffer and this index buffer.
		void DrawPrimitives() const;
	};





	//----- template implementation -----//

	template<class _AttribType>
	ArrayBuffer::AttributeAccessor<_AttribType>& 
		ArrayBuffer::AttributeAccessor<_AttribType>::operator=(
		const AttributeAccessor<_AttribType>&){}

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

	template<class _AttribType>
	ArrayBuffer::AttributeAccessor<_AttribType>::AttributeAccessor(
		const AttributeAccessor<_AttribType>& acc)
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
	template<class _AttribType>
	_AttribType& 
		ArrayBuffer::AttributeAccessor<_AttribType>::operator[](uinteger index){
		ONLYDBG_CODEBLOCK(
		if (index*sizeof(_AttribType) >= mFeatPtr->bufSize){
			LogFatal("attribute index exceeds valid range");
		});
		return *reinterpret_cast<_AttribType*>(
			addrof(mPtr, index*sizeof(_AttribType)));
	}
	// get the read-only const reference of attribute value at 'index'.
	template<class _AttribType>
	const _AttribType& 
		ArrayBuffer::AttributeAccessor<_AttribType>::operator[](uinteger index) const{
		ONLYDBG_CODEBLOCK(
		if (index*sizeof(_AttribType) >= mFeatPtr->bufSize){
			LogFatal("attribute index exceeds valid range");
		});
		return *reinterpret_cast<const _AttribType*>(
			addrof(mPtr, index*sizeof(_AttribType)));
	}


	// ----- implementation of vertex attribute array buffer methods -----

	// generate attribute buffer with relative informations.
	template<class _AttribType, ArrayBuffer::ElementType _ElemType>
	bool ArrayBuffer::GenAttributeBuffer(
		ShaderProgram::AttributeSemantic attribSemantic,
		const std::vector<_AttribType>& data, BufferHint storageHint)
	{
		if (attribSemantic == ShaderProgram::ATTR_INDICES){
			LogWarn("use GenPrimitiveBuffer to generate the primitive indices");
			return false;
		}

		// check uniqueness of attribute type.
		AttribFeatureList::iterator itr = mAttribFeats.begin();
		AttribFeatureList::iterator end = mAttribFeats.end();
		for (; itr != end; itr++){
			if (itr->attribTarget == attribSemantic){
				LogWarn("there is the buffer generation request of "
					"existent attribute type");
				return false;
			}
		}

		// check data count appropriacy.
		if (mDataCount == 0)
			mDataCount = data.size();
		else{
			if (mDataCount != data.size()){
				LogWarn("the number of requested attribute data is "
					"not equal to previous things");
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
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		mAttribFeats.push_back(feat);
		return true;
	}

	// get mapped vertices attribute accessor.
	template<class _AttribType>
	ArrayBuffer::AttributeAccessor<_AttribType> ArrayBuffer::GetAttributeAccessor(
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


	// ------ implementation of index array buffer methods -----

	// generate primitive array buffer with relative informations.
	template<class _PrimitiveType, ArrayBuffer::ElementType _ElemType>
	bool ArrayBuffer::GenPrimitiveBuffer(
		const std::vector<_PrimitiveType>& data, BufferHint storageHint)
	{
		// check whether primitive buffer is generated previously.
		if (mPrimitiveFeat.bufId != 0)
			return false;

		// check and fill element type and element count.
		if (_ElemType == ElementType::NONE){
			const std::type_info& prim_type = typeid(_PrimitiveType);
			if (prim_type == typeid(TriangleFace16)){
				mPrimitiveFeat.elementType = ElementType::UNSIGNED_SHORT;
				mPrimitiveFeat.elementCount = 3;
			}
			else if (prim_type == typeid(TriangleFace32)){
				mPrimitiveFeat.elementType = ElementType::UNSIGNED_INT;
				mPrimitiveFeat.elementCount = 3;
			}
			else if (prim_type == typeid(QuadFace16)){
				mPrimitiveFeat.elementType = ElementType::UNSIGNED_SHORT;
				mPrimitiveFeat.elementCount = 4;
			}
			else if (prim_type == typeid(QuadFace32)){
				mPrimitiveFeat.elementType = ElementType::UNSIGNED_INT;
				mPrimitiveFeat.elementCount = 4;
			}
			else{
				LogWarn("auto-detection of element properties "
					"only supports for _PrimitiveType of "
					"[TriangleFace16 | TriangleFace32 | QuadFace16 | QuadFace32]");
				return false;
			}
		}
		else{
			mPrimitiveFeat.elementType = _ElemType;

			switch (_ElemType){
			case ElementType::REAL:
				mPrimitiveFeat.elementCount = sizeof(_PrimitiveType) / sizeof(real);
				break;
			case ElementType::UNSIGNED_BYTE:
				mPrimitiveFeat.elementCount = sizeof(_PrimitiveType);
				break;
			case ElementType::UNSIGNED_SHORT:
				mPrimitiveFeat.elementCount = sizeof(_PrimitiveType) / 2;
				break;
			case ElementType::UNSIGNED_INT:
				mPrimitiveFeat.elementCount = sizeof(_PrimitiveType) / 4;
				break;
			}
		}

		// set other attribute features
		mPrimitiveFeat.attribTarget = ShaderProgram::ATTR_INDICES;
		mPrimitiveFeat.bufSize = sizeof(_PrimitiveType)*data.size();
		mPrimitiveFeat.bufHint = storageHint;

		// set primitive count.
		mPrimitiveCount = data.size();

		// generate buffer
		glGenBuffers(1, &mPrimitiveFeat.bufId);
		if (mPrimitiveFeat.bufId == 0){
			LogError("failed to generate buffer");
			return false;
		}

		// set buffer data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPrimitiveFeat.bufId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			mPrimitiveFeat.bufSize, &data[0], mPrimitiveFeat.bufHint);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return true;
	}

	// get mapped primitive attribute accessor.
	template<class _PrimitiveType>
	ArrayBuffer::AttributeAccessor<_PrimitiveType>
		ArrayBuffer::GetPrimitiveAccessor(AccessHint hint)
	{
			void* ptr = NULL;

			// bind buffer before mapping
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPrimitiveFeat.bufId);

			// access hint validation check and map buffer
			if (mPrimitiveFeat.bufHint == BufferHint::STATIC){
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
			return AttributeAccessor<_PrimitiveType>(ptr, &mPrimitiveFeat);
		}

}
#endif
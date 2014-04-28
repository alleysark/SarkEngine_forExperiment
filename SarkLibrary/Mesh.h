#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <list>
#include <GL/glew.h>
#include "core.h"

namespace sark{

	// mesh defines the vertices and indices of model.
	// mesh must have to have position data and its face data.
	// color, normal, texcoord datas are optional.
	class Mesh{
	public:
		// gl's buffer copying api has a dependency of type of element.
		// so it just redicines element type according to the library type
#ifdef SARKLIB_USING_DOUBLE
		#define GL_REAL GL_DOUBLE
#else
		#define GL_REAL GL_FLOAT
#endif

		// face defines a primitive polygon surface
		// it is packed byte as 1 to make sure when it copies into
		// the gpu memory
		#pragma pack(push, 1)
		class Face{
		public:
			uint16 a, b, c;

			Face();
			Face(uint16 _a, uint16 _b, uint16 _c);
		};
		#pragma pack(pop)

	private:
		// variable vertex buffer object pair.
		struct VBOPair{
			ObjectHandle hBufId;
			GLenum bufTarget;
			uinteger bufSize;
			void* bufData;

			integer attribTarget;
			GLenum attribType;
			uinteger attribSize;
		};

		// handle of vertex buffer object
		std::list<VBOPair> mVBOs;

		// handle of vertex array object
		ObjectHandle mhVertexArray;


		// vertex primitive face data.
		std::vector<Face> mFaces;

		// vertex position data. it couldn't be empty.
		std::vector<Position3> mPositions;

		// vertex normal data.
		std::vector<Normal> mNormals;
		
		// vertex color data.
		std::vector<ColorRGBA> mColors;

		// vertex texcoord series data.
		std::vector<Texcoord> mTexcoord0s;
		std::vector<Texcoord> mTexcoord1s;
		std::vector<Texcoord> mTexcoord2s;
		std::vector<Texcoord> mTexcoord3s;

		// mesh can't be assigned implicitly
		Mesh(const Mesh& mesh){}
		const Mesh&  operator=(const Mesh&){}

	public:
		Mesh();

		// buffer objects are deleted in destructor
		~Mesh();

		// full copy from another mesh object
		void CopyFrom(const Mesh& mesh);

		// set face data
		void SetFaces(const std::vector<Face>& faces);

		// set position data
		void SetPositions(const std::vector<Position3>& positions);

		// set normal data
		void SetNormals(const std::vector<Normal>& normals);

		// set color data
		void SetColors(const std::vector<ColorRGBA>& colors);

		// set texcoord0 data
		void SetTexcoord0s(const std::vector<Texcoord>& texcoords);
		// set texcoord1 data
		void SetTexcoord1s(const std::vector<Texcoord>& texcoords);
		// set texcoord2 data
		void SetTexcoord2s(const std::vector<Texcoord>& texcoords);
		// set texcoord3 data
		void SetTexcoord3s(const std::vector<Texcoord>& texcoords);

		
		// bind setted vertex datas into vbo and vao.
		bool BindDatas();


		// draw this mesh
		void Draw();


		// get position data
		const std::vector<Position3>& GetPositions() const;

		// get normal data
		const std::vector<Normal>& GetNormals() const;

		// get color data
		const std::vector<ColorRGBA>& GetColors() const;

		// get texcoord0 data
		const std::vector<Texcoord>& GetTexcoord0s() const;
		// get texcoord1 data
		const std::vector<Texcoord>& GetTexcoord1s() const;
		// get texcoord2 data
		const std::vector<Texcoord>& GetTexcoord2s() const;
		// get texcoord3 data
		const std::vector<Texcoord>& GetTexcoord3s() const;

		// get face data
		const std::vector<Face>& GetFaces() const;
	};

}
#endif
#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <list>
#include <GL/glew.h>
#include "core.h"

namespace sark{

	// mesh defines the vertices and indices of model.
	// mesh must have to have position data and it can have other datas
	// like normal, texcoord, etc. optionally.
	// (i think it doesn't need to be an extendable class...)
	class Mesh{
	public:
		#define OFFSET(i) ((int8*)NULL + i)

		// gl's buffer copying api has a dependency of type of element.
		// so it just redicines element type according to the library type
#ifdef SARKLIB_USING_DOUBLE
		#define VERTEX_TYPE GL_DOUBLE
#else
		#define VERTEX_TYPE GL_FLOAT
#endif
		#define INDEX_TYPE	GL_UNSIGNED_SHORT

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

		typedef GLuint BufferHandle;
		typedef GLenum ClientState;

	protected:
		// vertex buffer object id
		BufferHandle mhVertexBufId;
		// index buffer object id
		BufferHandle mhIndexBufId;

		// list of state which is used on vertex buffer
		std::list<ClientState> mUsedStates;

		// position data of vertex elements
		// it couldn't be empty.
		std::vector<Position3> mPositions;

		// face data for vertex elements
		std::vector<Face> mFaces;

		// normal data of vertex elements
		std::vector<Normal> mNormals;

		// texcoord data of vertex elements
		std::vector<Texcoord> mTexcoords;

	public:
		Mesh();

		// buffer objects are deleted in destructor
		~Mesh();

		// create mesh with given position and face data
		bool Create(
			std::vector<Position3>& positions, 
			std::vector<Face>& faces);

		// create mesh with given position, normal and face data
		bool Create(
			std::vector<Position3>& positions, 
			std::vector<Normal>& normals, 
			std::vector<Face>& faces);

		// create mesh with given position, normal, texcoord and face data
		bool Create(
			std::vector<Position3>& positions, 
			std::vector<Normal>& normals, 
			std::vector<Texcoord>& texcoords, 
			std::vector<Face>& faces);

	private:
		bool Create();

	public:
		// bind mesh buffer into the graphic system
		void Bind();

		// draw this.
		// it must have been binded before drawing.
		void Draw();

		// unbind mesh buffer from graphic system.
		// it must have been unbinded after binding
		void Unbind();
	};

}
#endif
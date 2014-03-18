#include "Mesh.h"

namespace sark{

	Mesh::Face::Face(){}
	Mesh::Face::Face(uint16 _a, uint16 _b, uint16 _c)
		: a(_a), b(_b), c(_c){}


	Mesh::Mesh()
		: mhVertexBufId(0), mhIndexBufId(0)
	{}

	// buffer objects are deleted in destructor
	Mesh::~Mesh(){
		glDeleteBuffersARB(1, &mhVertexBufId);
		glDeleteBuffersARB(1, &mhIndexBufId);

		mPositions.clear();
		mNormals.clear();
		mTexcoords.clear();
		mFaces.clear();
	}

	// create mesh with given position and face data
	bool Mesh::Create(std::vector<Position3>& positions, std::vector<Face>& faces){
		if (positions.size() == 0)
			return false;
		if (faces.size() == 0)
			return false;

		mPositions = positions;
		mFaces = faces;

		return Create();
	}

	// create mesh with given position, normal and face data
	bool Mesh::Create(std::vector<Position3>& positions, std::vector<Normal>& normals, std::vector<Face>& faces){
		if (positions.size() == 0)
			return false;
		if (positions.size() != normals.size())
			return false;
		if (faces.size() == 0)
			return false;

		mPositions = positions;
		mNormals = normals;
		mFaces = faces;

		return Create();
	}

	// create mesh with given position, normal, texcoord and face data
	bool Mesh::Create(std::vector<Position3>& positions, std::vector<Normal>& normals, std::vector<Texcoord>& texcoords, std::vector<Face>& faces){
		if (positions.size() == 0)
			return false;
		if (positions.size() != normals.size())
			return false;
		if (positions.size() != texcoords.size())
			return false;
		if (faces.size() == 0)
			return false;

		mPositions = positions;
		mNormals = normals;
		mTexcoords = texcoords;
		mFaces = faces;

		return Create();
	}

	bool Mesh::Create(){
		glGenBuffersARB(1, &mhVertexBufId);
		if (mhVertexBufId == 0)
			return false;
		glGenBuffersARB(1, &mhIndexBufId);
		if (mhIndexBufId == 0)
			return false;

		// set vertex buffer
		integer posBufSize = mPositions.size()*sizeof(Position3);
		integer normBufSize = mNormals.size()*sizeof(Normal);
		integer texBufSize = mTexcoords.size()*sizeof(Texcoord);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, mhVertexBufId);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, posBufSize + normBufSize + texBufSize, NULL, GL_STATIC_DRAW_ARB);

		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, posBufSize, &mPositions[0]);
		glVertexPointer(3, VERTEX_TYPE, 0, OFFSET(0));
		mUsedStates.push_back(GL_VERTEX_ARRAY);

		if (normBufSize != 0){
			glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, posBufSize, normBufSize, &mNormals[0]);
			glNormalPointer(VERTEX_TYPE, 0, OFFSET(posBufSize));
			mUsedStates.push_back(GL_NORMAL_ARRAY);
		}
		if (texBufSize != 0){
			glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, normBufSize, texBufSize, &mTexcoords[0]);
			glTexCoordPointer(2, VERTEX_TYPE, 0, OFFSET(normBufSize));
			mUsedStates.push_back(GL_TEXTURE_COORD_ARRAY);
		}

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		// set index buffer
		integer faceBufSize = mFaces.size()*sizeof(Face);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mhIndexBufId);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, faceBufSize, &mFaces[0], GL_STATIC_DRAW_ARB);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

		return true;
	}

	// bind mesh buffer into the graphic system
	void Mesh::Bind(){
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, mhVertexBufId);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mhIndexBufId);

		for (std::list<ClientState>::iterator itr = mUsedStates.begin(); itr != mUsedStates.end(); itr++){
			glEnableClientState((*itr));
		}
	}

	// draw this.
	// it must have been binded before drawing.
	void Mesh::Draw(){
		glDrawElements(GL_TRIANGLES, mFaces.size() * 3, INDEX_TYPE, 0);
	}

	// unbind mesh buffer from graphic system.
	// it must have been unbinded after binding
	void Mesh::Unbind(){
		for (std::list<ClientState>::iterator itr = mUsedStates.begin(); itr != mUsedStates.end(); itr++){
			glDisableClientState((*itr));
		}

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}

}
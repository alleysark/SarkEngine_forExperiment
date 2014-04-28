#include "Mesh.h"
#include "ShaderProgram.h"

namespace sark{

	Mesh::Face::Face(){}
	Mesh::Face::Face(uint16 _a, uint16 _b, uint16 _c)
		: a(_a), b(_b), c(_c){}


	Mesh::Mesh()
		: mhVertexArray(0)
	{}

	// buffer objects are deleted in destructor
	Mesh::~Mesh(){
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		for (std::list<VBOPair>::iterator itr = mVBOs.begin(); itr != mVBOs.end(); itr++){
			if (itr->hBufId != 0){
				glDeleteBuffers(1, &(itr->hBufId));
				glDisableVertexAttribArray(itr->attribTarget);
			}
		}
		mVBOs.clear();

		if (mhVertexArray != 0){
			glDeleteVertexArrays(1, &mhVertexArray);
			mhVertexArray = 0;
		}

		mFaces.clear();

		mPositions.clear();
		mNormals.clear();
		mColors.clear();
		mTexcoord0s.clear();
		mTexcoord1s.clear();
		mTexcoord2s.clear();
		mTexcoord3s.clear();
	}


	// full copy from another mesh object
	void Mesh::CopyFrom(const Mesh& mesh){
		if (!mesh.mFaces.empty())
			SetFaces(mesh.mFaces);
		if (!mesh.mPositions.empty())
			SetPositions(mesh.mPositions);
		if (!mesh.mNormals.empty())
			SetNormals(mesh.mNormals);
		if (!mesh.mColors.empty())
			SetColors(mesh.mColors);
		if (!mesh.mTexcoord0s.empty())
			SetTexcoord0s(mesh.mTexcoord0s);
		if (!mesh.mTexcoord1s.empty())
			SetTexcoord0s(mesh.mTexcoord1s);
		if (!mesh.mTexcoord2s.empty())
			SetTexcoord0s(mesh.mTexcoord2s);
		if (!mesh.mTexcoord3s.empty())
			SetTexcoord0s(mesh.mTexcoord3s);
		BindDatas();
	}

	// set face data
	void Mesh::SetFaces(const std::vector<Mesh::Face>& faces){
		mFaces = faces;

		VBOPair vboPair;
		glGenBuffers(1, &vboPair.hBufId);
		vboPair.bufTarget = GL_ELEMENT_ARRAY_BUFFER;
		vboPair.bufSize = mFaces.size()*sizeof(Face);
		vboPair.bufData = &mFaces[0];
		vboPair.attribTarget = ShaderProgram::ATTR_INDICES;
		vboPair.attribType = GL_UNSIGNED_SHORT;
		vboPair.attribSize = 3;
		mVBOs.push_back(vboPair);
	}

	// set position data
	void Mesh::SetPositions(const std::vector<Position3>& positions){
		mPositions = positions;

		VBOPair vboPair;
		glGenBuffers(1, &vboPair.hBufId);
		vboPair.bufTarget = GL_ARRAY_BUFFER;
		vboPair.bufSize = mPositions.size()*sizeof(Position3);
		vboPair.bufData = &mPositions[0];
		vboPair.attribTarget = ShaderProgram::ATTR_POSITION;
		vboPair.attribType = GL_REAL;
		vboPair.attribSize = 3;
		mVBOs.push_back(vboPair);
	}

	// set normal data
	void Mesh::SetNormals(const std::vector<Normal>& normals){
		mNormals = normals;
		
		VBOPair vboPair;
		glGenBuffers(1, &vboPair.hBufId);
		vboPair.bufTarget = GL_ARRAY_BUFFER;
		vboPair.bufSize = mNormals.size()*sizeof(Normal);
		vboPair.bufData = &mNormals[0];
		vboPair.attribTarget = ShaderProgram::ATTR_NORMAL;
		vboPair.attribType = GL_REAL;
		vboPair.attribSize = 3;
		mVBOs.push_back(vboPair);
	}

	// set color data
	void Mesh::SetColors(const std::vector<ColorRGBA>& colors){
		mColors = colors;
		
		VBOPair vboPair;
		glGenBuffers(1, &vboPair.hBufId);
		vboPair.bufTarget = GL_ARRAY_BUFFER;
		vboPair.bufSize = mColors.size()*sizeof(ColorRGBA);
		vboPair.bufData = &mColors[0];
		vboPair.attribTarget = ShaderProgram::ATTR_COLOR;
		vboPair.attribType = GL_REAL;
		vboPair.attribSize = 4;
		mVBOs.push_back(vboPair);
	}

	// set texcoord0 data
	void Mesh::SetTexcoord0s(const std::vector<Texcoord>& texcoords){
		mTexcoord0s = texcoords;
		
		VBOPair vboPair;
		glGenBuffers(1, &vboPair.hBufId);
		vboPair.bufTarget = GL_ARRAY_BUFFER;
		vboPair.bufSize = mTexcoord0s.size()*sizeof(Texcoord);
		vboPair.bufData = &mTexcoord0s[0];
		vboPair.attribTarget = ShaderProgram::ATTR_TEX_COORD0;
		vboPair.attribType = GL_REAL;
		vboPair.attribSize = 2;
		mVBOs.push_back(vboPair);
	}
	// set texcoord1 data
	void Mesh::SetTexcoord1s(const std::vector<Texcoord>& texcoords){
		mTexcoord1s = texcoords;
		
		VBOPair vboPair;
		glGenBuffers(1, &vboPair.hBufId);
		vboPair.bufTarget = GL_ARRAY_BUFFER;
		vboPair.bufSize = mTexcoord1s.size()*sizeof(Texcoord);
		vboPair.bufData = &mTexcoord1s[0];
		vboPair.attribTarget = ShaderProgram::ATTR_TEX_COORD1;
		vboPair.attribType = GL_REAL;
		vboPair.attribSize = 2;
		mVBOs.push_back(vboPair);
	}
	// set texcoord2 data
	void Mesh::SetTexcoord2s(const std::vector<Texcoord>& texcoords){
		mTexcoord2s = texcoords;
		
		VBOPair vboPair;
		glGenBuffers(1, &vboPair.hBufId);
		vboPair.bufTarget = GL_ARRAY_BUFFER;
		vboPair.bufSize = mTexcoord2s.size()*sizeof(Texcoord);
		vboPair.bufData = &mTexcoord2s[0];
		vboPair.attribTarget = ShaderProgram::ATTR_TEX_COORD2;
		vboPair.attribType = GL_REAL;
		vboPair.attribSize = 2;
		mVBOs.push_back(vboPair);
	}
	// set texcoord3 data
	void Mesh::SetTexcoord3s(const std::vector<Texcoord>& texcoords){
		mTexcoord3s = texcoords;
		
		VBOPair vboPair;
		glGenBuffers(1, &vboPair.hBufId);
		vboPair.bufTarget = GL_ARRAY_BUFFER;
		vboPair.bufSize = mTexcoord3s.size()*sizeof(Texcoord);
		vboPair.bufData = &mTexcoord3s[0];
		vboPair.attribTarget = ShaderProgram::ATTR_TEX_COORD3;
		vboPair.attribType = GL_REAL;
		vboPair.attribSize = 2;
		mVBOs.push_back(vboPair);
	}


	// bind setted vertex datas into vbo and vao.
	bool Mesh::BindDatas(){
		glGenVertexArrays(1, &mhVertexArray);
		glBindVertexArray(mhVertexArray);

		// set vertex attribute buffers
		for (std::list<VBOPair>::iterator itr = mVBOs.begin(); itr != mVBOs.end(); itr++){
			if (itr->hBufId == 0)
				return false;

			glBindBuffer(itr->bufTarget, itr->hBufId);
			glBufferData(itr->bufTarget, itr->bufSize, itr->bufData, GL_STATIC_DRAW);
			glEnableVertexAttribArray(itr->attribTarget);
			glVertexAttribPointer(itr->attribTarget, itr->attribSize, itr->attribType, 0, 0, NULL);
		}
		glBindVertexArray(0);

		return true;
	}


	// draw this mesh
	void Mesh::Draw() const{
		glBindVertexArray(mhVertexArray);
		glDrawElements(GL_TRIANGLES, mFaces.size() * 3, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
	}


	// get position data
	const std::vector<Position3>& Mesh::GetPositions() const{
		return mPositions;
	}

	// get normal data
	const std::vector<Normal>& Mesh::GetNormals() const{
		return mNormals;
	}

	// get color data
	const std::vector<ColorRGBA>& Mesh::GetColors() const{
		return mColors;
	}

	// get texcoord0 data
	const std::vector<Texcoord>& Mesh::GetTexcoord0s() const{
		return mTexcoord0s;
	}
	// get texcoord1 data
	const std::vector<Texcoord>& Mesh::GetTexcoord1s() const{
		return mTexcoord1s;
	}
	// get texcoord2 data
	const std::vector<Texcoord>& Mesh::GetTexcoord2s() const{
		return mTexcoord2s;
	}
	// get texcoord3 data
	const std::vector<Texcoord>& Mesh::GetTexcoord3s() const{
		return mTexcoord3s;
	}

	// get face data
	const std::vector<Mesh::Face>& Mesh::GetFaces() const{
		return mFaces;
	}

}
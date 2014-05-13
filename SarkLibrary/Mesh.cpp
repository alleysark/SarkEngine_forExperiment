#include "Mesh.h"
#include "ShaderProgram.h"

namespace sark{

	Mesh::Mesh(const Mesh&){}
	Mesh& Mesh::operator=(const Mesh&){
		return *this;
	}


	Mesh::Mesh(){}

	Mesh::~Mesh(){}

	// get reference of array buffer.
	// user can generate attribute and primitive buffers
	// and define how to draw data.
	ArrayBuffer& Mesh::GetArrayBuffer(){
		return mArrayBuf;
	}

	// draw array buffer. it binds buffers and draw primitives
	// if primitive buffer existed or just draw attribute array.
	// *note: this method is fully overridable.
	void Mesh::Draw() const{
		mArrayBuf.BindAttribBuffers();
		if (mArrayBuf.HasPrimitiveBuffer()){
			mArrayBuf.BindPrimitiveBuffer();
			mArrayBuf.DrawPrimitives();
			mArrayBuf.UnbindPrimitiveBuffer();
		}
		else{
			mArrayBuf.DrawArrays();
		}
		mArrayBuf.UnbindAttribBuffers();
	}

}
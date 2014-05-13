#ifndef __MESH_H__
#define __MESH_H__

#include "core.h"
#include "ArrayBuffer.h"

namespace sark{

	// mesh the data set of 3d model.
	class Mesh{
	protected:
		// array buffer of mesh.
		ArrayBuffer mArrayBuf;

	private:
		Mesh(const Mesh&);
		Mesh& operator=(const Mesh&);

	public:
		Mesh();

		virtual ~Mesh();

		// get reference of array buffer.
		// user can generate attribute and primitive buffers
		// and define how to draw data.
		ArrayBuffer& GetArrayBuffer();

		// draw array buffer. it binds buffers and draw primitives
		// if primitive buffer existed or just draw attribute array.
		// *note: this method is fully overridable.
		virtual void Draw() const;
	};

}
#endif
#include "RigidCube.h"

namespace sark{

	// create cube from given properties
	RigidCube::RigidCube(real width, real height, real depth){
		mWidth = width;
		mHeight = height;
		mDepth = depth;

		width /= 2.f;
		height /= 2.f;
		depth /= 2.f;
		Position3 pos[8] = {
			Position3(-width, -height, -depth),
			Position3(width, -height, -depth),
			Position3(width, -height, depth),
			Position3(-width, -height, depth),
			Position3(-width, height, -depth),
			Position3(width, height, -depth),
			Position3(width, height, depth),
			Position3(-width, height, depth),
		};
		Normal norms[8] = {
			Normal(-1, -1, -1),
			Normal(1, -1, -1),
			Normal(1, -1, 1),
			Normal(-1, -1, 1),
			Normal(-1, 1, -1),
			Normal(1, 1, -1),
			Normal(1, 1, 1),
			Normal(-1, 1, 1)
		};

		Mesh::Face faces[12] = {
			Mesh::Face(0,1,2),
			Mesh::Face(0,2,3),
			Mesh::Face(0,4,1),
			Mesh::Face(1,4,5),
			Mesh::Face(3,4,0),
			Mesh::Face(4,3,7),
			Mesh::Face(1,5,6),
			Mesh::Face(6,2,1),
			Mesh::Face(3,2,6),
			Mesh::Face(3,6,7),
			Mesh::Face(4,6,5),
			Mesh::Face(7,6,4)
		};

		mMesh.Create(
			std::vector<Position3>(std::begin(pos), std::end(pos)),
			std::vector<Normal>(std::begin(norms), std::end(norms)),
			std::vector<Mesh::Face>(std::begin(faces), std::end(faces)));
	}

	RigidCube::~RigidCube(){
	}

	void RigidCube::Update(){ }

	void RigidCube::Render(){
		mMesh.Bind();

		// TODO: change this gl-style matrix handling into user-shader style 
		glPushMatrix();
		glMultTransposeMatrixf(mTransform.GetMatrix().GetRawMatrix());
		mMesh.Draw();
		glPopMatrix();

		mMesh.Unbind();
	}

}
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

		mMesh.SetPositions({
			Position3(-width, -height, -depth),
			Position3(width, -height, -depth),
			Position3(width, -height, depth),
			Position3(-width, -height, depth),
			Position3(-width, height, -depth),
			Position3(width, height, -depth),
			Position3(width, height, depth),
			Position3(-width, height, depth),
		});
		mMesh.SetNormals({
			Normal(-1, -1, -1),
			Normal(1, -1, -1),
			Normal(1, -1, 1),
			Normal(-1, -1, 1),
			Normal(-1, 1, -1),
			Normal(1, 1, -1),
			Normal(1, 1, 1),
			Normal(-1, 1, 1)
		});
		mMesh.SetTexcoord0s({
			Texcoord(0, 1),
			Texcoord(1, 1),
			Texcoord(1, 0),
			Texcoord(0, 0),
			Texcoord(0, 1),
			Texcoord(1, 1),
			Texcoord(1, 0),
			Texcoord(0, 0)
		});

		mMesh.SetFaces({
			Mesh::Face(0, 1, 2),
			Mesh::Face(0, 2, 3),
			Mesh::Face(0, 4, 1),
			Mesh::Face(1, 4, 5),
			Mesh::Face(3, 4, 0),
			Mesh::Face(4, 3, 7),
			Mesh::Face(1, 5, 6),
			Mesh::Face(6, 2, 1),
			Mesh::Face(3, 2, 6),
			Mesh::Face(3, 6, 7),
			Mesh::Face(4, 6, 5),
			Mesh::Face(7, 6, 4)
		});

		mMesh.BindDatas();

		mOBox.Set(Position3(0, 0, 0), Vector4(1, 0, 0, width), Vector4(0, 1, 0, height), Vector4(0, 0, 1, depth), true);
	}

	RigidCube::~RigidCube(){
	}

	const IShape* RigidCube::GetBoundingShape() const{
		return &mUpdatedObox;
	}

	void RigidCube::Update(){
		mUpdatedObox.pos = mTransform.GetPosition();
		const Matrix4& mat = mTransform.GetMatrix();
		Matrix3 rotmat(mat.row[0].xyz, mat.row[1].xyz, mat.row[2].xyz);
		for (integer i = 0; i < 3; i++){
			mUpdatedObox.axis[i].xyz = rotmat * mOBox.axis[i].xyz;
			mUpdatedObox.axis[i].w = mOBox.axis[i].w;
		}
	}

	void RigidCube::Render(){
		mMesh.Draw();
	}

}
#include "RigidCube.h"
#include "TriangleMesh.h"
#include "primitives.hpp"

namespace sark{

	void RigidCube::CreateCube(){
		real width = mWidth / 2.f;
		real height = mHeight / 2.f;
		real depth = mDepth / 2.f;

		mMesh = new Mesh();
		ArrayBuffer& arrBuf = mMesh->GetArrayBuffer();

		arrBuf.GenAttributeBuffer<Position3>(
			ShaderProgram::ATTR_POSITION, {
			Position3(-width, -height, -depth),
			Position3(width, -height, -depth),
			Position3(width, -height, depth),
			Position3(-width, -height, depth),
			Position3(-width, height, -depth),
			Position3(width, height, -depth),
			Position3(width, height, depth),
			Position3(-width, height, depth)
		});
		arrBuf.GenAttributeBuffer<Normal>(
			ShaderProgram::ATTR_NORMAL, {
			Normal(-1, -1, -1),
			Normal(1, -1, -1),
			Normal(1, -1, 1),
			Normal(-1, -1, 1),
			Normal(-1, 1, -1),
			Normal(1, 1, -1),
			Normal(1, 1, 1),
			Normal(-1, 1, 1)
		});
		arrBuf.GenAttributeBuffer<Texcoord>(
			ShaderProgram::ATTR_TEX_COORD0, {
			Texcoord(0, 1),
			Texcoord(1, 1),
			Texcoord(1, 0),
			Texcoord(0, 0),
			Texcoord(0, 1),
			Texcoord(1, 1),
			Texcoord(1, 0),
			Texcoord(0, 0)
		});
		
		arrBuf.GenPrimitiveBuffer<TriangleFace16>({
			TriangleFace16(0, 1, 2),
			TriangleFace16(0, 2, 3),
			TriangleFace16(0, 4, 1),
			TriangleFace16(1, 4, 5),
			TriangleFace16(3, 4, 0),
			TriangleFace16(4, 3, 7),
			TriangleFace16(1, 5, 6),
			TriangleFace16(6, 2, 1),
			TriangleFace16(3, 2, 6),
			TriangleFace16(3, 6, 7),
			TriangleFace16(4, 6, 5),
			TriangleFace16(7, 6, 4)
		});
		arrBuf.SetDrawMode(ArrayBuffer::DrawMode::TRIANGLES);

		mOBox.Set(Position3(0, 0, 0), Vector4(1, 0, 0, width), Vector4(0, 1, 0, height), Vector4(0, 0, 1, depth), true);
	}

	// create cube from given properties
	RigidCube::RigidCube(real width, real height, real depth)
		: ASceneComponent("", NULL, true),
		mWidth(width), mHeight(height), mDepth(depth)
	{
		CreateCube();
	}

	// create cube from given properties
	RigidCube::RigidCube(const std::string& name, ASceneComponent* parent, bool activate,
		real width, real height, real depth)
		: ASceneComponent(name, parent, activate),
		mWidth(width), mHeight(height), mDepth(depth)
	{
		CreateCube();
	}

	RigidCube::~RigidCube(){
		if (mMesh != NULL)
			delete mMesh;
	}

	// get width of cube.
	const real& RigidCube::GetWidth() const{
		return mWidth;
	}
	// get height of cube.
	const real& RigidCube::GetHeight() const{
		return mHeight;
	}
	// get depth of cube.
	const real& RigidCube::GetDepth() const{
		return mDepth;
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
		mMesh->Draw();
	}

}
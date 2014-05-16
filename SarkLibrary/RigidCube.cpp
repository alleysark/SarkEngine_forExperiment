#include "RigidCube.h"
#include "primitives.hpp"

namespace sark{

	void RigidCube::CreateCube(){
		real width = mWidth / 2.f;
		real height = mHeight / 2.f;
		real depth = mDepth / 2.f;

		mMesh = new Mesh();
		ArrayBuffer& arrBuf = mMesh->GetArrayBuffer();

		arrBuf.GenAttributeBuffer<Position3>(
			AttributeSemantic::POSITION, {
			//top
			Position3(-width, height, -depth),
			Position3(-width, height, depth),
			Position3(width, height, depth),
			Position3(width, height, -depth),
			//front
			Position3(-width, height, depth),
			Position3(-width, -height, depth),
			Position3(width, -height, depth),
			Position3(width, height, depth),
			//left-side
			Position3(-width, height, -depth),
			Position3(-width, -height, -depth),
			Position3(-width, -height, depth),
			Position3(-width, height, depth),
			//right-side
			Position3(width, height, depth),
			Position3(width, -height, depth),
			Position3(width, -height, -depth),
			Position3(width, height, -depth),
			//back
			Position3(width, height, -depth),
			Position3(width, -height, -depth),
			Position3(-width, -height, -depth),
			Position3(-width, height, -depth),
			//bottom
			Position3(-width, -height, depth),
			Position3(-width, -height, -depth),
			Position3(width, -height, -depth),
			Position3(width, -height, depth)
		});
		arrBuf.GenAttributeBuffer<Normal>(
			AttributeSemantic::NORMAL, {
			Normal(0, 1, 0), Normal(0, 1, 0), Normal(0, 1, 0), Normal(0, 1, 0),
			Normal(0, 0, 1), Normal(0, 0, 1), Normal(0, 0, 1), Normal(0, 0, 1),
			Normal(-1, 0, 0), Normal(-1, 0, 0), Normal(-1, 0, 0), Normal(-1, 0, 0),
			Normal(1, 0, 0), Normal(1, 0, 0), Normal(1, 0, 0), Normal(1, 0, 0),
			Normal(0, 0, -1), Normal(0, 0, -1), Normal(0, 0, -1), Normal(0, 0, -1),
			Normal(0, -1, 0), Normal(0, -1, 0), Normal(0, -1, 0), Normal(0, -1, 0)
		});
		arrBuf.GenAttributeBuffer<Texcoord>(
			AttributeSemantic::TEXCOORD0, {
			Texcoord(0, 0), Texcoord(0, 1), Texcoord(1, 1), Texcoord(1, 0),
			Texcoord(0, 0), Texcoord(0, 1), Texcoord(1, 1), Texcoord(1, 0),
			Texcoord(0, 0), Texcoord(0, 1), Texcoord(1, 1), Texcoord(1, 0),
			Texcoord(0, 0), Texcoord(0, 1), Texcoord(1, 1), Texcoord(1, 0),
			Texcoord(0, 0), Texcoord(0, 1), Texcoord(1, 1), Texcoord(1, 0),
			Texcoord(0, 0), Texcoord(0, 1), Texcoord(1, 1), Texcoord(1, 0),
		});
		arrBuf.SetDrawMode(ArrayBuffer::DrawMode::QUADS);
	}

	// create cube from given properties
	RigidCube::RigidCube(real width, real height, real depth)
		: ASceneComponent("", NULL, true),
		mWidth(width), mHeight(height), mDepth(depth),
		mOBox(Vector3(0, 0, 0), Vector3(width/2.f, height/2.f, depth/2.f))
	{
		CreateCube();
	}

	// create cube from given properties
	RigidCube::RigidCube(const std::string& name, ASceneComponent* parent, bool activate,
		real width, real height, real depth)
		: ASceneComponent(name, parent, activate),
		mWidth(width), mHeight(height), mDepth(depth),
		mOBox(Vector3(0, 0, 0), Vector3(width / 2.f, height / 2.f, depth / 2.f))
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
		return &mOBox;
	}

	void RigidCube::Update(){
		mOBox.pos = mTransform.GetPosition();
		
		const Matrix4& mat = mTransform.GetMatrix();
		mOBox.rot.row[0] = mat.row[0].xyz;
		mOBox.rot.row[1] = mat.row[1].xyz;
		mOBox.rot.row[2] = mat.row[2].xyz;
		mOBox.rot.Transpose();
	}

	void RigidCube::Render(){
		mMesh->Draw();
	}

}
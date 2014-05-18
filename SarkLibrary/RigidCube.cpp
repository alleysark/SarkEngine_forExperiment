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
			Position3(-width, -height, -depth),
			Position3(width, -height, -depth),
			Position3(width, -height, depth),
			Position3(-width, -height, depth),

			Position3(-width, -height, -depth),
			Position3(-width, height, -depth),
			Position3(width, height, -depth),
			Position3(width, -height, -depth),

			Position3(width, -height, -depth),
			Position3(width, height, -depth),
			Position3(width, height, depth),
			Position3(width, -height, depth),

			Position3(width, -height, depth),
			Position3(width, height, depth),
			Position3(-width, height, depth),
			Position3(-width, -height, depth),

			Position3(-width, -height, depth),
			Position3(-width, height, depth),
			Position3(-width, height, -depth),
			Position3(-width, -height, -depth),

			Position3(-width, height, -depth),
			Position3(-width, height, depth),
			Position3(width, height, depth),
			Position3(width, height, -depth)
		});
		arrBuf.GenAttributeBuffer<Normal>(
			AttributeSemantic::NORMAL, {
			Normal(0, -1, 0), Normal(0, -1, 0), Normal(0, -1, 0), Normal(0, -1, 0),
			Normal(0, 0, -1), Normal(0, 0, -1), Normal(0, 0, -1), Normal(0, 0, -1),
			Normal(1, 0, 0), Normal(1, 0, 0), Normal(1, 0, 0), Normal(1, 0, 0),
			Normal(0, 0, 1), Normal(0, 0, 1), Normal(0, 0, 1), Normal(0, 0, 1),
			Normal(-1, 0, 0), Normal(-1, 0, 0), Normal(-1, 0, 0), Normal(-1, 0, 0),
			Normal(0, 1, 0), Normal(0, 1, 0), Normal(0, 1, 0), Normal(0, 1, 0)
		});
		arrBuf.GenAttributeBuffer<TriangleFace16>(
			AttributeSemantic::INDICES, {
			TriangleFace16(0, 1, 2), TriangleFace16(0, 2, 3),
			TriangleFace16(4, 5, 6), TriangleFace16(4, 6, 7),
			TriangleFace16(8, 9, 10), TriangleFace16(8, 10, 11),
			TriangleFace16(12, 13, 14), TriangleFace16(12, 14, 15),
			TriangleFace16(16, 17, 18), TriangleFace16(16, 18, 19),
			TriangleFace16(20, 21, 22), TriangleFace16(20, 22, 23)
		});
		arrBuf.SetDrawMode(ArrayBuffer::DrawMode::TRIANGLES);
	}

	// create cube from given properties
	RigidCube::RigidCube(real width, real height, real depth,
		real invMass,
		const Vector3& velocity, const Vector3& angularVelocity,
		bool gravityOn, bool fixed)
		: ASceneComponent("", NULL, true),
		mWidth(width), mHeight(height), mDepth(depth),
		mRigidBody(NULL), mOBox(NULL)
	{
		const real wsq = math::sqre(width);
		const real hsq = math::sqre(height);
		const real dsq = math::sqre(depth);
		mRigidBody = new RigidBody(this, invMass, Matrix3(
			invMass*12.f / (hsq + dsq), 0, 0,
			0, invMass*12.f / (wsq + dsq), 0,
			0, 0, invMass*12.f / (wsq + hsq)
			), velocity, angularVelocity, gravityOn, fixed);

		mOBox = new OrientedBox(Vector3(0), Vector3(width / 2.f, height / 2.f, depth / 2.f));

		CreateCube();
	}

	// create cube from given properties
	RigidCube::RigidCube(const std::string& name, ASceneComponent* parent, bool activate,
		real width, real height, real depth,
		real invMass,
		const Vector3& velocity, const Vector3& angularVelocity,
		bool gravityOn, bool fixed)
		: ASceneComponent(name, parent, activate),
		mWidth(width), mHeight(height), mDepth(depth),
		mRigidBody(NULL), mOBox(NULL)
	{
		const real wsq = math::sqre(width);
		const real hsq = math::sqre(height);
		const real dsq = math::sqre(depth);
		mRigidBody = new RigidBody(this, invMass, Matrix3(
			invMass*12.f / (hsq + dsq), 0, 0,
			0, invMass*12.f / (wsq + dsq), 0,
			0, 0, invMass*12.f / (wsq + hsq)
			), velocity, angularVelocity, gravityOn, fixed);

		mOBox = new OrientedBox(Vector3(0), Vector3(width / 2.f, height / 2.f, depth / 2.f));

		CreateCube();
	}

	RigidCube::~RigidCube(){
		if (mMesh != NULL)
			delete mMesh;
		if (mRigidBody != NULL)
			delete mRigidBody;
		if (mOBox != NULL)
			delete mOBox;
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
		return mOBox;
	}

	Mesh* RigidCube::GetMesh(){
		return mMesh;
	}

	RigidBody* RigidCube::GetRigidBody(){
		return mRigidBody;
	}

	void RigidCube::Update(){
		mOBox->pos = mTransform.GetPosition();
		
		const Matrix4& mat = mTransform.GetMatrix();
		mOBox->rot.row[0] = mat.row[0].xyz;
		mOBox->rot.row[1] = mat.row[1].xyz;
		mOBox->rot.row[2] = mat.row[2].xyz;
		mOBox->rot.Transpose();

		mRigidBody->Update();
	}

	void RigidCube::Render(){
		mMesh->Draw();
	}

}
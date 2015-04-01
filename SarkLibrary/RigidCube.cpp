#include "RigidCube.h"
#include "Mesh.h"
#include "RigidBody.h"
#include "ACollider.h"
#include "primitives.hpp"

namespace sark {

	void RigidCube::CreateCube() {
		real width = mWidth / 2.f;
		real height = mHeight / 2.f;
		real depth = mDepth / 2.f;

		ArrayBuffer& arrBuf = mMesh->GetArrayBuffer();
		arrBuf.GenAttributeBuffer<Position3>(
			AttributeSemantic::POSITION, {
			Position3(-width, -height, -depth),
			Position3(width, -height, -depth),
			Position3(width, -height, depth),
			Position3(-width, -height, depth),

			Position3(width, -height, -depth),
			Position3(-width, -height, -depth),
			Position3(-width, height, -depth),
			Position3(width, height, -depth),

			Position3(width, -height, depth),
			Position3(width, -height, -depth),
			Position3(width, height, -depth),
			Position3(width, height, depth),

			Position3(-width, -height, depth),
			Position3(width, -height, depth),
			Position3(width, height, depth),
			Position3(-width, height, depth),

			Position3(-width, -height, -depth),
			Position3(-width, -height, depth),
			Position3(-width, height, depth),
			Position3(-width, height, -depth),

			Position3(-width, height, depth),
			Position3(width, height, depth),
			Position3(width, height, -depth),
			Position3(-width, height, -depth)
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
		arrBuf.GenAttributeBuffer<Texcoord>(
			AttributeSemantic::TEXCOORD0, {
			Texcoord(0, 1), Texcoord(1, 1), Texcoord(1, 0), Texcoord(0, 0),
			Texcoord(0, 1), Texcoord(1, 1), Texcoord(1, 0), Texcoord(0, 0),
			Texcoord(0, 1), Texcoord(1, 1), Texcoord(1, 0), Texcoord(0, 0),
			Texcoord(0, 1), Texcoord(1, 1), Texcoord(1, 0), Texcoord(0, 0),
			Texcoord(0, 1), Texcoord(1, 1), Texcoord(1, 0), Texcoord(0, 0),
			Texcoord(0, 1), Texcoord(1, 1), Texcoord(1, 0), Texcoord(0, 0)
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
		bool gravityOn)
		: StaticModel("", NULL, true),
		mWidth(width), mHeight(height), mDepth(depth)
	{
		const real wsq = math::sqre(width);
		const real hsq = math::sqre(height);
		const real dsq = math::sqre(depth);
		mRigidBody = new RigidBody(this, invMass, Matrix3(
			invMass*12.f / (hsq + dsq), 0, 0,
			0, invMass*12.f / (wsq + dsq), 0,
			0, 0, invMass*12.f / (wsq + hsq)
			), velocity, angularVelocity, gravityOn);

		CreateCube();
	}

	// create cube from given properties
	RigidCube::RigidCube(const std::string& name, ASceneComponent* parent, bool activate,
		real width, real height, real depth,
		real invMass,
		const Vector3& velocity, const Vector3& angularVelocity,
		bool gravityOn)
		: StaticModel(name, parent, activate),
		mWidth(width), mHeight(height), mDepth(depth)
	{
		const real wsq = math::sqre(width);
		const real hsq = math::sqre(height);
		const real dsq = math::sqre(depth);
		mRigidBody = new RigidBody(this, invMass, Matrix3(
			invMass*12.f / (hsq + dsq), 0, 0,
			0, invMass*12.f / (wsq + dsq), 0,
			0, 0, invMass*12.f / (wsq + hsq)
			), velocity, angularVelocity, gravityOn);

		CreateCube();
	}

	RigidCube::~RigidCube() {
	}

	// get width of cube.
	const real& RigidCube::GetWidth() const {
		return mWidth;
	}
	// get height of cube.
	const real& RigidCube::GetHeight() const {
		return mHeight;
	}
	// get depth of cube.
	const real& RigidCube::GetDepth() const {
		return mDepth;
	}

}
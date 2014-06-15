#include "RigidSphere.h"
#include "Mesh.h"
#include "RigidBody.h"
#include "ACollider.h"
#include "primitives.hpp"

namespace sark{

	void RigidSphere::CreateSphere(){
		// idea by 'datenwolf'
		// (http://stackoverflow.com/questions/5988686/creating-a-3d-sphere-in-opengl-using-visual-c)
		
		std::vector<Position3> positions;
		std::vector<Normal> normals;
		std::vector<Texcoord> texcoords;
		std::vector<TriangleFace16> indices;
		
		real R = 1.f / (real)(mSlice - 1);
		real S = 1.f / (real)(mStack - 1);
		real PI_2 = math::PI / 2.f;
		uinteger r, s;

		for (r = 0; r < mSlice; r++){
			for (s = 0; s < mStack; s++) {
				real y = math::sin(-PI_2 + math::PI * r * R);
				real x = math::cos(2 * math::PI * s * S) * sin(math::PI * r * R);
				real z = math::sin(2 * math::PI * s * S) * sin(math::PI * r * R);

				texcoords.push_back({ s*S, (1 - r*R) });

				positions.push_back({ x * mRadius, y * mRadius, z * mRadius });

				normals.push_back({ x, y, z });
			}
		}

		for (r = 0; r < mSlice - 1; r++){
			for (s = 0; s < mStack - 1; s++){
				uint16 v0 = (uint16)(r * mStack + s);
				uint16 v1 = (uint16)(r * mStack + (s + 1));
				uint16 v2 = (uint16)((r + 1) * mStack + (s + 1));
				uint16 v3 = (uint16)((r + 1) * mStack + s);

				indices.push_back({ v0, v2, v1 });
				indices.push_back({ v0, v3, v2 });
			}
		}

		ArrayBuffer& arrBuf = mMesh->GetArrayBuffer();
		
		arrBuf.GenAttributeBuffer<Position3>(
			AttributeSemantic::POSITION, positions);
		arrBuf.GenAttributeBuffer<Normal>(
			AttributeSemantic::NORMAL, normals);
		arrBuf.GenAttributeBuffer<Texcoord>(
			AttributeSemantic::TEXCOORD0, texcoords);

		arrBuf.GenAttributeBuffer<TriangleFace16>(
			AttributeSemantic::INDICES, indices);

		arrBuf.SetDrawMode(ArrayBuffer::DrawMode::TRIANGLES);
	}

	// create sphere from given properties
	RigidSphere::RigidSphere(real radius, uinteger slice, uinteger stack,
		real invMass,
		const Vector3& velocity, const Vector3& angularVelocity,
		bool gravityOn)
		: StaticModel("", NULL, true),
		mRadius(radius), mSlice(slice), mStack(stack)
	{
		// compute invI0
		const real e = invMass * 5.f / 2.f*math::sqre(radius);
		mRigidBody = new RigidBody(this, invMass, Matrix3(
			e, 0, 0,
			0, e, 0,
			0, 0, e), velocity, angularVelocity, gravityOn);

		CreateSphere();
	}

	// create sphere from given properties
	RigidSphere::RigidSphere(const std::string& name, ASceneComponent* parent, bool activate,
		real radius, uinteger slice, uinteger stack,
		real invMass,
		const Vector3& velocity, const Vector3& angularVelocity,
		bool gravityOn)
		: StaticModel(name, parent, activate),
		mRadius(radius), mSlice(slice), mStack(stack)
	{
		// compute invI0
		const real e = invMass * 5.f / 2.f*math::sqre(radius);
		mRigidBody = new RigidBody(this, invMass, Matrix3(
			e, 0, 0,
			0, e, 0,
			0, 0, e), velocity, angularVelocity, gravityOn);

		CreateSphere();
	}

	RigidSphere::~RigidSphere(){
		if (mMesh != NULL)
			delete mMesh;
		if (mRigidBody != NULL)
			delete mRigidBody;
		if (mCollider != NULL)
			delete mCollider;
	}

	// get radius of sphere.
	const real& RigidSphere::GetRadius() const{
		return mRadius;
	}

	// get slice count.
	const uinteger& RigidSphere::GetSliceCount() const{
		return mSlice;
	}

	// get stack count.
	const uinteger& RigidSphere::GetStackCount() const{
		return mStack;
	}

}
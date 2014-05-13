#include "RigidSphere.h"
#include "TriangleMesh.h"
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

				texcoords.push_back({ s*S, r*R });

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

		mMesh = new Mesh();
		ArrayBuffer& arrBuf = mMesh->GetArrayBuffer();
		
		arrBuf.GenAttributeBuffer<Position3>(
			ShaderProgram::ATTR_POSITION, positions);
		arrBuf.GenAttributeBuffer<Normal>(
			ShaderProgram::ATTR_NORMAL, normals);
		arrBuf.GenAttributeBuffer<Texcoord>(
			ShaderProgram::ATTR_TEX_COORD0, texcoords);

		arrBuf.GenPrimitiveBuffer<TriangleFace16>(indices);
		arrBuf.SetDrawMode(ArrayBuffer::DrawMode::TRIANGLES);

		mSphere.Set(Position3(0, 0, 0), mRadius);
	}

	// create sphere from given properties
	RigidSphere::RigidSphere(real radius, uinteger slice, uinteger stack)
		: ASceneComponent("", NULL, true),
		mRadius(radius), mSlice(slice), mStack(stack)
	{
		CreateSphere();
	}

	// create sphere from given properties
	RigidSphere::RigidSphere(const std::string& name, ASceneComponent* parent, bool activate,
		real radius, uinteger slice, uinteger stack)
		: ASceneComponent(name, parent, activate),
		mRadius(radius), mSlice(slice), mStack(stack)
	{
		CreateSphere();
	}

	RigidSphere::~RigidSphere(){
		if (mMesh != NULL)
			delete mMesh;
	}

	// get radius of sphere.
	const real& RigidSphere::GetRadius() const{
		return mRadius;
	}

	const IShape* RigidSphere::GetBoundingShape() const{
		return &mSphere;
	}

	void RigidSphere::Update(){
		mSphere.pos = mTransform.GetPosition();
	}

	void RigidSphere::Render(){
		mMesh->Draw();
	}

}
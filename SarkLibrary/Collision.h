#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "core.h"
#include "ArrayBuffer.h"
#include "AScene.h"

namespace sark{

	// collision detector and resolver in here.
	// at present, it supports only for the indexed triangle mesh
	// and it does not resolve the 'resting state'.
	class Collision{
	public:
		// it detects and resolves the collisions
		// of the rigid bodies in a given scene layer.
		static void DetectAndResolve(AScene::Layer& physLayer);

	private:
		typedef ArrayBuffer::AttributeAccessor<Position3> PositionAccessor;
		typedef ArrayBuffer::AttributeAccessor<TriangleFace16> IndexAccessor;

		static bool Detect(const PositionAccessor& positions1, const IndexAccessor& indices1, const Matrix4& TM1,
			const PositionAccessor& positions2, const IndexAccessor& indices2, const Matrix4& TM2,
			Vector3& out_CN, Vector3& out_CP);

		static void Resolve(RigidBody* rigidBody1, RigidBody* rigidBody2, const Vector3& n, const Vector3& CP);
	};

}
#endif
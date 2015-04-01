#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "core.h"
#include "ArrayBuffer.h"
#include "AScene.h"

namespace sark {

	class ConvexHull;

	// collision detector and resolver in here.
	// at present, it supports only for the indexed triangle mesh
	// and it does not resolve the 'resting state'.
	class Collision {
	public:
		// coefficient of restitution.
		// it'll be deprecated soon.
		static real C_RESTITUT;

	public:
		// process the collisions.
		// it checks the bounding volume intersections on broad-phase
		// and then test mesh-level collisions to generate collision
		// datas on narrow-phase.
		// if there are collisions, it'll resolve them
		static void ProcessCollision(AScene::Layer& physLayer);

		// process the collisions about convexity objects.
		// it assumes that the scene components in given layer have
		// those own convex-hull as collider.
		static void ProcessConvexCollision(AScene::Layer& physLayer);

	public:
		typedef ArrayBuffer::AttributeAccessor<Position3> PositionAccessor;
		typedef ArrayBuffer::AttributeAccessor<TriangleFace16> IndexAccessor;

		// triangle-mesh level detection.
		static bool MeshLevelDetection(const PositionAccessor& positions1, const IndexAccessor& indices1, const Matrix4& TM1,
			const PositionAccessor& positions2, const IndexAccessor& indices2, const Matrix4& TM2,
			Vector3& out_CN, Vector3& out_CP);

		// convex level detection.
		static bool ConvexLevelDetection(const ConvexHull* convex1, const ConvexHull* convex2,
			Vector3& out_CN, Vector3& out_CP, real& out_depth);

		static void Resolve(RigidBody* rigidBody1, RigidBody* rigidBody2,
			const Vector3& CN, const Vector3& CP, const real depth);
	};

}
#endif
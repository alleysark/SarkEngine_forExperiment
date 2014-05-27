#include "Collision.h"
#include "GJK_EPA.h"
#include "ConvexHull.h"
#include "RigidBody.h"
#include "Mesh.h"
#include "tools.h"
#include "Debug.h"

namespace sark{

	// process the collisions.
	void Collision::ProcessCollision(AScene::Layer& physLayer){
		// temporal contact informations
		Vector3 CN; // contact normal
		Vector3 CP; // contact point

		AScene::Layer::ReplicaArrayIterator itr = physLayer.Begin();
		AScene::Layer::ReplicaArrayIterator jtr;
		AScene::Layer::ReplicaArrayIterator end = physLayer.End();
		for (; itr != end; itr++){
			if ((*itr)->GetRigidBody() == NULL
				|| (*itr)->GetMesh() == NULL)
				continue;

			ArrayBuffer& arrbuf1 = (*itr)->GetMesh()->GetArrayBuffer();
			if (arrbuf1.GetDrawMode() != ArrayBuffer::DrawMode::TRIANGLES){
				LogWarn("it supports only for the triangle mesh");
				continue;
			}

			if (arrbuf1.GetDataCount(AttributeSemantic::INDICES) == 0){
				LogWarn("it does not support indexless mesh");
				continue;
			}

			PositionAccessor positions1
				= arrbuf1.GetAttributeAccessor<Position3>(AttributeSemantic::POSITION);
			if (positions1.Empty())
				continue;

			IndexAccessor indices1
				= arrbuf1.GetAttributeAccessor<TriangleFace16>(AttributeSemantic::INDICES);
			if (indices1.Empty())
				continue;

			const ACollider* coll1 = (*itr)->GetCollider();

			// compare component 1 and component 2.
			for (jtr = itr + 1; jtr != end; jtr++){
				if ((*jtr)->GetRigidBody() == NULL
					|| (*jtr)->GetMesh() == NULL)
					continue;

				ArrayBuffer& arrbuf2 = (*jtr)->GetMesh()->GetArrayBuffer();
				if (arrbuf2.GetDrawMode() != ArrayBuffer::DrawMode::TRIANGLES){
					LogWarn("it supports only for the triangle mesh");
					continue;
				}

				if (arrbuf2.GetDataCount(AttributeSemantic::INDICES) == 0){
					LogWarn("it does not support indexless mesh");
					continue;
				}

				// broad phase.
				if (coll1 != NULL){
					const ACollider* coll2 = (*jtr)->GetCollider();
					if (coll2 != NULL){
						if (coll1->IntersectWith(coll2) == false)
							continue;
					}
				}

				// narrow phase.
				PositionAccessor positions2
					= arrbuf2.GetAttributeAccessor<Position3>(AttributeSemantic::POSITION);
				if (positions2.Empty())
					continue;

				IndexAccessor indices2
					= arrbuf2.GetAttributeAccessor<TriangleFace16>(AttributeSemantic::INDICES);
				if (indices2.Empty())
					continue;

				CN = 0.f;
				CP = 0.f;

				if (MeshLevelDetection(positions1, indices1, (*itr)->GetTransform().GetMatrix(),
					positions2, indices2, (*jtr)->GetTransform().GetMatrix(), CN, CP))
				{
					Resolve((*itr)->GetRigidBody(), (*jtr)->GetRigidBody(), CN, CP, 0);
				}
			}
		}
	}

	// process the collisions about convexity objects.
	void Collision::ProcessConvexCollision(AScene::Layer& physLayer){
		// temporal contact informations
		Vector3 CN; // contact normal
		Vector3 CP; // contact point
		real depth; // contact depth

		AScene::Layer::ReplicaArrayIterator itr = physLayer.Begin();
		AScene::Layer::ReplicaArrayIterator jtr;
		AScene::Layer::ReplicaArrayIterator end = physLayer.End();
		for (; itr != end; itr++){
			if ((*itr)->GetRigidBody() == NULL
				|| (*itr)->GetCollider() == NULL
				|| (*itr)->GetCollider()->GetType() != ACollider::CONVEXHULL)
				continue;

			const ConvexHull* convex1 = reinterpret_cast<const ConvexHull*>((*itr)->GetCollider());

			// compare component 1 and component 2.
			for (jtr = itr + 1; jtr != end; jtr++){
				if ((*jtr)->GetRigidBody() == NULL
					|| (*jtr)->GetCollider() == NULL
					|| (*jtr)->GetCollider()->GetType() != ACollider::CONVEXHULL)
					continue;

				const ConvexHull* convex2 = reinterpret_cast<const ConvexHull*>((*jtr)->GetCollider());

				if (ConvexLevelDetection(convex1, convex2, CN, CP, depth)){
					// *note: at this time, i just translate depth toward contact normal.
					// but it should be modified as correction impulse based method.
					(*itr)->GetTransform().TranslateMore(CN*depth);

					Resolve((*itr)->GetRigidBody(), (*jtr)->GetRigidBody(), CN, CP, depth);
				}
			}
		}
	}


	bool Collision::MeshLevelDetection(const PositionAccessor& positions1, const IndexAccessor& indices1, const Matrix4& TM1,
		const PositionAccessor& positions2, const IndexAccessor& indices2, const Matrix4& TM2,
		Vector3& out_CN, Vector3& out_CP)
	{
		uinteger count_1 = indices1.Count();
		uinteger count_2 = indices2.Count();
		Vector3 P, Q, n;
		uinteger contactCount = 0;

		Position3 transA2, transB2, transC2;

		// triangle mesh - triangle mesh intersection test
		for (uinteger i = 0; i < count_1; i++){
			const Position3& A1 = positions1[indices1[i].a];
			const Position3& B1 = positions1[indices1[i].b];
			const Position3& C1 = positions1[indices1[i].c];

			for (uinteger j = 0; j < count_2; j++){
				const Position3& A2 = positions2[indices2[j].a];
				const Position3& B2 = positions2[indices2[j].b];
				const Position3& C2 = positions2[indices2[j].c];

				transA2 = (TM2 * Vector4(A2, 1.f)).xyz;
				transB2 = (TM2 * Vector4(B2, 1.f)).xyz;
				transC2 = (TM2 * Vector4(C2, 1.f)).xyz;

				if (tool::Triangle_TriangleIntersection(
					(TM1 * Vector4(A1, 1.f)).xyz,
					(TM1 * Vector4(B1, 1.f)).xyz,
					(TM1 * Vector4(C1, 1.f)).xyz,
					transA2, transB2, transC2,
					&P, &Q, &n) == true)
				{
					out_CN += n;
					out_CP += (P + Q) / 2.f;
					contactCount++;
				}
			}
		}

		if (contactCount == 0)
			return false;

		out_CN /= (real)contactCount;
		out_CN.Normalize();
		out_CP /= (real)contactCount;
		return true;
	}

	// convex level detection.
	bool Collision::ConvexLevelDetection(const ConvexHull* convex1, const ConvexHull* convex2,
		Vector3& out_CN, Vector3& out_CP, real& out_depth)
	{
		GJK_EPA::Simplex simplex;
		if (!GJK_EPA::DoGJK(convex1, convex2, &simplex))
			return false;

		if (!GJK_EPA::DoEPA(convex1, convex2, simplex, &out_CN, &out_depth))
			return false;

		out_CP = tool::FarthestPointInDirection(convex1->GetTransPointSet(), out_CN);
		out_CN = -out_CN;
		return true;
	}

	void Collision::Resolve(RigidBody* rigidBody1, RigidBody* rigidBody2,
		const Vector3& CN, const Vector3& CP, const real depth)
	{
		const Vector3& v1 = rigidBody1->GetVelocity();
		const Vector3& w1 = rigidBody1->GetAngularVelocity();

		const Vector3& v2 = rigidBody2->GetVelocity();
		const Vector3& w2 = rigidBody2->GetAngularVelocity();

		Vector3 r1 = CP - rigidBody1->GetCM();
		Vector3 r2 = CP - rigidBody2->GetCM();

		// sвл(t); velocity of point s(t)
		// c = n.Dot(s1вл - s2вл)
		// sвл(t) = v(t) + w(t)xr(t)
		real c = CN.Dot((v1 + w1.Cross(r1)) - (v2 + w2.Cross(r2)));
		if (c > 0)
			return;
		else if (c == 0){
			// resting state
			return;
		}
		// else, it needs to be resolved.

		const real& invM1 = rigidBody1->GetInvMass();
		const real& invM2 = rigidBody2->GetInvMass();
		Matrix3 invI1 = rigidBody1->GetInvInertiaTensor();
		Matrix3 invI2 = rigidBody2->GetInvInertiaTensor();

		// *note: coefficient of restitution should be
		// computed with consideration of materials.
		const real restitution = 0.1f;

		real j = -(1 + restitution)*c 
			/ (CN.Dot((invI1 * r1.Cross(CN)).Cross(r1)) + CN.Dot((invI2*r2.Cross(CN)).Cross(r2)) + invM1 + invM2);
		j = math::max(j, 0);

		Vector3 J = j*CN;

		//v'(t) = v(t) + J/M
		rigidBody1->SetVelocity(v1 + J*invM1);
		rigidBody2->SetVelocity(v2 + -J*invM2);

		//е°'(t) = е°(t) + invI*(r(t)xJ)
		rigidBody1->SetAngularVelocity(w1 + invI1 * (r1.Cross(J)));
		rigidBody2->SetAngularVelocity(w2 + invI2 * (r2.Cross(-J)));
	}

}
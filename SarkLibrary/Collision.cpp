#include "Collision.h"
#include "RigidBody.h"
#include "Mesh.h"
#include "Debug.h"

namespace sark{

	void Collision::DetectAndResolve(AScene::Layer& physLayer){
		// temporal contact informations
		Vector3 CN; // contact normal
		Vector3 CP; // contact point

		AScene::Layer::ReplicaArrayIterator itr = physLayer.Begin();
		AScene::Layer::ReplicaArrayIterator jtr;
		AScene::Layer::ReplicaArrayIterator end = physLayer.End();
		for (; itr != end; itr++){
			if ((*itr)->GetRigidBody() == NULL)
				continue;
			if ((*itr)->GetMesh() == NULL)
				continue;

			ArrayBuffer& arrbuf_1 = (*itr)->GetMesh()->GetArrayBuffer();
			if (arrbuf_1.GetDrawMode() != ArrayBuffer::DrawMode::TRIANGLES){
				LogWarn("it supports only for the triangle mesh");
				continue;
			}

			if (arrbuf_1.GetDataCount(AttributeSemantic::INDICES) == 0){
				LogWarn("it does not support indexless mesh");
				continue;
			}

			PositionAccessor positions_1
				= arrbuf_1.GetAttributeAccessor<Position3>(AttributeSemantic::POSITION);
			if (positions_1.Empty())
				continue;

			IndexAccessor indices_1
				= arrbuf_1.GetAttributeAccessor<TriangleFace16>(AttributeSemantic::INDICES);
			if (indices_1.Empty())
				continue;

			// compare component 1 and component 2.
			for (jtr = itr + 1; jtr != end; jtr++){
				if ((*jtr)->GetRigidBody() == NULL)
					continue;
				if ((*jtr)->GetMesh() == NULL)
					continue;

				ArrayBuffer& arrbuf_2 = (*jtr)->GetMesh()->GetArrayBuffer();
				if (arrbuf_2.GetDrawMode() != ArrayBuffer::DrawMode::TRIANGLES){
					LogWarn("it supports only for the triangle mesh");
					continue;
				}

				if (arrbuf_2.GetDataCount(AttributeSemantic::INDICES) == 0){
					LogWarn("it does not support indexless mesh");
					continue;
				}

				PositionAccessor positions_2
					= arrbuf_2.GetAttributeAccessor<Position3>(AttributeSemantic::POSITION);
				if (positions_2.Empty())
					continue;

				IndexAccessor indices_2
					= arrbuf_2.GetAttributeAccessor<TriangleFace16>(AttributeSemantic::INDICES);
				if (indices_2.Empty())
					continue;

				CN = 0.f;
				CP = 0.f;

				if (Detect(positions_1, indices_1, (*itr)->GetTransform().GetMatrix(),
					positions_2, indices_2, (*jtr)->GetTransform().GetMatrix(), CN, CP))
				{
					Resolve((*itr)->GetRigidBody(), (*jtr)->GetRigidBody(), CN, CP);
				}
			}
		}
	}

	bool Collision::Detect(const PositionAccessor& positions1, const IndexAccessor& indices1, const Matrix4& TM1,
		const PositionAccessor& positions2, const IndexAccessor& indices2, const Matrix4& TM2,
		Vector3& out_CN, Vector3& out_CP)
	{
		uinteger count_1 = indices1.Count();
		uinteger count_2 = indices2.Count();
		Vector3 P, Q;
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

				if (Triangle_TriangleIntersection(
					(TM1 * Vector4(A1, 1.f)).xyz,
					(TM1 * Vector4(B1, 1.f)).xyz,
					(TM1 * Vector4(C1, 1.f)).xyz,
					transA2, transB2, transC2,
					&P, &Q) == true)
				{
					out_CN += ((transB2 - transA2).Cross(transC2 - transA2)).Normal();
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

	void Collision::Resolve(RigidBody* rigidBody1, RigidBody* rigidBody2, const Vector3& n, const Vector3& CP){
		const Vector3& v1 = rigidBody1->GetVelocity();
		const Vector3& w1 = rigidBody1->GetAngularVelocity();

		const Vector3& v2 = rigidBody2->GetVelocity();
		const Vector3& w2 = rigidBody2->GetAngularVelocity();

		Vector3 r1 = CP - rigidBody1->GetCM();
		Vector3 r2 = CP - rigidBody2->GetCM();

		// sвл(t); velocity of point s(t)
		// c = n.Dot(s1вл - s2вл)
		// sвл(t) = v(t) + w(t)xr(t)
		real c = n.Dot((v1 + w1.Cross(r1)) - (v2 + w2.Cross(r2)));
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


		const real restitute = 1.f;
		real j = -(1 + restitute)*c / (n.Dot((invI1 * r1.Cross(n)).Cross(r1)) + n.Dot((invI2*r2.Cross(n)).Cross(r2)) + invM1 + invM2);

		Vector3 J = j*n;

		//v'(t) = v(t) + J/M
		rigidBody1->SetVelocity(v1 + J*invM1);
		rigidBody2->SetVelocity(v2 + -J*invM2);

		//е°'(t) = е°(t) + invI*(r(t)xJ)
		rigidBody1->SetAngularVelocity(w1 + invI1 * (r1.Cross(J)));
		rigidBody2->SetAngularVelocity(w2 + invI2 * (r2.Cross(-J)));
	}

}
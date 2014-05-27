#include "ConvexHull.h"
#include "ASceneComponent.h"
#include "GJK_EPA.h"

namespace sark{

	// convex hull
	ConvexHull::ConvexHull(ASceneComponent* reference)
		: ACollider(reference){}

	ConvexHull::ConvexHull(ASceneComponent* reference,
		const PointSet& points)
		: ACollider(reference),
		mPoints(points), mTransPoints(points){}

	ConvexHull::ConvexHull(ASceneComponent* reference,
		const PointSet& points,
		const FaceSet& faces)
		: ACollider(reference),
		mPoints(points), mTransPoints(points), mFaces(faces){}

	ConvexHull::~ConvexHull(){}

	// get original(not transformed) point set.
	const ConvexHull::PointSet& ConvexHull::GetPointSet() const{
		return mPoints;
	}

	// get transformed point set.
	const ConvexHull::PointSet& ConvexHull::GetTransPointSet() const{
		return mTransPoints;
	}

	// get triangle face set.
	const ConvexHull::FaceSet& ConvexHull::GetFaceSet() const{
		return mFaces;
	}

	// get type
	const ACollider::Type ConvexHull::GetType() const{
		return ACollider::CONVEXHULL;
	}

	// intersection test with given shape.
	// *note: it does not generate any collision
	// informations then just test the intersection.
	bool ConvexHull::IntersectWith(const ACollider* coll) const{
		switch (coll->GetType()){
		case ACollider::SPHERE:
			break;
		case ACollider::AABOX:
			break;
		case ACollider::OBOX:
			break;
		case ACollider::CONVEXHULL:
			return GJK_EPA::DoGJK(this, reinterpret_cast<const ConvexHull*>(coll), NULL);
		}
		return false;
	}

	// update points through given transform matrix.
	// transMat is usually world transform.
	void ConvexHull::Update(){
		const Matrix4& transMat = mReference->GetTransform().GetMatrix();
		uinteger sz = mPoints.size();
		for (uinteger i = 0; i < sz; i++){
			mTransPoints[i] = (transMat * Vector4(mPoints[i], 1.f)).xyz;
		}
	}

}
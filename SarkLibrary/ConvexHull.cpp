#include "ConvexHull.h"
#include "GJK_EPA.h"

namespace sark{

	// convex hull
	ConvexHull::ConvexHull(){}

	ConvexHull::ConvexHull(const std::vector<Vector3>& points)
		: mPoints(points), mTransPoints(points){}

	ConvexHull::ConvexHull(const std::vector<Vector3>& points,
		const std::vector<TriangleFace16>& faces)
		: mPoints(points), mTransPoints(points), mFaces(faces)
	{}

	ConvexHull::~ConvexHull(){}

	// get original(not transformed) point set.
	const std::vector<Vector3>& ConvexHull::GetPointSet() const{
		return mPoints;
	}

	// get transformed point set.
	const std::vector<Vector3>& ConvexHull::GetTransPointSet() const{
		return mTransPoints;
	}

	// get triangle face set.
	const std::vector<TriangleFace16>& ConvexHull::GetFaceSet() const{
		return mFaces;
	}

	// get this shape type
	IShape::Type ConvexHull::GetType() const{
		return IShape::CONVEXHULL;
	}

	// intersection test with given shape.
	// *note: it does not generate any collision
	// informations then just test the intersection.
	bool ConvexHull::IntersectWith(const IShape* shapeB) const{
		switch (shapeB->GetType()){
		case IShape::RAY:
			break;
		case IShape::PLANE:
			break;
		case IShape::SPHERE:
			break;
		case IShape::AABOX:
			break;
		case IShape::OBOX:
			break;
		case IShape::CONVEXHULL:
			return GJK_EPA::DoGJK(this, reinterpret_cast<const ConvexHull*>(shapeB), NULL);
		}
		return false;
	}

	// update points through given transform matrix.
	// transMat is usually world transform.
	void ConvexHull::Update(const Matrix4& transMat){
		uinteger sz = mPoints.size();
		for (uinteger i = 0; i < sz; i++){
			mTransPoints[i] = (transMat * Vector4(mPoints[i], 1.f)).xyz;
		}
	}

}
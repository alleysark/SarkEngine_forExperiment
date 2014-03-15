#include "shapes.h"
#include <algorithm>

namespace sark{

	IShape::IShape(Type type){
		mType = type;
	}

	IShape::~IShape(){}

	IShape::Type IShape::GetType() const{
		return mType;
	}

	IShape::ExtIntersectionChecker IShape::extChecker = NULL;
	void IShape::SetExtendedIntersectionChecker(IShape::ExtIntersectionChecker checker){
		extChecker = checker;
	}

	// intersection check functions of basic shapes
	// referenced documents
	// * http://paulbourke.net/geometry/pointlinetriSlab/

	bool IShape::CheckIntersection(Line* line1, Line* line2, Position3* interPosBuf){
		Vector3& p1 = line1->pos;
		Vector3 p2 = line1->pos + line1->dir;
		Vector3& p3 = line2->pos;
		Vector3 p4 = line2->pos + line2->dir;

		#define _d(m,n,o,p) ((m.x-n.x)*(o.x-p.x) + (m.y-n.y)*(o.y-p.y) + (m.z-n.z)*(o.z-p.z))
		real d1343 = _d(p1, p3, p4, p3);
		real d4321 = _d(p4, p3, p2, p1);
		real d1321 = _d(p1, p3, p2, p1);
		real d4343 = _d(p4, p3, p4, p3);
		real d2121 = _d(p2, p1, p2, p1);
		#undef _d

		real s = (d1343*d4321 - d1321*d4343) / (d2121*d4343 - d4321*d4321);
		if (s < line1->minp || line1->maxp < s)
			return false;

		real t = (d1343 + s*d4321) / d4343;
		if (t < line2->minp || line2->maxp < t)
			return false;

		Vector3 pa = line1->pos + s*line1->dir;
		Vector3 pb = line2->pos + t*line2->dir;
		Vector3 h = pb - pa;
		real lenSq = h.MagnitudeSq();

		if (math::apprx_equal(lenSq, 0.0f)){
			if (interPosBuf != NULL)
				*interPosBuf = pa;
			return true;
		}
		return false;
	}
	bool IShape::CheckIntersection(Line* line, Sphere* sphere, Position3* interPosBuf){
		real a = line->dir.Dot(line->dir);
		real b = 2 * (line->pos - sphere->pos).Dot(line->dir);
		real c = line->pos.Dot(line->pos) + sphere->pos.Dot(sphere->pos) - sphere->pos.Dot(line->pos) - math::sqre(sphere->r);
		
		real det = math::sqre(b) - 4 * a*c;
		real t = 0.f;

		if (det < 0){
			return false;
		}
		else if (det == 0){
			t = -b / 2 * a;
			if (t < line->minp || line->maxp < t)
				return false;
		}
		else if (det > 0){
			det = math::sqrt(det);
			t = -b / 2 * a - det;
			if (t < line->minp || line->maxp < t){
				t = -b / 2 * a + det;
				if (t < line->minp || line->maxp < t){
					return false;
				}
			}
			else{
				real t2 = -b / 2 * a + det;
				if (line->minp <= t2 && t2 <= line->maxp){
					t = t + t2 / 2.0f;
				}
			}
		}
		if (interPosBuf != NULL){
			*interPosBuf = line->pos + t*line->dir;
		}
		return true;
	}
	bool IShape::CheckIntersection(Line* line, AxisAlignedBox* aabox, Position3* interPosBuf){
		return false;
	}
	bool IShape::CheckIntersection(Line* line, OrientedBox* obox, Position3* interPosBuf){
		return false;
	}
	bool IShape::CheckIntersection(Line* line, Polygon* poly, Position3* interPosBuf){
		return false;
	}

	bool IShape::CheckIntersection(Sphere* sphere1, Sphere* sphere2, Position3* interPosBuf){
		real dist = (sphere1->pos - sphere2->pos).Magnitude();
		if (dist <= sphere1->r + sphere2->r)
			return true;
		return false;
	}
	bool IShape::CheckIntersection(Sphere* sphere, AxisAlignedBox* aabox, Position3* interPosBuf){ return false; }
	bool IShape::CheckIntersection(Sphere* sphere, OrientedBox* obox, Position3* interPosBuf){ return false; }
	bool IShape::CheckIntersection(Sphere* sphere, Polygon* poly, Position3* interPosBuf){ return false; }

	bool IShape::CheckIntersection(AxisAlignedBox* aabox1, AxisAlignedBox* aabox2, Position3* interPosBuf){ return false; }
	bool IShape::CheckIntersection(AxisAlignedBox* aabox, OrientedBox* obox, Position3* interPosBuf){ return false; }
	bool IShape::CheckIntersection(AxisAlignedBox* aabox, Polygon* poly, Position3* interPosBuf){ return false; }

	bool IShape::CheckIntersection(OrientedBox* obox1, OrientedBox* obox2, Position3* interPosBuf){ return false; }
	bool IShape::CheckIntersection(OrientedBox* obox, Polygon* poly, Position3* interPosBuf){ return false; }

	bool IShape::CheckIntersection(Polygon* poly1, Polygon* poly2, Position3* interPosBuf){ return false; }

	// helpful intersection check functions
	bool IShape::_RangeIntersect(real fromA, real toA, real fromB, real toB){
		if (fromA > toA){
			std::swap<real>(fromA, toA);
		}
		if (fromB > toB){
			std::swap<real>(fromB, toB);
		}

		if (toA < fromB){
			return false;
		}
		if (toB < fromA){
			return false;
		}
		return true;
	}






	// ======================================================
	//			implemented shapes below..
	// ======================================================

	// line
	Line::Line(const Point3& position, const Vector3& direction, real minParam, real maxParam)
		: IShape(IShape::LINE){
		pos = position;
		dir = direction.Normal();
		minp = minParam;
		maxp = maxParam;
	}

	bool Line::Intersect(IShape* shape, Position3* interPosBuf){
		switch (shape->GetType()){
		case IShape::LINE:
			return CheckIntersection(this, dynamic_cast<Line*>(shape), interPosBuf);
			break;
		case IShape::SPHERE:
			return CheckIntersection(this, dynamic_cast<Sphere*>(shape), interPosBuf);
			break;
		case IShape::AABOX:
			return CheckIntersection(this, dynamic_cast<AxisAlignedBox*>(shape), interPosBuf);
			break;
		case IShape::OBOX:
			return CheckIntersection(this, dynamic_cast<OrientedBox*>(shape), interPosBuf);
			break;
		case IShape::POLY:
			return CheckIntersection(this, dynamic_cast<Polygon*>(shape), interPosBuf);
			break;
		default:
			if (extChecker != NULL)
				return extChecker(this, shape, interPosBuf);
			return false;
		}
	}


	// sphere
	Sphere::Sphere(const Point3& position, real radius)
		: IShape(IShape::SPHERE){
		pos = position;
		r = radius;
	}

	bool Sphere::Intersect(IShape* shape, Position3* interPosBuf){
		switch (shape->GetType()){
		case IShape::LINE:
			return CheckIntersection(dynamic_cast<Line*>(shape), this, interPosBuf);
			break;
		case IShape::SPHERE:
			return CheckIntersection(this, dynamic_cast<Sphere*>(shape), interPosBuf);
			break;
		case IShape::AABOX:
			return CheckIntersection(this, dynamic_cast<AxisAlignedBox*>(shape), interPosBuf);
			break;
		case IShape::OBOX:
			return CheckIntersection(this, dynamic_cast<OrientedBox*>(shape), interPosBuf);
			break;
		case IShape::POLY:
			return CheckIntersection(this, dynamic_cast<Polygon*>(shape), interPosBuf);
			break;
		default:
			if (extChecker != NULL)
				return extChecker(this, shape, interPosBuf);
			return false;
		}
	}


	// axis aligned box
	AxisAlignedBox::AxisAlignedBox(const Point3& posMin, const Point3& posMax)
		: IShape(IShape::AABOX){
		min = posMin;
		max = posMax;
	}

	bool AxisAlignedBox::Intersect(IShape* shape, Position3* interPosBuf){
		switch (shape->GetType()){
		case IShape::LINE:
			return CheckIntersection(dynamic_cast<Line*>(shape), this, interPosBuf);
			break;
		case IShape::SPHERE:
			return CheckIntersection(dynamic_cast<Sphere*>(shape), this, interPosBuf);
			break;
		case IShape::AABOX:
			return CheckIntersection(this, dynamic_cast<AxisAlignedBox*>(shape), interPosBuf);
			break;
		case IShape::OBOX:
			return CheckIntersection(this, dynamic_cast<OrientedBox*>(shape), interPosBuf);
			break;
		case IShape::POLY:
			return CheckIntersection(this, dynamic_cast<Polygon*>(shape), interPosBuf);
			break;
		default:
			if (extChecker != NULL)
				return extChecker(this, shape, interPosBuf);
			return false;
		}
	}


	// oriented box
	OrientedBox::OrientedBox(const Point3& position, const Vector3& e1, const Vector3& e2, const Vector3& e3, const Vector3& halfOfExt)
		: IShape(IShape::OBOX){
		pos = position;
		axis[0].xyz = e1.Normal();	axis[0].w = halfOfExt.x;
		axis[1].xyz = e2.Normal();	axis[1].w = halfOfExt.y;
		axis[2].xyz = e3.Normal();	axis[2].w = halfOfExt.z;
	}

	bool OrientedBox::Intersect(IShape* shape, Position3* interPosBuf){
		switch (shape->GetType()){
		case IShape::LINE:
			return CheckIntersection(dynamic_cast<Line*>(shape), this, interPosBuf);
			break;
		case IShape::SPHERE:
			return CheckIntersection(dynamic_cast<Sphere*>(shape), this, interPosBuf);
			break;
		case IShape::AABOX:
			return CheckIntersection(dynamic_cast<AxisAlignedBox*>(shape), this, interPosBuf);
			break;
		case IShape::OBOX:
			return CheckIntersection(this, dynamic_cast<OrientedBox*>(shape), interPosBuf);
			break;
		case IShape::POLY:
			return CheckIntersection(this, dynamic_cast<Polygon*>(shape), interPosBuf);
			break;
		default:
			if (extChecker != NULL)
				return extChecker(this, shape, interPosBuf);
			return false;
		}
	}


	// polygon
	Polygon::Polygon()
		: IShape(IShape::POLY){
		
	}

	bool Polygon::Intersect(IShape* shape, Position3* interPosBuf){
		switch (shape->GetType()){
		case IShape::LINE:
			return CheckIntersection(dynamic_cast<Line*>(shape), this, interPosBuf);
			break;
		case IShape::SPHERE:
			return CheckIntersection(dynamic_cast<Sphere*>(shape), this, interPosBuf);
			break;
		case IShape::AABOX:
			return CheckIntersection(dynamic_cast<AxisAlignedBox*>(shape), this, interPosBuf);
			break;
		case IShape::OBOX:
			return CheckIntersection(dynamic_cast<OrientedBox*>(shape), this, interPosBuf);
			break;
		case IShape::POLY:
			return CheckIntersection(this, dynamic_cast<Polygon*>(shape), interPosBuf);
			break;
		default:
			if (extChecker != NULL)
				return extChecker(this, shape, interPosBuf);
			return false;
		}
	}

}
#ifndef __SHAPES_H__
#define __SHAPES_H__

#include "core.h"

namespace sark{

	class Line;
	class Sphere;
	class AxisAlignedBox;
	class OrientedBox;
	class Polygon;
	
	// interface of shape things
	// shape can be intersected with other shapes
	class IShape{
	public:
		enum Type{ LINE=1, SPHERE, AABOX, OBOX, POLY, EXT_SHAPE };
		typedef bool(*ExtIntersectionChecker)(IShape* me, IShape* target, Position3* interPosBuf);
	protected:
		Type mType;
		static ExtIntersectionChecker extChecker;

	public:
		IShape(Type type);
		virtual ~IShape();
		Type GetType() const;

		virtual bool Intersect(IShape* shape, Position3* interPosBuf = NULL) = 0;

	public:
		static void SetExtendedIntersectionChecker(ExtIntersectionChecker checker);
		
		// intersection check functions

		static bool CheckIntersection(Line* line1, Line* line2,	Position3* interPosBuf);
		static bool CheckIntersection(Line* line, Sphere* sphere, Position3* interPosBuf);
		static bool CheckIntersection(Line* line, AxisAlignedBox* aabox, Position3* interPosBuf);
		static bool CheckIntersection(Line* line, OrientedBox* obox, Position3* interPosBuf);
		static bool CheckIntersection(Line* line, Polygon* tri_poly, Position3* interPosBuf);

		static bool CheckIntersection(Sphere* sphere1, Sphere* sphere2, Position3* interPosBuf);
		static bool CheckIntersection(Sphere* sphere, AxisAlignedBox* aabox, Position3* interPosBuf);
		static bool CheckIntersection(Sphere* sphere, OrientedBox* obox, Position3* interPosBuf);
		static bool CheckIntersection(Sphere* sphere, Polygon* tri_poly, Position3* interPosBuf);

		static bool CheckIntersection(AxisAlignedBox* aabox1, AxisAlignedBox* aabox2, Position3* interPosBuf);
		static bool CheckIntersection(AxisAlignedBox* aabox, OrientedBox* obox, Position3* interPosBuf);
		static bool CheckIntersection(AxisAlignedBox* aabox, Polygon* tri_poly, Position3* interPosBuf);

		static bool CheckIntersection(OrientedBox* obox1, OrientedBox* obox2, Position3* interPosBuf);
		static bool CheckIntersection(OrientedBox* obox, Polygon* tri_poly, Position3* interPosBuf);

		static bool CheckIntersection(Polygon* tri_poly1, Polygon* tri_poly2, Position3* interPosBuf);

		// helpful intersection check functions

		static bool _RangeIntersect(real rangeA1, real rangeA2, real rangeB1, real rangeB2);
	};


	// parametri_polyc equations of line
	class Line : public IShape{
	public:
		Position3 pos;
		Vector3 dir;
		real minp, maxp;
		
		#define DEFAULT_PARAM_RANGE 1000000000.0f
		Line(const Position3& position, const Vector3& direction, 
			real minParam = -DEFAULT_PARAM_RANGE, real maxParam = DEFAULT_PARAM_RANGE);
		virtual bool Intersect(IShape* shape, Position3* interPosBuf = NULL);
	};

	// center and radius form of sphere
	class Sphere : public IShape{
	public:
		Position3 pos;
		real r;

		Sphere(const Position3& position, real radius);
		virtual bool Intersect(IShape* shape, Position3* interPosBuf = NULL);
	};

	// box that its faces are aligned by coordinate axis orientation
	// it is represented by center position and half of width, height, depth
	class AxisAlignedBox : public IShape{
	public:
		Position3 min, max;

		AxisAlignedBox(const Position3& posMin, const Position3& posMax);
		virtual bool Intersect(IShape* shape, Position3* interPosBuf = NULL);
	};

	// box which has the orientation for itself
	class OrientedBox : public IShape{
		Position3 pos;
		Vector4 axis[3]; //vec3 as axis, w factor as extention

		OrientedBox(const Position3& position, const Vector3& e1, const Vector3& e2, const Vector3& e3, const Vector3& halfOfExt);
		virtual bool Intersect(IShape* shape, Position3* interPosBuf = NULL);
	};

	// triangle set polygon
	class Polygon : public IShape{
	public:
		class Face{
		public:
			Vertex3 *va, *vb, *vc;
			Face(const Vertex3* vtxA, const Vertex3* vtxB, const Vertex3* vtxC);
		};
		
	public:
		
		Polygon();
		virtual bool Intersect(IShape* shape, Position3* interPosBuf = NULL);
	};

}
#endif
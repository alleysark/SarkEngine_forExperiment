#ifndef __PRIMITIVES_HPP__
#define __PRIMITIVES_HPP__

#include "core.h"

namespace sark{

	// provided well-known primitive type.
	// *note: you can define your own structures to represent
	// other types of draw mode. but be warnned, it's better
	// to pack the structure to prevent 4-byte alignment problem.

	// triangle primitive.
	#pragma pack(push, 1)
	template<typename _T>
	class TrianglePrimitive{
	public:
		_T a, b, c;

		TrianglePrimitive()
			: a(0), b(0), c(0)
		{}
		TrianglePrimitive(_T _a, _T _b, _T _c)
			: a(_a), b(_b), c(_c)
		{}
	};
	#pragma pack(pop)

	typedef TrianglePrimitive<uint16> TriangleFace16;
	typedef TrianglePrimitive<uint32> TriangleFace32;

	// quad primitive.
	#pragma pack(push, 1)
	template<typename _T>
	class QuadPrimitive{
	public:
		_T a, b, c, d;

		QuadPrimitive()
			: a(0), b(0), c(0), d(0)
		{}
		QuadPrimitive(_T _a, _T _b, _T _c, _T _d)
			: a(_a), b(_b), c(_c), d(_d)
		{}
	};
	#pragma pack(pop)

	typedef QuadPrimitive<uint16> QuadFace16;
	typedef QuadPrimitive<uint32> QuadFace32;

}
#endif
#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "core.h"

namespace sark{

	class Transform{
	private:
		Position3 mPosition;
		Quaternion mRotator;
		
		Matrix4 mmatWorld;

	public:
		Transform();
		~Transform();

		// get transformation matrix
		const Matrix4& GetMatrix();

		// get position vector
		const Position3& GetPosition() const;

		// translate into the other position from given vector
		void Translate(const Position3& position);
		// translate into the other position from given vector
		void Translate(real x, real y, real z);

		// translate this position additionally
		void TranslateMore(const Position3& add);
		// translate this position additionally
		void TranslateMore(real add_x, real add_y, real add_z);

		// rotate it from given axis and theta
		void Rotate(const Vector3& axis, real theta);
		// rotate it from given rotating factor roll(z-axis), pitch(x-axis) and yaw(y-axis)
		void Rotate(real roll, real pitch, real yaw);
	};

}
#endif
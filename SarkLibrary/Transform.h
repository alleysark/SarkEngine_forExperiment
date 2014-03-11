#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "core.h"

namespace sarklib{

	class Transform{
	private:
		Position3 mPosition;
		Quaternion mRotator;
		
		Matrix4 mmatWorld;

	public:
		Transform(){
			mmatWorld.MakeIdentity();
		}
		~Transform(){}

		// get transformation matrix
		const Matrix4& GetMatrix(){
			if (mmatWorld.m[3][3] != 0)
				return mmatWorld;

			mmatWorld = mRotator.ToMatrix4(true);
			mmatWorld.m[0][3] = mPosition.x;
			mmatWorld.m[1][3] = mPosition.y;
			mmatWorld.m[2][3] = mPosition.z;
			mmatWorld.m[3][3] = 1.0f;
			return mmatWorld;
		}

		// get position vector
		const Position3& GetPosition() const{
			return mPosition;
		}

		// translate into the other position from given vector
		void Translate(const Position3& position){
			mPosition = position;
			mmatWorld.m[3][3] = 0.0f;
		}
		// translate into the other position from given vector
		void Translate(real x, real y, real z){
			mPosition.Set(x, y, z);
			mmatWorld.m[3][3] = 0.0f;
		}

		// translate this position additionally
		void TranslateMore(const Position3& add){
			mPosition += add;
			mmatWorld.m[3][3] = 0.0f;
		}
		// translate this position additionally
		void TranslateMore(real add_x, real add_y, real add_z){
			mPosition += Position3(add_x, add_y, add_z);
			mmatWorld.m[3][3] = 0.0f;
		}

		// rotate it from given axis and theta
		void Rotate(const Vector3& axis, real theta){
			mRotator.MakeRotatingQuat(axis, theta);
			mmatWorld.m[3][3] = 0.0f;
		}
		// rotate it from given rotating factor roll(z-axis), pitch(x-axis) and yaw(y-axis)
		void Rotate(real roll, real pitch, real yaw){
			mRotator.MakeRotatingQuat(roll, pitch, yaw);
			mmatWorld.m[3][3] = 0.0f;
		}
	};

}
#endif
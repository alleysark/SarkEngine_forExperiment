#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "core.h"

namespace sark{

	class ASceneComponent;

	// make given transformation matrix stained
	#define STAIN_TRANSMATRIX(mat4) ((mat4).m[3][3] = 1)

	// macro function for checking if matrix is stained
	#define IS_STAINED(mat4) ((mat4).m[3][3] == 0)


	class Transform{
	private:
		Position3 mPosition;
		Quaternion mRotator;
		
		// complete transform matrix. m[3][3] is a stain-check flag
		Matrix4 mTransformMat;

		// if this is the transform of scene component,
		// it can access the scene component object by this property
		// (ASceneComponent regard this as friend)
		ASceneComponent* mReference;
	public:
		Transform(ASceneComponent* reference = NULL);
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

	private:
		// all the methods of Transform class have to call this function
		// when the properties (translation and rotator quaternion) are changed.
		// if there is reference scene component, it sets the stained flag of absolute 
		// transform of reference scene component.
		void TransformStained();
	};

}
#endif
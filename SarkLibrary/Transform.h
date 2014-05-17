#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "core.h"

namespace sark{

	class ASceneComponent;

	// make given transformation matrix stained
	#define STAIN_TRANSMATRIX(mat4) ((mat4).m[3][3] = 0)

	// macro function for checking if matrix is stained
	#define IS_STAINED(mat4) ((mat4).m[3][3] == 0)


	// transformation managemenet class
	// it can be organized the hierarchical struct
	class Transform{
	private:
		// local rotation factor
		Quaternion mRotator;
		
		// local scaling factor
		Vector3 mScale;

		// local transform matrix. m[3][3] is a stain-check flag
		Matrix4 mLocalTM;

		// combined transform matrix of all ancestors
		Matrix4 mAbsoluteTM;


		// it can access the scene component object by this property
		// (ASceneComponent regard this as friend)
		ASceneComponent* mReference;

	public:
		Transform(ASceneComponent* reference = NULL);
		~Transform();

		// get absolute transformation matrix
		const Matrix4& GetMatrix();

		// get local transformation matrix
		const Matrix4& GetLocalMatrix();


		// get world space position
		const Position3 GetPosition();

		// get world space direction.
		// formal direction vector is Vector3::Forward
		// *note: if this and its ancestors scale themselves,
		// the returned direction cannot guarantee the correctness.
		const Vector3 GetDirection();

		// get world space rotation.
		// *note: if this and its ancestors scale themselves,
		// the returned rotation quaternion cannot guarantee
		// the correctness.
		const Quaternion GetRotation();

		// get local position (translation) 
		const Position3 GetLocalPosition() const;

		// get local rotator
		const Quaternion& GetLocalRotation() const;

		// get local scale
		const Vector3& GetLocalScale() const;


		// translate into the other position from given vector
		void Translate(const Position3& position);
		// translate into the other position from given vector
		void Translate(real x, real y, real z);

		// translate this position additionally
		void TranslateMore(const Position3& add);
		// translate this position additionally
		void TranslateMore(real add_x, real add_y, real add_z);

		// rotate it from given axis and theta
		void Rotate(const Vector3& axis, real theta, bool axis_normalized = false);
		// rotate it from given rotating factor roll(z-axis), pitch(x-axis) and yaw(y-axis)
		void Rotate(real roll, real pitch, real yaw);
		// rotate it from given rotation quaternion.
		void Rotate(const Quaternion& rotation);

		// rotate it additionally from given axis and theta
		void RotateMore(const Vector3& axis, real theta, bool axis_normalized = false);
		// rotate it additionally from given rotating factor roll(z-axis), pitch(x-axis) and yaw(y-axis)
		void RotateMore(real roll, real pitch, real yaw);
		// rotate it additionally from given rotation quaternion.
		void RotateMore(const Quaternion& rotation);

		// scale it by given scaling factor
		void Scale(const Vector3& scaleAs);
		// scale it by given scaling factor
		void Scale(real sx, real sy, real sz);

	private:
		// all the methods of Transform class have to call this function
		// when the properties (translation and rotator quaternion) are changed.
		// if there is reference scene component, it sets the stained flag of absolute 
		// transform of reference scene component.
		// it also sets the 'is transformed' factors of reference scene component
		// and its offspring's.
		void TransformStained(bool stainLocal = true);

		// apply whole transformation factors onto its matrices.
		void UpdateTransform();
	};

}
#endif
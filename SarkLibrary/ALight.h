#ifndef __A_LIGHT_H__
#define __A_LIGHT_H__

#include "core.h"
#include "ASceneComponent.h"

namespace sark{

	// pure abstract light class
	class ALight : public ASceneComponent{
	public:
		// light type definition
		enum LightType{
			POINT_LIGHT, SPOT_LIGHT, DIRECTIONAL_LIGHT
		};

	protected:
		// type of light
		LightType mLightType;

		// color factor of ambient property
		ColorRGBA mAmbient;

		// color factor of diffuse property
		ColorRGBA mDiffuse;

		// color factor of specular property
		ColorRGBA mSpecular;

		// position of light source.
		// *note: to get the position vector, use GetPosition() method of mTransform and
		// please be careful that GetLocalPosition() might give the incorrect result.
		// [Vector3 mPosition;]

		// direction vector of directional light or spot light.
		// *note: to get the direction vector, use GetDirection() method of mTransform.
		// the formal default direction vector is (0,0,-1) which is defined as Vector3::Forward
		// [Vector3 mDirection;]

		// attenuation factors.
		// attenuation = 1 / (k_c + k_l*d + k_q*sqre(d))
		// where k_c is constant attenuation factor,
		// k_l is linear attenuation factor and
		// k_q is quadratic attenuation factor.
		Vector3 mAttenuations;

	public:
		ALight(const std::string& name, ASceneComponent* parent, bool activate,
			LightType lightType, const ColorRGBA& ambient, const ColorRGBA& diffuse, const ColorRGBA& specular,
			real attenu_const, real attenu_linear, real attenu_quad);

		virtual ~ALight();

		// get light type
		const LightType& GetLightType() const;

		// get ambient color
		const ColorRGBA& GetAmbient() const;
		// set ambient color
		void SetAmbient(const ColorRGBA& ambient);

		// get diffuse color
		const ColorRGBA& GetDiffuse() const;
		// set diffuse color
		void SetDiffuse(const ColorRGBA& diffuse);

		// get specular color
		const ColorRGBA& GetSpecular() const;
		// set specular color
		void SetSpecular(const ColorRGBA& specular);

		// get attenuation factors. constant, linear, quadratic order.
		const real(&GetAttenuations() const)[3];
		// set attenuation factors
		void SetAttenuations(real attenu_const, real attenu_linear, real attenu_quad);
	};

}
#endif
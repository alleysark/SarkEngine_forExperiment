#include "ALight.h"

namespace sark {

	ALight::ALight(const std::string& name, ASceneComponent* parent, bool activate,
		LightType lightType, const ColorRGBA& ambient, const ColorRGBA& diffuse, const ColorRGBA& specular,
		real attenu_const, real attenu_linear, real attenu_quad)
		: ASceneComponent(name, parent, activate), mLightType(lightType),
		mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular),
		mAttenuations(attenu_const, attenu_linear, attenu_quad)
	{}

	ALight::~ALight() {}

	// get light type
	const ALight::LightType& ALight::GetLightType() const {
		return mLightType;
	}

	// get ambient color
	const ColorRGBA& ALight::GetAmbient() const {
		return mAmbient;
	}
	// set ambient color
	void ALight::SetAmbient(const ColorRGBA& ambient) {
		mAmbient = ambient;
	}

	// get diffuse color
	const ColorRGBA& ALight::GetDiffuse() const {
		return mDiffuse;
	}
	// set diffuse color
	void ALight::SetDiffuse(const ColorRGBA& diffuse) {
		mDiffuse = diffuse;
	}

	// get specular color
	const ColorRGBA& ALight::GetSpecular() const {
		return mSpecular;
	}
	// set specular color
	void ALight::SetSpecular(const ColorRGBA& specular) {
		mSpecular = specular;
	}

	// get attenuation factors. constant, linear, quadratic order.
	const real(&ALight::GetAttenuations() const)[3]{
		return mAttenuations.v;
	}
	// set attenuation factors
	void ALight::SetAttenuations(real attenu_const, real attenu_linear, real attenu_quad) {
		mAttenuations.Set(attenu_const, attenu_linear, attenu_quad);
	}

}
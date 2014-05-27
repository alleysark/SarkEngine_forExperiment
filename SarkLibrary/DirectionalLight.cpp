#include "DirectionalLight.h"
#include "Debug.h"

namespace sark{

	DirectionalLight::DirectionalLight(const ColorRGBA& ambient, const ColorRGBA& diffuse, const ColorRGBA& specular)
		: ALight("", NULL, true, LightType::DIRECTIONAL_LIGHT,
		ambient, diffuse, specular, 1.f, 0.f, 0.f)
	{}
	DirectionalLight::DirectionalLight(const std::string& name, ASceneComponent* parent, bool activate,
		const ColorRGBA& ambient, const ColorRGBA& diffuse, const ColorRGBA& specular)
		: ALight(name, parent, activate, LightType::DIRECTIONAL_LIGHT,
		ambient, diffuse, specular, 1.f, 0.f, 0.f)
	{}

	DirectionalLight:: ~DirectionalLight(){}

	void DirectionalLight::Update(){ }

	void DirectionalLight::Render(){ }

	// directional light doesn't have collider.
	// so it simply returns NULL.
	const ACollider* DirectionalLight::GetCollider() const{
		return NULL;
	}

	void DirectionalLight::SetCollider(ACollider* newColl){
		if (newColl != NULL){
			LogWarn("directional light can't have collider");
			delete newColl;
		}
	}

	Mesh* DirectionalLight::GetMesh(){
		return NULL;
	}

	RigidBody* DirectionalLight::GetRigidBody(){
		return NULL;
	}

}
#ifndef __DIRECTIONAL_LIGHT_H__
#define __DIRECTIONAL_LIGHT_H__

#include "core.h"
#include "ALight.h"

namespace sark{

	// directional light.
	// if the light source is far from objects, the rays
	// emitted from light almost be parallel to each other.
	// for that reason, light can be represented as 'direction' 
	class DirectionalLight : public ALight{
	public:
		DirectionalLight(const ColorRGBA& ambient, const ColorRGBA& diffuse, const ColorRGBA& specular);
		DirectionalLight(const std::string& name, ASceneComponent* parent, bool activate,
			const ColorRGBA& ambient, const ColorRGBA& diffuse, const ColorRGBA& specular);

		virtual ~DirectionalLight();

		void Update() override;

		void Render() override;

		// getting direction by Transform::GetDirection()
		// you can easily modify the direction by Transform::Rotate.

		// directional light doesn't have bounding shape.
		// so it simply returns NULL.
		const IShape* GetBoundingShape() const override;
	};

}
#endif
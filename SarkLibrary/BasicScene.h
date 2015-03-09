#ifndef __BASIC_SCENE_H__
#define __BASIC_SCENE_H__

#include "core.h"
#include "AScene.h"

namespace sark {

	class ALight;
	class AModel;

	// Basic scene
	class BasicScene : public AScene {
	public:
		enum {
			LAYER_MODEL_RENDER = 0,
			LAYER_LIGHT,
		};

	public:
		BasicScene();
		~BasicScene();

		// add model component
		bool AddModel(AModel* model);

		// add light component
		bool AddLight(ALight* light);


		void OnEnter() override;

		void OnLeave() override;

		void Update() override;

		void Render() override;

		void OnScreenChanged(uinteger width, uinteger height) override;

	private:
		// hide addition function
		bool AddSceneComponent(ASceneComponent* sceneComponent);
	};

}
#endif
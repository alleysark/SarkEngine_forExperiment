#include "BasicScene.h"
#include "ALight.h"
#include "AModel.h"
#include "Material.h"
#include "Input.h"

namespace sark {

	BasicScene::BasicScene() {
		// make basic layer.
		mLayers.push_back(Layer()); //LAYER_MODEL_RENDER
		mLayers.push_back(Layer()); //LAYER_LIGHT

		// make basic camera and set as main
		mCameras.push_back(new Camera(Position3(0, 10, 15), Position3(0, 0, 0)));
		mMainCam = mCameras[0];
	}

	BasicScene::~BasicScene() {
	}

	bool BasicScene::AddModel(AModel* model) {
		if (!AScene::AddSceneComponent(model))
			return false;

		mLayers[LAYER_MODEL_RENDER].Push(model);
		return true;
	}

	bool BasicScene::AddLight(ALight* light) {
		if (!AScene::AddSceneComponent(light))
			return false;

		mLayers[LAYER_LIGHT].Push(light);
		return true;
	}

	void BasicScene::OnEnter() { }

	void BasicScene::OnLeave() { }

	void BasicScene::Update() {
		auto itr = mComponents.begin();
		auto end = mComponents.end();
		for (; itr != end; itr++) {
			itr->second->Update();
		}
	}

	void BasicScene::Render() {
		glMultTransposeMatrixf(mMainCam->GetViewMatrix().GetRawMatrix());

		auto itr = mLayers[LAYER_MODEL_RENDER].Begin();
		auto end = mLayers[LAYER_MODEL_RENDER].End();
		for (; itr != end; itr++) {
			AModel* model = reinterpret_cast<AModel*>(*itr);
			Material* mtrl = model->GetMaterial();
			mtrl->Prepare(model);
			model->Render();
		}
	}

	void BasicScene::OnScreenChanged(uinteger width, uinteger height) {
		mMainCam->SetViewport(0, 0, width, height);
		mMainCam->Perspective(60, (real)width / (real)height, 0.1f, 1000.f);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultTransposeMatrixf(mMainCam->GetProjMatrix().GetRawMatrix());
		glMatrixMode(GL_MODELVIEW);
	}

}
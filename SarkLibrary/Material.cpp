#include "Material.h"
#include "AScene.h"
#include "AModel.h"
#include "ShaderProgram.h"

namespace sark {

	// material descriptor.
	// it must have its own shader program
	Material::Material(std::shared_ptr<ShaderProgram> shaderProg)
		: mShaderProg(shaderProg)
	{ }

	Material::~Material() {
	}

	// get shader program handler
	std::shared_ptr<ShaderProgram> Material::GetShaderProgram() {
		return mShaderProg;
	}

	// prepare to render
	void Material::Prepare(AScene* scene, AModel* model) {
		mShaderProg->Use();
		
		// set common uniforms
		Camera* cam = scene->GetMainCamera();
		mShaderProg->SetUniform("_MatView", cam->GetViewMatrix());
		mShaderProg->SetUniform("_MatProj", cam->GetProjMatrix());
		mShaderProg->SetUniform("_CameraPos", cam->GetEye());

		// set model-dependent uniforms
		mShaderProg->SetUniform("_MatWorld", model->GetTransform().GetMatrix());
	}

}
#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <memory>
#include "core.h"
#include "IUncopiable.hpp"

namespace sark {

	class AScene;
	class AModel;
	class ShaderProgram;

	// Material is sort of a descriptor for how 
	// the object should be rendered.
	class Material : IUncopiable {
	private:
		// material shader program
		s_ptr<ShaderProgram> mShaderProg;
		
	public:
		// material descriptor.
		// it must have its own shader program
		Material(s_ptr<ShaderProgram> shaderProg);

		virtual ~Material();

		// get shader program handler
		s_ptr<ShaderProgram> GetShaderProgram();

		// prepare to render
		void Prepare(AModel* model);
	};

}
#endif
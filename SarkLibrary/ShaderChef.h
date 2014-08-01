#ifndef __SHADER_SHEF_H__
#define __SHADER_SHEF_H__

#include <string>
#include <vector>
#include <list>
#include <hash_map>
#include <memory>
#include <GL/glew.h>

#include "core.h"
#include "ShaderProgram.h"

namespace sark{

	// shader chef class (shader creation helper)
	// it cooks shader program from given recipe.
	class ShaderChef {
	private:
		static std::list<std::shared_ptr<ShaderProgram>> mSpList;

		ShaderChef();
		ShaderChef(const ShaderChef&);

	public:
		~ShaderChef();

		// cook a shader program from given recipe.
		// whole shaders are compiled as 'version'. shader objects are attached
		// and are linked into program. it'll return NULL if it failed.
		static std::shared_ptr<ShaderProgram> CookShaderProgram(const ShaderProgram::Recipe& recipe);

	private:
		// create and compile shader object
		static ObjectHandle CreateShader(GLenum shaderType, const std::string& source);

		// check shader after compil. it'll log the info if there are compilation errors.
		static bool CheckShader(ObjectHandle obj);

		// check program after linking. it'll log the info if there are compilation errors.
		static bool CheckProgram(ObjectHandle obj);
	};

}
#endif
#ifndef __SHADER_SHEF_H__
#define __SHADER_SHEF_H__

#include <string>
#include <vector>
#include <list>
#include <hash_map>
#include <GL/glew.h>

#include "core.h"
#include "ShaderProgram.h"

namespace sark{


	// shader chef class (similar to factory).
	// it maintain shader sources and cook shader program
	// by registered sources.
	//
	// *note: it's better to clear whole registered sources when
	// there is no more requirement of making shader program.
	class ShaderChef {
	public:
		typedef std::hash_map<const std::string, std::string> IngredientDict;

		struct AttributePair {
			AttributeSemantic semantic; // location
			const char* name;
		};


		// recipe for shader cooking
		struct Recipe {
			std::vector<AttributePair> attributes;

			std::string vertexShader;

			std::string tessControlShader;

			std::string tessEvaluationShader;

			std::string geometryShader;

			std::string fragmentShader;
		};


	private:
		// cooking ingredients; shader source string
		IngredientDict mIngredients;

	private:
		ShaderChef();
		ShaderChef(const ShaderChef&);

		// singleton instance
		static ShaderChef* instance;

	public:
		static ShaderChef* GetInstance();
		
		~ShaderChef();

		// register shader source string.
		// it does not compile source.
		bool RegisterShaderSource(const std::string& name, const std::string& source);

		// register shader source from file.
		// it does not compile source.
		bool RegisterShaderSourceFromFile(const std::string& name, const std::string& filename);

		// cook a shader program from given recipe.
		// whole shaders are compiled as 'version'. shader objects are attached
		// and are linked into program. it'll return NULL if it failed.
		ShaderProgram* CookShaderProgram(const Recipe& recipe);

	private:
		// create and compile shader object
		ObjectHandle CreateShader(GLenum shaderType, const std::string& source);

		// check shader after compil. it'll log the info if there are compilation errors.
		bool CheckShader(ObjectHandle obj);

		// check program after linking. it'll log the info if there are compilation errors.
		bool CheckProgram(ObjectHandle obj);
	};

}
#endif
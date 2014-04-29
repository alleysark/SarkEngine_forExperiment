#ifndef __SHADER_SHEF_H__
#define __SHADER_SHEF_H__

#include <string>
#include <vector>
#include <list>
#include <hash_map>
#include <GL/glew.h>

#include "core.h"

namespace sark{

	class ShaderProgram;


	// shader chef class (similar to factory).
	// it maintain shader sources and cook shader program
	// by registered sources.
	//
	// *note: it's better to clear whole registered sources when
	// there is no more requirement of making shader program.
	class ShaderChef{
	public:
		enum CompileVersion{
			VERSION_330,
			VERSION_400,
			VERSION_410,
			VERSION_420,
			VERSION_430,
			VERSION_440
		};

		typedef std::hash_map<const std::string, std::string> IngredientDict;

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

		// cook a shader program from registered shader source ingredients.
		// whole shaders are compiled as 'version'. shader objects are attached
		// and are linked into program. it'll return NULL if it failed.
		ShaderProgram* MakeProgram(CompileVersion version,
			const std::vector<const char*>& vertexSourceNames,
			const std::vector<const char*>& fragmentSourceNames);
		
		// cook a shader program from registered shader source ingredients.
		// whole shaders are compiled as 'version'. shader objects are attached
		// and are linked into program. it'll return NULL if it failed.
		ShaderProgram* MakeProgram(CompileVersion version,
			const std::vector<const char*>& vertexSourceNames,
			const std::vector<const char*>& geometrySourceNames,
			const std::vector<const char*>& fragmentSourceNames);

		// clear all registered shader sources.
		void Clear();

	private:
		// create and compile shader object
		ObjectHandle CreateShader(GLenum shaderType,
			const CompileVersion& version, const std::vector<const char*>& sourceNames);

		// get version string
		const char* GetVersionString(const CompileVersion& version);

		// check shader after compil. it'll log the info if there are compilation errors.
		bool CheckShader(ObjectHandle obj);

		// check program after linking. it'll log the info if there are compilation errors.
		bool CheckProgram(ObjectHandle obj);
	};

}
#endif
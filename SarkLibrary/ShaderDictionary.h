#ifndef __SHADER_DICTIONARY_H__
#define __SHADER_DICTIONARY_H__

#include <string>
#include <vector>
#include <hash_map>
#include <GL/glew.h>

#include "core.h"

namespace sark{

	class ShaderProgram;


	// shader program dictionary.
	// it contains registered shader programs and supports shader getter.
	class ShaderDictionary{
	public:
		typedef GLhandleARB ObjectHandle;
		typedef std::hash_map<const std::string, ShaderProgram*> Dictionary;

	private:
		Dictionary mProgramDict;

	public:
		ShaderDictionary();
		~ShaderDictionary();

		// register shader program with full-dependent source file definitions.
		// it must be preceded before getting shader program.
		bool RegisterProgram(const std::string& programName,
			const std::vector<const char*>& vertexShaderFilenames,
			const std::vector<const char*>& fragmentShaderFilenames);

		// get registered shader program.
		// if there does not exist a matched shader program of given name,
		// it'll return the NULL.
		const ShaderProgram* GetShaderProgram(const std::string& programName);

		// clear all registered shader program.
		void Clear();

	private:
		// create shader object
		ObjectHandle CreateShader(GLenum shaderType, const std::vector<const char*>& shaders);

		// check shader source compilation
		// it'll log the info if there are compilation errors.
		bool CheckCompilation(ObjectHandle obj);
	};

}
#endif
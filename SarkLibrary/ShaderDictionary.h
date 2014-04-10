#ifndef __SHADER_DICTIONARY_H__
#define __SHADER_DICTIONARY_H__

#include <string>
#include <vector>
#include <list>
#include <hash_map>
#include <GL/glew.h>

#include "core.h"

namespace sark{

	class ShaderProgram;


	// shader program dictionary.
	// it contains registered shader programs and supports shader getter.
	class ShaderDictionary{
	public:
		enum CompileVersion{
			VERSION_330,
			VERSION_400,
			VERSION_410,
			VERSION_420,
			VERSION_430,
			VERSION_440
		};

		typedef GLuint ObjectHandle;
		typedef std::hash_map<const std::string, ShaderProgram*> Dictionary;
	private:
		Dictionary mProgramDict;

	public:
		ShaderDictionary();
		~ShaderDictionary();

		// register shader program with full-dependent source files.
		// it must be preceded before getting shader program.
		// shaders are compiled as 'version' and other version notations
		// of each shader sources are ignored.
		bool RegisterProgramFromFiles(const std::string& programName,
			const std::vector<const char*>& vertexShaderFiles,
			const std::vector<const char*>& fragmentShaderFiles,
			CompileVersion version = VERSION_330);

		// register shader program with full-dependent string sources.
		// it must be preceded before getting shader program.
		// shaders are compiled as 'version' and other version notations
		// of each shader sources are ignored.
		bool RegisterProgramFromSources(const std::string& programName,
			const std::vector<const char*>& vertexShaderSources,
			const std::vector<const char*>& fragmentShaderSources,
			CompileVersion version = VERSION_330);


		// get registered shader program.
		// if there does not exist a matched shader program of given name,
		// it'll return the NULL.
		ShaderProgram* GetShaderProgram(const std::string& programName);

		// clear all registered shader program.
		void Clear();

	private:
		// create shader object
		ObjectHandle CreateShader(GLenum shaderType, CompileVersion version,
			std::list<std::string>& sources);

		// read shader string sources from files.
		bool ReadSources(const std::vector<const char*>& files,
			std::list<std::string>& buffer);

		// check shader after compil. it'll log the info if there are compilation errors.
		bool CheckShader(ObjectHandle obj);

		// check program after linking. it'll log the info if there are compilation errors.
		bool CheckProgram(ObjectHandle obj);
	};

}
#endif
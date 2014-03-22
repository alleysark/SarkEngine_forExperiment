#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include <string>
#include <hash_map>
#include <GL/glew.h>
#include "core.h"

namespace sark{

	class ShaderDictionary;

	// programmable shader program.
	// it'll be created by ShaderDictionary 
	class ShaderProgram{
	public:	
		typedef GLhandleARB ProgramHandle;
		typedef GLint Location;
		typedef std::hash_map<const std::string, Location> LocationDictionary;

	private:
		// program handler
		ProgramHandle mhProgram;

		// locations of uniform/attribute variable are cached in this container
		LocationDictionary mLocationDict;

	public:
		// shader program is created from ShaderDictionary
		// with full liked shaders.
		ShaderProgram(ProgramHandle hProgram);

		~ShaderProgram();

		// before rendering, user should've Use() to use shader program.
		void Use();

		// after using shader program, user should've Unuse()
		// to disable this shader program.
		void Unuse();

	private:
		// get uniform location by variable name
		Location GetUniformLocation(const std::string& name);

		// get attribute location by variable name
		Location GetAttributeLocation(const std::string& name);

	public:
		//--------  uniform/attribute variable setting methods --------

		// doubles
		void SetUniform(const std::string& name, double val0){
			glUniform1d(GetUniformLocation(name), val0);
		}
		void SetUniform(const std::string& name, double val0, double val1){
			glUniform2d(GetUniformLocation(name), val0, val1);
		}
		void SetUniform(const std::string& name, double val0, double val1, double val2){
			glUniform3d(GetUniformLocation(name), val0, val1, val2);
		}
		void SetUniform(const std::string& name, double val0, double val1, double val2, double val3){
			glUniform4d(GetUniformLocation(name), val0, val1, val2, val3);
		}

		// floats
		void SetUniform(const std::string& name, float val0){
			glUniform1f(GetUniformLocation(name), val0);
		}
		void SetUniform(const std::string& name, float val0, float val1){
			glUniform2f(GetUniformLocation(name), val0, val1);
		}
		void SetUniform(const std::string& name, float val0, float val1, float val2){
			glUniform3f(GetUniformLocation(name), val0, val1, val2);
		}
		void SetUniform(const std::string& name, float val0, float val1, float val2, float val3){
			glUniform4f(GetUniformLocation(name), val0, val1, val2, val3);
		}

	
	};

}
#endif
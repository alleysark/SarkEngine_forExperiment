#include "ShaderProgram.h"

namespace sark{

	// shader program is created from ShaderDictionary
	// with full liked shaders.
	ShaderProgram::ShaderProgram(ProgramHandle hProgram)
		: mhProgram(hProgram)
	{}

	ShaderProgram::~ShaderProgram(){
		glDeleteObjectARB(mhProgram);
		mhProgram = 0;
		mLocationDict.clear();
	}

	// before rendering, user should've Use() to use shader program.
	void ShaderProgram::Use(){
		glUseProgramObjectARB(mhProgram);
	}

	// after using shader program, user should've Unuse()
	// to disable this shader program.
	void ShaderProgram::Unuse(){
		glUseProgramObjectARB(0);
	}


	// get uniform location by variable name
	ShaderProgram::Location ShaderProgram::GetUniformLocation(const std::string& name){
		LocationDictionary::iterator find = mLocationDict.find(name);
		if (find != mLocationDict.end())
			return find->second;
		return (mLocationDict[name] = glGetUniformLocationARB(mhProgram, name.c_str()));
	}

	// get attribute location by variable name
	ShaderProgram::Location ShaderProgram::GetAttributeLocation(const std::string& name){
		LocationDictionary::iterator find = mLocationDict.find(name);
		if (find != mLocationDict.end())
			return find->second;
		return (mLocationDict[name] = glGetAttribLocationARB(mhProgram, name.c_str()));
	}

}
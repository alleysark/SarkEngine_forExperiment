#include "ShaderDictionary.h"
#include "ShaderProgram.h"
#include "Debug.h"
#include <fstream>

namespace sark{

	ShaderDictionary::ShaderDictionary(){
	}

	ShaderDictionary::~ShaderDictionary(){}


	// register shader program with full-dependent source file definitions.
	// it must be preceded before getting shader program.
	bool ShaderDictionary::RegisterProgram(const std::string& programName,
		const std::vector<const char*>& vtxShaders,
		const std::vector<const char*>& fragShaders)
	{
		Dictionary::iterator find = mProgramDict.find(programName);
		if (find != mProgramDict.end())
			return false;

		ObjectHandle progObj = glCreateProgram();
		if (progObj == 0)
			return false;

		ObjectHandle vtxObj = CreateShader(GL_VERTEX_SHADER, vtxShaders);
		if (vtxObj == 0){
			glDeleteProgram(progObj);
			return false;
		}
		ObjectHandle fragObj = CreateShader(GL_FRAGMENT_SHADER, fragShaders);
		if (fragObj == 0){
			glDeleteShader(vtxObj);
			glDeleteProgram(progObj);
			return false;
		}

		glAttachShader(progObj, vtxObj);
		glAttachShader(progObj, fragObj);

		glLinkProgram(progObj);

		if (!CheckProgram(progObj)){
			glDeleteShader(vtxObj);
			glDeleteShader(fragObj);
			glDeleteProgram(progObj);
			return false;
		}

		glDetachShader(progObj, vtxObj);
		glDetachShader(progObj, fragObj);

		glDeleteShader(vtxObj);
		glDeleteShader(fragObj);

		ShaderProgram* program = new ShaderProgram(progObj);
		mProgramDict[programName] = program;
		return true;
	}

	// get registered shader program.
	// if there does not exist a matched shader program of given name,
	// it'll return the NULL.
	ShaderProgram* ShaderDictionary::GetShaderProgram(const std::string& programName){
		Dictionary::iterator find = mProgramDict.find(programName);
		if (find == mProgramDict.end())
			return NULL;
		return find->second;
	}

	// clear all registered shader program.
	void ShaderDictionary::Clear(){
		Dictionary::iterator progItr = mProgramDict.begin();
		for (; progItr != mProgramDict.end(); progItr++){
			delete progItr->second;
		}
		mProgramDict.clear();
	}

	// create shader object
	ShaderDictionary::ObjectHandle ShaderDictionary::CreateShader(GLenum shaderType, const std::vector<const char*>& shaders){
		ObjectHandle newShaderHandle = glCreateShader(shaderType);
		if (newShaderHandle == 0)
			return 0;

		uinteger count = shaders.size();
		std::ifstream fin;
		char** sources = new char*[count];
		for (uinteger i = 0; i < count; i++){
			fin.open(shaders[i], std::ios::beg);
			if (!fin.is_open()){
				for (uinteger k = 0; k < i; k++){
					delete[] sources[k];
				}
				delete[] sources;
				return 0;
			}

			fin.seekg(0, std::ios::end);
			uinteger length = (uinteger)fin.tellg();
			fin.seekg(0, std::ios::beg);

			sources[i] = new char[length + 1];
			memset(sources[i], NULL, length + 1);
			fin.read(sources[i], length);
			fin.close();
		}

		glShaderSource(newShaderHandle, count, (const char**)(sources), NULL);
		for (uinteger i = 0; i < count; i++){
			delete[] sources[i];
		}
		delete[] sources;

		glCompileShader(newShaderHandle);

		if (!CheckShader(newShaderHandle)){
			return 0;
		}

		return newShaderHandle;
	}


	// check shader after compilation. it'll log the info if there are compilation errors.
	bool ShaderDictionary::CheckShader(ObjectHandle obj){
		GLint state;
		glGetShaderiv(obj, GL_COMPILE_STATUS, &state);
		if (GL_FALSE == state){
			int infologLength = 0;
			glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1){
				int charsWritten = 0;
				char *infoLog = new char[infologLength];
				glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
				LogError(infoLog);
				delete[] infoLog;
				
			}
			return false;
		}
		return true;
	}

	// check program after linking. it'll log the info if there are compilation errors.
	bool ShaderDictionary::CheckProgram(ObjectHandle obj){
		GLint state;
		glGetProgramiv(obj, GL_LINK_STATUS, &state);
		if (GL_FALSE == state) {
			int infologLength = 0;
			glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1){
				int charsWritten = 0;
				char *infoLog = new char[infologLength];
				glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
				LogError(infoLog);
				delete[] infoLog;
			}
			return false;
		}
		return true;
	}

}
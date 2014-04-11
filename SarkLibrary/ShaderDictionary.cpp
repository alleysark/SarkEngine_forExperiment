#include "ShaderDictionary.h"
#include "ShaderProgram.h"
#include "Debug.h"
#include <fstream>

namespace sark{

	ShaderDictionary::ShaderDictionary(){
	}

	ShaderDictionary::~ShaderDictionary(){}


	// register shader program with full-dependent source files.
	// it must be preceded before getting shader program.
	// shaders are compiled as 'version' and other version notations
	// of each shader sources are ignored.
	bool ShaderDictionary::RegisterProgramFromFiles(const std::string& programName,
		const std::vector<const char*>& vertexShaderFiles,
		const std::vector<const char*>& fragmentShaderFiles,
		CompileVersion version)
	{
		Dictionary::iterator find = mProgramDict.find(programName);
		if (find != mProgramDict.end())
			return false;

		ObjectHandle progObj = glCreateProgram();
		if (progObj == 0)
			return false;

		std::vector<std::string> sourceBuf;
		std::vector<const char*> sources;

		// read vertex shader sources
		if (!ReadSources(vertexShaderFiles, sourceBuf)){
			glDeleteProgram(progObj);
			return false;
		}
		// create vertex shader
		sources.push_back(GetVersionString(version));
		for (std::vector<std::string>::iterator itr = sourceBuf.begin();
			itr != sourceBuf.end(); itr++){
			sources.push_back(itr->c_str());
		}
		ObjectHandle vtxObj = CreateShader(GL_VERTEX_SHADER, sources);
		if (vtxObj == 0){
			glDeleteProgram(progObj);
			return false;
		}

		// read fragment shader sources
		sourceBuf.clear();
		if (!ReadSources(fragmentShaderFiles, sourceBuf)){
			glDeleteShader(vtxObj);
			glDeleteProgram(progObj);
			return false;
		}
		// create fragment shader
		sources.clear();
		sources.push_back(GetVersionString(version));
		for (std::vector<std::string>::iterator itr = sourceBuf.begin();
			itr != sourceBuf.end(); itr++){
			sources.push_back(itr->c_str());
		}
		ObjectHandle fragObj = CreateShader(GL_FRAGMENT_SHADER, sources);
		if (fragObj == 0){
			glDeleteShader(vtxObj);
			glDeleteProgram(progObj);
			return false;
		}

		glAttachShader(progObj, vtxObj);
		glAttachShader(progObj, fragObj);

		// bind pre-defined attribute semantics
		for (int attrLoca = 0; attrLoca < ShaderProgram::ATTR_COUNT; attrLoca++){
			glBindAttribLocation(progObj, attrLoca, ShaderProgram::AttribVarNames[attrLoca]);
		}

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

	// register shader program with full-dependent string sources.
	// it must be preceded before getting shader program.
	// shaders are compiled as 'version' and other version notations
	// of each shader sources are ignored.
	bool ShaderDictionary::RegisterProgramFromSources(const std::string& programName,
		const std::vector<const char*>& vertexShaderSources,
		const std::vector<const char*>& fragmentShaderSources,
		CompileVersion version)
	{
		Dictionary::iterator find = mProgramDict.find(programName);
		if (find != mProgramDict.end())
			return false;

		ObjectHandle progObj = glCreateProgram();
		if (progObj == 0)
			return false;

		std::vector<const char*> sources;

		// create vertex shader
		sources.push_back(GetVersionString(version));
		for (std::vector<const char*>::const_iterator itr = vertexShaderSources.cbegin();
			itr != vertexShaderSources.cend(); itr++){
			sources.push_back(*itr);
		}
		ObjectHandle vtxObj = CreateShader(GL_VERTEX_SHADER, sources);
		if (vtxObj == 0){
			glDeleteProgram(progObj);
			return false;
		}

		// create fragment shader
		sources.clear();
		sources.push_back(GetVersionString(version));
		for (std::vector<const char*>::const_iterator itr = fragmentShaderSources.cbegin();
			itr != fragmentShaderSources.cend(); itr++){
			sources.push_back(*itr);
		}
		ObjectHandle fragObj = CreateShader(GL_FRAGMENT_SHADER, sources);
		if (fragObj == 0){
			glDeleteShader(vtxObj);
			glDeleteProgram(progObj);
			return false;
		}

		glAttachShader(progObj, vtxObj);
		glAttachShader(progObj, fragObj);

		// bind pre-defined attribute semantics
		for (int attrLoca = 0; attrLoca < ShaderProgram::ATTR_COUNT; attrLoca++){
			glBindAttribLocation(progObj, attrLoca, ShaderProgram::AttribVarNames[attrLoca]);
		}

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
	ShaderDictionary::ObjectHandle ShaderDictionary::CreateShader(GLenum shaderType,
		const std::vector<const char*>& sources)
	{
		ObjectHandle newShaderHandle = glCreateShader(shaderType);
		if (newShaderHandle == 0)
			return 0;

		glShaderSource(newShaderHandle, sources.size(), (const char**)(&sources[0]), NULL);

		glCompileShader(newShaderHandle);

		if (!CheckShader(newShaderHandle)){
			return 0;
		}

		return newShaderHandle;
	}

	// read shader string sources from files
	bool ShaderDictionary::ReadSources(const std::vector<const char*>& files,
		std::vector<std::string>& buffer)
	{
		std::ifstream fin;
		uinteger count = files.size();
		for (uinteger i = 0; i < count; i++){
			fin.open(files[i], std::ios::beg);
			if (!fin.is_open()){
				buffer.clear();
				return false;
			}

			buffer.push_back(
				std::string(std::istreambuf_iterator<char>(fin),
				std::istreambuf_iterator<char>())
				);
		}
		fin.close();
		return true;
	}

	// get version string
	const char* ShaderDictionary::GetVersionString(CompileVersion version){
		switch (version){
		case VERSION_330:
		default:
			return "#version 330\r\n";

		case VERSION_400:
			return "#version 400\r\n";
		case VERSION_410:
			return "#version 410\r\n";
		case VERSION_420:
			return "#version 420\r\n";
		case VERSION_430:
			return "#version 430\r\n";
		}
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
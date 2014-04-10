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

		// read vertex shader sources
		std::list<std::string> sourceBuf;
		if (!ReadSources(vertexShaderFiles, sourceBuf)){
			glDeleteProgram(progObj);
			return false;
		}
		// create vertex shader
		ObjectHandle vtxObj = CreateShader(GL_VERTEX_SHADER, version, sourceBuf);
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
		ObjectHandle fragObj = CreateShader(GL_FRAGMENT_SHADER, version, sourceBuf);
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

		// create vertex shader
		std::list<std::string> sourceBuf(vertexShaderSources.begin(), vertexShaderSources.end());
		ObjectHandle vtxObj = CreateShader(GL_VERTEX_SHADER, version, sourceBuf);
		if (vtxObj == 0){
			glDeleteProgram(progObj);
			return false;
		}

		// create fragment shader
		sourceBuf.clear();
		sourceBuf.assign(fragmentShaderSources.begin(), fragmentShaderSources.end());
		ObjectHandle fragObj = CreateShader(GL_FRAGMENT_SHADER, version, sourceBuf);
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
		CompileVersion version, std::list<std::string>& sources)
	{
		switch (version){
		case VERSION_330:
			sources.push_front("#version 330\r\n");
			break;
		case VERSION_400:
			sources.push_front("#version 400\r\n");
			break;
		case VERSION_410:
			sources.push_front("#version 410\r\n");
			break;
		case VERSION_420:
			sources.push_front("#version 420\r\n");
			break;
		case VERSION_430:
			sources.push_front("#version 430\r\n");
			break;
		default:
			return 0;
		}

		ObjectHandle newShaderHandle = glCreateShader(shaderType);
		if (newShaderHandle == 0)
			return 0;

		std::vector<const char*> raw_vec;
		for (std::list<std::string>::iterator itr = sources.begin(); itr != sources.end(); itr++){
			raw_vec.push_back(itr->c_str());
		}
		glShaderSource(newShaderHandle, raw_vec.size(), (const char**)(&raw_vec[0]), NULL);

		glCompileShader(newShaderHandle);

		if (!CheckShader(newShaderHandle)){
			return 0;
		}

		return newShaderHandle;
	}

	// read shader string sources from files
	bool ShaderDictionary::ReadSources(const std::vector<const char*>& files,
		std::list<std::string>& buffer)
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
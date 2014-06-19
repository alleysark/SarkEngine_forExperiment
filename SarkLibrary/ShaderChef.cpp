#include "ShaderChef.h"
#include "ShaderProgram.h"
#include "Debug.h"
#include <fstream>

namespace sark{

	ShaderChef::ShaderChef(){}
	ShaderChef::ShaderChef(const ShaderChef&){}

	ShaderChef::~ShaderChef(){}

	ShaderChef* ShaderChef::instance = NULL;

	ShaderChef* ShaderChef::GetInstance(){
		if (instance == NULL)
			instance = new ShaderChef();
		return instance;
	}

	// register shader source string.
	bool ShaderChef::RegisterShaderSource(const std::string& name,
		const std::string& source)
	{
		IngredientDict::iterator itr = mIngredients.find(name);
		if (itr != mIngredients.end())
			return false;

		mIngredients[name] = source;
		return true;
	}

	// register shader source from file
	bool ShaderChef::RegisterShaderSourceFromFile(const std::string& name,
		const std::string& filename)
	{
		IngredientDict::iterator itr = mIngredients.find(name);
		if (itr != mIngredients.end())
			return false;

		std::ifstream fin;
		fin.open(filename, std::ios::beg);
		if (!fin.is_open()){
			return false;
		}
		
		mIngredients[name] = std::string((std::istreambuf_iterator<char>(fin)),
			std::istreambuf_iterator<char>());
		fin.close();
		return true;
	}

	// cook a shader program from registered shader source ingredients.
	// whole shaders are compiled as 'version'. shader objects are attached
	// and are linked into program. it'll return NULL if it failed.
	ShaderProgram* ShaderChef::MakeProgram(CompileVersion version,
		const std::vector<AttributePair>& bindingAttrs,
		const std::vector<const char*>& vertexSourceNames,
		const std::vector<const char*>& fragmentSourceNames)
	{
		#define CLEAR_OBJS(pobj, vobj, fobj) do{\
				if (pobj != 0) glDeleteProgram(pobj); \
				if (vobj != 0) glDeleteShader(vobj); \
				if (fobj != 0) glDeleteShader(fobj); \
			}while (0)

		// create program object
		ObjectHandle progObj = glCreateProgram();
		if (progObj == 0)
			return NULL;

		// create vertex shader object
		ObjectHandle vtxObj = CreateShader(GL_VERTEX_SHADER, version, vertexSourceNames);
		if (vtxObj == 0){
			CLEAR_OBJS(progObj, vtxObj, 0);
			return NULL;
		}

		// create fragment shader object
		ObjectHandle fragObj = CreateShader(GL_FRAGMENT_SHADER, version, fragmentSourceNames);
		if (fragObj == 0){
			CLEAR_OBJS(progObj, vtxObj, fragObj);
			return NULL;
		}

		// attach shader object
		glAttachShader(progObj, vtxObj);
		glAttachShader(progObj, fragObj);

		// bind pre-defined attribute semantics
		// and copy the sementic list.
		uinteger bndAttrSz = bindingAttrs.size();
		ShaderProgram::AttributeList attrs(bndAttrSz);
		for (uinteger i = 0; i < bndAttrSz; i++){
			glBindAttribLocation(progObj, bindingAttrs[i].semantic, bindingAttrs[i].name);
			attrs.push_back(bindingAttrs[i].semantic);
		}

		// link program and check link error
		glLinkProgram(progObj);
		if (!CheckProgram(progObj)){
			CLEAR_OBJS(progObj, vtxObj, fragObj);
			return NULL;
		}

		// detach and delete shader objects
		// to clarify shader object deletion.
		glDetachShader(progObj, vtxObj);
		glDetachShader(progObj, fragObj);
		glDeleteShader(vtxObj);
		glDeleteShader(fragObj);

		ShaderProgram* shaderProg = new ShaderProgram(progObj, attrs);
		return shaderProg;
		#undef CLEAR_OBJS
	}

	// cook a shader program from registered shader source ingredients.
	// whole shaders are compiled as 'version'. shader objects are attached
	// and are linked into program. it'll return NULL if it failed.
	ShaderProgram* ShaderChef::MakeProgram(CompileVersion version,
		const std::vector<AttributePair>& bindingAttrs,
		const std::vector<const char*>& vertexSourceNames,
		const std::vector<const char*>& geometrySourceNames,
		const std::vector<const char*>& fragmentSourceNames)
	{
		#define CLEAR_OBJS(pobj, vobj, gobj, fobj) do{\
				if (pobj != 0) glDeleteProgram(pobj); \
				if (vobj != 0) glDeleteShader(vobj); \
				if (gobj != 0) glDeleteShader(gobj); \
				if (fobj != 0) glDeleteShader(fobj); \
			}while (0)

		// create program object
		ObjectHandle progObj = glCreateProgram();
		if (progObj == 0)
			return NULL;

		// create vertex shader object
		ObjectHandle vtxObj = CreateShader(GL_VERTEX_SHADER, version, vertexSourceNames);
		if (vtxObj == 0){
			CLEAR_OBJS(progObj, vtxObj, 0, 0);
			return NULL;
		}

		// create geometry shader object
		ObjectHandle geoObj = CreateShader(GL_GEOMETRY_SHADER, version, geometrySourceNames);
		if (geoObj != 0){
			CLEAR_OBJS(progObj, vtxObj, geoObj, 0);
		}

		// create fragment shader object
		ObjectHandle fragObj = CreateShader(GL_FRAGMENT_SHADER, version, fragmentSourceNames);
		if (fragObj == 0){
			CLEAR_OBJS(progObj, vtxObj, geoObj, fragObj);
			return NULL;
		}

		// attach shader object
		glAttachShader(progObj, vtxObj);
		glAttachShader(progObj, geoObj);
		glAttachShader(progObj, fragObj);

		// bind pre-defined attribute semantics
		// and copy the sementic list.
		uinteger bndAttrSz = bindingAttrs.size();
		ShaderProgram::AttributeList attrs(bndAttrSz);
		for (uinteger i = 0; i < bndAttrSz; i++){
			glBindAttribLocation(progObj, bindingAttrs[i].semantic, bindingAttrs[i].name);
			attrs.push_back(bindingAttrs[i].semantic);
		}

		// link program and check link error
		glLinkProgram(progObj);
		if (!CheckProgram(progObj)){
			CLEAR_OBJS(progObj, vtxObj, geoObj, fragObj);
			return NULL;
		}

		// detach and delete shader objects
		// to clarify shader object deletion.
		glDetachShader(progObj, vtxObj);
		glDetachShader(progObj, geoObj);
		glDetachShader(progObj, fragObj);
		glDeleteShader(vtxObj);
		glDeleteShader(geoObj);
		glDeleteShader(fragObj);

		ShaderProgram* shaderProg = new ShaderProgram(progObj, attrs);
		return shaderProg;
		#undef CLEAR_OBJS
	}

	// clear all registered shader sources.
	void ShaderChef::Clear(){
		mIngredients.clear();
	}

	// create shader object
	ObjectHandle ShaderChef::CreateShader(GLenum shaderType,
		const CompileVersion& version, const std::vector<const char*>& sourceNames)
	{
		// create shader object
		ObjectHandle shObj = glCreateShader(shaderType);
		if (shObj == 0)
			return 0;

		std::vector<const char*>::const_iterator itr;
		std::vector<const char*>::const_iterator end;
		IngredientDict::const_iterator findItr;
		
		// construct source chunk
		uinteger count = sourceNames.size() + 1;
		const char** sources = new const char*[count];
		sources[0] = GetVersionString(version);
		itr = sourceNames.begin();
		end = sourceNames.end();
		for (uinteger i = 1; itr != end; itr++, i++){
			findItr = mIngredients.find((*itr));
			if (findItr == mIngredients.cend()){
				delete[] sources;
				glDeleteShader(shObj);
				return 0;
			}
			sources[i] = findItr->second.c_str();
		}

		// set shader source
		glShaderSource(shObj, count, sources, NULL);
		delete[] sources;

		// compile shader object
		glCompileShader(shObj);

		// check compilation error
		if (!CheckShader(shObj)){
			glDeleteShader(shObj);
			return 0;
		}

		return shObj;
	}

	// get version string
	const char* ShaderChef::GetVersionString(const CompileVersion& version){
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
	bool ShaderChef::CheckShader(ObjectHandle obj){
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
	bool ShaderChef::CheckProgram(ObjectHandle obj){
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
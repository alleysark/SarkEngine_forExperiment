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

	// cook a shader program from given recipe.
	// whole shaders are compiled as 'version'. shader objects are attached
	// and are linked into program. it'll return NULL if it failed.
	ShaderProgram* ShaderChef::CookShaderProgram(const Recipe& recipe){
		ObjectHandle progObj = 0,
			vtxObj = 0, tcsObj = 0, tesObj = 0, geoObj = 0, fragObj = 0;

		#define CLEAR_SHOBJS() do{\
				if (vtxObj != 0) glDeleteShader(vtxObj); \
				if (tcsObj != 0) glDeleteShader(tcsObj); \
				if (tesObj != 0) glDeleteShader(tesObj); \
				if (geoObj != 0) glDeleteShader(geoObj); \
				if (fragObj != 0) glDeleteShader(fragObj); \
			}while (0)

		// create program object
		progObj = glCreateProgram();
		if (progObj == 0)
			return NULL;

		// create vertex shader object
		if (recipe.vertexShader.empty()) {
			LogWarn("vertex shader is essential ingredient");
			glDeleteProgram(progObj);
			CLEAR_SHOBJS();
			return NULL;
		}

		vtxObj = CreateShader(GL_VERTEX_SHADER, recipe.vertexShader);
		if (vtxObj == 0){
			LogWarn("failed to create vertex shader");
			glDeleteProgram(progObj);
			CLEAR_SHOBJS();
			return NULL;
		}

		// create tessellation control shader
		if (!recipe.tessControlShader.empty()) {
			tcsObj = CreateShader(GL_TESS_CONTROL_SHADER, recipe.tessControlShader);
			if (tcsObj == 0) {
				LogWarn("failed to create tessellation control shader");
				glDeleteProgram(progObj);
				CLEAR_SHOBJS();
				return NULL;
			}
		}

		// create tessellation evaluation shader
		if (!recipe.tessEvaluationShader.empty()) {
			tesObj = CreateShader(GL_TESS_EVALUATION_SHADER, recipe.tessEvaluationShader);
			if (tesObj == 0) {
				LogWarn("failed to create tessellation evaluation shader");
				glDeleteProgram(progObj);
				CLEAR_SHOBJS();
				return NULL;
			}
		}

		// create geometry shader
		if (!recipe.geometryShader.empty()) {
			geoObj = CreateShader(GL_GEOMETRY_SHADER, recipe.geometryShader);
			if (geoObj == 0) {
				LogWarn("failed to create geometry shader");
				glDeleteProgram(progObj);
				CLEAR_SHOBJS();
				return NULL;
			}
		}

		// create fragment shader object
		fragObj = CreateShader(GL_FRAGMENT_SHADER, recipe.fragmentShader);
		if (fragObj == 0){
			LogWarn("failed to create fragment shader");
			glDeleteProgram(progObj);
			CLEAR_SHOBJS();
			return NULL;
		}

		// attach shader objects
		glAttachShader(progObj, vtxObj);
		if (tcsObj != 0) glAttachShader(progObj, tcsObj);
		if (tesObj != 0) glAttachShader(progObj, tesObj);
		if (geoObj != 0) glAttachShader(progObj, geoObj);
		glAttachShader(progObj, fragObj);

		// bind pre-defined attribute semantics
		// and copy the sementic list.
		uinteger attribSz = recipe.attributes.size();
		ShaderProgram::AttributeList attrs(attribSz);
		for (uinteger i = 0; i < attribSz; i++){
			glBindAttribLocation(progObj, recipe.attributes[i].semantic, recipe.attributes[i].name);
			attrs.push_back(recipe.attributes[i].semantic);
		}

		// link program and check link error
		glLinkProgram(progObj);
		if (!CheckProgram(progObj)){
			glDeleteProgram(progObj);
			CLEAR_SHOBJS(progObj, vtxObj, fragObj);
			return NULL;
		}

		// detach used shader objects
		glDetachShader(progObj, vtxObj);
		if (tcsObj != 0) glDetachShader(progObj, tcsObj);
		if (tesObj != 0) glDetachShader(progObj, tesObj);
		if (geoObj != 0) glDetachShader(progObj, geoObj);
		glDetachShader(progObj, fragObj);

		// clear all shader objects
		CLEAR_SHOBJS();

		ShaderProgram* shaderProg = new ShaderProgram(progObj, attrs);

		return shaderProg;
		#undef CLEAR_SHOBJS
	}

	// create shader object
	ObjectHandle ShaderChef::CreateShader(GLenum shaderType, const std::string& source) {
		// create shader object
		ObjectHandle shObj = glCreateShader(shaderType);
		if (shObj == 0)
			return 0;

		// set shader source
		const char* raw_str = source.c_str();
		glShaderSource(shObj, 1, &raw_str, NULL);

		// compile shader object
		glCompileShader(shObj);

		// check compilation error
		if (!CheckShader(shObj)){
			glDeleteShader(shObj);
			return 0;
		}

		return shObj;
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
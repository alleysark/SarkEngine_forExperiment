#include "ShaderProgram.h"
#include "Sampler.h"
#include "Texture.h"
#include "Debug.h"

namespace sark{

	const char* const ShaderProgram::AttribVarNames[] = {
		"position",
		"normal",
		"color",
		"texCoord0",
		"texCoord1",
		"texCoord2",
		"texCoord3",
		"indices"
	};

	// shader program is created from ShaderDictionary
	// with full liked shaders.
	ShaderProgram::ShaderProgram(ObjectHandle hProgram)
		: mhProgram(hProgram)
	{}

	ShaderProgram::~ShaderProgram(){
		glDeleteProgram(mhProgram);
		mhProgram = 0;
		mLocationDict.clear();
	}

	// before rendering, user should've Use() to use shader program.
	void ShaderProgram::Use(){
		glUseProgram(mhProgram);
	}

	// after using shader program, user should've Unuse()
	// to disable this shader program.
	void ShaderProgram::Unuse(){
		glUseProgram(0);
	}


	// get uniform location by variable name
	ShaderProgram::Location ShaderProgram::GetUniformLocation(const std::string& name){
		LocationDictionary::iterator find = mLocationDict.find(name);
		if (find != mLocationDict.end())
			return find->second;

		Location loca = glGetUniformLocation(mhProgram, name.c_str());
		if (loca == -1){
			LogWarn("inappropriate uniform variable name");
			return -1;
		}
		return (mLocationDict[name] = loca);
	}

	// get attribute location by variable name
	ShaderProgram::Location ShaderProgram::GetAttributeLocation(const std::string& name){
		LocationDictionary::iterator find = mLocationDict.find(name);
		if (find != mLocationDict.end())
			return find->second;
		
		Location loca = glGetAttribLocation(mhProgram, name.c_str());
		if (loca == -1){
			LogWarn("inappropriate attribute variable name");
			return -1;
		}
		return (mLocationDict[name] = loca);
	}




	//========================================================
	//		uniform/attribute variable setting methods
	//========================================================

	//---------------------
	//		sampler
	//---------------------

	// set sampler with texture into the uniform sampler 'name'
	// as active texture unit 'activeTex'
	void ShaderProgram::SetSampler(const std::string& name,
		Sampler* sampler, Texture* texture, uint16 activeTex){
		glActiveTexture(GL_TEXTURE0 + activeTex);
		texture->Bind();
		SetUniform(name, activeTex);
		sampler->Bind(activeTex);
	}


	//---------------------
	//		boolean
	//---------------------

	// set bool uniform value (it's size of 4-byte in glsl)
	void ShaderProgram::SetUniform(const std::string& name, bool val){
		glUniform1ui(GetUniformLocation(name), (val ? 1 : 0));
	}
	// set bool uniform array (it's size of 4-byte in glsl)
	void ShaderProgram::SetUniform(const std::string& name, bool vals[], uinteger count){
		uint32* buf = new uint32[count];
		for (uinteger i = 0; i < count; i++)
			buf[i] = (vals[i] ? 1 : 0);
		glUniform1uiv(GetUniformLocation(name), count, buf);
		delete[] buf;
	}


	//---------------------
	//		signed int
	//---------------------

	// set int uniform value
	void ShaderProgram::SetUniform(const std::string& name, int32 val){
		glUniform1i(GetUniformLocation(name), val);
	}
	// set int uniform array
	void ShaderProgram::SetUniform(const std::string& name, int32 vals[], uinteger count){
		glUniform1iv(GetUniformLocation(name), count, vals);
	}
	// set ivec2 uniform value
	void ShaderProgram::SetUniform(const std::string& name, int32 vecs[][2], uinteger count){
		glUniform2iv(GetUniformLocation(name), count, reinterpret_cast<const int32*>(vecs));
	}
	// set ivec3 uniform value
	void ShaderProgram::SetUniform(const std::string& name, int32 vecs[][3], uinteger count){
		glUniform3iv(GetUniformLocation(name), count, reinterpret_cast<const int32*>(vecs));
	}
	// set ivec4 uniform value
	void ShaderProgram::SetUniform(const std::string& name, int32 vecs[][4], uinteger count){
		glUniform4iv(GetUniformLocation(name), count, reinterpret_cast<const int32*>(vecs));
	}


	//---------------------
	//	   unsigned int
	//---------------------

	// set uint uniform value
	void ShaderProgram::SetUniform(const std::string& name, uint32 val){
		glUniform1ui(GetUniformLocation(name), val);
	}
	// set uint uniform array
	void ShaderProgram::SetUniform(const std::string& name, uint32 vals[], uinteger count){
		glUniform1uiv(GetUniformLocation(name), count, vals);
	}
	// set uvec2 uniform value
	void ShaderProgram::SetUniform(const std::string& name, uint32 vecs[][2], uinteger count){
		glUniform2uiv(GetUniformLocation(name), count, reinterpret_cast<const uint32*>(vecs));
	}
	// set uvec3 uniform value
	void ShaderProgram::SetUniform(const std::string& name, uint32 vecs[][3], uinteger count){
		glUniform3uiv(GetUniformLocation(name), count, reinterpret_cast<const uint32*>(vecs));
	}
	// set uvec4 uniform value
	void ShaderProgram::SetUniform(const std::string& name, uint32 vecs[][4], uinteger count){
		glUniform4uiv(GetUniformLocation(name), count, reinterpret_cast<const uint32*>(vecs));
	}

#ifndef SARKLIB_USING_DOUBLE
	//---------------------
	//		real
	//---------------------

	// set float uniform value
	void ShaderProgram::SetUniform(const std::string& name, real val){
		glUniform1f(GetUniformLocation(name), val);
	}
	// set float uniform array
	void ShaderProgram::SetUniform(const std::string& name, real vals[], uinteger count){
		glUniform1fv(GetUniformLocation(name), count, vals);
	}


	//---------------------
	//		vector
	//---------------------

	// set vec2 uniform value
	void ShaderProgram::SetUniform(const std::string& name, const Vector2& vec){
		glUniform2fv(GetUniformLocation(name), 1, vec.v);
	}
	// set vec2 uniform array
	void ShaderProgram::SetUniform(const std::string& name, const Vector2 vecs[], uinteger count){
		glUniform2fv(GetUniformLocation(name), count, static_cast<const real*>(vecs[0].v));
	}

	// set vec3 uniform value
	void ShaderProgram::SetUniform(const std::string& name, const Vector3& vec){
		glUniform3fv(GetUniformLocation(name), 1, vec.v);
	}
	// set vec3 uniform array
	void ShaderProgram::SetUniform(const std::string& name, const Vector3 vecs[], uinteger count){
		glUniform3fv(GetUniformLocation(name), count, static_cast<const real*>(vecs[0].v));
	}

	// set vec4 uniform value
	void ShaderProgram::SetUniform(const std::string& name, const Vector4& vec){
		glUniform4fv(GetUniformLocation(name), 1, vec.v);
	}
	// set vec4 uniform array
	void ShaderProgram::SetUniform(const std::string& name, const Vector4 vecs[], uinteger count){
		glUniform4fv(GetUniformLocation(name), count, static_cast<const real*>(vecs[0].v));
	}


	//---------------------
	//		matrix
	//---------------------

	// set mat3 uniform value
	void ShaderProgram::SetUniform(const std::string& name, const Matrix3& mat){
		glUniformMatrix3fv(GetUniformLocation(name), 1, true, mat.GetRawMatrix());
	}
	// set mat3 uniform array
	void ShaderProgram::SetUniform(const std::string& name, const Matrix3 mats[], uinteger count){
		glUniformMatrix3fv(GetUniformLocation(name), count, true, reinterpret_cast<const real*>(mats[0].m));
	}

	// set mat4 uniform value
	void ShaderProgram::SetUniform(const std::string& name, const Matrix4& mat){
		glUniformMatrix4fv(GetUniformLocation(name), 1, true, mat.GetRawMatrix());
	}
	// set mat4 uniform array
	void ShaderProgram::SetUniform(const std::string& name, const Matrix4 mats[], uinteger count){
		glUniformMatrix4fv(GetUniformLocation(name), count, true, reinterpret_cast<const real*>(mats[0].m));
	}
#else
	//---------------------
	//		real
	//---------------------

	// set double uniform value
	void ShaderProgram::SetUniform(const std::string& name, real val){
		glUniform1d(GetUniformLocation(name), val);
	}
	// set double uniform array
	void ShaderProgram::SetUniform(const std::string& name, real vals[], uinteger count){
		glUniform1dv(GetUniformLocation(name), count, vals);
	}


	//---------------------
	//		vector
	//---------------------

	// set dvec2 uniform value
	void ShaderProgram::SetUniform(const std::string& name, const Vector2& vec){
		glUniform2dv(GetUniformLocation(name), 1, vec.v);
	}
	// set dvec2 uniform array
	void ShaderProgram::SetUniform(const std::string& name, const Vector2 vecs[], uinteger count){
		glUniform2dv(GetUniformLocation(name), count, static_cast<const real*>(vecs[0].v));
	}

	// set dvec3 uniform value
	void ShaderProgram::SetUniform(const std::string& name, const Vector3& vec){
		glUniform3dv(GetUniformLocation(name), 1, vec.v);
	}
	// set dvec3 uniform array
	void ShaderProgram::SetUniform(const std::string& name, const Vector3 vecs[], uinteger count){
		glUniform3dv(GetUniformLocation(name), count, static_cast<const real*>(vecs[0].v));
	}

	// set dvec4 uniform value
	void ShaderProgram::SetUniform(const std::string& name, const Vector4& vec){
		glUniform4dv(GetUniformLocation(name), 1, vec.v);
	}
	// set dvec4 uniform array
	void ShaderProgram::SetUniform(const std::string& name, const Vector4 vecs[], uinteger count){
		glUniform4dv(GetUniformLocation(name), count, static_cast<const real*>(vecs[0].v));
	}


	//---------------------
	//		matrix
	//---------------------

	// set dmat3 uniform value
	void ShaderProgram::SetUniform(const std::string& name, const Matrix3& mat){
		glUniformMatrix3dv(GetUniformLocation(name), 1, true, mat.GetRawMatrix());
	}
	// set dmat3 uniform array
	void ShaderProgram::SetUniform(const std::string& name, const Matrix3 mats[], uinteger count){
		glUniformMatrix3dv(GetUniformLocation(name), count, true, reinterpret_cast<const real*>(mats[0].m));
	}

	// set dmat4 uniform value
	void ShaderProgram::SetUniform(const std::string& name, const Matrix4& mat){
		glUniformMatrix4dv(GetUniformLocation(name), 1, true, mat.GetRawMatrix());
	}
	// set dmat4 uniform array
	void ShaderProgram::SetUniform(const std::string& name, const Matrix4 mats[], uinteger count){
		glUniformMatrix4dv(GetUniformLocation(name), count, true, reinterpret_cast<const real*>(mats[0].m));
	}
#endif


	//========================================================
	//			attribute variable setting methods
	//========================================================

	//---------------------
	//		boolean
	//---------------------

	// set bool vertex attribute value (it's size of 4-byte in glsl)
	void ShaderProgram::SetAttrib(const std::string& name, bool val){
		glVertexAttribI1ui(GetAttributeLocation(name), (val ? 1 : 0));
	}


	//---------------------
	//		signed int
	//---------------------

	// set int vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, int32 val){
		glVertexAttribI1i(GetAttributeLocation(name), val);
	}
	// set ivec2 vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, int32 val1, int32 val2){
		glVertexAttribI2i(GetAttributeLocation(name), val1, val2);
	}
	// set ivec3 vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, int32 val1, int32 val2, int32 val3){
		glVertexAttribI3i(GetAttributeLocation(name), val1, val2, val3);
	}
	// set ivec4 vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, int32 val1, int32 val2, int32 val3, int32 val4){
		glVertexAttribI4i(GetAttributeLocation(name), val1, val2, val3, val4);
	}


	//---------------------
	//	   unsigned int
	//---------------------

	// set uint vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, uint32 val){
		glVertexAttribI1ui(GetAttributeLocation(name), val);
	}
	// set uvec2 vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, uint32 val1, uint32 val2){
		glVertexAttribI2ui(GetAttributeLocation(name), val1, val2);
	}
	// set uvec3 vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, uint32 val1, uint32 val2, uint32 val3){
		glVertexAttribI3ui(GetAttributeLocation(name), val1, val2, val3);
	}
	// set uvec4 vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, uint32 val1, uint32 val2, uint32 val3, uint32 val4){
		glVertexAttribI4ui(GetAttributeLocation(name), val1, val2, val3, val4);
	}

#ifndef SARKLIB_USING_DOUBLE
	//---------------------
	//		real
	//---------------------

	// set real vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, real val){
		glVertexAttrib1f(GetAttributeLocation(name), val);
	}


	//---------------------
	//		vector
	//---------------------

	// set vec2 vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, const Vector2& vec){
		glVertexAttrib2fv(GetAttributeLocation(name), vec.v);
	}

	// set vec3 vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, const Vector3& vec){
		glVertexAttrib3fv(GetAttributeLocation(name), vec.v);
	}

	// set vec4 vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, const Vector4& vec){
		glVertexAttrib4fv(GetAttributeLocation(name), vec.v);
	}
#else
	//---------------------
	//		real
	//---------------------

	// set real vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, real val){
		glVertexAttribL1d(GetAttributeLocation(name), val);
	}


	//---------------------
	//		vector
	//---------------------

	// set vec2 vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, const Vector2& vec){
		glVertexAttribL2dv(GetAttributeLocation(name), vec.v);
	}

	// set vec3 vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, const Vector3& vec){
		glVertexAttribL3dv(GetAttributeLocation(name), vec.v);
	}

	// set vec4 vertex attribute value
	void ShaderProgram::SetAttrib(const std::string& name, const Vector4& vec){
		glVertexAttribL4dv(GetAttributeLocation(name), vec.v);
	}
#endif
	
}
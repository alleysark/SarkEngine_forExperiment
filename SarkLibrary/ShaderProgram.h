#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include <string>
#include <map>
#include <GL/glew.h>
#include "core.h"

namespace sark{

	class ShaderDictionary;

	class Sampler;
	class Texture;

	// programmable shader program.
	// it'll be created by ShaderDictionary 
	class ShaderProgram{
	public:	
		typedef GLint Location;
		typedef std::map<const std::string, Location> LocationDictionary;

		enum AttribVarSemantic{
			ATTR_POSITION=0,
			ATTR_NORMAL,
			ATTR_COLOR,
			ATTR_TEX_COORD0,
			ATTR_TEX_COORD1,
			ATTR_TEX_COORD2,
			ATTR_TEX_COORD3,
			ATTR_INDICES,

			//for counting semantic.
			ATTR_COUNT
		};
		static const char* const AttribVarNames[];

	private:
		// program handler
		ObjectHandle mhProgram;

		// locations of uniform/attribute variable are cached in this container
		LocationDictionary mLocationDict;

	public:
		// shader program is created from ShaderDictionary
		// with full liked shaders.
		ShaderProgram(ObjectHandle hProgram);

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
		//========================================================
		//			uniform variable setting methods
		//========================================================

		//---------------------
		//		sampler
		//---------------------

		// set sampler with texture into the uniform sampler 'name'
		// as active texture unit 'activeTex'
		void SetSampler(const std::string& name,
			Sampler* sampler, Texture* texture, uint16 activeTex = 0);


		//---------------------
		//		boolean
		//---------------------

		// set bool uniform value (it's size of 4-byte in glsl)
		void SetUniform(const std::string& name, bool val);
		// set bool uniform array (it's size of 4-byte in glsl)
		void SetUniform(const std::string& name, bool vals[], uinteger count);


		//---------------------
		//		signed int
		//---------------------

		// set int uniform value
		void SetUniform(const std::string& name, int32 val);
		// set int uniform array
		void SetUniform(const std::string& name, int32 vals[], uinteger count);
		// set ivec2 uniform value
		void SetUniform(const std::string& name, int32 vecs[][2], uinteger count);
		// set ivec3 uniform value
		void SetUniform(const std::string& name, int32 vecs[][3], uinteger count);
		// set ivec4 uniform value
		void SetUniform(const std::string& name, int32 vecs[][4], uinteger count);


		//---------------------
		//	   unsigned int
		//---------------------

		// set uint uniform value
		void SetUniform(const std::string& name, uint32 val);
		// set uint uniform array
		void SetUniform(const std::string& name, uint32 vals[], uinteger count);
		// set uvec2 uniform value
		void SetUniform(const std::string& name, uint32 vecs[][2], uinteger count);
		// set uvec3 uniform value
		void SetUniform(const std::string& name, uint32 vecs[][3], uinteger count);
		// set uvec4 uniform value
		void SetUniform(const std::string& name, uint32 vecs[][4], uinteger count);


		//---------------------
		//		real
		//---------------------

		// set real uniform value
		void SetUniform(const std::string& name, real val);
		// set real uniform array
		void SetUniform(const std::string& name, real vals[], uinteger count);


		//---------------------
		//		vector
		//---------------------

		// set vec2 uniform value
		void SetUniform(const std::string& name, const Vector2& vec);
		// set vec2 uniform array
		void SetUniform(const std::string& name, const Vector2 vecs[], uinteger count);

		// set vec3 uniform value
		void SetUniform(const std::string& name, const Vector3& vec);
		// set vec3 uniform array
		void SetUniform(const std::string& name, const Vector3 vecs[], uinteger count);

		// set vec4 uniform value
		void SetUniform(const std::string& name, const Vector4& vec);
		// set vec4 uniform array
		void SetUniform(const std::string& name, const Vector4 vecs[], uinteger count);


		//---------------------
		//		matrix
		//---------------------

		// set mat3 uniform value
		void SetUniform(const std::string& name, const Matrix3& mat);
		// set mat3 uniform array
		void SetUniform(const std::string& name, const Matrix3 mats[], uinteger count);

		// set mat4 uniform value
		void SetUniform(const std::string& name, const Matrix4& mat);
		// set mat4 uniform array
		void SetUniform(const std::string& name, const Matrix4 mats[], uinteger count);



		//========================================================
		//			attribute variable setting methods
		//========================================================

		//---------------------
		//		boolean
		//---------------------

		// set bool vertex attribute value (it's size of 4-byte in glsl)
		void SetAttrib(const std::string& name, bool val);


		//---------------------
		//		signed int
		//---------------------

		// set int vertex attribute value
		void SetAttrib(const std::string& name, int32 val);
		// set ivec2 vertex attribute value
		void SetAttrib(const std::string& name, int32 val1, int32 val2);
		// set ivec3 vertex attribute value
		void SetAttrib(const std::string& name, int32 val1, int32 val2, int32 val3);
		// set ivec4 vertex attribute value
		void SetAttrib(const std::string& name, int32 val1, int32 val2, int32 val3, int32 val4);


		//---------------------
		//	   unsigned int
		//---------------------

		// set uint vertex attribute value
		void SetAttrib(const std::string& name, uint32 val);
		// set uvec2 vertex attribute value
		void SetAttrib(const std::string& name, uint32 val1, uint32 val2);
		// set uvec3 vertex attribute value
		void SetAttrib(const std::string& name, uint32 val1, uint32 val2, uint32 val3);
		// set uvec4 vertex attribute value
		void SetAttrib(const std::string& name, uint32 val1, uint32 val2, uint32 val3, uint32 val4);


		//---------------------
		//		real
		//---------------------

		// set real vertex attribute value
		void SetAttrib(const std::string& name, real val);


		//---------------------
		//		vector
		//---------------------

		// set vec2 vertex attribute value
		void SetAttrib(const std::string& name, const Vector2& vec);
		
		// set vec3 vertex attribute value
		void SetAttrib(const std::string& name, const Vector3& vec);
		
		// set vec4 vertex attribute value
		void SetAttrib(const std::string& name, const Vector4& vec);

	};

}
#endif
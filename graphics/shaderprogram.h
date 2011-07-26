#ifndef _SHADER_PROGRAM_INCLUDED_
#define _SHADER_PROGRAM_INCLUDED_

#include <vector>
#include "shader.h"

/// ShaderProgram class
class ShaderProgram
{
	private:
		GLuint _id;
		std::vector<Shader *> _shaders;
	public:
		ShaderProgram();
		ShaderProgram(const char *vp_filename, const char *fp_filename);
		~ShaderProgram();
		void AttachShader(Shader *sh);
		void Link();
		void Use();
		void Uniform(const char* name, GLint value);
		void Uniform(const char* name, GLfloat value);
		void Uniform(const char* name, GLfloat v0, GLfloat v1);
		void Uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2);
		void Uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		void Uniform(const char* name, GLsizei count, const GLfloat *array);
		void Uniform3v(const char* name, const GLfloat *array);
		void UniformMatrix3(const char* name, const GLfloat *array);
		void Uniform4v(const char* name, const GLfloat *array);
		void UniformMatrix4(const char* name, const GLfloat *array);
		void PrintInfoLog();
};

#endif


#ifndef _SHADER_PROGRAM_INCLUDED_
#define _SHADER_PROGRAM_INCLUDED_

#include <vector>
#include "shader.h"

/// ShaderProgram class
class ShaderProgram
{
	private:
		GLuint m_id;
		std::vector<Shader *> m_shaders;
	public:
		ShaderProgram();
		ShaderProgram(const char *vp_filename, const char *fp_filename);
		~ShaderProgram();
		void attachShader(Shader *sh);
		void link();
		void use();
		void uniform(const char* name, GLint value);
		void uniform(const char* name, GLfloat value);
		void uniform(const char* name, GLfloat v0, GLfloat v1);
		void uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2);
		void uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		void uniform(const char* name, GLsizei count, const GLfloat *array);
		void uniform3v(const char* name, const GLfloat *array);
		void uniformMatrix3(const char* name, const GLfloat *array);
		void uniform4v(const char* name, const GLfloat *array);
		void uniformMatrix4(const char* name, const GLfloat *array);
		void printInfoLog();
};

#endif


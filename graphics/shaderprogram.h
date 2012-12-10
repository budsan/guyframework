#pragma once

#include <vector>
#include "shader.h"

#include "color.h"
#include "math/algebra3.h"

namespace Guy {

class ShaderProgram
{
	friend class Effect;

	protected:
		GLuint m_id;
	public:
		ShaderProgram();
		ShaderProgram(const char *vp_filename, const char *fp_filename);
		~ShaderProgram();

		void attachShader(Shader &sh);
		bool link();

		void bind();
		void unbind();

		void uniform(const char* name, GLint value);
		void uniform(const char* name, GLfloat value);
		void uniform(const char* name, GLfloat v0, GLfloat v1);
		void uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2);
		void uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		void uniform(const char* name, GLsizei count, const GLfloat *array);

		void uniform(const char *name, const rgb&  c);
		void uniform(const char *name, const rgba& c);

		void uniform(const char* name, const math::vec2f &v);
		void uniform(const char* name, const math::vec3f &v);
		void uniform(const char* name, const math::vec4f &v);

		void uniform3v(const char* name, const GLfloat *array);
		void uniformMatrix3(const char* name, const GLfloat *array);
		void uniform4v(const char* name, const GLfloat *array);
		void uniformMatrix4(const char* name, const GLfloat *array);

		void uniform(const char* name, const math::mat3f &m);
		void uniform(const char* name, const math::mat4f &m);

		void printInfoLog();
};

} // namespace Guy

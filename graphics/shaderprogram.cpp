#include <vector>

#include "graphics.h"
#include "shaderprogram.h"
#include "shader.h"

#include <iostream> // only for debug

namespace Guy {

ShaderProgram::ShaderProgram()
{
	GL_ASSERT(m_id = glCreateProgram());
}

ShaderProgram::ShaderProgram(const char *vp_filename, const char *fp_filename)
{
	GUY_ASSERT(vp_filename);
	GUY_ASSERT(fp_filename);
	GL_ASSERT(m_id = glCreateProgram());

	Shader vertex(GL_VERTEX_SHADER);
	vertex.load(vp_filename);
	if (!vertex.compile())
	{
		vertex.printInfoLog();
		GUY_ERROR("Compile failed for vertex shader '%s'.", vp_filename);
	}

	Shader fragment(GL_FRAGMENT_SHADER);
	fragment.load(fp_filename);
	if (!fragment.compile())
	{
		fragment.printInfoLog();
		GUY_ERROR("Compile failed for vertex shader '%s'.", fp_filename);
	}

	attachShader(vertex);
	attachShader(fragment);

	if (!link())
	{
		printInfoLog();
		GUY_ERROR("Linking program failed (%s,%s).", vp_filename, fp_filename);
	}
}


ShaderProgram::~ShaderProgram()
{
	GL_ASSERT(glDeleteProgram(m_id));
}

void ShaderProgram::attachShader(Shader &sh)
{
	sh.attach(m_id);
}

bool ShaderProgram::link()
{
	GLint success;
	GL_ASSERT(glLinkProgram(m_id));
	GL_ASSERT(glGetProgramiv(m_id, GL_LINK_STATUS, &success) );
	return success == GL_TRUE;
}

void ShaderProgram::bind()
{
	GL_ASSERT(glUseProgram(m_id));
}

void ShaderProgram::unbind()
{
	GL_ASSERT(glUseProgram(0));
}

void ShaderProgram::uniform(const char* name, GLint value)
{
	GLint loc, oldprog;

	GL_ASSERT(glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog));
	GL_ASSERT(glUseProgram(m_id));

	GL_ASSERT(loc = glGetUniformLocation(m_id, name));
	GL_ASSERT(glUniform1i(loc, value));

	GL_ASSERT(glUseProgram(oldprog));
}


void ShaderProgram::uniform(const char* name, GLfloat value)
{
	GLint loc, oldprog;

	GL_ASSERT(glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog));
	GL_ASSERT(glUseProgram(m_id));

	GL_ASSERT(loc = glGetUniformLocation(m_id, name));
	GL_ASSERT(glUniform1f(loc, value));

	GL_ASSERT(glUseProgram(oldprog));
}


void ShaderProgram::uniform(const char* name, GLfloat v0, GLfloat v1)
{
	GLint loc, oldprog;

	GL_ASSERT(glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog));
	GL_ASSERT(glUseProgram(m_id));

	GL_ASSERT(loc = glGetUniformLocation(m_id, name));
	GL_ASSERT(glUniform2f(loc, v0, v1));

	GL_ASSERT(glUseProgram(oldprog));
}


void ShaderProgram::uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2)
{
	GLint loc, oldprog;

	GL_ASSERT(glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog));
	GL_ASSERT(glUseProgram(m_id));

	GL_ASSERT(loc = glGetUniformLocation(m_id, name));
	GL_ASSERT(glUniform3f(loc, v0, v1, v2));

	GL_ASSERT(glUseProgram(oldprog));
}


void ShaderProgram::uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	GLint loc, oldprog;

	GL_ASSERT(glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog));
	GL_ASSERT(glUseProgram(m_id));

	GL_ASSERT(loc = glGetUniformLocation(m_id, name));
	GL_ASSERT(glUniform4f(loc, v0, v1, v2, v3));

	GL_ASSERT(glUseProgram(oldprog));
}


void ShaderProgram::uniform(const char* name, GLsizei count, const GLfloat *array)
{ 
	GLint loc, oldprog;

	GL_ASSERT(glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog));
	GL_ASSERT(glUseProgram(m_id));

	GL_ASSERT(loc = glGetUniformLocation(m_id, name));
	GL_ASSERT(glUniform1fv(loc, count, array));

	GL_ASSERT(glUseProgram(oldprog));
}

void ShaderProgram::uniform(const char *name, const rgb&  c)
{
	uniform3v(name, c.raw());
}

void ShaderProgram::uniform(const char *name, const rgba& c)
{
	uniform4v(name, c.raw());
}

void ShaderProgram::uniform(const char* name, const math::vec2f &v)
{
	uniform(name, (GLfloat) v.x, (GLfloat) v.y);
}

void ShaderProgram::uniform(const char* name, const math::vec3f &v)
{
	uniform(name, (GLfloat) v.x, (GLfloat) v.y, (GLfloat) v.z);
}

void ShaderProgram::uniform(const char* name, const math::vec4f &v)
{
	uniform(name, (GLfloat) v.x, (GLfloat) v.y, (GLfloat) v.z, (GLfloat) v.w);
}


void ShaderProgram::uniform3v(const char* name, const GLfloat *array)
{
	GLint loc, oldprog;

	GL_ASSERT(glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog));
	GL_ASSERT(glUseProgram(m_id));

	GL_ASSERT(loc = glGetUniformLocation(m_id, name));
	GL_ASSERT(glUniform3fv(loc, 1, array));

	GL_ASSERT(glUseProgram(oldprog));
}


void ShaderProgram::uniformMatrix3(const char* name, const GLfloat *array)
{
	GLint loc, oldprog;

	GL_ASSERT(glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog));
	GL_ASSERT(glUseProgram(m_id));

	GL_ASSERT(loc = glGetUniformLocation(m_id, name));
	GL_ASSERT(glUniformMatrix3fv(loc, 1, 0, array));

	GL_ASSERT(glUseProgram(oldprog));
}


void ShaderProgram::uniform4v(const char* name, const GLfloat *array)
{
	GLint loc, oldprog;

	GL_ASSERT(glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog));
	GL_ASSERT(glUseProgram(m_id));

	GL_ASSERT(loc = glGetUniformLocation(m_id, name));
	GL_ASSERT(glUniform4fv(loc, 1, array));

	GL_ASSERT(glUseProgram(oldprog));
}


void ShaderProgram::uniformMatrix4(const char* name, const GLfloat *array)
{
	GLint loc, oldprog;

	GL_ASSERT(glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog));
	GL_ASSERT(glUseProgram(m_id));

	GL_ASSERT(loc = glGetUniformLocation(m_id, name));
	GL_ASSERT(glUniformMatrix4fv(loc, 1, 0, array));

	GL_ASSERT(glUseProgram(oldprog));
}

void ShaderProgram::uniform(const char* name, const math::mat3f &m)
{
	uniformMatrix3(name, m.v);
}

void ShaderProgram::uniform(const char* name, const math::mat4f &m)
{
	uniformMatrix4(name, m.v);
}

void ShaderProgram::printInfoLog()
{
	int length = 0;
	char *infoLog;

	GL_ASSERT(glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length));
	if (length > 1) {
		infoLog = new char[length];
		GL_ASSERT(glGetProgramInfoLog(m_id, length, NULL, infoLog));
		printLog(infoLog);
		delete[] infoLog;
	}
}

} //namespace Guy

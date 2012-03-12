#include <vector>

#include "graphics.h"
#include "shaderprogram.h"
#include "shader.h"

#include <iostream> // only for debug
ShaderProgram::ShaderProgram()
{
	m_id = glCreateProgram();
}

ShaderProgram::ShaderProgram(const char *vp_filename, const char *fp_filename)
{
	m_id = glCreateProgram();

	Shader *vertex = new Shader(GL_VERTEX_SHADER);
	Shader *fragment = new Shader(GL_FRAGMENT_SHADER);

	vertex->load(vp_filename);
	vertex->compile();
//	vertex->PrintInfoLog(); // redundant info on nvidia
	fragment->load(fp_filename);
	fragment->compile();
//	fragment->PrintInfoLog(); // redundant info on nvidia
	attachShader(vertex);
	attachShader(fragment);
	link();
	printInfoLog();
}


ShaderProgram::~ShaderProgram()
{
	std::vector<Shader *>::const_iterator cii;
	for(cii = m_shaders.begin(); cii != m_shaders.end(); cii++) {
		delete *cii;
	}

	glDeleteProgram(m_id);
}


void ShaderProgram::attachShader(Shader *sh)
{
	m_shaders.push_back(sh);
	sh->attach(m_id);
}


void ShaderProgram::link()
{
	glLinkProgram(m_id);
}


void ShaderProgram::use()
{
	glUseProgram(m_id);
}


void ShaderProgram::uniform(const char* name, GLint value)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(m_id);

	loc = glGetUniformLocation(m_id, name);
	glUniform1i(loc, value);

	glUseProgram(oldprog);
}


void ShaderProgram::uniform(const char* name, GLfloat value)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(m_id);

	loc = glGetUniformLocation(m_id, name);
	glUniform1f(loc, value);

	glUseProgram(oldprog);
}


void ShaderProgram::uniform(const char* name, GLfloat v0, GLfloat v1)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(m_id);

	loc = glGetUniformLocation(m_id, name);
	glUniform2f(loc, v0, v1);

	glUseProgram(oldprog);
}


void ShaderProgram::uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(m_id);

	loc = glGetUniformLocation(m_id, name);
	glUniform3f(loc, v0, v1, v2);

	glUseProgram(oldprog);
}


void ShaderProgram::uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(m_id);

	loc = glGetUniformLocation(m_id, name);
	glUniform4f(loc, v0, v1, v2, v3);

	glUseProgram(oldprog);
}


void ShaderProgram::uniform(const char* name, GLsizei count, const GLfloat *array)
{ 
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(m_id);

	loc = glGetUniformLocation(m_id, name);
	glUniform1fv(loc, count, array);

	glUseProgram(oldprog);
}


void ShaderProgram::uniform3v(const char* name, const GLfloat *array)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(m_id);

	loc = glGetUniformLocation(m_id, name);
	glUniform3fv(loc, 1, array);

	glUseProgram(oldprog);
}


void ShaderProgram::uniformMatrix3(const char* name, const GLfloat *array)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(m_id);

	loc = glGetUniformLocation(m_id, name);
	glUniformMatrix3fv(loc, 1, 0, array);

	glUseProgram(oldprog);
}


void ShaderProgram::uniform4v(const char* name, const GLfloat *array)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(m_id);

	loc = glGetUniformLocation(m_id, name);
	glUniform4fv(loc, 1, array);

	glUseProgram(oldprog);
}


void ShaderProgram::uniformMatrix4(const char* name, const GLfloat *array)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(m_id);

	loc = glGetUniformLocation(m_id, name);
	glUniformMatrix4fv(loc, 1, 0, array);

	glUseProgram(oldprog);
}


void ShaderProgram::printInfoLog()
{
	int length = 0;
	char *infoLog;

	glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);
	if (length > 1) {
		infoLog = new char[length];
		glGetProgramInfoLog(m_id, length, NULL, infoLog);
		std::cout << infoLog;
		delete[] infoLog;
	}
}

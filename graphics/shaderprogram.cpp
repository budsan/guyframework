#include <vector>

#include "graphics.h"
#include "shaderprogram.h"
#include "shader.h"

#include <iostream> // only for debug
ShaderProgram::ShaderProgram()
{
	_id = glCreateProgram();
}

ShaderProgram::ShaderProgram(const char *vp_filename, const char *fp_filename)
{
	_id = glCreateProgram();

	Shader *vertex = new Shader(GL_VERTEX_SHADER);
	Shader *fragment = new Shader(GL_FRAGMENT_SHADER);

	vertex->Load(vp_filename);
	vertex->Compile();
//	vertex->PrintInfoLog(); // redundant info on nvidia
	fragment->Load(fp_filename);
	fragment->Compile();
//	fragment->PrintInfoLog(); // redundant info on nvidia
	AttachShader(vertex);
	AttachShader(fragment);
	Link();
	PrintInfoLog();
}


ShaderProgram::~ShaderProgram()
{
	std::vector<Shader *>::const_iterator cii;
	for(cii = _shaders.begin(); cii != _shaders.end(); cii++) {
		delete *cii;
	}

	glDeleteProgram(_id);
}


void ShaderProgram::AttachShader(Shader *sh)
{
	_shaders.push_back(sh);
	sh->Attach(_id);
}


void ShaderProgram::Link()
{
	glLinkProgram(_id);
}


void ShaderProgram::Use()
{
	glUseProgram(_id);
}


void ShaderProgram::Uniform(const char* name, GLint value)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(_id);

	loc = glGetUniformLocation(_id, name);
	glUniform1i(loc, value);

	glUseProgram(oldprog);
}


void ShaderProgram::Uniform(const char* name, GLfloat value)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(_id);

	loc = glGetUniformLocation(_id, name);
	glUniform1f(loc, value);

	glUseProgram(oldprog);
}


void ShaderProgram::Uniform(const char* name, GLfloat v0, GLfloat v1)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(_id);

	loc = glGetUniformLocation(_id, name);
	glUniform2f(loc, v0, v1);

	glUseProgram(oldprog);
}


void ShaderProgram::Uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(_id);

	loc = glGetUniformLocation(_id, name);
	glUniform3f(loc, v0, v1, v2);

	glUseProgram(oldprog);
}


void ShaderProgram::Uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(_id);

	loc = glGetUniformLocation(_id, name);
	glUniform4f(loc, v0, v1, v2, v3);

	glUseProgram(oldprog);
}


void ShaderProgram::Uniform(const char* name, GLsizei count, const GLfloat *array)
{ 
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(_id);

	loc = glGetUniformLocation(_id, name);
	glUniform1fv(loc, count, array);

	glUseProgram(oldprog);
}


void ShaderProgram::Uniform3v(const char* name, const GLfloat *array)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(_id);

	loc = glGetUniformLocation(_id, name);
	glUniform3fv(loc, 1, array);

	glUseProgram(oldprog);
}


void ShaderProgram::UniformMatrix3(const char* name, const GLfloat *array)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(_id);

	loc = glGetUniformLocation(_id, name);
	glUniformMatrix3fv(loc, 1, 0, array);

	glUseProgram(oldprog);
}


void ShaderProgram::Uniform4v(const char* name, const GLfloat *array)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(_id);

	loc = glGetUniformLocation(_id, name);
	glUniform4fv(loc, 1, array);

	glUseProgram(oldprog);
}


void ShaderProgram::UniformMatrix4(const char* name, const GLfloat *array)
{
	GLint loc, oldprog;

	glGetIntegerv(GL_CURRENT_PROGRAM, &oldprog);
	glUseProgram(_id);

	loc = glGetUniformLocation(_id, name);
	glUniformMatrix4fv(loc, 1, 0, array);

	glUseProgram(oldprog);
}


void ShaderProgram::PrintInfoLog()
{
	int length = 0;
	char *infoLog;

	glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);
	if (length > 1) {
		infoLog = new char[length];
		glGetProgramInfoLog(_id, length, NULL, infoLog);
		std::cout << infoLog;
		delete[] infoLog;
	}
}

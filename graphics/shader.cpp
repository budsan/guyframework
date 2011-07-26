#include <iostream>
#include <fstream>
#include <stdexcept>

#include <GL/glew.h>
#include <GL/gl.h>

#include "shader.h"

Shader::Shader(GLenum type)
{
	_id = glCreateShader(type);
}


Shader::~Shader()
{
	glDeleteShader(_id);
}


void Shader::Compile() const
{
	glCompileShader(_id);
}


bool Shader::Load(const char *filename)
{
	int length;
	char *buffer;

	std::ifstream is;
	is.open(filename, std::ios::in);
	if (is.fail())
	{
		return false;
	}

	// get length of file
	is.seekg(0, std::ios::end);
	length = is.tellg();
	is.seekg(0, std::ios::beg);

	// allocate memory:
	buffer = new char[length+1];

	// read data as a block:
	is.read(buffer, length);
	is.close();

	buffer[length] = '\0';
	const char *source = buffer;
	glShaderSource(_id, 1, &source, NULL);

	delete[] buffer;

	return true;
}


void Shader::Attach(GLuint program) const
{
	glAttachShader(program, _id);
}


void Shader::PrintInfoLog() const
{
	int length = 0;
	char *infoLog;

	glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length);
	if (length > 1) {
		infoLog = new char[length];
		glGetShaderInfoLog(_id, length, NULL, infoLog);
		std::cout << infoLog;
		delete[] infoLog;
	}
}

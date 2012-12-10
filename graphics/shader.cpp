#include "shader.h"
#include "graphics.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

namespace Guy {

Shader::Shader(GLenum type)
{
	GL_ASSERT(m_id = glCreateShader(type));
}


Shader::~Shader()
{
	GL_ASSERT(glDeleteShader(m_id));
}

bool Shader::load(const char *filename)
{
	int length;
	char *buffer;

	std::ifstream is;
	is.open(filename, std::ios::in);
	if (is.fail())
	{
		GUY_ERROR("Failed to read vertex shader from file '%s'.", filename);
		return false;
	}

	// get length of file
	is.seekg(0, std::ios::end);
	length = (int) is.tellg();
	is.seekg(0, std::ios::beg);

	// allocate memory:
	buffer = new char[length+1];

	// read data as a block:
	is.read(buffer, length);
	is.close();

	buffer[length] = '\0';
	const char *source = buffer;
	GL_ASSERT(glShaderSource(m_id, 1, &source, NULL));

	delete[] buffer;

	return true;
}

bool Shader::compile(const char *filename)
{
	if (load(filename)) return compile();
	return false;
}

bool Shader::compile() const
{
	GLint status;
	GL_ASSERT(glCompileShader(m_id));
	GL_ASSERT(glGetShaderiv(m_id, GL_COMPILE_STATUS, &status));
	return status == GL_TRUE;
}


void Shader::attach(GLuint program) const
{
	GL_ASSERT(glAttachShader(program, m_id));
}


void Shader::printInfoLog() const
{
	int length = 0;
	char *infoLog;

	GL_ASSERT(glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length));
	if (length > 1) {
		infoLog = new char[length];
		GL_ASSERT(glGetShaderInfoLog(m_id, length, NULL, infoLog));
		printLog(infoLog);
		delete[] infoLog;
	}
}

} //namespace Guy

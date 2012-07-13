#include "shader.h"
#include "graphics.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

namespace Guy {

Shader::Shader(GLenum type)
{
	m_id = glCreateShader(type);
}


Shader::~Shader()
{
	glDeleteShader(m_id);
}


void Shader::compile() const
{
	glCompileShader(m_id);
}


bool Shader::load(const char *filename)
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
	length = (int) is.tellg();
	is.seekg(0, std::ios::beg);

	// allocate memory:
	buffer = new char[length+1];

	// read data as a block:
	is.read(buffer, length);
	is.close();

	buffer[length] = '\0';
	const char *source = buffer;
	glShaderSource(m_id, 1, &source, NULL);

	delete[] buffer;

	return true;
}


void Shader::attach(GLuint program) const
{
	glAttachShader(program, m_id);
}


void Shader::printInfoLog() const
{
	int length = 0;
	char *infoLog;

	glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);
	if (length > 1) {
		infoLog = new char[length];
		glGetShaderInfoLog(m_id, length, NULL, infoLog);
		std::cout << infoLog;
		delete[] infoLog;
	}
}

} //namespace Guy

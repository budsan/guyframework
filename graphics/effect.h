#pragma once

#include "graphics.h"
#include "shaderprogram.h"

#include <memory>

#include <vector>
#include <string>
#include <map>

namespace Guy {

class Uniform;

class Effect
{
public:

	Effect(const std::shared_ptr<ShaderProgram>& _program);
	~Effect();

	ShaderProgram& getShaderProgram();

	void bind();
	void unbind();

private:

	void buildParameterLists();

	std::shared_ptr<ShaderProgram> m_program;
	std::map<std::string, GLint> m_vertexAttributes;
	std::map<std::string, Uniform*> m_uniforms;
};

class Uniform
{
	friend class Effect;

public:

	const char* getName() const;
	GLenum getType() const;
	Effect* getEffect() const;

private:

	Uniform();
	Uniform(const Uniform& copy);

	~Uniform();

	Uniform& operator=(const Uniform&);

	std::string m_name;
	GLint m_location;
	GLenum m_type;
	unsigned int m_index;
	Effect* m_effect;
};


} // namespace Guy

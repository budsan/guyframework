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

    Effect(std::shared_ptr<ShaderProgram> program);
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
    const GLenum getType() const;
    Effect* getEffect() const;

private:

    Uniform();
    Uniform(const Uniform& copy);
    ~Uniform();
    Uniform& operator=(const Uniform&);

    std::string _name;
    GLint _location;
    GLenum _type;
    unsigned int _index;
    Effect* _effect;
};


} // namespace Guy

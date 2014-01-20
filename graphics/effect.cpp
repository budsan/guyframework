#include "effect.h"
#include "shaderprogram.h"

namespace Guy {

Effect::Effect(std::shared_ptr<ShaderProgram> program) : m_program(program)
{

}

Effect::~Effect()
{

}

ShaderProgram& Effect::getShaderProgram()
{

}

void Effect::bind()
{

}

void Effect::unbind()
{

}


void Effect::buildParameterLists()
{
	/*
	GLuint program = m_program->m_id;

	// Query and store vertex attribute meta-data from the program.
	// NOTE: Rather than using glBindAttribLocation to explicitly specify our own
	// preferred attribute locations, we're going to query the locations that were
	// automatically bound by the GPU. While it can sometimes be convenient to use
	// glBindAttribLocation, some vendors actually reserve certain attribute indices
	// and therefore using this function can create compatibility issues between
	// different hardware vendors.
	GLint activeAttributes;
	GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &activeAttributes) );
	if (activeAttributes > 0)
	{
		GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length) );
		if (length > 0)
		{
			GLchar* attribName = new GLchar[length + 1];
			GLint attribSize;
			GLenum attribType;
			GLint attribLocation;
			for (int i = 0; i < activeAttributes; ++i)
			{
				// Query attribute info.
				GL_ASSERT( glGetActiveAttrib(program, i, length, NULL, &attribSize, &attribType, attribName) );
				attribName[length] = '\0';

				// Query the pre-assigned attribute location.
				GL_ASSERT( attribLocation = glGetAttribLocation(program, attribName) );

				// Assign the vertex attribute mapping for the effect.
				effect->_vertexAttributes[attribName] = attribLocation;
			}
			SAFE_DELETE_ARRAY(attribName);
		}
	}

	// Query and store uniforms from the program.
	GLint activeUniforms;
	GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeUniforms) );
	if (activeUniforms > 0)
	{
		GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length) );
		if (length > 0)
		{
			GLchar* uniformName = new GLchar[length + 1];
			GLint uniformSize;
			GLenum uniformType;
			GLint uniformLocation;
			unsigned int samplerIndex = 0;
			for (int i = 0; i < activeUniforms; ++i)
			{
				// Query uniform info.
				GL_ASSERT( glGetActiveUniform(program, i, length, NULL, &uniformSize, &uniformType, uniformName) );
				uniformName[length] = '\0';  // null terminate
				if (uniformSize > 1 && length > 3)
				{
					// This is an array uniform. I'm stripping array indexers off it since GL does not
					// seem to be consistent across different drivers/implementations in how it returns
					// array uniforms. On some systems it will return "u_matrixArray", while on others
					// it will return "u_matrixArray[0]".
					char* c = strrchr(uniformName, '[');
					if (c)
					{
						*c = '\0';
					}
				}

				// Query the pre-assigned uniform location.
				GL_ASSERT( uniformLocation = glGetUniformLocation(program, uniformName) );

				Uniform* uniform = new Uniform();
				uniform->_effect = effect;
				uniform->_name = uniformName;
				uniform->_location = uniformLocation;
				uniform->_type = uniformType;
				uniform->_index = uniformType == GL_SAMPLER_2D ? (samplerIndex++) : 0;

				effect->_uniforms[uniformName] = uniform;
			}
			SAFE_DELETE_ARRAY(uniformName);
		}
	}
	*/
}

} // namespace Guy

#pragma once
#include "graphics.h"

namespace Guy {

class Shader
{
	private:
		GLuint m_id;
	public:
		Shader(GLenum type);
		~Shader();

		bool load(const char *filename);
		bool compile(const char *filename);
		bool compile() const;

		void attach(GLuint program) const;
		void printInfoLog() const;
};

} // namespace Guy


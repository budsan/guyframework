#ifndef _SHADER_INCLUDED_
#define _SHADER_INCLUDED_

#include "graphics.h"


class Shader
{
	private:
		GLuint m_id;
	public:
		Shader(GLenum type);
		~Shader();
		bool load(const char *filename);

		void compile() const;
		void attach(GLuint program) const;
		void printInfoLog() const;
};

#endif


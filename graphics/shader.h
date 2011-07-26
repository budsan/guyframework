#ifndef _SHADER_INCLUDED_
#define _SHADER_INCLUDED_

#include "graphics.h"


class Shader
{
	private:
		GLuint _id;
	public:
		Shader(GLenum type);
		~Shader();
		bool Load(const char *filename);

		void Compile() const;
		void Attach(GLuint program) const;
		void PrintInfoLog() const;
};

#endif


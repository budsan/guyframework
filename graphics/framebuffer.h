#pragma once

#include "graphics.h"

#include <stdexcept>
#include <vector>

namespace Guy {

/// Frame Buffer Object class
class FrameBuffer
{
	protected:
		int m_width;
		int m_height;
		GLuint m_frameId;
		GLuint m_depthId;
		GLuint m_stencilId;
		std::vector<GLuint> m_texId;
		GLenum *m_buffers; // glDrawBuffers() needs a C array

		int m_maxColorAttachments;
	public:
		/// Default Constructor
		FrameBuffer();
		/// Constructor used when size is known
		FrameBuffer(int width, int height);
		/// Destructor
		~FrameBuffer();
		/// Set FBO size when using default constructor
		void setSize(int width, int height) { m_width = width; m_height = height; };
		/// Attach a render target to the FBO
		void attachRender(GLenum iformat) throw(std::domain_error, std::invalid_argument);
		/// Attach a texture to the FBO
		void attachTexture(GLenum iformat, GLint filter = GL_LINEAR) throw(std::domain_error, std::out_of_range, std::invalid_argument);
		/// Bind the FBO as input, for reading from
		void bindInput();
		/// Bind the nth texture of the FBO as input
		void bindInput(int num) throw(std::out_of_range);
		/// Bind the FBO as output, for writing into
		void bindOutput() throw(std::domain_error);
		/// Bind the nth texture of the FBO as output
		void bindOutput(int num) throw(std::out_of_range);
		/// Bind the specified FBO texture to the context (deprecated)
		void bindTex(int num = 0) throw(std::out_of_range);
		/// Blit from an FBO to another
		void blitTo(FrameBuffer *dest, GLbitfield mask, GLenum filter = GL_LINEAR);
		/// Check OpenGL status of the FBO
		void check();

		/// Disable rendering to FBO
		static void unbind();
};

} // namespace Guy

#ifndef _FRAME_BUFFER_INCLUDED_
#define _FRAME_BUFFER_INCLUDED_

#include "graphics.h"

#include <stdexcept>
#include <vector>

/// Frame Buffer Object class
class FrameBuffer
{
	protected:
		int _width;
		int _height;
		GLuint _frame_id;
		GLuint _depth_id;
		GLuint _stencil_id;
		std::vector<GLuint> _tex_id;
		GLenum *_buffers; // glDrawBuffers() needs a C array

		int _max_color_attachments;
	public:
		/// Default Constructor
		FrameBuffer();
		/// Constructor used when size is known
		FrameBuffer(int width, int height);
		/// Destructor
		~FrameBuffer();
		/// Set FBO size when using default constructor
		void setSize(int width, int height) { _width = width; _height = height; };
		/// Attach a render target to the FBO
		void AttachRender(GLenum iformat) throw(std::domain_error, std::invalid_argument);
		/// Attach a texture to the FBO
		void AttachTexture(GLenum iformat, GLint filter = GL_LINEAR) throw(std::domain_error, std::out_of_range, std::invalid_argument);
		/// Bind the FBO as input, for reading from
		void BindInput();
		/// Bind the nth texture of the FBO as input
		void BindInput(int num) throw(std::out_of_range);
		/// Bind the FBO as output, for writing into
		void BindOutput() throw(std::domain_error);
		/// Bind the nth texture of the FBO as output
		void BindOutput(int num) throw(std::out_of_range);
		/// Bind the specified FBO texture to the context (deprecated)
		void BindTex(int num = 0) throw(std::out_of_range);
		/// Blit from an FBO to another
		void BlitTo(FrameBuffer *dest, GLbitfield mask, GLenum filter = GL_LINEAR);
		/// Check OpenGL status of the FBO
		void Check();

		/// Disable rendering to FBO
		static void Unbind();
};

#endif

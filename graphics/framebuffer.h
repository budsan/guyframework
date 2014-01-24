#pragma once

#include "graphics.h"

#include <stdexcept>
#include <vector>

namespace Guy {


class FrameBuffer
{
public:
	FrameBuffer();
	FrameBuffer(int width, int height);
	~FrameBuffer();

	void setSize(int width, int height) { m_width = width; m_height = height; }
	void attachRender(GLenum iformat) throw(std::domain_error, std::invalid_argument);
	void attachTexture(GLenum iformat, GLint filter = GL_LINEAR) throw(std::domain_error, std::out_of_range, std::invalid_argument);
	void bindInput();
	void bindInput(int num) throw(std::out_of_range);
	void bindOutput() throw(std::domain_error);
	void bindOutput(int num) throw(std::out_of_range);
	void bindTex(int num = 0) throw(std::out_of_range);
	void blitTo(FrameBuffer *dest, GLbitfield mask, GLenum filter = GL_LINEAR);
	void check();

	static void unbind();
protected:
	int m_width;
	int m_height;
	GLuint m_frameId;
	GLuint m_depthId;
	GLuint m_stencilId;
	std::vector<GLuint> m_texId;
	GLenum *m_buffers;

	int m_maxColorAttachments;

};

} // namespace Guy

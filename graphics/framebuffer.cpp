#include <stdexcept>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#include "framebuffer.h"
#include <iostream>


FrameBuffer::FrameBuffer() : _width(0), _height(0), _frame_id(0), _depth_id(0), _stencil_id(0), _buffers(0)
{
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &_max_color_attachments);
	_buffers = new GLenum[_max_color_attachments];

	glGenFramebuffers(1, &_frame_id);
}


FrameBuffer::FrameBuffer(int width, int height) : _width(0), _height(0), _frame_id(0), _depth_id(0), _stencil_id(0), _buffers(0)
{
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &_max_color_attachments);
	_buffers = new GLenum[_max_color_attachments];

	glGenFramebuffersEXT(1, &_frame_id);
	_width = width;
	_height = height;
}


FrameBuffer::~FrameBuffer()
{
	GLuint tex_id;

	std::vector<GLuint>::const_iterator cii;
	for(cii = _tex_id.begin(); cii != _tex_id.end(); cii++) {
		tex_id = *cii;
		glDeleteTextures(1, &tex_id);
	}

	if (_depth_id) {
		glDeleteRenderbuffersEXT(1, &_depth_id);
	}
	if (_stencil_id) {
		glDeleteRenderbuffersEXT(1, &_stencil_id);
	}
	glDeleteFramebuffersEXT(1, &_frame_id);
	delete[] _buffers;
}


void FrameBuffer::AttachRender(GLenum iformat)
	throw (std::domain_error, std::invalid_argument)
{
	GLenum attachment;
	GLuint render_id;

	if (_width == 0 || _height == 0) {
		throw std::domain_error("FrameBuffer::AttachRender - one of the dimensions is zero");
	}

	if (iformat == GL_DEPTH_COMPONENT24 || iformat == GL_DEPTH_COMPONENT) {
		attachment = GL_DEPTH_ATTACHMENT_EXT;
	}
	else if (iformat == GL_STENCIL_INDEX1_EXT || iformat == GL_STENCIL_INDEX4_EXT || iformat == GL_STENCIL_INDEX8_EXT ||
		iformat == GL_STENCIL_INDEX16_EXT || iformat == GL_STENCIL_INDEX) {
		attachment = GL_STENCIL_ATTACHMENT_EXT;
	}
	else if (iformat == GL_DEPTH24_STENCIL8 || iformat == GL_DEPTH_STENCIL) {
		attachment = GL_DEPTH_STENCIL_ATTACHMENT;
	}
	else {
		throw std::invalid_argument("FrameBuffer::AttachRender - unrecognized internal format");
	}

	glGenRenderbuffersEXT(1, &render_id);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _frame_id);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, render_id);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, iformat, _width, _height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment, GL_RENDERBUFFER_EXT, render_id);

	if (attachment == GL_DEPTH_ATTACHMENT_EXT || attachment == GL_DEPTH_STENCIL_ATTACHMENT) {
		_depth_id = render_id;
	}
	else if (attachment == GL_STENCIL_ATTACHMENT_EXT) {
		_stencil_id = render_id;
	}
}


void FrameBuffer::AttachTexture(GLenum iformat, GLint filter)
	throw(std::domain_error, std::out_of_range, std::invalid_argument)
{
	GLenum format;
	GLenum type;
	GLenum attachment;
	GLuint tex_id;

	if (_width == 0 || _height == 0) {
		throw std::domain_error("FrameBuffer::AttachTexture - one of the dimensions is zero");
	}

	if (int(_tex_id.size()) == _max_color_attachments) {
		throw std::out_of_range("FrameBuffer::AttachTexture - GL_MAX_COLOR_ATTACHMENTS exceeded");
	}

	attachment = GL_COLOR_ATTACHMENT0_EXT + _tex_id.size(); // common attachment for color textures
	if (iformat == GL_RGBA16F_ARB || iformat == GL_RGBA32F_ARB) {
		format = GL_RGBA;
		type = GL_FLOAT;
	}
	else if (iformat == GL_RGB16F_ARB || iformat == GL_RGB32F_ARB) {
		format = GL_RGB;
		type = GL_FLOAT;
	}
	else if (iformat == GL_LUMINANCE_ALPHA16F_ARB || iformat == GL_LUMINANCE_ALPHA32F_ARB) {
		format = GL_LUMINANCE_ALPHA;
		type = GL_FLOAT;
	}
	else if (iformat == GL_LUMINANCE16F_ARB || iformat == GL_LUMINANCE32F_ARB) {
		format = GL_LUMINANCE;
		type = GL_FLOAT;
	}
	else if (iformat == GL_RGBA8 || iformat == GL_RGBA || iformat == 4) {
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
	}
	else if (iformat == GL_RGB8 || iformat == GL_RGB || iformat == 3) {
		format = GL_RGB;
		type = GL_UNSIGNED_BYTE;
	}
	else if (iformat == GL_LUMINANCE8_ALPHA8 || iformat == GL_LUMINANCE_ALPHA || iformat == 2) {
		format = GL_LUMINANCE_ALPHA;
		type = GL_UNSIGNED_BYTE;
	}
	else if (iformat == GL_LUMINANCE8 || iformat == GL_LUMINANCE16 || iformat == GL_LUMINANCE || iformat == 1) {
		format = GL_LUMINANCE;
		type = GL_UNSIGNED_BYTE;
	}
	else if (iformat == GL_DEPTH_COMPONENT24 || iformat == GL_DEPTH_COMPONENT) {
		format = GL_DEPTH_COMPONENT;
		type = GL_UNSIGNED_INT;
		attachment = GL_DEPTH_ATTACHMENT_EXT;
		filter = GL_NEAREST;
	}
	else if (iformat == GL_STENCIL_INDEX1_EXT || iformat == GL_STENCIL_INDEX4_EXT || iformat == GL_STENCIL_INDEX8_EXT ||
		iformat == GL_STENCIL_INDEX16_EXT || iformat == GL_STENCIL_INDEX) {
		format = GL_STENCIL_INDEX;
		type = GL_UNSIGNED_BYTE;
		attachment = GL_STENCIL_ATTACHMENT_EXT;
		filter = GL_NEAREST;
	}
	else if (iformat == GL_DEPTH24_STENCIL8 || iformat == GL_DEPTH_STENCIL) {
		format = GL_DEPTH_STENCIL;
		type = GL_UNSIGNED_INT_24_8;
		attachment = GL_DEPTH_STENCIL_ATTACHMENT;
		filter = GL_NEAREST;
	}
	else {
		throw std::invalid_argument("FrameBuffer::AttachTexture - unrecognized internal format");
	}

	glGenTextures(1, &tex_id);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _frame_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, iformat, _width, _height, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	if (format == GL_DEPTH_STENCIL) { // packed depth and stencil added separately
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_id, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex_id, 0);
	}
	else {
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, tex_id, 0);
	}

	_tex_id.push_back(tex_id);
	_buffers[_tex_id.size() - 1] = attachment;
}


void FrameBuffer::BindInput()
{
	for (int i = 0; i < int(_tex_id.size()); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _tex_id[i]);
	}
}


void FrameBuffer::BindInput(int num) throw(std::out_of_range)
{
	if (num + 1 > int(_tex_id.size())) {
		throw std::out_of_range("FrameBuffer::BindInput - texture vector size exceeded");
	}

	glBindTexture(GL_TEXTURE_2D, _tex_id[num]);
}


void FrameBuffer::BindOutput() throw(std::domain_error)
{
	if (_tex_id.empty()) {
		throw std::domain_error("FrameBuffer::BindOutput - no textures to bind");
	}	

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _frame_id);
	if (_tex_id.size() == 1) {
		glDrawBuffer(_buffers[0]);
	}
	else {
		glDrawBuffers(_tex_id.size(), _buffers);
	}
}


void FrameBuffer::BindOutput(int num) throw(std::out_of_range)
{
	if (num + 1 > int(_tex_id.size())) {
		throw std::out_of_range("FrameBuffer::BindOutput - texture vector size exceeded");
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _frame_id);
	glDrawBuffer(_buffers[num]);
}


void FrameBuffer::BindTex(int num) throw(std::out_of_range)
{
	// Implemented through BindInput()
	BindInput(num);
}


void FrameBuffer::BlitTo(FrameBuffer *dest, GLbitfield mask, GLenum filter)
{
	int old_read, old_draw;

	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING_EXT, &old_read);
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING_EXT, &old_draw);;

	if ((mask & GL_DEPTH_BUFFER_BIT) || (mask & GL_STENCIL_BUFFER_BIT)) {
		filter = GL_NEAREST;
	}

	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, _frame_id);
	if (dest)
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, dest->_frame_id);
	else
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);

	glBlitFramebufferEXT(0, 0, _width, _height, 0, 0, _width, _height, mask, filter);

	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, old_read);
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, old_draw);
}


void FrameBuffer::Check()
{
	GLenum status;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _frame_id);
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		std::cout << "FBO Status error: " << status << std::endl;
		throw std::invalid_argument("FrameBuffer::Check - status error");
	}
}


void FrameBuffer::Unbind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);
}

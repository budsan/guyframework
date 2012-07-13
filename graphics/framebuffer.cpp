#include "framebuffer.h"
#include "graphics.h"

#include <stdexcept>
#include <vector>
#include <iostream>

namespace Guy {

FrameBuffer::FrameBuffer() : m_width(0), m_height(0), m_frameId(0), m_depthId(0), m_stencilId(0), m_buffers(0)
{
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &m_maxColorAttachments);
	m_buffers = new GLenum[m_maxColorAttachments];

	glGenFramebuffers(1, &m_frameId);
}


FrameBuffer::FrameBuffer(int width, int height) : m_width(0), m_height(0), m_frameId(0), m_depthId(0), m_stencilId(0), m_buffers(0)
{
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &m_maxColorAttachments);
	m_buffers = new GLenum[m_maxColorAttachments];

	glGenFramebuffersEXT(1, &m_frameId);
	m_width = width;
	m_height = height;
}


FrameBuffer::~FrameBuffer()
{
	GLuint tex_id;

	std::vector<GLuint>::const_iterator cii;
	for(cii = m_texId.begin(); cii != m_texId.end(); cii++) {
		tex_id = *cii;
		glDeleteTextures(1, &tex_id);
	}

	if (m_depthId) {
		glDeleteRenderbuffersEXT(1, &m_depthId);
	}
	if (m_stencilId) {
		glDeleteRenderbuffersEXT(1, &m_stencilId);
	}
	glDeleteFramebuffersEXT(1, &m_frameId);
	delete[] m_buffers;
}


void FrameBuffer::attachRender(GLenum iformat)
	throw (std::domain_error, std::invalid_argument)
{
	GLenum attachment;
	GLuint render_id;

	if (m_width == 0 || m_height == 0) {
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
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameId);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, render_id);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, iformat, m_width, m_height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment, GL_RENDERBUFFER_EXT, render_id);

	if (attachment == GL_DEPTH_ATTACHMENT_EXT || attachment == GL_DEPTH_STENCIL_ATTACHMENT) {
		m_depthId = render_id;
	}
	else if (attachment == GL_STENCIL_ATTACHMENT_EXT) {
		m_stencilId = render_id;
	}
}


void FrameBuffer::attachTexture(GLenum iformat, GLint filter)
	throw(std::domain_error, std::out_of_range, std::invalid_argument)
{
	GLenum format;
	GLenum type;
	GLenum attachment;
	GLuint tex_id;

	if (m_width == 0 || m_height == 0) {
		throw std::domain_error("FrameBuffer::AttachTexture - one of the dimensions is zero");
	}

	if (int(m_texId.size()) == m_maxColorAttachments) {
		throw std::out_of_range("FrameBuffer::AttachTexture - GL_MAX_COLOR_ATTACHMENTS exceeded");
	}

	attachment = GL_COLOR_ATTACHMENT0_EXT + (GLenum) m_texId.size(); // common attachment for color textures
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
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameId);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, iformat, m_width, m_height, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	if (format == GL_DEPTH_STENCIL) { // packed depth and stencil added separately
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_id, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex_id, 0);
	}
	else {
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, tex_id, 0);
	}

	m_texId.push_back(tex_id);
	m_buffers[m_texId.size() - 1] = attachment;
}


void FrameBuffer::bindInput()
{
	for (int i = 0; i < int(m_texId.size()); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_texId[i]);
	}
}


void FrameBuffer::bindInput(int num) throw(std::out_of_range)
{
	if (num + 1 > int(m_texId.size())) {
		throw std::out_of_range("FrameBuffer::BindInput - texture vector size exceeded");
	}

	glBindTexture(GL_TEXTURE_2D, m_texId[num]);
}


void FrameBuffer::bindOutput() throw(std::domain_error)
{
	if (m_texId.empty()) {
		throw std::domain_error("FrameBuffer::BindOutput - no textures to bind");
	}	

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameId);
	if (m_texId.size() == 1) {
		glDrawBuffer(m_buffers[0]);
	}
	else {
		glDrawBuffers((GLsizei) m_texId.size(), m_buffers);
	}
}


void FrameBuffer::bindOutput(int num) throw(std::out_of_range)
{
	if (num + 1 > int(m_texId.size())) {
		throw std::out_of_range("FrameBuffer::BindOutput - texture vector size exceeded");
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameId);
	glDrawBuffer(m_buffers[num]);
}


void FrameBuffer::bindTex(int num) throw(std::out_of_range)
{
	// Implemented through BindInput()
	bindInput(num);
}


void FrameBuffer::blitTo(FrameBuffer *dest, GLbitfield mask, GLenum filter)
{
	int old_read, old_draw;

	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING_EXT, &old_read);
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING_EXT, &old_draw);;

	if ((mask & GL_DEPTH_BUFFER_BIT) || (mask & GL_STENCIL_BUFFER_BIT)) {
		filter = GL_NEAREST;
	}

	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, m_frameId);
	if (dest)
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, dest->m_frameId);
	else
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);

	glBlitFramebufferEXT(0, 0, m_width, m_height, 0, 0, m_width, m_height, mask, filter);

	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, old_read);
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, old_draw);
}


void FrameBuffer::check()
{
	GLenum status;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameId);
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		std::cout << "FBO Status error: " << status << std::endl;
		throw std::invalid_argument("FrameBuffer::Check - status error");
	}
}


void FrameBuffer::unbind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);
}

} //namespace Guy

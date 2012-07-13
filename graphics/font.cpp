#include "font.h"
#include "graphics.h"

namespace Guy {

#define USING_GL11

bool Font::load(const char* path, int point_size, int dpi)
{
	FT_Library library;
	FT_Face face;
	int c;
	int i, j;

	if (!path){
		fprintf(stderr, "Invalid path to font file\n");
		return NULL;
	}

	if(FT_Init_FreeType(&library)) {
		fprintf(stderr, "Error loading Freetype library\n");
		return NULL;
	}
	if (FT_New_Face(library, path,0, &face)) {
		fprintf(stderr, "Error loading font %s\n", path);
		return NULL;
	}

	if(FT_Set_Char_Size ( face, point_size * 64, point_size * 64, dpi, dpi)) {
		fprintf(stderr, "Error initializing character parameters\n");
		return NULL;
	}

	this->pt = (float) point_size;

	glGenTextures(1, &(this->font_texture));

	//Let each glyph reside in 32x32 section of the font texture
	int segment_size_x = 0, segment_size_y = 0;
	int num_segments_x = 16;
	int num_segments_y = 8;

	FT_GlyphSlot slot;
	FT_Bitmap bmp;
	int glyph_width, glyph_height;

	//First calculate the max width and height of a character in a passed font
	for(c = 0; c < 128; c++) {
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			fprintf(stderr, "FT_Load_Char failed\n");
			free(this);
			return NULL;
		}

		slot = face->glyph;
		bmp = slot->bitmap;

		glyph_width = bmp.width;
		glyph_height = bmp.rows;

		if (glyph_width > segment_size_x) {
			segment_size_x = glyph_width;
		}

		if (glyph_height > segment_size_y) {
			segment_size_y = glyph_height;
		}
	}

	int font_tex_width  = nextPowerOfTwo(num_segments_x * segment_size_x);
	int font_tex_height = nextPowerOfTwo(num_segments_y * segment_size_y);

	int bitmap_offset_x = 0, bitmap_offset_y = 0;

	GLubyte* font_texture_data = (GLubyte*) calloc(2 * font_tex_width * font_tex_height, sizeof(GLubyte));

	if (!font_texture_data) {
		fprintf(stderr, "Failed to allocate memory for font texture\n");
		return false;
	}

	// Fill font texture bitmap with individual bmp data and record appropriate size, texture coordinates and offsets for every glyph
	for(c = 0; c < 128; c++) {
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			fprintf(stderr, "FT_Load_Char failed\n");
			return false;
		}

		slot = face->glyph;
		bmp = slot->bitmap;

		glyph_width = bmp.width;
		glyph_height = bmp.rows;

		div_t temp = div(c, num_segments_x);

		bitmap_offset_x = segment_size_x * temp.rem;
		bitmap_offset_y = segment_size_y * temp.quot;

		for (j = 0; j < glyph_height; j++) {
			for (i = 0; i < glyph_width; i++) {
				font_texture_data[2 * ((bitmap_offset_x + i) + (j + bitmap_offset_y) * font_tex_width) + 0] =
						font_texture_data[2 * ((bitmap_offset_x + i) + (j + bitmap_offset_y) * font_tex_width) + 1] =
						(i >= bmp.width || j >= bmp.rows)? 0 : bmp.buffer[i + bmp.width * j];
			}
		}

		this->advance[c]  = (float) (slot->advance.x >> 6);
		this->tex_x1[c]   = (float) bitmap_offset_x / (float) font_tex_width;
		this->tex_x2[c]   = (float)(bitmap_offset_x + bmp.width) / (float)font_tex_width;
		this->tex_y1[c]   = (float) bitmap_offset_y / (float) font_tex_height;
		this->tex_y2[c]   = (float)(bitmap_offset_y + bmp.rows) / (float)font_tex_height;
		this->width[c]    = (float) bmp.width;
		this->height[c]   = (float) bmp.rows;
		this->offset_x[c] = (float) slot->bitmap_left;
		this->offset_y[c] = (float)((slot->metrics.horiBearingY-face->glyph->metrics.height) >> 6);
	}

	glBindTexture(GL_TEXTURE_2D, this->font_texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, font_tex_width, font_tex_height, 0, GL_LUMINANCE_ALPHA , GL_UNSIGNED_BYTE, font_texture_data);

	free(font_texture_data);

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	this->initialized = 1;
	return true;
}

math::vec2f Font::measure(const char* msg)
{
	math::vec2f result(0,0);

	if (!this->initialized) {
		fprintf(stderr, "Font has not been loaded\n");
		return result;
	}

	if (!msg) return result;
	const unsigned int msg_len = (unsigned int) strlen(msg);

	//Width of a text rectangle is a sum advances for every glyph in a string
	for(unsigned int i = 0; i < msg_len; ++i) {
		int c = msg[i];
		result.x += this->advance[c];
	}

	//Height of a text rectangle is a high of a tallest glyph in a string
	for(unsigned int i = 0; i < msg_len; ++i) {
		int c = msg[i];

		if (result.y < this->height[c]) {
			result.y = this->height[c];
		}
	}

	return result;
}

void Font::draw2D(const char* msg, float x, float y, float r, float g, float b, float a) {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixf(
		math::mat4f::fromOrtho(
			(float) viewport[0], (float) viewport[2],
			(float) viewport[1], (float) viewport[3],1, -1).v);
	draw(msg, x, y, r, g, b, a);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void Font::draw(const char* msg, float x, float y, float r, float g, float b, float a) {
	int i, c;
	GLfloat *vertices;
	GLfloat *texture_coords;
	GLshort *indices;

	if (m_alignment != Font::LEFT)
	{
		math::vec2f m = measure(msg);
		if (m_alignment == Font::CENTER)
			x = x -(m.x/2);
		else
			x = x - m.x;
	}

	float pen_x = 0.0f;

	if (!this->initialized) {
		fprintf(stderr, "Font has not been loaded\n");
		return;
	}

	if (!msg) {
		return;
	}

	const unsigned int msg_len = (unsigned int) strlen(msg);

	vertices = (GLfloat*) malloc(sizeof(GLfloat) * 8 * msg_len);
	texture_coords = (GLfloat*) malloc(sizeof(GLfloat) * 8 * msg_len);

	indices = (GLshort*) malloc(sizeof(GLfloat) * 6 * msg_len);

	for(i = 0; i < (int) msg_len; ++i) {
		c = msg[i];

		vertices[8 * i + 0] = x + pen_x + this->offset_x[c];
		vertices[8 * i + 1] = y + this->offset_y[c];
		vertices[8 * i + 2] = vertices[8 * i + 0] + this->width[c];
		vertices[8 * i + 3] = vertices[8 * i + 1];
		vertices[8 * i + 4] = vertices[8 * i + 0];
		vertices[8 * i + 5] = vertices[8 * i + 1] + this->height[c];
		vertices[8 * i + 6] = vertices[8 * i + 2];
		vertices[8 * i + 7] = vertices[8 * i + 5];

		texture_coords[8 * i + 0] = this->tex_x1[c];
		texture_coords[8 * i + 1] = this->tex_y2[c];
		texture_coords[8 * i + 2] = this->tex_x2[c];
		texture_coords[8 * i + 3] = this->tex_y2[c];
		texture_coords[8 * i + 4] = this->tex_x1[c];
		texture_coords[8 * i + 5] = this->tex_y1[c];
		texture_coords[8 * i + 6] = this->tex_x2[c];
		texture_coords[8 * i + 7] = this->tex_y1[c];

		indices[i * 6 + 0] = (GLshort) (4 * i + 0);
		indices[i * 6 + 1] = (GLshort) (4 * i + 1);
		indices[i * 6 + 2] = (GLshort) (4 * i + 2);
		indices[i * 6 + 3] = (GLshort) (4 * i + 2);
		indices[i * 6 + 4] = (GLshort) (4 * i + 1);
		indices[i * 6 + 5] = (GLshort) (4 * i + 3);

		//Assume we are only working with typewriter fonts
		pen_x += this->advance[c];
	}
#ifdef USING_GL11
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glColor4f(r, g, b, a);

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);
	glBindTexture(GL_TEXTURE_2D, this->font_texture);

	glDrawElements(GL_TRIANGLES, 6 * msg_len, GL_UNSIGNED_SHORT, indices);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
#elif defined USING_GL20
	if (!text_program_initialized) {
		GLint status;

		// Create shaders if this hasn't been done already
		const char* v_source =
				"precision mediump float;"
				"attribute vec2 a_position;"
				"attribute vec2 a_texcoord;"
				"varying vec2 v_texcoord;"
				"void main()"
				"{"
				"   gl_Position = vec4(a_position, 0.0, 1.0);"
				"    v_texcoord = a_texcoord;"
				"}";

		const char* f_source =
				"precision lowp float;"
				"varying vec2 v_texcoord;"
				"uniform sampler2D u_font_texture;"
				"uniform vec4 u_col;"
				"void main()"
				"{"
				"    vec4 temp = texture2D(u_font_texture, v_texcoord);"
				"    gl_FragColor = u_col * temp;"
				"}";

		// Compile the vertex shader
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);

		if (!vs) {
			fprintf(stderr, "Failed to create vertex shader: %d\n", glGetError());
			return;
		} else {
			glShaderSource(vs, 1, &v_source, 0);
			glCompileShader(vs);
			glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
			if (GL_FALSE == status) {
				GLchar log[256];
				glGetShaderInfoLog(vs, 256, NULL, log);

				fprintf(stderr, "Failed to compile vertex shader: %s\n", log);

				glDeleteShader(vs);
			}
		}

		// Compile the fragment shader
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

		if (!fs) {
			fprintf(stderr, "Failed to create fragment shader: %d\n", glGetError());
			return;
		} else {
			glShaderSource(fs, 1, &f_source, 0);
			glCompileShader(fs);
			glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
			if (GL_FALSE == status) {
				GLchar log[256];
				glGetShaderInfoLog(fs, 256, NULL, log);

				fprintf(stderr, "Failed to compile fragment shader: %s\n", log);

				glDeleteShader(vs);
				glDeleteShader(fs);

				return;
			}
		}

		// Create and link the program
		text_rendering_program = glCreateProgram();
		if (text_rendering_program)
		{
			glAttachShader(text_rendering_program, vs);
			glAttachShader(text_rendering_program, fs);
			glLinkProgram(text_rendering_program);

			glGetProgramiv(text_rendering_program, GL_LINK_STATUS, &status);
			if (status == GL_FALSE)    {
				GLchar log[256];
				glGetProgramInfoLog(fs, 256, NULL, log);

				fprintf(stderr, "Failed to link text rendering shader program: %s\n", log);

				glDeleteProgram(text_rendering_program);
				text_rendering_program = 0;

				return;
			}
		} else {
			fprintf(stderr, "Failed to create a shader program\n");

			glDeleteShader(vs);
			glDeleteShader(fs);
			return;
		}

		// We don't need the shaders anymore - the program is enough
		glDeleteShader(fs);
		glDeleteShader(vs);

		glUseProgram(text_rendering_program);

		// Store the locations of the shader variables we need later
		positionLoc = glGetAttribLocation(text_rendering_program, "a_position");
		texcoordLoc = glGetAttribLocation(text_rendering_program, "a_texcoord");
		textureLoc = glGetUniformLocation(text_rendering_program, "u_font_texture");
		colorLoc = glGetUniformLocation(text_rendering_program, "u_col");

		text_program_initialized = 1;
	}

	glEnable(GL_BLEND);

	//Map text coordinates from (0...surface width, 0...surface height) to (-1...1, -1...1)
	//this make our vertex shader very simple and also works irrespective of orientation changes
	EGLint surface_width, surface_height;

	eglQuerySurface(egl_disp, egl_surf, EGL_WIDTH, &surface_width);
	eglQuerySurface(egl_disp, egl_surf, EGL_HEIGHT, &surface_height);

	for(i = 0; i < 4 * msg_len; ++i) {
		vertices[2 * i + 0] = 2 * vertices[2 * i + 0] / surface_width - 1.0f;
		vertices[2 * i + 1] = 2 * vertices[2 * i + 1] / surface_height - 1.0f;
	}

	//Render text
	glUseProgram(text_rendering_program);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font->font_texture);
	glUniform1i(textureLoc, 0);

	glUniform4f(colorLoc, r, g, b, a);

	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, vertices);

	glEnableVertexAttribArray(texcoordLoc);
	glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, 0, texture_coords);

	//Draw the string
	glDrawElements(GL_TRIANGLES, 6 * msg_len, GL_UNSIGNED_SHORT, indices);

	glDisableVertexAttribArray(positionLoc);
	glDisableVertexAttribArray(texcoordLoc);
#else
	fprintf(stderr, "bbutil should be compiled with either USING_GL11 or USING_GL20 -D flags\n");
#endif

	free(vertices);
	free(texture_coords);
	free(indices);
}

} // namespace Guy

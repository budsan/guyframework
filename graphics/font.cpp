#include "font.h"
#include "base/log.h"

#define CHARSET_SIZE 128

Font::Font() {
	myTextures = NULL;
	myMetrics  = NULL;
	myListFirst = 0;
	myAlignment = (BASELINE << 2) | LEFT;
}

bool Font::Load(const char *filename, unsigned int height) {

	FT_Library library;
	if (FT_Init_FreeType( &library ))
	{
		#ifdef _DEBUG
		LOG << "FT_Init_FreeType failed" << std::endl;
		#endif
		return false;
	}

	FT_Face face;
	if (FT_New_Face( library, filename, 0, &face ))
	{
		#ifdef _DEBUG
		LOG << "FT_New_Face failed (there is probably a problem with your font file)" << std::endl;
		#endif
		return false;
	}

	FT_Set_Char_Size( face, height << 6, height << 6, 96, 96);

	if (myTextures != NULL) Clean();

	this->myHeight = height;
	myTextures = new GLuint[CHARSET_SIZE];
	myMetrics  = new glyph_metrics[CHARSET_SIZE];
	myListFirst = glGenLists(CHARSET_SIZE);
	glGenTextures(CHARSET_SIZE, myTextures );

	for(unsigned int i=0 ; i < CHARSET_SIZE ; i++)
	{
		bool result = RenderFaceToTexture(face, i);
		if (!result) {
			Clean();
			return false;
		}
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	return true;
}

void Font::setAlignment(VertAlignment v, HorzAlignment h) {
	myAlignment = (short(v) << 2) | short(h);
}

#ifdef _MATH_VEC2_DEFINED_
using namespace math;
void Font::Print(vec2f v, const char *fmt, ...) {
	char text[256];
	va_list ap;

	if (fmt == NULL) return;
	else {
		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);
	}

	PrintGL(v.x, v.y, text);
}

void Font::Print2D(vec2f v, const char *fmt, ...)
{	
	char text[256];
	va_list ap;

	if (fmt == NULL) return;
	else {
		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);
	}
	
	glPushAttrib(GL_TRANSFORM_BIT|GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
	
	PrintGL(v.x, v.y, text);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}
#endif

void Font::Print(float x, float y, const char *fmt, ...)
{	
	char text[256];
	va_list ap;

	if (fmt == NULL) return;
	else {
		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);
	}
	
	PrintGL(x, y, text);
}

void Font::Print2D(float x, float y, const char *fmt, ...)
{	
	char text[256];
	va_list ap;

	if (fmt == NULL) return;
	else {
		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);
	}
	
	glPushAttrib(GL_TRANSFORM_BIT|GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
	
	PrintGL(x, y, text);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

void Font::PrintGL(float x, float y, char *text)
{
	GLuint font = myListFirst;
	float height = this->myHeight/.63f;

	const char *startLine=text;
	std::vector<std::string> lines;

	const char * c = text;
	for(  ; *c ; c++ )
	{
		if(*c=='\n')
		{
			std::string line;
			for (const char *n = startLine; n < c ; n++)
				line.append(1,*n);
			
			lines.push_back(line);
			startLine = c + 1;
		}
	}

	if (startLine)
	{
		std::string line;
		for(const char *n=startLine;n<c;n++) line.append(1,*n);
		lines.push_back(line);
	}

	std::vector<int> align = std::vector< int >(lines.size());
	CalcAlignment(lines, align);

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glListBase(font);

	float modelview_matrix[16];	
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	for(unsigned int i=0;i<lines.size();i++)
	{
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(x,y-height*i,0);
		glMultMatrixf(modelview_matrix);
		glTranslatef(align[i], 0, 0);
		glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
		glPopMatrix();
	}
	glPopAttrib();

	//glPushAttrib(GL_TRANSFORM_BIT);
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glPopAttrib();
}

bool Font::RenderFaceToTexture(FT_Face face, char ch)
{
	
	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
	{
		#ifdef _DEBUG
		LOG << "FT_Load_Glyph failed" << std::endl;
		#endif
		return false;
	}

	FT_Glyph glyph;
	if(FT_Get_Glyph( face->glyph, &glyph ))
	{
		#ifdef _DEBUG
		LOG << "FT_Get_Glyph failed" << std::endl;
		#endif
		return false;
	}

	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;

	int width  = NextPowerOfTwo( bitmap.width );
	int height = NextPowerOfTwo( bitmap.rows  );

	GLubyte* tempbuff = new GLubyte[ 2 * width * height];

	for(int j=0; j < height;j++)
	{
		for(int i=0; i < width; i++)
		{
			tempbuff[2*(i+j*width)+0] = ~0;
			tempbuff[2*(i+j*width)+1] =
				(i>=bitmap.width || j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
		}
	}

	glBindTexture( GL_TEXTURE_2D, myTextures[(int)ch]);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);/
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height,
//	              0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, tempbuff );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA, width, height,
			   GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, tempbuff );

	delete [] tempbuff;

	glyph_metrics &m = myMetrics[(int)ch];
	m.advance = face->glyph->advance.x >> 6;
	m.width   = bitmap.width;
	m.rows    = bitmap.rows;
	m.top     = bitmap_glyph->top;
	m.left    = bitmap_glyph->left;
	
	m.texwidth  = (float)m.width / (float)width;
	m.texheight = (float)m.rows  / (float)height;


	glNewList(myListFirst+ch,GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D,myTextures[(int)ch]);
	glPushMatrix();
	glTranslatef(m.left, m.top - m.rows, 0);

	glBegin(GL_QUADS);
	glTexCoord2d(         0,          0); glVertex2f(0,m.rows);
	glTexCoord2d(         0,m.texheight); glVertex2f(0,0);
	glTexCoord2d(m.texwidth,m.texheight); glVertex2f(m.width,0);
	glTexCoord2d(m.texwidth,          0); glVertex2f(m.width,m.rows);
	glEnd();

	glPopMatrix();
	glTranslatef(m.advance ,0,0);

	glEndList();

	return true;
}

void Font::CalcAlignment(std::vector<std::string>& lines, std::vector<int>& align) {
	int vertAlignment, horzAlignment;
	vertAlignment = myAlignment >> 2;
	horzAlignment = myAlignment &0x03;

	if (horzAlignment == LEFT) return;

	for(unsigned int l = 0; l < lines.size(); l++)
	{
		int total_advance = 0;
		const char *current = lines[l].c_str();
		int current_size = lines[l].size();

		for (int i = 0; i < current_size; i++)
		{
			const glyph_metrics &m = myMetrics[(int)current[i]];
			total_advance += m.advance;
		}

		switch (horzAlignment) {
			case LEFT:
				align[l] = 0;
				break;
			case RIGHT:
				align[l] = -total_advance;
				break;
			case CENTER:
				align[l] = -total_advance/2;
				break;
			default:
				align[l] = 0;
				break;
		}
	}
}

void Font::Clean() {
	
	if (myTextures != NULL)
	{
		for (int i = 0; i < CHARSET_SIZE; i++) {
			//TODO: DELETE TEXTURES		
		}
		delete myTextures;
		myTextures = NULL;
	}

	if (myMetrics != NULL)
	{
		delete myMetrics;
		myMetrics = NULL;
	}

	//TODO: CLEAN m_listfirst
	myListFirst = 0;
	myAlignment = (BASELINE << 2) | LEFT;
}

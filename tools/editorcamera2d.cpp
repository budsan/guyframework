#include "editorcamera2d.h"
#include <GL/gl.h>

Camera2D::Camera2D()
{
	init = false;
	sizeGrid = 1.0f;
	showGrid = false;
	gridMagnetEnabled = false;

	reset();
}

void Camera2D::Draw()
{
	if (showGrid)
	{
		float sizeGridAux = sizeGrid;
		sizeGridAux /= 2;

		double grid = 0.0001f;
		while (grid < zoom) grid *= 10.0;
		grid /= 100.0;

		for (unsigned int i = 0; i < 3; i++, grid *= 10.0)
		{
			if (i < 2 && grid < sizeGridAux) continue;

			float col = (grid/zoom)*2.0f;
			if (col > 0.8f) col = 0.8f;

			if (grid > 1.1 || grid < 0.9)
				glColor3f(col,.0f,.0f);
			else
				glColor3f(col/2,col,.0f);

			glLineWidth(1.0f);
			DrawGrid(grid);
		}
	}

#define REFERENCE_SIZE 100
#ifdef REFERENCE_SIZE
#define RS (REFERENCE_SIZE/2.0)
	static const float pts[] = {-RS,-RS,+RS,-RS,+RS,+RS,-RS,+RS,-RS,-RS};
#undef RS

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LINE_SMOOTH);
	glColor3f(1.0f,0.0f,0.0f);
	glLineWidth(3.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, pts);
	glDrawArrays(GL_LINE_STRIP, 0, 5);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopAttrib();
#endif
}

bbox2f Camera2D::getBounding()
{
	vec2f min(-1.0f, -1.0f);
	vec2f max(+1.0f, +1.0f);

	if (aspect >= 1.0f)
	{
		min.x *= aspect;
		max.x *= aspect;
	}
	else
	{
		min.y /= aspect;
		max.y /= aspect;
	}

	min *= zoom; max *= zoom;
	min += pos; max += pos;
	return bbox2f(min,max);
}

mat4f Camera2D::getMatrix()
{
	if (aspect > 1.0)
	{
		float zoomAux = zoom*aspect;
		return mat4f::from_ortho(pos.x-zoomAux,pos.x+zoomAux,
		      pos.y-zoom   ,pos.y+zoom   , -1, 1);
	}
	else
	{
		float zoomAux = zoom/aspect;
		return mat4f::from_ortho(pos.x-zoom   ,pos.x+zoom,
		      pos.y-zoomAux,pos.y+zoomAux, -1, 1);
	}
}

void Camera2D::setCamera()
{
	if (!init) return;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	mat4f mat = getMatrix();
	glLoadMatrixf(mat.val);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Camera2D::reset()
{
	pos = vec2f(0, 0);
	zoom = 55.0f;
}

void Camera2D::resizeScreen(int width, int height)
{
	glViewport(0, 0, width, height);
	w = width; h = height;
	aspect = (float)w/(float)h;

	init = true;
}

void Camera2D::setZoom(float _zoom, const vec2i &_pos)
{
	vec2f now = click(_pos);
	vec2f dif = (now-pos)/zoom;

	zoom -= zoom*_zoom*0.001f;
	if (zoom < 0.001) zoom = 0.001;

	dif *= zoom;
	pos = now-dif;

	move(vec2f(0,0));
}

void Camera2D::move(const vec2i &now, const vec2i &before)
{
	vec2f mov(now.x - before.x, now.y - before.y);
	mov.x = -2*mov.x/(float)w;
	mov.y =  2*mov.y/(float)h;

	if (aspect > 0) mov.x *= aspect;
	else            mov.y /= aspect;

	move(mov);
}

void Camera2D::move(const vec2f &dir)
{
	pos += dir*zoom;
}

vec2f Camera2D::click(const vec2i &now) const
{
	vec2f cpos(now.x, now.y);
	cpos.x =( 2.0f*cpos.x/(float)w)-1.0f;
	cpos.y =(-2.0f*cpos.y/(float)h)+1.0f;

	if (aspect > 1.0f) cpos.x *= aspect;
	else               cpos.y /= aspect;

	cpos *= zoom;
	cpos += pos;

	return cpos;
}

vec2f Camera2D::clickMagnet(const vec2i &now) const
{
	vec2f cpos = click(now);
	Magnet(cpos);
	return cpos;
}

void Camera2D::enableGridMagnet(bool enable)
{
	gridMagnetEnabled = enable;
}

void Camera2D::Magnet(vec2f &cpos) const
{
	if (gridMagnetEnabled)
	{
		cpos /= sizeGrid;
		cpos.x = floor(cpos.x+0.5f);
		cpos.y = floor(cpos.y+0.5f);
		cpos *= sizeGrid;
	}
}

void Camera2D::biggerGrid()
{
	sizeGrid *= 10.f;
}

void Camera2D::smallerGrid()
{
	if (sizeGrid < 0.001f) return;
	sizeGrid /= 10.f;
}

void Camera2D::resetGrid()
{
	sizeGrid = 1.0f;
}

void Camera2D::DrawGrid(float size)
{
	bbox2f b = getBounding();

	float x = floor(b.min.x/size)*size;
	float y = floor(b.min.y/size)*size;

	glBegin(GL_LINES);
	while(x <= b.max.x)
	{
		if ( x >= b.min.x)
		{
			glVertex2f(x, b.min.y);
			glVertex2f(x, b.max.y);
		}
		x += size;
	}

	while(y <= b.max.y)
	{
		if ( y >= b.min.y)
		{
			glVertex2f(b.min.x, y);
			glVertex2f(b.max.x, y);
		}
		y += size;
	}
	glEnd();
}

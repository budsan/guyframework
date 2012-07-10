#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "math/vec2.h"
#include "math/bbox.h"
#include "math/mat4.h"

using math::vec2f;
using math::vec2i;
using math::bbox2f;
using math::mat4f;

class Camera2D
{
public:
	Camera2D();

	void Draw();
	bbox2f getBounding();

	void resizeScreen(int width, int height);

	mat4f getMatrix();
	void setCamera();
	void reset();

	void setZoom(float zoom, const vec2i &pos);
	float getZoom() const {return zoom;}
	void move(const vec2i &now, const vec2i &before);
	void move(const vec2f &dir);
	vec2f getPos() const { return pos;}

	vec2f click(const vec2i &now) const;
	vec2f clickMagnet(const vec2i &now) const;

	void Magnet(vec2f &cpos) const;

	int width()  const {return w;}
	int height() const {return h;}

	bool showGrid;
	void enableGridMagnet(bool enable);
	void biggerGrid();
	void smallerGrid();
	void resetGrid();

private:
	void DrawGrid(float size);

	bool init;
	vec2f pos;
	float zoom;

	float aspect;
	int w, h;

	float sizeGrid;
	bool gridMagnetEnabled;
};

#endif // CAMERA2D_H

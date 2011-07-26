#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QWidget>
#include <QtOpenGL/qgl.h>
#include <QKeyEvent>
#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>

#include <string>
#include "tools/editorcamera2d.h"
#include "tools/particleemitter.h"

#include "particleparameters.h"

class EditorWidget : public QGLWidget
{
	Q_OBJECT

public:
	 EditorWidget(QWidget * parent);
	~EditorWidget();

	ParticleParameters * getParticleParameters() { return parameters;}
signals:

public slots:
	void newEmitter();
	void save();
	bool saveAs();
	void open();

private slots:
	void update();

protected:

	virtual void initializeGL();
	virtual void paintGL( void );
	virtual void resizeGL (int width, int height);

	//INPUT
	virtual void keyPressEvent(QKeyEvent *e);
	virtual void keyReleaseEvent(QKeyEvent *e);

	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void wheelEvent(QWheelEvent* event);

private:
	QTimer *timer;
	QTime  *ticks;
	int before;
	ParticleParameters *parameters;
	Camera2D cam;

	float aspect;
	int w, h;

	enum action {NONE, MOVE};
	int currAction;
	vec2i lastMousePos;

	std::string filename;
};

#endif // EDITORWIDGET_H

#include "editorwidget.h"

#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <cmath>
#include "graphics/color.h"

EditorWidget::EditorWidget(QWidget *parent) : QGLWidget(parent)
{
	setFocusPolicy(Qt::ClickFocus);
	currAction = NONE;
	parameters = new ParticleParameters(this);
	timer = new QTimer(this);
	ticks = new QTime();
	newEmitter();
}

EditorWidget::~EditorWidget()
{
	delete ticks;
	delete timer;
	delete parameters;
}

void EditorWidget::initializeGL()
{
	//glClearColor(0.4f, 0.4f, 0.8f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glShadeModel(GL_FLAT);
	glCullFace(GL_BACK);
	glPointSize(10.0f);
	//glEnable(GL_POINT_SMOOTH);
	//glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);

	cam.reset();
	cam.showGrid = true;
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(20);
	ticks->start();
	before = 0;
}

void EditorWidget::paintGL( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	cam.setCamera();
	cam.Draw();
	//glBegin(GL_LINES);
	//glColor3f(1,0,0);glVertex2f(0,0);glVertex2f(0.5f,0);
	//glColor3f(0,0,1);glVertex2f(0,0);glVertex2f(0,0.5f);
	//glEnd();

	parameters->getEmitter().draw();
}

void EditorWidget::resizeGL (int width, int height)
{
	w = width; h = height;
	aspect = (float)w/(float)h;
	cam.resizeScreen(width, height);
}

void EditorWidget::newEmitter()
{
	filename = std::string();
	parameters->newEmitter();
}

void EditorWidget::save()
{
	if (filename.empty()) {
		saveAs();
		return;
	}

	std::ofstream file(filename.c_str(),
			   std::ofstream::out|
			   std::ofstream::trunc|
			   std::ofstream::binary);

	if (!file.is_open()) return;
	parameters->getEmitter().write(file);
}

bool EditorWidget::saveAs()
{
	timer->stop();
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Emitter"), "", tr("Emitter files (*.emp)"));
	timer->start();

	filename = std::string((const char*)fileName.toAscii());
	if (filename.empty()) return false;
	save();

	return true;
}

void EditorWidget::open()
{
	timer->stop();
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Emitter"), "", tr("Emitter files (*.emp)"));
	filename = std::string((const char*)fileName.toAscii());
	timer->start();

	std::ifstream file(filename.c_str(),
			   std::ifstream::in|
			   std::ifstream::binary);

	if (!file.is_open()) return;
	parameters->getEmitter().read(file);
	parameters->getEmitter().restart();
	parameters->UpdateUI();

}

void EditorWidget::update()
{
	int now = ticks->elapsed();
	float deltaTime = float(now - before)/1000.f;
	parameters->getEmitter().update(deltaTime);

	updateGL();
	before = now;
}

void EditorWidget::keyPressEvent(QKeyEvent *e)
{
	//if (e->modifiers() &Qt::ShiftModifier);
	//else if (e->modifiers() &Qt::ControlModifier);
	//else;
	switch( e->key() )
	{
	case Qt::Key_Home:
		cam.reset();
		break;
        case Qt::Key_Space:
		parameters->getEmitter().restart();
                break;
	default: e->ignore();
	 }
}

/*-keyReleaseEvent()---------------------------------------------------------*/
void EditorWidget::keyReleaseEvent(QKeyEvent *e)
{
	switch( e->key() )
	{
	case Qt::Key_Shift:
	case Qt::Key_Control:
	default: e->ignore();
	 }
}

/*-mousePressEvent()---------------------------------------------------------*/
void EditorWidget::mousePressEvent( QMouseEvent *e)
{
	currAction = MOVE;
	lastMousePos.x = e->x(); lastMousePos.y = e->y();
}

/*-mouseReleaseEvent()-------------------------------------------------------*/
void EditorWidget::mouseReleaseEvent(QMouseEvent *e)
{
	currAction = NONE;
}

/*-mouseMoveEvent()----------------------------------------------------------*/
void EditorWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (currAction == MOVE)
	{
		vec2i now(e->x(), e->y());
		cam.move(now, lastMousePos);
		lastMousePos.x = e->x(); lastMousePos.y = e->y();
	}
}

/*-wheelEvent()--------------------------------------------------------------*/
void EditorWidget::wheelEvent(QWheelEvent* e)
{
	float zoom = (float)e->delta();
	vec2i pos(e->x(), e->y());
	cam.setZoom(zoom, pos);
}

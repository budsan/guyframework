#include "particleparameters.h"
#include "ui_particleparameters.h"

#include <QColorDialog>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEG2RAD ((M_PI*2.0)/(360.0))
#define RAG2DEG ((360.0)/(M_PI*2.0))

ParticleParameters::ParticleParameters(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParticleParameters)
{
	ui->setupUi(this);

	m_applyAutomatically = false;
	ui->ApplyAutoCheckbox->setChecked(false);
	newEmitter();
}

ParticleParameters::~ParticleParameters()
{
    delete ui;
}

void ParticleParameters::newEmitter()
{
	m_parameters.setAngle(M_PI*2.0f);
	m_parameters.setDirection(vec2f(0,0));
	m_parameters.setFrequency(0.1);
	m_parameters.setGravity(vec2f(0,0));
	m_parameters.setParticleColor(rgba(1,1,1,1), rgba(1,1,1,0));
	m_parameters.setParticleLive(1, 5);
	m_parameters.setParticleNumber(-1);
	m_parameters.setParticleSpeed(2, 5);
	m_parameters.setParticleSize(8, 16);
	m_parameters.setPosition(vec2f(0,0));
	m_parameters.setParticleAccumulativeColor(false);
	m_parameters.setParticleMaterial("glow");
	m_parameters.restart();
	UpdateUI();
}

void ParticleParameters::UpdateUI()
{
	ui->ApertureSpin->setValue(m_parameters.getAngle()*RAG2DEG);
	ui->DirectionSpin->setValue(m_parameters.getDirection()*RAG2DEG);
	const math::vec2f &pos = m_parameters.position();
    const math::vec2f &grav = m_parameters.gravity();
	ui->PosXSpin->setValue(pos.x);
	ui->PosYSpin->setValue(pos.y);
	ui->GravXSpin->setValue(grav.x);
	ui->GravYSpin->setValue(grav.y);
    ui->FrequencySpin->setValue(m_parameters.frequency());
    StartColor = toQColor(m_parameters.particleColorStart());
    EndColor = toQColor(m_parameters.particleColorEnd());
	setBackgroundColor(ui->StartColorBut, StartColor);
	setBackgroundColor(ui->EndColorBut, EndColor);
    ui->MinLiveSpin->setValue(m_parameters.particleLiveMin());
    ui->MaxLiveSpin->setValue(m_parameters.particleLiveMax());
    ui->NumPartSpin->setValue(m_parameters.particleCount());
    ui->MinSpeedSpin->setValue(m_parameters.particleSpeedMin());
    ui->MaxSpeedSpin->setValue(m_parameters.particleSpeedMax());
    ui->StartSizeSpin->setValue(m_parameters.particleSizeStart());
    ui->EndSizeSpin->setValue(m_parameters.particleSizeEnd());
    ui->AccumColorCheckbox->setChecked(m_parameters.particleAccumulativeColor());
    ui->MaterialLineEdit->setText(QString(m_parameters.particleMaterial().c_str()));
}

void ParticleParameters::ApplyChanges()
{
	math::vec2f pos(ui->PosXSpin->value(), ui->PosYSpin->value());
	math::vec2f grav(ui->GravXSpin->value(), ui->GravYSpin->value());
	double frequency = ui->FrequencySpin->value();
	int numParticle = ui->NumPartSpin->value();
	if (numParticle == -1 && frequency == 0) numParticle = 1;

	m_parameters.setAngle(ui->ApertureSpin->value() * DEG2RAD);
	m_parameters.setDirection(ui->DirectionSpin->value() * DEG2RAD);
	m_parameters.setPosition(pos);
	m_parameters.setGravity(grav);
	m_parameters.setFrequency(frequency);
	m_parameters.setParticleColor(toRGBA(StartColor), toRGBA(EndColor));
	m_parameters.setParticleLive(ui->MinLiveSpin->value(), ui->MaxLiveSpin->value());
	m_parameters.setParticleNumber(numParticle);
	m_parameters.setParticleSpeed(ui->MinSpeedSpin->value(), ui->MaxSpeedSpin->value());
	m_parameters.setParticleSize(ui->StartSizeSpin->value(), ui->EndSizeSpin->value());
	m_parameters.setParticleAccumulativeColor(ui->AccumColorCheckbox->isChecked());
	m_parameters.setParticleMaterial(ui->MaterialLineEdit->text().toStdString());
}

void ParticleParameters::setBackgroundColor(QPushButton* but, const QColor &col)
{
	but->setAutoFillBackground(true);
	QString s = "background-color: ";
	but->setStyleSheet(s + col.name());
}

rgba ParticleParameters::toRGBA(const QColor &col)
{
	return rgba(float(col.red())/255.0f,
		    float(col.green())/255.0f,
		    float(col.blue())/255.0f,
		    float(col.alpha())/255.0f);
}

QColor ParticleParameters::toQColor(const rgba   &col)
{
	return QColor((int)floor(col.r*255.0f),
		      (int)floor(col.g*255.0f),
		      (int)floor(col.b*255.0f),
		      (int)floor(col.a*255.0f));
}

void ParticleParameters::on_StartColorBut_clicked()
{
	QColor col = QColorDialog::getColor(StartColor, this,
		tr("Select Color"), QColorDialog::ShowAlphaChannel);
	if (col.isValid())
	{
		StartColor = col;
		setBackgroundColor(ui->StartColorBut, col);
	}
}

void ParticleParameters::on_EndColorBut_clicked()
{
	QColor col = QColorDialog::getColor(EndColor, this,
		tr("Select Color"), QColorDialog::ShowAlphaChannel);
	if (col.isValid())
	{
		EndColor = col;
		setBackgroundColor(ui->EndColorBut, col);
	}
}

void ParticleParameters::on_ApplyBut_clicked()
{
    ApplyChanges();
    m_parameters.restart();
}
